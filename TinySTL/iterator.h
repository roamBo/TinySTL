#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_
#include<cstddef>
#include"type_traits.h"

namespace mystl{
    //five types of  iterator 
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirectional_iterator_tag{};
    
    //iterator template
    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator{
        typedef Category iterator_category;
        typedef T value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Distance difference_type;
    };

    //iterator traits
    template<class T>
    class has_iterator_cat{
    private:
        class two{public: char a; char b;};
        //compile time type check technique, belong to template meta program and SFINAE
        template<class U> static two test(...);// ... can substitute any type and the lowest priority
        template<class U> static char test(typename U::iterator_category* = 0);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };
}
#endif