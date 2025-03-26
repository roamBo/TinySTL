//This file include some general utilities, such as move, forward, swap, pair etc.
#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"

//compiler will integrate mystl namespace with all files.


namespace mystl{
/*
    perfect forwarding: 
        Design generic libraries and scenarios that require efficient parameter passing and avoid copy operation.
        The purpose is to preserve the type and value category(lvalue or rvalue) of parameters when writing generic functions
*/
    //move
    //function:unconditionally convert any type of parameter to a right valued reference 


    /*
        &&:convert the original type to a right-valueed reference(& left-valued reference)(in template have reference fold, odd is left valued referecne, even is right)
        in template. That will be considered as left or right valued reference
        such as:
        template<class T>
        void func(T &&a){}

        int a = 123;
        func(a); interpret as left valued reference, void func<int&>(int&), (reference fold) func(&& &a) == func(&a)
        func(123); interpret as right valued reference void func<int>(int&&) 
    */

    /*
        remove_reference: (<type_traits>)
        extract reference type(remove reference)
        func(a); get int
        func(123); gei int

        It uses the template specialization
        template <typename T>        // not specialization
        struct remove_reference {
                using type = T;
        };

        template <typename T>         // specialization 1
        struct remove_reference<T &> {
                using type = T;
        };

        template <typename T>         // specialization 2
        struct remove_reference<T &&> {
                using type = T;
        };


    */

    template<class T>
    //typename to tell compiler it is a variable type (form template).
    /*
        noexcept: (start c++11)
        Declare that the function does not throw exceptions and supports exception safety for movement operations.
        So compiler can use this information to optimize and avoid generating additional code to handle exceptions.
    */

    
    /*
        move semantics:(It is a performance optimization, start c++11)
        assign values to class objects containing pointer members through 'move' than 'deep copy'.(like steal)
        use right valued reference and temporary object to avoid performance loss by calling constructon function.

        c++ class have :default constructor, destructor, copy constructor, copy assignment operator
                        move constructor, move assignment operator
    */
    typename std::remove_reference<T>::type&& move(T &&arg) noexcept{

        /*
            this is a anonymous object or temporary object. Such objects are not referenced by any variables after creation, and their lifecycle is limited to the expression they are in.
            and anonymous objects are always used to return values , as construction parameters for other objects or move construction like `vector<int> vec = vector<int>(10);`
        */

        /*
            type conversion:
            syntax: xxxx_cast<new_type_name>(expression);
                static_cast: conversion that can be determined at compile time, but it can not conversion const, volitale or __unaligned attributes.
                dynamic_cast: only used for pointer or reference conversion between base class and derived class in class inheritance structure, and with virtual functions in the base class, type checking will be performed at runtime.
                const_cast: remove the constancy of pointers or references.
                reinterpret_cast: It is used for converting between types of pointers or references, or between pointers and integers, and is a simple copy and reinterpret of comparsion bits.

        */

        //std::remove_reference<T>::type&&: ensure that the converted type is n non-reference right value reference (to avoid reference stacking)
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }


    //forward
    //function: pass the type and value category of the parameter unchanged to other functions.

    //c++11: value include left value(lvalue), right value(rvalue) and pure right value(xvalue, can be move but can not be copy)

    //left value(std::remove_reference<T>::type& ensure arg is a left value).
    template<class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept{
        return static_cast<T&&>(arg);
    }

    //right value
    template<class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept{
        //static_assert(constant bool expression, prompt string): tools for error checking during compilation
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }

    //swap(use move constructor, the premise is this type have move constructor and compatible without move constructor)

    template<class Tp>
    void swap(Tp& lhs, Tp& rhs){
        auto temp(mystl::move(lhs));//or: T temp = mystl::move(lhs); If do not have move, this operation will call copy construct function
        lhs = mystl::move(rhs);
        rhs = mystl::move(temp);
    }

    template<class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2){
        for(;first1 != last1;first1++, (void)first2++){
            mystl::swap(*first1, *first2);
        }
        return first2;
    }

    template<class Tp, size_t N>
    void swap(Tp (&a)[N], Tp (&b)[N]){
        mystl::swap_range(a, a+N, b);
    }
    /*
        summary 25.3.23
            learn use move constructor to deal deep copy(decrease performance) from copy constructor
                two concepts: move semantic, right value reference
            
            learn perfect forwarding

            learn type convert

            learn three function: move, forward, swap(use move constructor)

            learn template meta program and template parameter list at the previous file "type_traits.h"
    */

    //--------------------------------------------------------------------------------------
    //pair
    template<class Tp1, class Tp2>
    struct pair{
        typedef Tp1 first_type;
        typedef Tp2 second_type;

        first_type first;
        second_type second;
        /*
            SFINAE(a content of template meta program): substitution failure is not a error
            when a template parameter fails to replace with an explicitly specifiied type, discard the spacialization from the overload set instead of causing compilation failure.
        */
        /*
            std::enable_if<condition, void>, if condition is ture, the third parameter is void. If not, touch SFINAE, this constructor is discarded from the overload set.
            condition: if Other1 and Other2 have default constructor is ture;

            constexpr: Allow the consturction of pair objects at compile time(requiring the default constructors of members first and second to also be constexpr).

            Initialization: Call the default constructors of first and second to initialize.

            the third parameter uses unnamed template parameter conbine SFINAE, and avoid misuse.
            The standard library widely uses unnamed template parameters  to implement conditional constraints.
        */

        //default constructor(use default template parameter, when instance it can be left out)
        template<class Other1 = Tp1, class Other2 = Tp2, 
            typename = typename std::enable_if<
            std::is_default_constructible<Other1>::value && 
            std::is_default_constructible<Other2>::value, void>::type> 
        constexpr pair() : first(), second(){}

        /*
            constexpr and const: the value or function return value of constexpr is determined in compile time, and the value of const is determined in run time.
        */
        //the third parameter use non-type template parameter
        //implicit consturctible for this type
        template<class U1 = Tp1, class U2 = Tp2, 
                typename std::enable_if<
                std::is_copy_constructible<U1>::value && 
                std::is_copy_constructible<U2>::value && 
                std::is_convertible<const U1&, Tp1>::value && 
                std::is_convertible<const U2&, Tp2>::value, int>::type = 0>
        constexpr pair(const Tp1 &a, const Tp2 &b):first(a), second(b){}

        /*
            summary 25.3.24
            learn default template parameter

            learn SFINAE

            learn constexpr

            learn unnamed template parameter and non-type template parameter
        */

        //explicit  constructible for this type
        template<class U1 = Tp1, class U2 = Tp2, 
            typename std::enable_if<
            std::is_copy_constructible<U1>::value &&
            std::is_copy_constructible<u2>::value &&
            (!std::is_convertible<const U1&, Tp1>::value ||
             !std::is_convertible<const U2&, Tp2>::value), int>::type = 0>
        explicit constexpr pair(const Tp1 &a, const Tp2 &b) : first(a), second(b){}
        /*explicit: prohibit implicit conversions between class objects and prohibit implicit calls to copy constructors.*/
        
        
        /*default: Remind the compiler. Although I wrote constructors with parameters also asked the compiler to generate a default constructor.*/
        pair(const pair& rhs) = default;//copy constructor
        pair(pair&& rhs) = default;//move constructor

        //implicit constructible for other type 
        template<class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Tp1, Other1>::value &&
            std::is_constructible<Tp2, Other2>::value &&
            std::is_convertible<Other1&&, Tp1>::value &&
            std::is_convertible<Other2&&, Tp2>::value, int>::type = 0>
        constexpr pair(Other1&& a, Other2&& b): 
                first(mystl::forward<Other1>(a)),
                second(mystl::forward<Other2>(b)){}
        /*
            summary 25.3.25
            1.explicit
            2.default
            3.is_copy_constructible<> from <type_trails>
        */
    };
}

#endif