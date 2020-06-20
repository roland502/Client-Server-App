
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <array>
#include <boost/asio.hpp>

#include "sqlite3.h"
#include "sqlite_modern_cpp.h"
#include "database.h"
#include "server.h"

using namespace std;
using namespace boost;

event_struct split_string_and_create_event(string event_string)
{
	event_struct ev;
    auto sstr = stringstream{event_string};
	auto token = string{};
	char const delimiter = ',';
	if(getline(sstr,token,delimiter))
		ev.user_name = token;
	if(getline(sstr,token,delimiter))
		ev.text = token;
	if(getline(sstr,token,delimiter))
		ev.priority = token;
	if(getline(sstr,token,delimiter))
		ev.timestamp = token;
	return ev;
}

string check_if_show_user_events(string string_to_check)
{
    auto sstr = stringstream{string_to_check};
	auto token = string{};
	char const delimiter = ',';
	getline(sstr,token,delimiter);
	
	if(!token.empty() && token == "USER_EVENTS")
	{
		getline(sstr,token,delimiter);
		return token;
	}
	
	return "ADD_USER_EVENT";
}

string merge_all_strings(vector<string> vec)
{
	ostringstream oss;
    oss << "123456789|";
	for(string_view const &s : vec) 
	{
		oss << s;
		oss << "|";
	}
    cout << std::flush;
	return oss.str();
}

class session : public std::enable_shared_from_this<session>
{
public:
	session(asio::ip::tcp::socket socket):tcp_socket(move(socket)){}
	void start() 
	{
		read();
	}
private:
   std::array<char, 100000>  data;
   asio::ip::tcp::socket   tcp_socket;
   
   void read()
   {
      auto self(shared_from_this());

      tcp_socket.async_read_some(
         asio::buffer(data, data.size()),
         [this, self](error_code const ec, std::size_t const length)
      {
         if (!ec)
         {
            auto ret = string(data.data(), length);
			string user_name = check_if_show_user_events(ret);
			if(user_name == "ADD_USER_EVENT")
			{
				event_struct event = split_string_and_create_event(ret);
                insert_user_with_event(event.user_name, event);
           		write(event_added_string);
            }
			else
			{
				vector<string> vec_ret = show_all_events(user_name);
            	string string_to_send = merge_all_strings(vec_ret);    
           		write(string_to_send);
			}
            
         }
      });
   }

	
   void write(std::string_view response)
   {
      auto self(shared_from_this());

      tcp_socket.async_write_some(
         asio::buffer(response.data(), response.length()),
         [this, self](std::error_code const ec, std::size_t const)
      {
         if (!ec)
         {
            read();
         }
      });
   }
};

class server
{
public:
   server(asio::io_context& context, short const port)
      : tcp_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
      , tcp_socket(context)
   {
      cout << "server running on port " << port << endl;

      accept();
   }

private:
   void accept()
   {
      tcp_acceptor.async_accept(tcp_socket, [this](error_code ec)
      {
         if (!ec)
         {
            std::make_shared<session>(move(tcp_socket))->start();
         }

         accept();
      });
   }

   asio::ip::tcp::acceptor tcp_acceptor;
   asio::ip::tcp::socket   tcp_socket;
};

void run_server(short const port)
{
   try
   {
      asio::io_context context;

      server srv(context, port);

      context.run();
   }
   catch (std::exception const & e)
   {
      cerr << "exception: " << e.what() << endl;
   }
}

int main()
{
   run_server(12345);
}

