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

namespace matrix::markdown::parser {

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
    static const std::string& version() { static const std::string v = "1.0.0"; return v; }

    Parser(std::shared_ptr<ParserConfig> config = nullptr);
    std::string Parse(std::istream& markdown) const;

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
    void runLineParser(std::string& line) const;
    std::shared_ptr<BlockParser> getBlockParserForLine(const std::string& line) const;
    std::shared_ptr<BlockParser> createChecklistParser() const;
    std::shared_ptr<BlockParser> createOrderedListParser() const;
    std::shared_ptr<BlockParser> createUnorderedListParser() const;
}; // class Parser

// -----------------------------------------------------------------------------

} // namespace matrix::markdown::parser
