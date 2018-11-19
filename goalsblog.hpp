
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>


namespace eosio {
    static const account_name _self = N(bob.tc);
	static const account_name _goals = N(tt.tc);
    static const uint64_t MIN_PERMLINK_LENGTH = 10;
    static const uint64_t MAX_PERMLINK_LENGTH = 256;

    uint128_t combine_ids(const uint64_t &x, const uint64_t &y) {
        return (uint128_t{x} << 64) | y;
    };

        // @abi table goals
    struct goals{
        uint64_t id;
        account_name username;
        bool activated = false;
        bool in_protocol = false;
        bool completed = false;
        bool reported = false;
        bool validated = false;
        account_name host;
        uint64_t lepts_for_each_pool;
        std::string shortdescr;
        std::string descr;
        eosio::asset target;
        eosio::asset activation_amount;
        eosio::asset available;
        std::string report;
        uint64_t rotation_num;
        uint64_t total_votes;
        std::vector<account_name> voters;
        std::vector<uint64_t> balance_ids;
        eosio::asset withdrawed;
        uint64_t primary_key()const { return id; }
        uint64_t by_votes() const { return total_votes; }
        uint64_t by_activated() const {return activated; }
        uint64_t by_completed() const {return completed; }
        uint64_t by_in_protocol() const {return in_protocol; }
        account_name by_username() const {return username; }
        account_name by_host() const {return host;}
        uint64_t by_rotation_num() const {return rotation_num;}
        EOSLIB_SERIALIZE( goals, (id)(username)(activated)(in_protocol)(completed)(reported)
            (validated)(host)(lepts_for_each_pool)(shortdescr)(descr)(target)(activation_amount)
            (available)(report)(rotation_num)(total_votes)(voters)(balance_ids)(withdrawed))
    };

    typedef eosio::multi_index <N(goals), goals,
        indexed_by<N(total_votes), const_mem_fun<goals, uint64_t, &goals::by_votes>>,
        indexed_by<N(activated), const_mem_fun<goals, uint64_t, &goals::by_activated>>,
        indexed_by<N(completed), const_mem_fun<goals, uint64_t, &goals::by_completed>>,
        indexed_by<N(username), const_mem_fun<goals, account_name, &goals::by_username>>,
        indexed_by<N(host), const_mem_fun<goals, account_name, &goals::by_host>>,
        indexed_by<N(in_protocol), const_mem_fun<goals, uint64_t, &goals::by_in_protocol>>,
        indexed_by<N(rotation_num), const_mem_fun<goals, uint64_t, &goals::by_rotation_num>>
    > goals_index;


    //@abi table comments
    struct comments {
        uint64_t id;
        uint64_t goal_id;
        account_name author;
        eosio::time_point_sec created;
        eosio::time_point_sec last_update;
        
        std::string body;
        std::string permlink;
        std::string title;
        std::string meta;

        uint64_t primary_key() const {return id;}
        uint64_t by_author() const {return author;}

        EOSLIB_SERIALIZE(comments, (id)(goal_id)(author)(created)(last_update)(body)(permlink)(title)(meta))
    };

    typedef eosio::multi_index<N(comments), comments,
    indexed_by<N(author), const_mem_fun<comments, uint64_t, &comments::by_author>>
    > comments_index;


    //@abi action
    struct post {
        account_name host;
        uint64_t goal_id;
        account_name author;
        std::string body;
        std::string permlink;
        std::string title;
        std::string meta;
        EOSLIB_SERIALIZE(struct post, (host)(goal_id)(author)(body)(permlink)(title)(meta))
    };

    //@abi action
    struct edit {
        account_name host;
        account_name author;
        uint64_t comment_id;
        uint64_t goal_id;
        std::string permlink;
        std::string body;
        std::string title;
        std::string meta;
        EOSLIB_SERIALIZE(edit, (host)(author)(comment_id)(goal_id)(permlink)(body)(title)(meta))
    };

    //@abi action
    struct del {
        account_name host;
        account_name author;
        uint64_t comment_id;
        uint64_t goal_id;
        std::string permlink;
        

        EOSLIB_SERIALIZE(del, (host)(author)(comment_id)(goal_id)(permlink))
    };

 

}