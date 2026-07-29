#pragma once
#include <cstring>
#include <string_view>
#include <type_traits>
#include <vector>
#include <string>
#include <unistd.h>
#include<iostream>

bool StartsWith(std::string_view string, std::string_view text); // — проверяет, что строка `string` начинается с `text`.

bool EndsWith(std::string_view string, std::string_view text); //` — проверяет, что строка `string` оканчивается на `text`.

std::string_view StripPrefix(std::string_view string, std::string_view prefix); //` — возвращает `string` с убранным `prefix`,
//если `string` не начинается на `prefix`, возвращает `string`.

std::string_view StripSuffix(std::string_view string, std::string_view suffix);//` — тоже самое, но с суффиксом.

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n = std::string_view::npos);//` — тоже самое, что и `s.substr(pos, n)`,
//но если `n` больше `s.size()`, то возвращается `s`.

std::string_view StripAsciiWhitespace(std::string_view);

//` — `strip` строки, удаляем все символы с обоих концов
//вида [isspace](https://en.cppreference.com/w/cpp/string/byte/isspace).

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim);//` — делаем `split` строки по `delim`. Подумайте,
//прежде чем копипастить из уже имеющейся задачи. Обойдитесь одной аллокацией памяти.
std::string ReadN(const std::string& filename, size_t n);//` — открывает файл и читает `n` байт из filename. Используйте Linux
//Syscalls `open`, `read`, `close`. Если открыть или прочитать файл нельзя, возвращает пустую строчку.

std::string AddSlash(std::string_view path);
//` — добавляет к `path` файловой системы символ `/`, если его не было.

std::string_view RemoveSlash(std::string_view path);///` — убирает `/` из `path`, если это не сам путь `/` и путь заканчивается
//на `/`.

std::string_view Dirname(std::string_view path);// — известно, что `path` — корректный путь до файла без слеша на конце,
//верните папку, в которой этот файл лежит без слеша на конце, если это не корень.

std::string_view Basename(std::string_view path);// — известно, что `path` — корректный путь до файла, верните его название.

std::string CollapseSlashes(std::string_view);//` — известно, что `path` — корректный путь, но `/` могут повторяться,
//надо убрать все повторения.

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter);
//` — склеить все строки в одну через
//`delimiter`. Обойдитесь одной аллокацией памяти.

template<typename T,bool is_convertible = !std::is_convertible_v<T, const char*>, bool is_unsigned = std::is_unsigned_v<T>, bool ar = std::is_arithmetic_v<T>>
struct CalculateSize;

template<typename T>
struct CalculateSize<T, true, true, true>{
    size_t operator()(std::remove_const_t<T> val){
        //std::cout<<__PRETTY_FUNCTION__<<std::endl;
        size_t ans = 0;
        if(val == 0){return 1;}
        while(val){
            val/=10;
            ans++;
        }
        return ans;
    }
};

template<typename T>
struct CalculateSize<T, true, false, true>{
    size_t operator()(std::remove_const_t<T> val){
        size_t ans = 0;
        std::make_unsigned_t<std::remove_const_t<T>> vall = val;
        if(val < 0){ans++; vall = -static_cast<std::make_unsigned_t<std::remove_const_t<T>>>(val);}
        if(vall == 0){ans = 1;return 1;}
        while(vall){
            vall/=10;
            ans++;
        }
        return ans;
    }
};

template<typename T>
struct CalculateSize<T, true, false, false>{
    size_t operator()(const T& val){
       return val.size();
    }
};

template<typename T>
struct CalculateSize<T, false, false, false>{
    size_t operator()(const char* val){
       return strlen(val);
    }
};



template<typename T,bool is_ariphmetic = !std::is_convertible_v<T, const char*>, bool is_unsigned = std::is_unsigned_v<T>, bool ar = std::is_arithmetic_v<T>>
struct ToStr;

template<typename T>
struct ToStr<T, true, true, true>{
    void operator()(std::remove_const_t<T> val, std::string& s, size_t size, size_t& pos){
        size_t bsize = size;
        if(val == 0){s[pos++] = '0';return;}
        while(val){
            s[pos+(--size)] = ((val%10) + '0');
            val/=10;
        }
        pos+=bsize;
    }
};


template<typename T>
struct ToStr<T, true, false, true>{ 
    void operator()(std::remove_const_t<T> val, std::string& s, size_t size, size_t& pos){
        if(val == 0){s[pos++] = '0';return;}
        std::make_unsigned_t<std::remove_const_t<T>> vall = val;
        if(val < 0){s[pos++] = '-';vall = -static_cast<std::make_unsigned_t<std::remove_const_t<T>>>(val);size--;}
        size_t bsize = size;
        while(vall){
            s[pos+(--size)] = ((vall%10) + '0');
            vall/=10;
        }
        pos+=bsize;
    }
};

template<typename T>
struct ToStr<T, true, false, false>{
    void operator()(const T& val, std::string& s, size_t size, size_t& pos)
    {
        size_t n = size; size_t i = 0;
        while(i<n){
            s[pos++] = val[i];
            i++;
        }
    }
};

template<typename T>
struct ToStr<T, false, false, false>
{
    void operator()(const char* val, std::string& s, size_t, size_t& pos){
        while(*val != '\0'){
            s[pos++] = *val;
            val++;
        }
    }
};


template<typename ...Args>
std::string StrCat(const Args& ...args){
    std::string s;
    if(sizeof...(args) > 0){
        size_t Sizes[sizeof...(args)];size_t i = 0, size = 0;
        ((Sizes[i++] = CalculateSize<std::remove_reference_t<decltype(args)>>()(args)), ...);
        for(auto p: Sizes){size += p;}
        s.resize(size);size_t pos = 0;i =0;

    (ToStr<std::decay_t<std::remove_reference_t<decltype(args)>>>()(args, s, Sizes[i++], pos), ...);
    }
    return s;
}



/*` — склеить все аргументы в один в их строковом представлении.
Должны поддерживаться числа (`int, long, long long` и их `unsigned` версии), также все строковые
типы (`std::string, std::string_view, const char*`). Аргументов в `StrCat` не больше пяти.
Придумайте как это сделать за одну аллокацию памяти.
*/
