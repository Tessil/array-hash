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
    explicit move_only_test(int64_t value) : m_value(new int64_t(value)) {
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
            return lhs.m_value == nullptr && lhs.m_value == nullptr;
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
    
    int64_t value() const {
        return *m_value;
    }
private:    
    std::unique_ptr<int64_t> m_value;
};



namespace std {
    template<>
    struct hash<move_only_test> {
        std::size_t operator()(const move_only_test& val) const {
            return std::hash<int64_t>()(val.value());
        }
    };
};




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
class ci_char_traits : public std::char_traits<CharT> {
public:
    static bool eq(CharT lhs, CharT rhs) {
        return std::toupper(lhs) == std::toupper(rhs);
    }
    
    static bool lt(CharT lhs, CharT rhs) {
        return std::toupper(lhs) < std::toupper(rhs);
    }
    
    static int compare(const CharT* s1, const CharT* s2, std::size_t count) {
        for(std::size_t i=0; i < count; i++) {
            if(std::toupper(s1[i]) < std::toupper(s2[i])) {
                return -1;
            }
            
            if(std::toupper(s1[i]) > std::toupper(s2[i])) {
                return 1;
            }
        }
        
        return 0;
    }
    
    static const CharT* find(const CharT* s, std::size_t count, const CharT& ch) {
        for(std::size_t i=0; i < count; i++) {
            if(std::toupper(s[i]) == std::toupper(ch)) {
                return s + i;
            }
        }
        
        return nullptr;
    }
};






class utils {
public:
    template<typename CharT, typename Traits = std::char_traits<CharT>>
    static std::basic_string<CharT, Traits> get_key(size_t counter);
    
    template<typename T>
    static T get_value(size_t counter);
    
    template<typename AMap>
    static AMap get_filled_map(size_t nb_elements);
};



template<>
inline std::basic_string<char, std::char_traits<char>> utils::get_key<char, std::char_traits<char>>(size_t counter) {
    return "Key " + std::to_string(counter);
}

template<>
inline std::basic_string<wchar_t, std::char_traits<wchar_t>> utils::get_key<wchar_t, std::char_traits<wchar_t>>(size_t counter) {
    return L"Key " + std::to_wstring(counter);
}

template<>
inline std::basic_string<char16_t, std::char_traits<char16_t>> utils::get_key<char16_t, std::char_traits<char16_t>>(size_t counter) {
    std::string num = std::to_string(counter);
    std::u16string key = u"Key ";
    
    // We know that num is an ASCII string
    for(char c: num) {
        key += static_cast<char16_t>(c);
    }
    
    return key;
}

template<>
inline std::basic_string<char32_t, std::char_traits<char32_t>> utils::get_key<char32_t, std::char_traits<char32_t>>(size_t counter) {
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
    using traits_tt = typename AMap::traits_type; 
    using value_tt = typename AMap::mapped_type;
    
    AMap map(nb_elements);
    for(size_t i = 0; i < nb_elements; i++) {
        map.insert(utils::get_key<char_tt, traits_tt>(i), utils::get_value<value_tt>(i));
    }
    
    return map;
}

#endif
