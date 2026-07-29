#pragma once

#include <type_traits>
#include <utility>

// Me think, why waste time write lot code, when few code do trick.
template<typename T, bool is_first, bool is_class, bool is_final>
struct MakeClass;

template<typename T, bool is_first, bool is_final>
struct MakeClass<T, is_first, false, is_final>{
    inline T& Get(){return val;}
    inline const T& Get() const{return val;}
    T val;
};
template<typename T, bool is_first>
struct MakeClass<T, is_first, true, true>{
    inline T& Get(){return val;}
    inline const T& Get() const{return val;}
    T val;
};
template<typename T, bool is_first>
struct MakeClass<T, is_first, true, false>:T{
    inline T& Get(){return static_cast<T&>(*this);}
    inline const T& Get() const{return static_cast<const T&>(*this);}
};

template <typename F, typename S>
class CompressedPair:MakeClass<F, true, std::is_class_v<F>, std::is_final_v<F>>,
MakeClass<S, false, std::is_class_v<S>, std::is_final_v<S>>
{
    using First = MakeClass<F, true, std::is_class_v<F>, std::is_final_v<F>>;
    using Second = MakeClass<S, false, std::is_class_v<S>, std::is_final_v<S>>;
public:

    CompressedPair():First{}, Second{}{}
    template<typename U, typename V>
    CompressedPair(U&& first, V&& second) : First{std::forward<U>(first)}, Second{std::forward<V>(second)} {
    }

    F& GetFirst() {
        return First::Get();
    }
    const F& GetFirst() const{
        return First::Get();
    }

    S& GetSecond(){
        return Second::Get();
    }

    const S& GetSecond() const {
        return Second::Get();
    };
};

