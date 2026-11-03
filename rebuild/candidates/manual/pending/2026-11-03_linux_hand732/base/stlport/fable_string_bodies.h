#ifndef FABLE_STLPORT_STRING_BODIES_H
#define FABLE_STLPORT_STRING_BODIES_H
// Real STLport 4.6.2 string member bodies (_string.h and _string.c),
// with allocator and iterator configuration resolved for the retail build.
// Original notices are retained in fable_string.h.
#include "fable_string.h"
#include <stdio.h>
namespace std {
#pragma auto_inline(off)
template<class T,class A> void _String_base<T,A>::_M_throw_length_error() const { puts("basic_string");abort(); }
template<class T,class A> void _String_base<T,A>::_M_throw_out_of_range() const { puts("basic_string");abort(); }
template<class C,class T,class A>
basic_string<C,T,A>& basic_string<C,T,A>::assign(const C* first,const C* last) {
    ptrdiff_t n=last-first;
    if(size_type(n)<=size()) {
        T::copy(this->_M_start,first,n);
        if(this->_M_start+n!=this->_M_finish) {
            T::assign(this->_M_start[n],_M_null());this->_M_finish=this->_M_start+n;
        }
    } else {
        T::copy(this->_M_start,first,size());append(first+size(),last);
    }
    return *this;
}
template<class C,class T,class A> template<class I>
basic_string<C,T,A>& basic_string<C,T,A>::_M_assign_dispatch(I first,I last,const __false_type&) {
    C* cur=this->_M_start;
    while(first!=last && cur!=this->_M_finish) { T::assign(*cur,*first);++first;++cur; }
    if(first==last) {
        if(cur!=this->_M_finish) {T::assign(*cur,_M_null());this->_M_finish=cur;}
    } else append(first,last);
    return *this;
}
template<class C,class T,class A> template<class I>
basic_string<C,T,A>& basic_string<C,T,A>::append(I first,I last,const forward_iterator_tag&) {
    if(first!=last) {
        const size_type oldSize=size();difference_type n=last-first;
        if(size_type(n)>max_size() || oldSize>max_size()-size_type(n)) this->_M_throw_length_error();
        if(oldSize+n>capacity()) {
            const size_type len=oldSize+(max)(oldSize,size_type(n))+1;
            C* newStart=this->_M_end_of_storage.allocate(len);
            C* newFinish=newStart;
            newFinish=uninitialized_copy(this->_M_start,this->_M_finish,newStart);
            newFinish=uninitialized_copy(first,last,newFinish);_M_construct_null(newFinish);
            _Destroy(this->_M_start,this->_M_finish+1);this->_M_deallocate_block();
            this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+len;
        } else {
            I next=first;++next;uninitialized_copy(next,last,this->_M_finish+1);
            _M_construct_null(this->_M_finish+n);T::assign(*end(),*first);this->_M_finish+=n;
        }
    }
    return *this;
}
template<class C,class T,class A> void basic_string<C,T,A>::reserve(size_type n) {
    if(n>=capacity()) {
        if(n>max_size()) this->_M_throw_length_error();
        size_type len=n+1;
        C* newStart=this->_M_end_of_storage.allocate(len);
        C* newFinish=uninitialized_copy(this->_M_start,this->_M_finish,newStart);
        _M_construct_null(newFinish);_Destroy(this->_M_start,this->_M_finish+1);this->_M_deallocate_block();
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+len;
    }
}
template<class C,class T,class A> basic_string<C,T,A> operator+(const C* s,const basic_string<C,T,A>& y) {
    typedef basic_string<C,T,A> Str;typedef typename Str::_Reserve_t Reserve;
    size_t n=T::length(s);Str result(Reserve(),n+y.size());result.append(s,s+n);result.append(y);return result;
}
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>& x,const C* s) {
    typedef basic_string<C,T,A> Str;typedef typename Str::_Reserve_t Reserve;
    size_t n=T::length(s);Str result(Reserve(),x.size()+n,x.get_allocator());result.append(x);result.append(s,s+n);return result;
}
#pragma auto_inline(on)
}
#endif
