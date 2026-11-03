#ifndef FABLE_STLPORT_SET_H
#define FABLE_STLPORT_SET_H
#include "stlport/fable_map.h"
namespace std {
template<class T>struct _Identity:public unary_function<T,T>{const T& operator()(const T& value)const{return value;}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,Compare,A> _Rep_type;_Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator iterator;
#ifdef FABLE_RETAIL_CTHINGMANAGER_TEXT
    iterator find(const K& value)const{return _M_t.find(value);}
    iterator end()const{return _M_t.end();}
#endif
    set():_M_t(Compare(),A()){}
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
    size_t erase(const K& value){return _M_t.erase(value);}
};
}
#endif
