#include <iostream>
#include <sstream>
#include <vector>
#include <string_view>

#include "sqlite3.h"
#include "sqlite_modern_cpp.h"
#include "database.h"
#include "server.h"

using namespace std;

sqlite::database db(R"(my_db)");

sqlite_int64 get_user_id(string const & name)
{
   sqlite_int64 id = 0;
   
   db << R"(select rowid from users where name=?;)" << name >> [&id](sqlite_int64 const rowid) {id = rowid; };
   return id;
}
sqlite_int64 insert_user_with_event(string name, event_struct event)
{
   sqlite_int64 id = get_user_id(name);
   
   if(id == 0) { 
    db << "insert into users values(?);" << name;
	
	ostringstream oss; 
	oss << "create table " << "Event" << name << "(ev_text text not null,priority text not null,timestamp text not null)";   	 
	db << oss.str();
   }
  
   ostringstream insert_oss;
   insert_oss << "insert into " << "Event" << name << " values(?,?,?);"; 
   db << insert_oss.str() << event.text << event.priority << event.timestamp;
   
   return db.last_insert_rowid();
}

vector<string> show_all_events(string name)
{
   vector<string> results;
   sqlite_int64 id = get_user_id(name);
   if(id == 0) 
   {
  	 return results; 
   }

   ostringstream oss; 
   oss << "select ev_text,priority,timestamp from " << "Event"<< name;
   db << oss.str() >> [&results](string const ev_text,string const priority,string const timestamp)
		{
			results.emplace_back(ev_text);
			results.emplace_back(priority);
			results.emplace_back(timestamp);
		};     
	
   return results;
}
