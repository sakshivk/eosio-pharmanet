#include <boost/test/unit_test.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/permission_object.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/testing/tester.hpp>

using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

BOOST_AUTO_TEST_SUITE(pharmanet_tests)

BOOST_AUTO_TEST_CASE(post) try {
    tester t{setup_policy::none};

    // Load contract
    t.create_account(N(pharmanet));
    t.set_code(N(pharmanet), read_wasm("pharmanet.wasm"));
    t.set_abi(N(pharmanet), read_abi("pharmanet.abi").data());
    t.produce_block();

    // Create users
    t.create_account(N(john));
    t.create_account(N(jane));

    // Test "post" action
    t.push_action(
        N(pharmanet), N(post), N(john),
        mutable_variant_object //
        ("id", 1)              //
        ("reply_to", 0)        //
        ("user", "john")       //
        ("content", "post 1")  //
    );
    t.push_action(
        N(pharmanet), N(post), N(jane),
        mutable_variant_object //
        ("id", 2)              //
        ("reply_to", 0)        //
        ("user", "jane")       //
        ("content", "post 2")  //
    );
    t.push_action(
        N(pharmanet), N(post), N(john),
        mutable_variant_object       //
        ("id", 3)                    //
        ("reply_to", 2)              //
        ("user", "john")             //
        ("content", "post 3: reply") //
    );

    // Can't reply to non-existing record
    BOOST_CHECK_THROW(
        [&] {
            t.push_action(
                N(pharmanet), N(post), N(john),
                mutable_variant_object       //
                ("id", 4)                    //
                ("reply_to", 99)             //
                ("user", "john")             //
                ("content", "post 3: reply") //
            );
        }(),
        fc::exception);
}
FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
