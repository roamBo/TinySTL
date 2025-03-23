//This file include some general utilities, such as move, forward, swap, pair etc.
#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"

//compiler will integrate mystl namespace with all files.


namespace mystl{
//perfect forwarding: Design generic libraries and scenarios that require efficient parameter passing and avoid copy operation.

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
        start form c++11, use right valued reference and temporary object to avoid performance loss by calling constructon function.

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
}

#endif