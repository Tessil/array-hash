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
#ifndef TSL_UTILS_H
#define TSL_UTILS_H

#include <cctype>
#include <functional>
#include <locale>
#include <memory>
#include <ostream>
#include <string>

#include <boost/numeric/conversion/cast.hpp>

class move_only_test {
public:
    explicit move_only_test(std::int64_t value): m_value(new std::string(std::to_string(value))) {
    }
    
    explicit move_only_test(std::string value): m_value(new std::string(std::move(value))) {
    }
    
    move_only_test(const move_only_test&) = delete;
    move_only_test(move_only_test&&) = default;
    move_only_test& operator=(const move_only_test&) = delete;
    move_only_test& operator=(move_only_test&&) = default;
    
    friend std::ostream& operator<<(std::ostream& stream, const move_only_test& value) {
        if(value.m_value == nullptr) {
            stream << "null";
        }
        else {
            stream << *value.m_value;
        }
        
        return stream;
    }
    
    friend bool operator==(const move_only_test& lhs, const move_only_test& rhs) { 
        if(lhs.m_value == nullptr || rhs.m_value == nullptr) {
            return lhs.m_value == nullptr && rhs.m_value == nullptr;
        }
        else {
            return *lhs.m_value == *rhs.m_value; 
        }
    }
    
    friend bool operator!=(const move_only_test& lhs, const move_only_test& rhs) { 
        return !(lhs == rhs); 
    }
    
    friend bool operator<(const move_only_test& lhs, const move_only_test& rhs) {
        if(lhs.m_value == nullptr && rhs.m_value == nullptr) {
            return false;
        }
        else if(lhs.m_value == nullptr) {
            return true;
        }
        else if(rhs.m_value == nullptr) {
            return false;
        }
        else {
            return *lhs.m_value < *rhs.m_value; 
        }
    }
    
    std::string value() const {
        return *m_value;
    }
    
private:    
    std::unique_ptr<std::string> m_value;
};



namespace std {
    template<>
    struct hash<move_only_test> {
        std::size_t operator()(const move_only_test& val) const {
            return std::hash<std::string>()(val.value());
        }
    };
}




template<class CharT>
struct ci_str_hash {
    std::size_t operator()(const CharT* key, std::size_t key_size) const {
        std::size_t hash = 0;
        for (unsigned int i = 0; i < key_size; ++i) {
            hash = std::toupper(key[i]) + (hash * 31);
        }
        
        return hash;
    }
}; 

template<class CharT>
struct ci_str_equal {
    bool operator()(const CharT* key_lhs, std::size_t key_size_lhs,
                    const CharT* key_rhs, std::size_t key_size_rhs) const
    {
        if(key_size_lhs != key_size_rhs) {
            return false;
        }
        
        for(std::size_t i = 0; i < key_size_lhs; i++) {
            if(std::toupper(key_lhs[i]) != std::toupper(key_rhs[i])) {
                return false;
            }
        }
        
        return true;
    }
};






class utils {
public:
    template<typename CharT>
    static std::basic_string<CharT> get_key(size_t counter);
    
    template<typename T>
    static T get_value(size_t counter);
    
    template<typename AMap>
    static AMap get_filled_map(size_t nb_elements);
};



template<>
inline std::basic_string<char> utils::get_key<char>(size_t counter) {
    return "Key " + std::to_string(counter);
}

template<>
inline std::basic_string<wchar_t> utils::get_key<wchar_t>(size_t counter) {
    return L"Key " + std::to_wstring(counter);
}

template<>
inline std::basic_string<char16_t> utils::get_key<char16_t>(size_t counter) {
    std::string num = std::to_string(counter);
    std::u16string key = u"Key ";
    
    // We know that num is an ASCII string
    for(char c: num) {
        key += static_cast<char16_t>(c);
    }
    
    return key;
}

template<>
inline std::basic_string<char32_t> utils::get_key<char32_t>(size_t counter) {
    std::string num = std::to_string(counter);
    std::u32string key = U"Key ";
    
    // We know that num is an ASCII string
    for(char c: num) {
        key += static_cast<char32_t>(c);
    }
    
    return key;
}



template<>
inline int64_t utils::get_value<int64_t>(size_t counter) {
    return boost::numeric_cast<int64_t>(counter*2);
}

template<>
inline std::string utils::get_value<std::string>(size_t counter) {
    return "Value " + std::to_string(counter);
}

template<>
inline move_only_test utils::get_value<move_only_test>(size_t counter) {
    return move_only_test(boost::numeric_cast<int64_t>(counter*2));
}


template<typename AMap>
inline AMap utils::get_filled_map(size_t nb_elements) {
    using char_tt = typename AMap::char_type; 
    using value_tt = typename AMap::mapped_type;
    
    AMap map(nb_elements);
    for(size_t i = 0; i < nb_elements; i++) {
        map.insert(utils::get_key<char_tt>(i), utils::get_value<value_tt>(i));
    }
    
    return map;
}

/**
 * serializer helpers to test serialize(...) and deserialize(...) functions
 */
class serializer {
public:
    serializer(std::stringstream& ostream): m_ostream(ostream) {
    }
    
    template<class T>
    void operator()(const T& val) const {
        serialize_impl(val);
    }
    
    void operator()(const char* data, std::uint64_t size) const {
        m_ostream.write(data, size);
    }
    
private:
    void serialize_impl(const std::string& val) const {
        serialize_impl(boost::numeric_cast<std::uint64_t>(val.size()));
        m_ostream.write(val.data(), val.size());
    }

    void serialize_impl(const move_only_test& val) const {
        serialize_impl(val.value());
    }

    template<class T, 
             typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void serialize_impl(const T& val) const {
        m_ostream.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    
private:
    std::stringstream& m_ostream;
};

class deserializer {
public:
    deserializer(std::stringstream& istream): m_istream(istream) {
    }
    
    template<class T>
    T operator()() const {
        return deserialize_impl<T>();
    }
    
    void operator()(char* data_out, std::uint64_t size) const {
        m_istream.read(data_out, boost::numeric_cast<std::size_t>(size));
    }

private:
    template<class T, 
             typename std::enable_if<std::is_same<std::string, T>::value>::type* = nullptr>
    T deserialize_impl() const {
        const std::size_t str_size = boost::numeric_cast<std::size_t>(deserialize_impl<std::uint64_t>());
        
        // TODO std::string::data() return a const pointer pre-C++17. Avoid the inefficient double allocation.
        std::vector<char> chars(str_size);
        m_istream.read(chars.data(), str_size);
        
        return std::string(chars.data(), chars.size());
    }

    template<class T, 
             typename std::enable_if<std::is_same<move_only_test, T>::value>::type* = nullptr>
    move_only_test deserialize_impl() const {
        return move_only_test(deserialize_impl<std::string>());
    }

    template<class T, 
             typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    T deserialize_impl() const {
        T val;
        m_istream.read(reinterpret_cast<char*>(&val), sizeof(val));

        return val;
    }
    
private:
    std::stringstream& m_istream;
};

#endif
