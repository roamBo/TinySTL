//This file is used to collect type traits.
#ifndef MYTINYSTL_TYPE_TRAILS_H_
#define MYTINYSTL_TYPE_TRAILS_H_

//use standard library 
#include<type_traits>

namespace mystl{
    //meta function(always show as template class or function)
    template<class T, T v>
    struct m_integral_constant{
        //constent expression 
        static constexpr T value = v;
    };

    //"using" to define new alias of template m_integral_constant
    template<bool b>
    using m_bool_constant = m_integral_constant<bool ,b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;


//type traits (template meta programming, TMP, also is a powerful Programming Paradigm)

    //is_pair

    //forward declaration is a uncomplete declaration, just tell there have a template struct
    //forward declaration begin
    template<class T1, class T2>
    struct pair;
    //forward declaration end

    //to determine is it correct
    template<class T>
    struct is_pair : mystl::m_false_type {};

    template<class T1, class T2>
    struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};
}

#endif 