#include "string_operations.h"
#include <fcntl.h>

bool StartsWith(std::string_view string, std::string_view text){
    if(text.size() > string.size()){return false;}
    for(size_t i = 0; i < text.size(); i++){
        if(text[i] != string[i]){return false;}
    }
    return true;
}

bool EndsWith(std::string_view string, std::string_view text){
    if(text.size() > string.size()){return false;}
    size_t n = string.size();
    size_t m = text.size();
    for(size_t i = n - m; i < n; i++){
        if(text[i - n + m] != string[i]){return false;}
    }
    return true;
}

std::string_view StripPrefix(std::string_view string, std::string_view prefix){
    if(StartsWith(string, prefix)){
        return string.substr(prefix.size()); //string.begin() + prefix.size(), string.end();
    }
    return string;
}


std::string_view StripSuffix(std::string_view string, std::string_view suffix){
    if(EndsWith(string, suffix))
    {
        return string.substr(0, string.size() - suffix.size());
    }
    return string;
}

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n){
    if(n > s.size()){return s;}
    return s.substr(pos, n);
}

std::string_view StripAsciiWhitespace(std::string_view s){
    size_t i = 0;size_t j = s.size()-1;
    size_t n = s.size( );
    while(i < n&&std::isspace(s[i]))
    {
        i++;
    }
    if(i != n){
        while(j > i && std::isspace(s[j])){
            j--;
        }
    }
    //s.erase(0, i);
    //s.erase(j + 1 - i, n - j - 1 - i);
    return s.substr(i, j - i  + 1);
}


std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim){
    std::vector<std::string_view> tokens;
    size_t i = 0; size_t matches = 0; size_t n = text.size(); size_t mn = delim.size();
    if(n == 0){return {""};}
    size_t to_reserve = 0; size_t last_match = 0;
    while(i < n){
        if(delim[matches] == text[i]){
            matches++;
        }
        else{
            matches = 0;
        }
        if(matches == mn){
            to_reserve++;
            matches = 0;
        }
        i++;
    }
    tokens.reserve(to_reserve + 2);
    i = 0;
    last_match = 0;matches = 0;
    while(i < n){
        if(delim[matches] == text[i]){
                matches++;
            if(matches == mn){
                 size_t ln = i - mn - last_match + 1;
                if(ln  > n){ln = 0;}
 
                tokens.emplace_back(text.substr(last_match, ln));
                last_match = i + 1;
                matches = 0;
            }
        }
        else{
            matches = 0;
        }
        
        i++;
    }
    if(matches == mn){
        tokens.emplace_back("");
    }
    else{
        tokens.emplace_back(text.substr(last_match, i- last_match));
    }
    return tokens;
}

std::string ReadN(const std::string& filename, size_t n){
    int fd = open(filename.data(), O_RDONLY);
    if(fd == -1){return "";}
    std::string s; s.reserve(n );s.resize(n);
    int num = read(fd, s.data(), n);
    if(num == -1){return "";}
    s.resize(num);
    s[num] = '\0';
    close(fd);
    return s;
}

std::string AddSlash(std::string_view path){
    std::string s; s.reserve(path.size() + 2);
    s = path;
    auto n = path.size();
    if(n != 0 && path[n-1] == '/'){return s;}
    s+="/";
    return s;
};

std::string_view RemoveSlash(std::string_view path){
    
    auto n = path.size();
    if(n!=0 && path[n-1] == '/' && n > 1){path.remove_suffix(1);}
    return path;
}

std::string_view Dirname(std::string_view path){
    int n = path.size();
    int i = n - 1;
    while(i >= 0&&path[i] != '/'){i --;}
    int j = i - 1;
    while(j >= 0&&path[j] != '/'){j--;}
    if(i == j && i == 0){return "";}
    if(j == -1){return path.substr(j + 1, i - j);}
    return path.substr(0, i);
}

std::string_view Basename(std::string_view path){
    size_t n = path.size();
    int i = n - 1;
    while(i >= 0 && path[i] != '/'){
        i--;
    }
    return path.substr(i + 1, n - i - 1);
}

std::string CollapseSlashes(std::string_view pat){
    std::string path(pat);
    size_t n = path.size();
    bool prev_slash = false;
    size_t i = 0;
    size_t offset = 0;
    while(i < n){
        if(path[i] == '/' && !prev_slash){
            prev_slash = true;
            path[i - offset] = path[i];
        }
        else if(path[i] == '/' && prev_slash)
        {
            offset++;
        }
        else{
            prev_slash = false;
            path[i - offset] = path[i];
        }
        i++;
    }
    path.resize(n - offset);
    return path;
}


std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter){
    size_t n = strings.size(); 
    size_t p = (n==0?0:n-1);
    size_t size =p*delimiter.size();
    std::string buf;
    for(const auto& st: strings){
        size+=st.size();
    }
    buf.reserve(size + 2);
    for(size_t i = 0; i < p; i++){
        buf+=strings[i];
        buf+=delimiter;
    }
    if(p != 0){
        buf+=strings[n -1];
    }
    return  buf;

}


/*
Your implementations here
*/