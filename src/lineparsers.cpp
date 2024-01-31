#include "lineparsers.h"

namespace matrix::markdown::parser{

BreakLineParser::BreakLineParser(){
    re_str = R"((\r\n|\r))";
    replacement = "<br>";
}

EmphasizedParser::EmphasizedParser(){
    re_str = R"((?!.*`.*|.*<code>.*)_(?!.*`.*|.*<\/code>.*)([^_]*)_(?!.*`.*|.*<\/code>.*))";
    replacement = "<em>$1</em>";
}

ImageParser::ImageParser(){
    re_str = R"(\!\[([^\]]*)\]\(([^\]]*)\))";
    replacement = "<img src=\"$2\" alt=\"$1\"/>";
}


InlineCodeParser::InlineCodeParser(){
    re_str = "`([^`]*)`";
    replacement = "<code>$1</code>";
}


ItalicParser::ItalicParser(){
    re_str = R"((?!.*`.*|.*<code>.*)\*(?!.*`.*|.*<\/code>.*)([^\*]*)\*(?!.*`.*|.*<\/code>.*))";
    replacement = "<i>$1</i>";
}


LinkParser::LinkParser(){
    re_str = R"(\[([^\]]*)\]\(([^\]]*)\))";
    replacement = "<a href=\"$2\">$1</a>";
}


StrikeThroughParser::StrikeThroughParser(){
    re_str = R"((?!.*`.*|.*<code>.*)\~\~(?!.*`.*|.*<\/code>.*)([^\~]*)\~\~(?!.*`.*|.*<\/code>.*))";
    replacement = "<s>$1</s>";
}

void StrongParser::Parse(std::string& line){
    static std::vector<std::regex> res
    {
        std::regex{R"((?!.*`.*|.*<code>.*)\*\*(?!.*`.*|.*<\/code>.*)([^\*\*]*)\*\*(?!.*`.*|.*<\/code>.*))"},
        std::regex{R"((?!.*`.*|.*<code>.*)__(?!.*`.*|.*<\/code>.*)([^__]*)__(?!.*`.*|.*<\/code>.*))"}
    };
    static std::string replacement = "<strong>$1</strong>";
    for (const auto& re : res)
    {
        line = std::regex_replace(line, re, replacement);
    }
}

}

