#ifndef LOADER_HPP
#define LOADER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "parser.hpp"

namespace loader{

using boost::asio::ip::tcp;

class Loader
{
public:
    Loader(boost::asio::io_service& io_service);

    void load(const std::string& url);

private:
    void handle_resolve(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& error);
    void handle_write_request(const boost::system::error_code& error);
    void handle_read_status(const boost::system::error_code& error);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_content(const boost::system::error_code& error);

private:
    boost::asio::ip::tcp::resolver m_resolver;
    boost::asio::ip::tcp::socket m_socket;
    std::string m_server, m_path;
    boost::asio::streambuf m_request;
    boost::asio::streambuf m_reply;
};

}

#endif // LOADER_HPP

