
#ifndef _DATA_H
#define _DATA_H
#include <iostream>
#include <vector>
#include <string>
#include <string_view>

#include "sqlite3.h"
#include "server.h"
sqlite_int64 insert_user_with_event(std::string name, event_struct event_name);
std::vector<std::string> show_all_events(std::string name);
#endif
