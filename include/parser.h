/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <memory>
#include <functional>
#include <string>

#include "parserconfig.h"

// BlockParser
#include "checklistparser.h"
#include "codeblockparser.h"
#include "headlineparser.h"
#include "horizontallineparser.h"
#include "htmlparser.h"
#include "latexblockparser.h"
#include "orderedlistparser.h"
#include "paragraphparser.h"
#include "quoteparser.h"
#include "tableparser.h"
#include "unorderedlistparser.h"

// LineParser
#include "lineparsers.h"

// -----------------------------------------------------------------------------

namespace matrix::markdown::parser {

// -----------------------------------------------------------------------------

/**
 * Parser
 *
 * Transforms Markdown to HTML
 *
 * @class
 */
class Parser
{
public:
  /**
   * Version info
   *
   * Check https://github.com/progsource/maddy/blob/master/CHANGELOG.md
   * for the changelog.
  */
  static const std::string& version() { static const std::string v = "1.3.0"; return v; }

  /**
   * ctor
   *
   * Initializes all `LineParser`
   *
   * @method
   */
  Parser(std::shared_ptr<ParserConfig> config = nullptr)
    : config(config)
  {
    // deprecated backward compatibility
    // will be removed in 1.4.0 latest including the booleans
    if (this->config && !this->config->isEmphasizedParserEnabled)
    {
      this->config->enabledParsers &= ~matrix::markdown::parser::types::EMPHASIZED_PARSER;
    }
    if (this->config && !this->config->isHTMLWrappedInParagraph)
    {
      this->config->enabledParsers |= matrix::markdown::parser::types::HTML_PARSER;
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::BREAKLINE_PARSER) != 0
    )
    {
      this->breakLineParser = std::make_shared<BreakLineParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::EMPHASIZED_PARSER) != 0
    )
    {
      this->emphasizedParser = std::make_shared<EmphasizedParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::IMAGE_PARSER) != 0
    )
    {
      this->imageParser = std::make_shared<ImageParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::INLINE_CODE_PARSER) != 0
    )
    {
      this->inlineCodeParser = std::make_shared<InlineCodeParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::ITALIC_PARSER) != 0
    )
    {
      this->italicParser = std::make_shared<ItalicParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::LINK_PARSER) != 0
    )
    {
      this->linkParser = std::make_shared<LinkParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::STRIKETHROUGH_PARSER) != 0
    )
    {
      this->strikeThroughParser = std::make_shared<StrikeThroughParser>();
    }

    if (
      !this->config ||
      (this->config->enabledParsers & matrix::markdown::parser::types::STRONG_PARSER) != 0
    )
    {
      this->strongParser = std::make_shared<StrongParser>();
    }
  }
  

  /**
   * Parse
   *
   * @method
   * @param {const std::istream&} markdown
   * @return {std::string} HTML
   */
  std::string
  Parse(std::istream& markdown) const
  {
    std::string result = "";
    std::shared_ptr<BlockParser> currentBlockParser = nullptr;

    for (std::string line; std::getline(markdown, line);)
    {
      if (!currentBlockParser)
      {
        currentBlockParser = getBlockParserForLine(line);
      }

      if (currentBlockParser)
      {
        currentBlockParser->AddLine(line);

        if (currentBlockParser->IsFinished())
        {
          result += currentBlockParser->GetResult().str();
          currentBlockParser = nullptr;
        }
      }
    }

    // make sure, that all parsers are finished
    if (currentBlockParser)
    {
      std::string emptyLine = "";
      currentBlockParser->AddLine(emptyLine);
      if (currentBlockParser->IsFinished())
      {
        result += currentBlockParser->GetResult().str();
        currentBlockParser = nullptr;
      }
    }

    return result;
  }

private:
  std::shared_ptr<ParserConfig> config;
  std::shared_ptr<BreakLineParser> breakLineParser;
  std::shared_ptr<EmphasizedParser> emphasizedParser;
  std::shared_ptr<ImageParser> imageParser;
  std::shared_ptr<InlineCodeParser> inlineCodeParser;
  std::shared_ptr<ItalicParser> italicParser;
  std::shared_ptr<LinkParser> linkParser;
  std::shared_ptr<StrikeThroughParser> strikeThroughParser;
  std::shared_ptr<StrongParser> strongParser;

  // block parser have to run before
  void
  runLineParser(std::string& line) const
  {
    // Attention! ImageParser has to be before LinkParser
    if (this->imageParser)
    {
      this->imageParser->Parse(line);
    }

    if (this->linkParser)
    {
      this->linkParser->Parse(line);
    }

    // Attention! StrongParser has to be before EmphasizedParser
    if (this->strongParser)
    {
      this->strongParser->Parse(line);
    }

    if (this->emphasizedParser)
    {
      this->emphasizedParser->Parse(line);
    }

    if (this->strikeThroughParser)
    {
      this->strikeThroughParser->Parse(line);
    }

    if (this->inlineCodeParser)
    {
      this->inlineCodeParser->Parse(line);
    }

    if (this->italicParser)
    {
      this->italicParser->Parse(line);
    }

    if (this->breakLineParser)
    {
      this->breakLineParser->Parse(line);
    }
  }

  std::shared_ptr<BlockParser>
  getBlockParserForLine(const std::string& line) const
  {
    std::shared_ptr<BlockParser> parser;

    if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::CODE_BLOCK_PARSER) != 0
      ) &&
      matrix::markdown::parser::CodeBlockParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::CodeBlockParser>(
        nullptr,
        nullptr
      );
    }
    else if (
      this->config &&
      (this->config->enabledParsers & matrix::markdown::parser::types::LATEX_BLOCK_PARSER) != 0 &&
      matrix::markdown::parser::LatexBlockParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<LatexBlockParser>(
        nullptr,
        nullptr
      );
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::HEADLINE_PARSER) != 0
      ) &&
      matrix::markdown::parser::HeadlineParser::IsStartingLine(line)
    )
    {
      if (!this->config || this->config->isHeadlineInlineParsingEnabled)
      {
        parser = std::make_shared<matrix::markdown::parser::HeadlineParser>(
          [this](std::string& line){ this->runLineParser(line); },
          nullptr,
          true
        );
      }
      else
      {
        parser = std::make_shared<matrix::markdown::parser::HeadlineParser>(
          nullptr,
          nullptr,
          false
        );
      }
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::HORIZONTAL_LINE_PARSER) != 0
      ) &&
      matrix::markdown::parser::HorizontalLineParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::HorizontalLineParser>(
        nullptr,
        nullptr
      );
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::QUOTE_PARSER) != 0
      ) &&
      matrix::markdown::parser::QuoteParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::QuoteParser>(
        [this](std::string& line){ this->runLineParser(line); },
        [this](const std::string& line){ return this->getBlockParserForLine(line); }
      );
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::TABLE_PARSER) != 0
      ) &&
      matrix::markdown::parser::TableParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::TableParser>(
        [this](std::string& line){ this->runLineParser(line); },
        nullptr
      );
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::CHECKLIST_PARSER) != 0
      ) &&
      matrix::markdown::parser::ChecklistParser::IsStartingLine(line)
    )
    {
      parser = this->createChecklistParser();
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::ORDERED_LIST_PARSER) != 0
      ) &&
      matrix::markdown::parser::OrderedListParser::IsStartingLine(line)
    )
    {
      parser = this->createOrderedListParser();
    }
    else if (
      (
        !this->config ||
        (this->config->enabledParsers & matrix::markdown::parser::types::UNORDERED_LIST_PARSER) != 0
      ) &&
      matrix::markdown::parser::UnorderedListParser::IsStartingLine(line)
    )
    {
      parser = this->createUnorderedListParser();
    }
    else if (
      this->config &&
      (this->config->enabledParsers & matrix::markdown::parser::types::HTML_PARSER) != 0 &&
      matrix::markdown::parser::HtmlParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::HtmlParser>(nullptr, nullptr);
    }
    else if (
      matrix::markdown::parser::ParagraphParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<matrix::markdown::parser::ParagraphParser>(
        [this](std::string& line){ this->runLineParser(line); },
        nullptr,
        (!this->config || (this->config->enabledParsers & matrix::markdown::parser::types::PARAGRAPH_PARSER) != 0)
      );
    }

    return parser;
  }

  std::shared_ptr<BlockParser>
  createChecklistParser() const
  {
    return std::make_shared<matrix::markdown::parser::ChecklistParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (
          (
            !this->config ||
            (this->config->enabledParsers & matrix::markdown::parser::types::CHECKLIST_PARSER) != 0
          ) &&
          matrix::markdown::parser::ChecklistParser::IsStartingLine(line)
        )
        {
          parser = this->createChecklistParser();
        }

        return parser;
      }
    );
  }

  std::shared_ptr<BlockParser>
  createOrderedListParser() const
  {
    return std::make_shared<matrix::markdown::parser::OrderedListParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (
          (
            !this->config ||
            (this->config->enabledParsers & matrix::markdown::parser::types::ORDERED_LIST_PARSER) != 0
          ) &&
          matrix::markdown::parser::OrderedListParser::IsStartingLine(line)
        )
        {
          parser = this->createOrderedListParser();
        }
        else if (
          (
            !this->config ||
            (this->config->enabledParsers & matrix::markdown::parser::types::UNORDERED_LIST_PARSER) != 0
          ) &&
          matrix::markdown::parser::UnorderedListParser::IsStartingLine(line)
        )
        {
          parser = this->createUnorderedListParser();
        }

        return parser;
      }
    );
  }

  std::shared_ptr<BlockParser>
  createUnorderedListParser() const
  {
    return std::make_shared<matrix::markdown::parser::UnorderedListParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (
          (
            !this->config ||
            (this->config->enabledParsers & matrix::markdown::parser::types::ORDERED_LIST_PARSER) != 0
          ) &&
          matrix::markdown::parser::OrderedListParser::IsStartingLine(line)
        )
        {
          parser = this->createOrderedListParser();
        }
        else if (
          (
            !this->config ||
            (this->config->enabledParsers & matrix::markdown::parser::types::UNORDERED_LIST_PARSER) != 0
          ) &&
          matrix::markdown::parser::UnorderedListParser::IsStartingLine(line)
        )
        {
          parser = this->createUnorderedListParser();
        }

        return parser;
      }
    );
  }
}; // class Parser

// -----------------------------------------------------------------------------

} // namespace matrix::markdown::parser
