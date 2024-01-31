#include <iostream>

#include "parser.h"

// https://github.com/progsource/maddy

const std::string testMarkdown = "# This is a test\n\
\n\
This should result in a praragraph\n\
it's that simple.\n\
\n\
* an *unordered* list\n\
  * with some **hierarchy**\n\
    1. and an _ordered_\n\
    * list\n\
    * directly\n\
  * inside\n\
\n\
```\n\
var c = 'blub';\n\
```\n\
\n\
> A Quote\n\
>\n\
> With some ~~text~~  blocks inside\n\
>\n\
> * even a list\n\
> * should be\n\
> * possible\n\
>\n\
\n\
And well `inline code` should also work.\n\
\n\
## Another Headline\n\
\n\
And not to forget [link to progsource](http://progsource.de) should work.\n\
And well - let's see how an image would be shown:\n\
\n\
![an image](http://progsource.de/img/progsource.png)\n\
\n\
---\n\
\n\
<a name=\"to top\"></a>\n\
\n\
### and more headlines\n\
\n\
- [ ] how\n\
- [ ] about\n\
  - [ ] a\n\
  - [x] nice\n\
- [x] check\n\
- [ ] list\n\
\n\
#### even a table\n\
\n\
|table>\n\
Left header|middle header|last header\n\
- | - | -\n\
cell 1|cell **2**|cell 3\n\
cell 4|cell 5|cell 6\n\
- | - | -\n\
foot a|foot b|foot c\n\
|<table\n\
\n\
##### h5\n\
###### h6\n\
\n\
";

int main(int argc, char *argv[])    
{
    std::cout << "Hello World!" << std::endl;
    // matrix::markdown::parser::Token();
    // matrix::markdown::parser::Parser parser;
    // std::cout << parser.version << std::endl;

    std::stringstream markdown(testMarkdown);

    auto config = std::make_shared<matrix::markdown::parser::ParserConfig>();
    // config->isHeadlineInlineParsingEnabled = false;
    auto parser = std::make_shared<matrix::markdown::parser::Parser>(config);
    const std::string output = parser->Parse(markdown);

    std::cout << output << std::endl;


   return 0;
}
