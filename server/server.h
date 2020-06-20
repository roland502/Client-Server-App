#ifndef  _SERVER_H_
#define _SERVER_H_
#include <string>

typedef struct 
{
	std::string user_name;
	std::string text;
	std::string priority;
	std::string timestamp;
}event_struct;

std::string const event_added_string {"New event added to database"};

#endif
