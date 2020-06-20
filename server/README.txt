


server.cpp :

function 'split_string_and_create_event(string event_string)' get string sended by client, split it, and fill event_struct defined in file server.h
  
function 'check_if_show_user_events(string string_to_check)' check if client want to add new event(return string "ADD_USER_EVENT") 
or if client whats to show all his events(return "USER_EVENT").

function 'merge_all_strings(vector<string> vec)' gets vector of strings which one string is one field of all fields of one event of all events of one user, and merge all strings separated witch char '|' later this string will be send to client.

database.cpp :
 
function 'insert_user_with_event(string name, event_struct event)' gets user name, check if user have already added event ,if not function create new table in database which store events of user,if user name is 'user_name' new table which store user events will be named 'Eventuser_name'.

  
function 'show_all_events(string name)' gets user(variable name) all events from 'Eventuser' table, it returns vector<string> each string of this vector if one filed of 'Eventuser' table 

 
