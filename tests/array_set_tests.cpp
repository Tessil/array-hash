/**
 * MIT License
 * 
 * Copyright (c) 2017 Tessil
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#define BOOST_TEST_DYN_LINK

#include <tuple>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <tsl/array_set.h>
#include "utils.h"


BOOST_AUTO_TEST_SUITE(test_array_set)


using test_types = boost::mpl::list<
                        tsl::array_set<char>,
                        tsl::array_set<wchar_t>,
                        tsl::array_set<char16_t>,
                        tsl::array_set<char32_t>,
                        tsl::array_set<char, tsl::ah::str_hash<char>, tsl::ah::str_equal<char>, false>,
                        tsl::array_set<wchar_t, tsl::ah::str_hash<wchar_t>, tsl::ah::str_equal<wchar_t>, false>,
                        tsl::array_set<char16_t, tsl::ah::str_hash<char16_t>, tsl::ah::str_equal<char16_t>, false>,
                        tsl::array_set<char32_t, tsl::ah::str_hash<char32_t>, tsl::ah::str_equal<char32_t>, false>,
                        tsl::array_pg_set<char16_t>,
                        tsl::array_set<char16_t, tsl::ah::str_hash<char16_t>, tsl::ah::str_equal<char16_t>, false,
                                       std::uint16_t, std::uint16_t, tsl::ah::mod_growth_policy<>>
                    >;


/**
 * insert
 */                                      
BOOST_AUTO_TEST_CASE_TEMPLATE(test_insert, ASet, test_types) {
    // insert x values, insert them again, check values
    using char_tt = typename ASet::char_type;
    
    
    const size_t nb_values = 1000;
    ASet map;
    typename ASet::iterator it;
    bool inserted;
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        std::tie(it, inserted) = map.insert(key);
        
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
        BOOST_CHECK(inserted);
    }
    BOOST_CHECK_EQUAL(map.size(), nb_values);
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        std::tie(it, inserted) = map.insert(key);
        
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
        BOOST_CHECK(!inserted);
    }
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        it = map.find(key);
        
        BOOST_CHECK(it != map.end());
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
    }
}

BOOST_AUTO_TEST_CASE(test_insert_more_than_max_size) {
    tsl::array_set<char, tsl::ah::str_hash<char>, tsl::ah::str_equal<char>, true, 
                   std::uint16_t, std::uint8_t> map;
    for(std::size_t i=0; i < map.max_size(); i++) {
        map.insert(utils::get_key<char>(i));
    }
    
    BOOST_CHECK_EQUAL(map.size(), map.max_size());
    BOOST_CHECK_THROW(map.insert(utils::get_key<char>(map.max_size())), 
                      std::length_error);
}

BOOST_AUTO_TEST_CASE(test_insert_with_too_long_string) {
    tsl::array_set<char, tsl::ah::str_hash<char>, tsl::ah::str_equal<char>, true,
                   std::uint8_t, std::uint16_t> map;
    for(std::size_t i=0; i < 10; i++) {
        map.insert(utils::get_key<char>(i));
    }
    
    const std::string long_string("a", map.max_key_size());
    BOOST_CHECK(map.insert(long_string).second);
    
    const std::string too_long_string("a", map.max_key_size() + 1);
    BOOST_CHECK_THROW(map.insert(too_long_string), std::length_error);
}

/**
 * serialize and deserialize
 */
BOOST_AUTO_TEST_CASE(test_serialize_desearialize) {
    // insert x values; delete some values; serialize set; deserialize in new set; check equal.
    // for deserialization, test it with and without hash compatibility.
    const std::size_t nb_values = 1000;

    std::stringstream buffer;
    buffer.exceptions(buffer.badbit | buffer.failbit | buffer.eofbit);


    tsl::array_set<char> set(0);
    
    set.insert("");
    for(std::size_t i = 1; i < nb_values + 40; i++) {
        set.insert(utils::get_key<char>(i));
    }

    for(std::size_t i = nb_values; i < nb_values + 40; i++) {
        set.erase(utils::get_key<char>(i));
    }
    BOOST_CHECK_EQUAL(set.size(), nb_values);

    set.serialize(serializer(buffer));




    auto set_deserialized = decltype(set)::deserialize(deserializer(buffer), true);
    BOOST_CHECK(set == set_deserialized);

    buffer.seekg(0);
    set_deserialized = decltype(set)::deserialize(deserializer(buffer), false);
    BOOST_CHECK(set_deserialized == set);
}

BOOST_AUTO_TEST_SUITE_END()
