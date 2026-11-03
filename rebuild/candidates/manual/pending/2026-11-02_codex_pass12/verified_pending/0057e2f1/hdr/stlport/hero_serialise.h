#ifndef FABLE_STLPORT_HERO_SERIALISE_H
#define FABLE_STLPORT_HERO_SERIALISE_H
#include "stlport/fable_containers.h"
#include "engine/CCharString.h"
// Original STLport4.6 _iterator.h back insertion adaptor and copy declaration.
namespace std {
struct output_iterator_tag {};
template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator {typedef Category iterator_category;typedef T value_type;typedef Distance difference_type;typedef Pointer pointer;typedef Reference reference;};
template<class Container> class back_insert_iterator:public iterator<output_iterator_tag,void,void,void,void> {
protected:Container* container;
public:typedef Container container_type;
 explicit back_insert_iterator(Container& x):container(&x){}
 back_insert_iterator& operator=(const typename Container::value_type& value){container->push_back(value);return *this;}
 back_insert_iterator& operator*(){return *this;}
 back_insert_iterator& operator++(){return *this;}
 back_insert_iterator operator++(int){return *this;}
};
template<class Container> inline back_insert_iterator<Container> __fastcall back_inserter(Container& x){return back_insert_iterator<Container>(x);}
template<class Input,class Output> Output __fastcall copy(Input,Input,Output);
template<> vector<CCharString>::~vector();
}
#endif
