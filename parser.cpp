#include <QIODevice>
#include <QRegExp>
#include <iostream>

#include "parser.hpp"

namespace parser{

Parser::FileList Parser::parse(QIODevice &dev) const
{
    FileList files;

    QString pattern = "/[^\\s^\"]+[\\d][.][^\"]+", str(dev.readAll());
    QRegExp reg(pattern);

    int pos= 0;
    while (pos >= 0)
    {
        pos = reg.indexIn(str, pos);
        QString rez = reg.cap(0);
        files.push_back(rez);
        if(pos >=0)
            pos += reg.matchedLength();
    }

    for(auto elem : files)
    {
        std::cout << elem.toStdString() << std::endl;
    }

    return files;
}

}
