
#pragma once
#include <regex>


namespace matrix::markdown::parser {

class LineParser {
public:
    LineParser() {}
    virtual ~LineParser() {}
    
    virtual void Parse(std::string& line) {
        line = std::regex_replace(line, std::regex(re_str), replacement);
    }

public:
    std::string re_str;
    std::string replacement;

}; // class LineParser


class BreakLineParser : public LineParser {public: BreakLineParser();};
class ImageParser : public LineParser {public: ImageParser();};
class EmphasizedParser : public LineParser {public: EmphasizedParser();};
class InlineCodeParser : public LineParser {public: InlineCodeParser();};
class ItalicParser : public LineParser {public: ItalicParser();};
class LinkParser : public LineParser {public: LinkParser();};
class StrikeThroughParser : public LineParser {public: StrikeThroughParser();};
class StrongParser : public LineParser {public: void Parse(std::string& line);};




}