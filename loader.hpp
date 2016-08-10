#ifndef LOADER_HPP
#define LOADER_HPP

#include <boost/asio.hpp>
#include "parser.hpp"

namespace loader{

class Loader
{
public:

    void load(const std::string& url);

private:
    boost::asio::io_service m_io;

};

}

#endif // LOADER_HPP

