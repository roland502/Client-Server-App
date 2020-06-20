
#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
#include <chrono>
#include <ctime>

#include <boost/asio.hpp>

#define ADD_USER_AND_EVENT 1
#define USER_EVENTS 2
#define CLOSE_PROGRAM 3

using namespace boost;
using namespace std;

void show_reply_message(std::array<char,100000> reply,int reply_length)
{
	string reply_string = string(reply.data(),reply_length);
    auto sstr = stringstream{reply_string};
	auto token = string{};
	char const delimiter = '|';
	const string event_added_string {"New event added to database"};
	
	getline(sstr,token,delimiter);
	if(token == event_added_string)
    {
        cout << endl;
		cout << token << endl;
        return;
    }
	token.clear();
	
    cout << "text" << setw(25) << "priority" << setw(20) << "timestamp"<< endl;
	ostringstream oss;		
    
    int cnt = 0;
	int width = 10;
    while(getline(sstr,token,delimiter))
	{
		if(!token.empty())
        {
			oss << token;
            width += width;
            cnt++;
        }
		if(cnt < 3) 
			oss << setfill(' ') << setw(width);
		else
        {
			cnt = 0;
            width = 10;
        }
	}
    
	cout << oss.str() << endl;	
}

int input_routine_menu()
{
	cout << "To write user and event press 1" << endl;
	cout << "To get user events press 2" << endl;
	cout << "To quit client program press 3" << endl;
}

string get_current_time_and_convert_to_string()
{
	auto curr_time = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(curr_time);
	return ctime(&time);
}

void run_client(string_view host, short const port)
{
   try
   {
      asio::io_context context;
      asio::ip::tcp::socket tcp_socket(context);
      asio::ip::tcp::resolver resolver(context);
      asio::connect(tcp_socket, 
                    resolver.resolve({ host.data(), to_string(port) }));
      
      while (true)
      {
         ostringstream oss;
         int input_number;
     	 string user_name;
         
         input_routine_menu();
         cin >> input_number; 
		 if(cin.fail()) {
			cin.clear();
 			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
         }
		 
         switch(input_number)
         {         
            case ADD_USER_AND_EVENT:
            {
         		cout << "Name of user ";

         		cin >> user_name;
         		if (cin.fail())
            		break;
         
         		cout << "event text ";
		 		string text;
         		cin >> text;
         		if (cin.fail())
            		break;
         		
                cout << "event priority ";
		 		string priority;
         		cin >> priority;
         		if (cin.fail())
            		break;
		
         		string time_stamp = get_current_time_and_convert_to_string();
		 		oss << user_name <<","<< text << "," << priority << "," <<  time_stamp;	
            	break;
            }
            case USER_EVENTS:
			{
         		cout << "Name of user ";

         		cin >> user_name;
         		if (cin.fail())
            		break;
				oss << "USER_EVENTS," << user_name;
				break;
			}
            case CLOSE_PROGRAM:
			{
            	return;
            }
			default:
			{
				continue;	
            }
			
         }
         
         tcp_socket.write_some(asio::buffer(oss.str(), oss.str().length()));

         std::array<char, 100000> reply;
         auto reply_length = tcp_socket.read_some(asio::buffer(reply, reply.size()));

		 show_reply_message(reply,reply_length); 
         std::cout << std::endl << std::endl;
      }
   }
   catch (std::exception const & e)
   {
      std::cerr << "exception: " << e.what() << std::endl;
   }
}

int main()
{
   run_client("localhost", 12345);
}
