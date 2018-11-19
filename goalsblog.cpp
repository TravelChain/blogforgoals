#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/print.hpp>
#include "goalsblog.hpp"
using namespace eosio;

struct impl {
	
  void validate_permlink( const string& permlink )
        {
           eosio_assert( permlink.size() > MIN_PERMLINK_LENGTH && permlink.size() < MAX_PERMLINK_LENGTH, "Permlink is not a valid size." );

           for( auto c : permlink )
           {
              switch( c )
              {
                 case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
                 case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                 case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case '0':
                 case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                 case '-':
                    break;
                 default:
                    eosio_assert( false, "Invalid permlink character");
              }
           }
        }

  void posting_action(const post &op){

    require_auth(op.author);

    comments_index comments (_self, op.host);
    auto idx = comments.template get_index<N(author)>();
    goals_index goals (_goals, op.host);
    auto goal = goals.find(op.goal_id);
    eosio_assert(goal != goals.end(), "Goal is not exist or already closed");
    

    validate_permlink(op.permlink);

    comments.emplace(op.author, [&](auto &c){
      c.id = comments.available_primary_key();
      c.goal_id = op.goal_id;
      c.author = op.author;
      c.body = op.body;
      c.permlink = op.permlink;
      c.title = op.title;
      c.meta = op.meta;
    });
  }


  void edit_action(const edit &op){
    
    require_auth(op.author);

    comments_index comments (_self, op.host);
    auto comment = comments.find(op.comment_id);

    comments.modify(comment, op.author, [&](auto &c){
      c.body = op.body;
      c.title = op.title;
      c.meta = op.meta;
    });
  }


  void del_action(const del &op){
  
    require_auth(op.author);

    comments_index comments (_self, op.host);
    auto comment = comments.find(op.comment_id);

    comments.erase(comment);
  }






	void apply( uint64_t /*receiver*/, uint64_t code, uint64_t action ) {
      
      if (code == _self) {
        if (action == N(post)){
          posting_action(eosio::unpack_action_data<post>());
        } else if (action == N(edit)){
          edit_action(eosio::unpack_action_data<edit>());
        } else if (action == N(del)){
          del_action(eosio::unpack_action_data<del>());
        }
        
  };
};
};
extern "C" {
   
   /// The apply method implements the dispatch of events to this contract
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
      impl().apply(receiver, code, action);
   };

}; 