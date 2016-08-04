#ifndef PARSER_HPP
#define PARSER_HPP

#include <QStringList>

class QIODevice;

namespace parser{

    class Parser
    {
        typedef QStringList FileList;
    public:
        FileList parse(QIODevice& dev) const;
    };

}

#endif // PARSER_HPP

