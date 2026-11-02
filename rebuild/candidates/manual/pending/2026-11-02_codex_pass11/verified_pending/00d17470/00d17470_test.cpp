#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
typedef unsigned long FableU64;typedef long FableI64;
#else
typedef unsigned __int64 FableU64;typedef __int64 FableI64;
#endif
#define FABLE_STATIC_ASSERT(x)
static int fixture_rand();
#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING

#ifdef FABLE_RETAIL_HERO_LOGBOOK
#ifndef FABLE_ENGINE_CWIDESTRINGDATA_H
#define FABLE_ENGINE_CWIDESTRINGDATA_H
#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H





#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_RETAIL_SHOP
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_RELATIVE_PATH
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
#ifdef FABLE_RETAIL_HERO_LOGBOOK

template<class C,class I> class __char_traits_base {
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> {};
#else
template<class C> class char_traits;
#endif
template<> class char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& a,const char& b) { a=b; }
    static size_t length(const char* p) { return strlen(p); }
    static int compare(const char* a,const char* b,size_t n) { return memcmp(a,b,n); }
    static char* copy(char* a,const char* b,size_t n) { return (char*)memmove(a,b,n); }
};
inline char* uninitialized_copy(const char* first,const char* last,char* result) {
    return (char*)__copy_trivial(first,last,result);
}
template<class T,class A> class _String_base {
public:
    typedef A allocator_type;
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    size_t max_size() const { return size_t(-1)/sizeof(T)-1; }
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
    void _M_allocate_block(size_t n) {
        if(n<=max_size()+1 && n>0) {
            _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
        } else _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); }
    _String_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { }
    _String_base(const A& a,size_t n):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { _M_allocate_block(n); }
    ~_String_base() { _M_deallocate_block(); }
};
struct _String_reserve_t { };
template<class C,class Traits=char_traits<C>,class A=allocator<C> >
class basic_string:protected _String_base<C,A> {
    typedef basic_string<C,Traits,A> _Self;
    typedef _String_base<C,A> _Base;
public:
    typedef C* iterator;
    typedef const C* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef A allocator_type;
    typedef _String_reserve_t _Reserve_t;
    static C _M_null() { return C(); }
    enum { npos=-1 };
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string();
    basic_string(const C* s,const A& a=A());
    basic_string(const _Self& s);
    basic_string(_Reserve_t,size_t n,const A& a=A()):_Base(a,n+1) { _M_terminate_string(); }
    #ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_initialize_dispatch(f,l,__false_type()); }
#else
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_range_initialize(f,l); }
#endif
    ~basic_string() { _Destroy(this->_M_start,this->_M_finish+1); }
    size_type size() const { return this->_M_finish-this->_M_start; }
    size_type capacity() const { return this->_M_end_of_storage._M_data-this->_M_start-1; }
    size_type max_size() const { return _Base::max_size(); }
    const C* c_str() const { return this->_M_start; }
    const C* data() const { return this->_M_start; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    C& operator[](size_type n) { return *(this->_M_start+n); }
    const C& operator[](size_type n) const { return *(this->_M_start+n); }
    _Self& operator=(const _Self& s) { if(&s!=this) assign(s._M_start,s._M_finish); return *this; }
    _Self& operator=(const C* s) { return assign(s,s+Traits::length(s)); }
    _Self& assign(const C* first,const C* last);
    template<class I> _Self& assign(I first,I last) { return _M_assign_dispatch(first,last,__false_type()); }
    _Self& append(const _Self& s) { return append(s._M_start,s._M_finish); }
    _Self& append(const C* s) { return append(s,s+Traits::length(s)); }
    template<class I> _Self& append(I f,I l) { return _M_append_dispatch(f,l,__false_type()); }
    _Self& operator+=(const _Self& s) { return append(s); }
    _Self& operator+=(const C* s) { return append(s); }
    _Self& operator+=(C c) { push_back(c);return *this; }
    _Self substr(size_type pos=0,size_type n=npos) const {
        if(pos>size()) this->_M_throw_out_of_range();
        return _Self(this->_M_start+pos,this->_M_start+pos+(min)(n,size()-pos));
    }
    void reserve(size_type n=0);
    void push_back(C c) {
        if(this->_M_finish+1==this->_M_end_of_storage._M_data) reserve(size()+(max)(size(),size_type(1)));
        _M_construct_null(this->_M_finish+1);Traits::assign(*this->_M_finish,c);++this->_M_finish;
    }
private:
    void _M_construct_null(C* p) { *p=C(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    template<class I> void _M_range_initialize(I f,I l,const forward_iterator_tag&) {
        difference_type n=l-f;this->_M_allocate_block(n+1);
        this->_M_finish=uninitialized_copy(f,l,this->_M_start);_M_terminate_string();
    }
#ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> void _M_initialize_dispatch(I f,I l,const __false_type&) { _M_range_initialize(f,l); }
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,random_access_iterator_tag()); }
#else
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,forward_iterator_tag()); }
#endif
    template<class I> _Self& _M_assign_dispatch(I f,I l,const __false_type&);
    template<class I> _Self& append(I f,I l,const forward_iterator_tag&);
    template<class I> _Self& _M_append_dispatch(I f,I l,const __false_type&) { return append(f,l,forward_iterator_tag()); }
};
template<class C,class T,class A> basic_string<C,T,A>::basic_string():_Base(allocator_type()) {
    this->_M_start=this->_M_end_of_storage.allocate(8);this->_M_finish=this->_M_start;
    this->_M_end_of_storage._M_data=this->_M_start+8;_M_terminate_string();
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const C* s,const A& a):_Base(a) {
    _M_range_initialize(s,s+T::length(s));
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const basic_string<C,T,A>& s):_Base(s.get_allocator()) {
    _M_range_initialize(s._M_start,s._M_finish);
}
template<class C,class T,class A> inline bool __fastcall operator==(const basic_string<C,T,A>& x,const basic_string<C,T,A>& y) {
    return x.size()==y.size()&&T::compare(x.data(),y.data(),x.size())==0;
}
template<class C,class T,class A> inline bool operator==(const basic_string<C,T,A>& s,const C* c) {
    size_t n=T::length(c);return s.size()==n&&T::compare(s.data(),c,n)==0;
}
template<class C,class T,class A> inline bool operator!=(const basic_string<C,T,A>& s,const C* c) { return !(s==c); }
template<class C,class T,class A> basic_string<C,T,A> operator+(const C*,const basic_string<C,T,A>&);
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>&,const C*);
typedef basic_string<char> string;
}
#endif



class CWideStringData {
protected:
    std::basic_string<unsigned short> Data;
public:
    int NoRefs;
    bool operator==(const CWideStringData& other)const { const std::basic_string<unsigned short>& data=Data;return data==other.Data; }
};
#endif

#endif
class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    CCharString ToCharString() const;
#endif
#ifdef FABLE_RETAIL_CENTERED_TEXT
    operator const unsigned short*() const;
    const unsigned short* GetAsCharArray() const;
    bool operator!=(const unsigned short*) const;
    const CWideString& operator=(const CWideString&);
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    bool operator==(const CWideString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString&)const;
#endif
    ~CWideString();
protected:
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    static int NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;
};
#endif
#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; } Channels;
        unsigned int IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Channels.R=r; Channels.G=g; Channels.B=b; Channels.A=a; }
    CRGBColour(const CRGBColour& colour) { Channels.R=colour.Channels.R; Channels.G=colour.Channels.G; Channels.B=colour.Channels.B; Channels.A=colour.Channels.A; }
    unsigned char GetR() const { return Channels.R; }
    unsigned char GetG() const { return Channels.G; }
    unsigned char GetB() const { return Channels.B; }
    unsigned char GetA() const { return Channels.A; }
};
#endif


#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_STLPORT_HTTP
#include <stdlib.h>
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H









































#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CCPPointerInfo {
public:
    unsigned int RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned int refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned int GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#endif 

#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H


#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#ifdef FABLE_RETAIL_CAMERA_DRAW
inline float __fastcall GFSqr(float n){return n*n;}
#endif
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned int GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned int*)&n &= 0x7fffffffUL;
#else
 *(unsigned int*)&n &= 0x7fffffffU;
#endif
 return n;
}
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
float __fastcall GFFastOneOverSqrt(float n){
#ifdef _MSC_VER
 typedef unsigned int word_type;
#else
 typedef unsigned int word_type;
#endif
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
#endif

#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class C2DCoordI { public: int X,Y;C2DCoordI(){}C2DCoordI(int x,int y):X(x),Y(y){} };
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
#ifdef FABLE_RETAIL_VECTOR_SPLINE_CTOR
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    C3DVector(float x,float y,float z){Initialise(x,y,z);}
#else
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#endif
#if defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((int)X,(int)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>

#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
protected:
    T* PData;
    unsigned int StringLength;
    unsigned int DataLength : 31;
    bool UseFastExtend : 1;
#if defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH)
    static int __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_RELATIVE_PATH
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    unsigned int GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    int NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    int GetLength() const;
    int FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(int length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();

protected:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        int length);

private:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, int length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        int length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];


#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, int length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
#ifdef FABLE_RETAIL_SUBTITLE
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned int GetCRC() const; 
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    int GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { int offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_RELATIVE_PATH
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

class CScriptThing;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other)
{
    ShareData(other.Data,other.Info);
    return *this;
}
#pragma auto_inline(on)
#ifndef FABLE_ENGINE_CSCRIPTTHING_H
#define FABLE_ENGINE_CSCRIPTTHING_H















#ifdef FABLE_RETAIL_NSCRIPT
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};

#endif 

#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H







class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef FableU64 FableNScriptUInt64;                           
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority;
#endif
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef FableU64 FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority { };
#endif
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif 

class CScriptThing : public CBaseClass {
public:
    CScriptThing();                                                      
#ifndef FABLE_RETAIL_SCRIPT_QUEST
    CScriptThing(const CScriptThing&);                                   
#endif
    virtual ~CScriptThing();                                             
    virtual const CCharString& GetName() const;                                                              
    virtual CCharString GetDefName() const;                                                                  
    virtual CCharString GetDataString() const;                                                               
    virtual void SetDataString(const CCharString&);                                                          
    virtual C3DVector GetFocalPos() const;                                                                   
    virtual const C3DVector& GetPos() const;                                                                 
    virtual C3DVector GetHomePos() const;                                                                    
    virtual CCharString GetCurrentMapName() const;                                                           
    virtual CCharString GetHomeMapName() const;                                                              
    virtual float GetAngleXY() const;                                                                        
    virtual CThing* GetPThing() const;                                                                       
    virtual FableNScriptUInt64 GetPThingUniqueID() const;                                                     
    virtual EAIStateGroupType GetCurrentStateGroupType() const;                                              
    virtual EScriptAIPriority GetCurrentScriptPriority() const;                                              
    virtual bool IsSneaking() const;                                                                         
    virtual bool IsAwareOfHero() const;                                                                      
    virtual bool MsgIsTriggeredBy(const CCharString&) const;                                                 
    virtual bool MsgIsKilledBy(const CCharString&) const;                                                    
    virtual CScriptThing MsgWhoKilledMe() const;                                                             
    virtual bool MsgIsUsedBy(const CCharString&) const;                                                      
    virtual bool MsgIsHitBy(const CCharString&) const;                                                       
    virtual bool MsgIsHitByWithFlourish(const CCharString&) const;                                           
    virtual bool MsgIsHitByWithDecapitate(const CCharString&) const;                                         
    virtual bool MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const;                         
    virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const;                           
    virtual CScriptThing MsgWhoHitMe() const;                                                                
    virtual bool MsgIsTalkedToBy(const CCharString&) const;                                                  
    virtual CScriptThing MsgWhoTalkedToMe() const;                                                           
    virtual bool MsgExpressionPerformedTo(CCharString&) const;                                               
    virtual CScriptThing MsgWhoExpressedToMe() const;                                                        
    virtual int MsgHowLongWasExpressionPerformed() const;                                                   
    virtual bool MsgIsKnockedOutBy(const CCharString&) const;                                                
    virtual CScriptThing MsgWhoKnockedMeOut() const;                                                         
    virtual bool MsgReceivedMoney(int&) const;                                                              
    virtual bool MsgIsPresentedWithItem(CCharString&) const;                                                 
    virtual bool MsgReceivedInventoryItem() const;                                                           
    virtual bool MsgPerformedSpecialAbility(EHeroAbility) const;                                             
    virtual bool MsgPerformedAnySpecialAbility() const;                                                      
    virtual bool MsgPerformedAnyAggressiveSpecialAbility() const;                                            
    virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const;                                         
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const;                       
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const;                                  
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const;                        
    virtual bool MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const;                     
    virtual bool MsgOpenedChest() const;                                                                     
    virtual bool MsgHitEnemyWithMeleeWeapon() const;                                                         
    virtual bool MsgHitFriendWithMeleeWeapon() const;                                                        
    virtual bool MsgHitEnemyWithRangedWeapon() const;                                                        
    virtual bool MsgHitFriendWithRangedWeapon() const;                                                       
    virtual bool MsgHitEnemyWithBareHands() const;                                                           
    virtual bool MsgHitFriendWithBareHands() const;                                                          
    virtual bool MsgAttackedWithMeleeWeaponWithoutHittingAnything() const;                                   
    virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const;                                     
    virtual bool MsgTalkedToAnyone() const;                                                                  
    virtual bool MsgGetThingsKilled(std::vector<unsigned int,std::allocator<unsigned int> >&) const;       
    virtual bool MsgPerformedFlourish() const;                                                               
    virtual bool MsgPerformedSuccessfulFlourish() const;                                                     
    virtual bool MsgOnMoralityChanged(int&) const;                                                          
    virtual bool MsgOnCutSceneAnimEvent(CCharString&) const;                                                 
    virtual bool MsgIsKicked() const;                                                                        
    virtual bool IsUnconscious() const;                                                                      
    virtual bool IsUsable() const;                                                                           
    virtual bool IsBeingCarriedBy(const CCharString&) const;                                                 
    virtual bool IsOpenDoor() const;                                                                         
    virtual bool IsSummonedCreature() const;                                                                 
    virtual void SetAsUsable(bool);                                                                          
    virtual void SetFriendsWithEverythingFlag(bool);                                                         
    virtual bool GetActivationTriggerStatus() const;                                                         
    virtual void SetActivationTriggerStatus(bool);                                                           
    virtual void SetToKillOnLevelUnload(bool);                                                               
    virtual void UpdateThingAttachment();                                                                    
    virtual void IncrementScriptCounter();                                                                   
    virtual void DecrementScriptCounter();                                                                   
    virtual int GetScriptCounter();                                                                         
    virtual bool IsAlive() const;                                                                            
    virtual bool IsDead() const;                                                                             
    virtual bool IsNull() const;                                                                             
    protected: virtual bool IsEqualTo(const CScriptThing&) const; public:                                    
#ifndef FABLE_RETAIL_WASP
#ifndef FABLE_RETAIL_SCRIPT_QUEST
    CScriptThing& operator=(const CScriptThing&);
#endif                        
#endif
    bool operator==(const CScriptThing&) const;                          
    bool operator!=(const CScriptThing&) const;                          
    void SetImplementation(CCountedPointer<CScriptThing>);               
#ifdef FABLE_RETAIL_SCRIPT_QUEST
private:
#endif
#ifdef FABLE_RETAIL_WASP
private:
#endif
    CCountedPointer<CScriptThing> PImp;                                  
#ifdef FABLE_RETAIL_SCRIPT_QUEST
protected:
    static C3DVector NullCoord;
#endif
#ifdef FABLE_RETAIL_WASP
public:
#endif
    static CCharString NullString;                                       
private:
    void ValidatePImp() const;                                           
};
#else 
#include <stddef.h>
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CCPPointerInfo;

#pragma pack(push, 1)
class CScriptThing {
public:
    void*           __vftable;                               
    CScriptThing*   PImp_Data;                               
    CCPPointerInfo* PImp_Info;                               
};
#pragma pack(pop)


#endif 

#endif 

#ifndef FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H
#define FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H











#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CGameScriptInterfaceBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};

#endif 


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#endif 

#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H







class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef FableU64 FableNScriptUInt64;                           
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority;
#endif
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef FableU64 FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority { };
#endif
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif 

#ifdef FABLE_RETAIL_WASP
#ifndef FABLE_ENGINE_CHEROLOGBOOK_H
#define FABLE_ENGINE_CHEROLOGBOOK_H
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING

#ifdef FABLE_RETAIL_HERO_LOGBOOK
#ifndef FABLE_ENGINE_CWIDESTRINGDATA_H
#define FABLE_ENGINE_CWIDESTRINGDATA_H
#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H





#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_RETAIL_SHOP
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_RELATIVE_PATH
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
#ifdef FABLE_RETAIL_HERO_LOGBOOK

template<class C,class I> class __char_traits_base {
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> {};
#else
template<class C> class char_traits;
#endif
template<> class char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& a,const char& b) { a=b; }
    static size_t length(const char* p) { return strlen(p); }
    static int compare(const char* a,const char* b,size_t n) { return memcmp(a,b,n); }
    static char* copy(char* a,const char* b,size_t n) { return (char*)memmove(a,b,n); }
};
inline char* uninitialized_copy(const char* first,const char* last,char* result) {
    return (char*)__copy_trivial(first,last,result);
}
template<class T,class A> class _String_base {
public:
    typedef A allocator_type;
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    size_t max_size() const { return size_t(-1)/sizeof(T)-1; }
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
    void _M_allocate_block(size_t n) {
        if(n<=max_size()+1 && n>0) {
            _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
        } else _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); }
    _String_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { }
    _String_base(const A& a,size_t n):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { _M_allocate_block(n); }
    ~_String_base() { _M_deallocate_block(); }
};
struct _String_reserve_t { };
template<class C,class Traits=char_traits<C>,class A=allocator<C> >
class basic_string:protected _String_base<C,A> {
    typedef basic_string<C,Traits,A> _Self;
    typedef _String_base<C,A> _Base;
public:
    typedef C* iterator;
    typedef const C* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef A allocator_type;
    typedef _String_reserve_t _Reserve_t;
    static C _M_null() { return C(); }
    enum { npos=-1 };
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string();
    basic_string(const C* s,const A& a=A());
    basic_string(const _Self& s);
    basic_string(_Reserve_t,size_t n,const A& a=A()):_Base(a,n+1) { _M_terminate_string(); }
    #ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_initialize_dispatch(f,l,__false_type()); }
#else
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_range_initialize(f,l); }
#endif
    ~basic_string() { _Destroy(this->_M_start,this->_M_finish+1); }
    size_type size() const { return this->_M_finish-this->_M_start; }
    size_type capacity() const { return this->_M_end_of_storage._M_data-this->_M_start-1; }
    size_type max_size() const { return _Base::max_size(); }
    const C* c_str() const { return this->_M_start; }
    const C* data() const { return this->_M_start; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    C& operator[](size_type n) { return *(this->_M_start+n); }
    const C& operator[](size_type n) const { return *(this->_M_start+n); }
    _Self& operator=(const _Self& s) { if(&s!=this) assign(s._M_start,s._M_finish); return *this; }
    _Self& operator=(const C* s) { return assign(s,s+Traits::length(s)); }
    _Self& assign(const C* first,const C* last);
    template<class I> _Self& assign(I first,I last) { return _M_assign_dispatch(first,last,__false_type()); }
    _Self& append(const _Self& s) { return append(s._M_start,s._M_finish); }
    _Self& append(const C* s) { return append(s,s+Traits::length(s)); }
    template<class I> _Self& append(I f,I l) { return _M_append_dispatch(f,l,__false_type()); }
    _Self& operator+=(const _Self& s) { return append(s); }
    _Self& operator+=(const C* s) { return append(s); }
    _Self& operator+=(C c) { push_back(c);return *this; }
    _Self substr(size_type pos=0,size_type n=npos) const {
        if(pos>size()) this->_M_throw_out_of_range();
        return _Self(this->_M_start+pos,this->_M_start+pos+(min)(n,size()-pos));
    }
    void reserve(size_type n=0);
    void push_back(C c) {
        if(this->_M_finish+1==this->_M_end_of_storage._M_data) reserve(size()+(max)(size(),size_type(1)));
        _M_construct_null(this->_M_finish+1);Traits::assign(*this->_M_finish,c);++this->_M_finish;
    }
private:
    void _M_construct_null(C* p) { *p=C(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    template<class I> void _M_range_initialize(I f,I l,const forward_iterator_tag&) {
        difference_type n=l-f;this->_M_allocate_block(n+1);
        this->_M_finish=uninitialized_copy(f,l,this->_M_start);_M_terminate_string();
    }
#ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> void _M_initialize_dispatch(I f,I l,const __false_type&) { _M_range_initialize(f,l); }
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,random_access_iterator_tag()); }
#else
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,forward_iterator_tag()); }
#endif
    template<class I> _Self& _M_assign_dispatch(I f,I l,const __false_type&);
    template<class I> _Self& append(I f,I l,const forward_iterator_tag&);
    template<class I> _Self& _M_append_dispatch(I f,I l,const __false_type&) { return append(f,l,forward_iterator_tag()); }
};
template<class C,class T,class A> basic_string<C,T,A>::basic_string():_Base(allocator_type()) {
    this->_M_start=this->_M_end_of_storage.allocate(8);this->_M_finish=this->_M_start;
    this->_M_end_of_storage._M_data=this->_M_start+8;_M_terminate_string();
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const C* s,const A& a):_Base(a) {
    _M_range_initialize(s,s+T::length(s));
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const basic_string<C,T,A>& s):_Base(s.get_allocator()) {
    _M_range_initialize(s._M_start,s._M_finish);
}
template<class C,class T,class A> inline bool __fastcall operator==(const basic_string<C,T,A>& x,const basic_string<C,T,A>& y) {
    return x.size()==y.size()&&T::compare(x.data(),y.data(),x.size())==0;
}
template<class C,class T,class A> inline bool operator==(const basic_string<C,T,A>& s,const C* c) {
    size_t n=T::length(c);return s.size()==n&&T::compare(s.data(),c,n)==0;
}
template<class C,class T,class A> inline bool operator!=(const basic_string<C,T,A>& s,const C* c) { return !(s==c); }
template<class C,class T,class A> basic_string<C,T,A> operator+(const C*,const basic_string<C,T,A>&);
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>&,const C*);
typedef basic_string<char> string;
}
#endif



class CWideStringData {
protected:
    std::basic_string<unsigned short> Data;
public:
    int NoRefs;
    bool operator==(const CWideStringData& other)const { const std::basic_string<unsigned short>& data=Data;return data==other.Data; }
};
#endif

#endif
class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    CCharString ToCharString() const;
#endif
#ifdef FABLE_RETAIL_CENTERED_TEXT
    operator const unsigned short*() const;
    const unsigned short* GetAsCharArray() const;
    bool operator!=(const unsigned short*) const;
    const CWideString& operator=(const CWideString&);
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    bool operator==(const CWideString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString&)const;
#endif
    ~CWideString();
protected:
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    static int NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;
};
#endif

#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_RETAIL_SHOP
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_RELATIVE_PATH
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif



namespace std {
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
#endif
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
public:
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
#else
    _Rb_tree_base(const A&);
#endif
#ifdef FABLE_RETAIL_SCRIPT_QUEST
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
#else
    ~_Rb_tree_base();
#endif
#endif
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#else
    _Rb_tree(const Compare&,const A&);
#endif
#ifdef FABLE_RETAIL_SCRIPT_QUEST
    ~_Rb_tree(){clear();}
#else
    ~_Rb_tree();
#endif
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
private:
    void _M_erase(_Rb_tree_node<V>*);
public:
    void clear() {
        if(_M_node_count!=0) {
            _M_erase(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_parent));
            this->_M_header._M_data->_M_left=this->_M_header._M_data;
            this->_M_header._M_data->_M_parent=0;
            this->_M_header._M_data->_M_right=this->_M_header._M_data;
            _M_node_count=0;
        }
    }
    iterator find(const K&);
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
protected:
    typedef _Rb_tree_node<V>* _Link_type;
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
public:
    iterator find(const K& key){return iterator(_M_find(key));}
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#ifdef FABLE_RETAIL_WASP
    ~map();
#endif
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    size_t size() const { return _M_t.size(); }
#endif
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
#endif



class CHeroLogBook {
public:
    class CQuestDesc;
    class CHeroLogBookEntry {
    public:
        enum ECategory {CATEGORY_QUEST,CATEGORY_STORY,CATEGORY_TUTORIAL,CATEGORY_BASICS,CATEGORY_OBJECTS,CATEGORY_TOWNS,CATEGORY_HERO,CATEGORY_COMBAT};
        CHeroLogBookEntry(const CWideString& name_tag,const CWideString& abbreviated_name,const CWideString& text_tag):Name(name_tag),AbbreviatedName(abbreviated_name),Content(text_tag){}
        CHeroLogBookEntry(const CHeroLogBookEntry& entry):Name(entry.Name),AbbreviatedName(entry.AbbreviatedName),Content(entry.Content){}
        const CWideString& GetName()const{return Name;}
    private:
        CWideString Name,AbbreviatedName,Content;
    };
    void AddEntry(const CWideString&,const CWideString&,const CWideString&,CHeroLogBookEntry::ECategory,bool);
private:
    unsigned char GivenQuests[12]; 
    int QuestBeingViewed;
    std::map<CHeroLogBookEntry::ECategory,std::vector<CHeroLogBookEntry> > LogEntries;
};
#endif

#endif

class CGameScriptInterfaceBase : public CBaseClassNonCopyable {
public:
    virtual ~CGameScriptInterfaceBase();                                 
    virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned int) const = 0; 
    virtual void TraceMessage(const CCharString&) const = 0;                                             
    virtual void Validate() const = 0;                                                                   
    virtual void SetDebugCameraType(int) const = 0;                                                     
    virtual void DeactivateBoastUi() const = 0;                                                          
    virtual bool IsXbox() const = 0;                                                                     
    virtual void NewScriptFrame() const = 0;                                                             
    virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) = 0; 
    virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const = 0;                            
    virtual bool IsActiveThreadTerminating() const = 0;                                                  
    virtual bool IsLevelLoaded(const CCharString&) const = 0;                                            
    virtual bool IsRegionLoaded(const CCharString&) const = 0;                                           
    virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const = 0;                               
    virtual bool IsRegionDefLoaded(const CCharString&) const = 0;                                        
    virtual const CCharString& GetRegionName() const = 0;                                                
    virtual bool MsgIsLevelLoaded(const CCharString&) const = 0;                                         
    virtual bool MsgIsLevelUnloaded(const CCharString&) const = 0;                                       
    virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;       
    virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;     
    virtual bool MsgIsRegionLoaded(const CCharString&) const = 0;                                        
    virtual bool MsgIsRegionUnloaded(const CCharString&) const = 0;                                      
    virtual bool MsgOnRegionLoaded(CCharString&) const = 0;                                              
    virtual bool MsgOnRegionUnloaded(CCharString&) const = 0;                                            
    virtual bool MsgOnRegionPreunload(CCharString&) const = 0;                                           
    virtual bool MsgOnQuestCompleted(const CCharString&) const = 0;                                      
    virtual bool MsgOnAnyQuestCompleted(CCharString&) const = 0;                                         
    virtual bool MsgOnQuestFailed(const CCharString&) const = 0;                                         
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const = 0;                     
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const = 0;                        
    virtual bool MsgOnQuestAccepted(CCharString&) const = 0;                                             
    virtual bool MsgOnFeatAccepted(int&) const = 0;                                                     
    virtual bool MsgIsBoastMade(int) const = 0;                                                         
    virtual bool MsgOnBoastMade(int&, CCharString&) const = 0;                                          
    virtual bool MsgOnBoastsMade(std::vector<std::pair<int,CCharString>,std::allocator<std::pair<int,CCharString> > >&) const = 0; 
    virtual void RemoveBoastMessage() const = 0;                                                         
    virtual bool IsQuestStartScreenActive(CCharString&) const = 0;                                       
    virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const = 0;                                   
    virtual bool MsgOnLeavingExperienceSpendingScreen() const = 0;                                       
    virtual int MsgIsQuestionAnsweredYesOrNo() const = 0;                                               
    virtual bool MsgIsGameInfoClickedPast() const = 0;                                                   
    virtual bool MsgIsTutorialClickedPast() const = 0;                                                   
    virtual bool MsgIsActionModeButtonPressed() const = 0;                                               
    virtual bool MsgOnExpressionPerformed(CCharString&) const = 0;                                       
    virtual bool MsgIsCutSceneSkipped() const = 0;                                                       
    virtual void RemoveAllCutSceneSkippedMessages() const = 0;                                           
    virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const = 0;                  
    virtual bool MsgOnHeroUsedTeleporter(CCharString&) const = 0;                                        
    virtual bool MsgOnHeroUsedGuildSeal() const = 0;                                                     
    virtual bool MsgOnGameSavedManually() const = 0;                                                     
    virtual bool MsgOnHeroSlept(bool&) const = 0;                                                        
    virtual bool MsgOnHeroFiredRangedWeapon() const = 0;                                                 
    virtual bool MsgOnHeroCastSpell(EHeroAbility&) const = 0;                                            
    virtual bool MsgOnHeroPickedPocket(CScriptThing&) const = 0;                                         
    virtual bool MsgOnHeroPickedLock(CScriptThing&) const = 0;                                           
    virtual bool MsgOnFishingGameFinished(CScriptThing&) const = 0;                                      
    virtual bool MsgOnTavernGameFinished(const CScriptThing&) const = 0;                                 
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const = 0;                                 
    virtual bool MsgOnChestOpeningCancelled() const = 0;                                                 
    virtual void DontPopulateNextLoadedRegion() const = 0;                                               
    virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const = 0;             
    virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const = 0; 
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const = 0;                                       
    virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const = 0;                            
    virtual void PostAddScriptedEntities() const = 0;                                                    
    virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const = 0;                                 
    virtual bool IsPlayerHoldingLockTargetButton() const = 0;                                            
    virtual bool IsPlayerHoldingFireRangedWeaponButton() const = 0;                                      
    virtual bool IsPlayerHoldingFirstPersonTargetingButton() const = 0;                                  
    virtual bool IsHeroInProjectileWeaponMode() const = 0;                                               
    virtual CScriptThing& GetHero() = 0;                                                                 
    virtual CScriptThing GetHeroTargetedThing() const = 0;                                               
    virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const = 0;       
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const = 0;                           
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const = 0;                     
    virtual int GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual int GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const = 0;    
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const = 0;   
    virtual int GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual int GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const = 0;       
    virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const = 0;      
    virtual CScriptThing GetThingWithUID(FableU64) const = 0;                                    
    virtual int GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual int GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool IsThingWithThisUIDAlive(FableU64) const = 0;                                    
    virtual int RegisterTimer() = 0;                                                                    
    virtual void DeregisterTimer(int) = 0;                                                              
    virtual void SetTimer(int, int) = 0;                                                               
    virtual int GetTimer(int) const = 0;                                                               
    virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const = 0; 
    virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const = 0; 
    virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; 
    virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const = 0;            
    virtual void SetCreatureCreationDelayFrames(int) = 0;                                               
    virtual void ResetCreatureCreationDelayFrames() = 0;                                                 
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const = 0; 
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const = 0; 
    virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; 
    virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const = 0; 
    virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const = 0; 
    virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const = 0; 
    virtual CScriptThing CreateExperienceOrb(const C3DVector&, int) const = 0;                          
    virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const = 0; 
    virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const = 0; 
    virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const = 0;            
    virtual void ClearAllRumbles() const = 0;                                                            
    virtual void RemoveThing(const CScriptThing&, bool, bool) const = 0;                                 
    virtual void ShowOnScreenMessage(const CCharString&, float) const = 0;                               
    virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const = 0; 
    virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const = 0; 
    virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const = 0;              
    virtual void AddScreenTitleMessage(const CCharString&, float, bool) const = 0;                       
    virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const = 0; 
    virtual void DisplayGameInfo(const CCharString&) const = 0;                                          
    virtual void DisplayGameInfoText(const CWideString&) const = 0;                                      
    virtual bool IsSafeToDisplayGameInfo() const = 0;                                                    
    virtual bool DisplayTutorial(ETutorialCategory) const = 0;                                           
    virtual bool IsTutorialSystemEnabled() const = 0;                                                    
    virtual void GiveHeroWeapon(const CCharString&, bool) const = 0;                                     
    virtual void GiveHeroObject(const CCharString&, int, bool) const = 0;                               
    virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const = 0;                             
    virtual void GiveHeroItem(const CScriptThing&) const = 0;                                            
    virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const = 0;                        
    virtual void TakeObjectFromHero(const CCharString&) const = 0;                                       
    virtual void GiveHeroGold(int) const = 0;                                                           
    virtual int GetHeroGold() const = 0;                                                                
    virtual void GiveHeroExperience(int) const = 0;                                                     
    virtual void SetHeroAbleToGainExperience(bool) const = 0;                                            
    virtual void SheatheHeroWeapons() const = 0;                                                         
    virtual void SetHeroWillAsUsable(bool) const = 0;                                                    
    virtual void SetHeroWeaponsAsUsable(bool) const = 0;                                                 
    virtual void SetWeaponOutCrimeEnabled(bool) const = 0;                                               
    virtual void SetGuardsIgnoreCrimes(bool) const = 0;                                                  
    virtual void RemoveAllHeroWeapons() const = 0;                                                       
    virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const = 0;                        
    virtual void ConfiscateAllHeroItems() const = 0;                                                     
    virtual void ConfiscateAllHeroWeapons() const = 0;                                                   
    virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const = 0;                            
    virtual void ReturnAllConfiscatedItemsToHero() const = 0;                                            
    virtual bool GiveHeroTutorial(ETutorialCategory) const = 0;                                          
    virtual void MakeHeroCarryItemInHand(const CCharString&) const = 0;                                  
    virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const = 0;                     
    virtual void AddTattooToHero(const CCharString&) const = 0;                                          
    virtual void GiveHeroAbility(EHeroAbility, bool) const = 0;                                          
    virtual bool IsPlayerZTargetingThing(const CScriptThing&) const = 0;                                 
    virtual bool IsPlayerCreatureBlocking() const = 0;                                                   
    virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const = 0;                          
    virtual int GetPlayerCreatureCombatMultiplier() const = 0;                                          
    virtual int GetPlayerCreatureCombatMultiplierRunningNumHits() const = 0;                            
    virtual void ResetPlayerCreatureCombatMultiplier() const = 0;                                        
    virtual bool IsPlayerCreatureFlourishEnabled() const = 0;                                            
    virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const = 0;                             
    virtual void ResetPlayerCreatureOnlyTarget() const = 0;                                              
    virtual void RespawnHero(bool) const = 0;                                                            
    virtual void GiveHeroMorality(float) const = 0;                                                      
    virtual float GetHeroMorality() const = 0;                                                           
    virtual EMorality GetHeroMoralityCategory() const = 0;                                               
    virtual void ChangeHeroMoralityDueToTheft() const = 0;                                               
    virtual void ChangeHeroMoralityDueToPicklock() const = 0;                                            
    virtual void GiveHeroRenownPoints(int) const = 0;                                                   
    virtual int GetHeroRenownLevel() const = 0;                                                         
    virtual bool IsHeroRenownLevelFull() const = 0;                                                      
    virtual void IncreaseHeroRenownLevel() const = 0;                                                    
    virtual int GetHeroStrengthLevel() const = 0;                                                       
    virtual int GetHeroSkillLevel() const = 0;                                                          
    virtual int GetHeroWillLevel() const = 0;                                                           
    virtual int GetHeroStatLevel(EHeroTrainableStatType) const = 0;                                     
    virtual int GetHeroStatMax(EHeroTrainableStatType) const = 0;                                       
    virtual void SetHeroAge(float) const = 0;                                                            
    virtual float GetHeroAge() const = 0;                                                                
    virtual void SetHeroAsTeenager(bool) const = 0;                                                      
    virtual void SetHeroAsApprentice(bool) const = 0;                                                    
    virtual float GetDistanceHeroCanBeHeardFrom() const = 0;                                             
    virtual int GetHeroRoughExperienceLevel() const = 0;                                                
    virtual int GetHeroExperienceAvailableToSpend() const = 0;                                          
    virtual float GetHeroFatness() const = 0;                                                            
    virtual float GetHeroScariness() const = 0;                                                          
    virtual float GetHeroAttractiveness() const = 0;                                                     
    virtual float GetHeroWillEnergyLevel() const = 0;                                                    
    virtual void SetHeroWillEnergyLevel(float) const = 0;                                                
    virtual void SetHeroWillEnergyAsAbleToRefill(bool) const = 0;                                        
    virtual int GetNumberOfItemsOfTypeInInventory(const CCharString&) const = 0;                        
    virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const = 0;          
    virtual bool IsHeroHandLampLit() const = 0;                                                          
    virtual void SetHeroHandLampAsLit(bool) const = 0;                                                   
    virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const = 0;               
    virtual bool IsHeroNaked() const = 0;                                                                
    virtual void RemoveHeroClothing() const = 0;                                                         
    virtual void SetHeroAsWearing(const CCharString&) const = 0;                                         
    virtual void ChangeHeroHairstyle(const CCharString&) const = 0;                                      
    virtual void RemoveHeroHairstyle() const = 0;                                                        
    virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const = 0;                        
    virtual bool IsPlayerCarryingItemOfType(const CCharString&) const = 0;                               
    virtual bool IsPlayerWieldingWeapon() const = 0;                                                     
    virtual bool IsEntityWieldingWeapon(const CScriptThing&) const = 0;                                  
    virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const = 0;                             
    virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const = 0;                            
    virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const = 0;                            
    virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const = 0;                           
    virtual bool IsEntityAbleToAttack(const CScriptThing&) const = 0;                                    
    virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const = 0;                     
    virtual void ApplyHeroPenaltyForDeath() const = 0;                                                   
    virtual void GiveHeroTitle(const CCharString&) const = 0;                                            
    virtual EHeroTitle GetHeroTitle() const = 0;                                                         
    virtual void EntitySetAsMarryable(const CScriptThing&, bool) const = 0;                              
    virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const = 0;          
    virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const = 0;                         
    virtual bool IsEntityMarriedToHero(const CScriptThing&) const = 0;                                   
    virtual bool IsEntityMarriable(const CScriptThing&) const = 0;                                       
    virtual bool GetHeroHasMarried() const = 0;                                                          
    virtual bool GetHeroHasCurrentMarriage() const = 0;                                                  
    virtual bool GetHeroHasDivorcedMarriage() const = 0;                                                 
    virtual bool GetHeroHasChildren() const = 0;                                                         
    virtual bool GetHeroHasMurderedWife() const = 0;                                                     
    virtual bool IsHeroChild() const = 0;                                                                
    virtual void CancelHeroTeleportEffects() const = 0;                                                  
    virtual int GetNumberOfTimesHeroHasHadSex() const = 0;                                              
    virtual void SetNumberOfTimesHeroHasHadSex(int) const = 0;                                          
    virtual void SetHeroAsHavingHadSex(bool) const = 0;                                                  
    virtual void SetHeroAsHavingHadGaySex(bool) const = 0;                                               
    virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const = 0; 
    virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const = 0;           
    virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const = 0; 
    virtual void GiveHeroExpression(const CCharString&, int, bool) const = 0;                           
    virtual bool HeroHasExpression(const CCharString&) const = 0;                                        
    virtual bool IsHeroPerformingExpression(const CCharString&) const = 0;                               
    virtual bool IsDPadButtonHeldForExpression(const CCharString&) const = 0;                            
    virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const = 0;     
    virtual void EntityStopFollowing(const CScriptThing&) const = 0;                                     
    virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool IsEntityFollowingHero(const CScriptThing&) const = 0;                                   
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const = 0;    
    virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const = 0;   
    virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const = 0; 
    virtual void TeleportAllFollowersToHeroPosition() const = 0;                                         
    virtual void EntityTeleportToHeroPosition(const CScriptThing&) const = 0;                            
    virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const = 0; 
    virtual float GetWaterHeightAtPosition(const C3DVector&) const = 0;                                  
    virtual bool IsFishingSpotEnabled(const CScriptThing&) const = 0;                                    
    virtual void DisableFishingSpot(const CScriptThing&) const = 0;                                      
    virtual void UpdateFishWeight(const CScriptThing&) const = 0;                                        
    virtual float GetBestFishWeight(const CCharString&) const = 0;                                       
    virtual void HeroGoFishing(bool) const = 0;                                                          
    virtual int GetHeroFishingLevel() const = 0;                                                        
    virtual void HeroGoDigging() const = 0;                                                              
    virtual void HeroStopDigging() const = 0;                                                            
    virtual void HeroPlayOracleMinigame() const = 0;                                                     
    virtual bool IsHeroPlayingOracleMinigame() const = 0;                                                
    virtual bool HasHeroWonOracleMinigame() const = 0;                                                   
    virtual void HeroPlayFireheartMinigame() const = 0;                                                  
    virtual void HeroQuitFireheartMinigame() const = 0;                                                  
    virtual bool HasHeroForceQuitFireheartMinigame() const = 0;                                          
    virtual float GetHeroHealth() const = 0;                                                             
    virtual float GetHeroHealthMax() const = 0;                                                          
    virtual float GetHeroHealthPercentage() const = 0;                                                   
    virtual int GetHeroWillEnergy() const = 0;                                                          
    virtual int GetHeroWillEnergyMax() const = 0;                                                       
    virtual int GetHeroWillEnergyPercentage() const = 0;                                                
    virtual void ChangeHeroHealthBy(float, bool, bool) const = 0;                                        
    virtual float GetHealth(const CScriptThing&) const = 0;                                              
    virtual void ModifyThingHealth(const CScriptThing&, float, bool) const = 0;                          
    virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const = 0;                         
    virtual void SetThingAsKilled(const CScriptThing&) const = 0;                                        
    virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned int) const = 0;                 
    virtual void TellHeroQuestObjectiveCompleted(unsigned int) const = 0;                               
    virtual void TellHeroQuestObjectiveFailed(unsigned int) const = 0;                                  
    virtual void AddQuestRegion(const CCharString&, const CCharString&) const = 0;                       
    virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const = 0;                 
    virtual bool IsHeroOnQuest() const = 0;                                                              
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const = 0; 
    virtual void SetGuildMasterMessages(bool) const = 0;                                                 
    virtual void ActivateQuest(const CCharString&) const = 0;                                            
    virtual void ActivateMultipleQuests(const CArray<CCharString>&) const = 0;                           
    virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const = 0;                     
    virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const = 0;    
    virtual void DeactivateQuest(const CCharString&, unsigned int) const = 0;                           
    virtual void DeactivateQuestLater(const CCharString&, unsigned int) const = 0;                      
    virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const = 0;                   
    virtual void PrepareQuestsWhenFinalQuestIsCompleted() const = 0;                                     
    virtual bool IsQuestActive(const CCharString&) const = 0;                                            
    virtual bool IsQuestRegistered(const CCharString&) const = 0;                                        
    virtual bool IsQuestCompleted(const CCharString&) const = 0;                                         
    virtual bool IsQuestFailed(const CCharString&) const = 0;                                            
    virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const = 0;                    
    virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const = 0;         
    virtual void SetQuestAsPersistent(const CCharString&, bool) const = 0;                               
    virtual const CCharString& GetExclusiveQuestScriptName() const = 0;                                  
    virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const = 0;             
    virtual void RemoveQuestCardFromGuild(const CCharString&) const = 0;                                 
    virtual void RemoveQuestCardFromHero(const CCharString&) const = 0;                                  
    virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const = 0;      
    virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const = 0; 
    virtual void SetQuestCardGoldReward(const CCharString&, int) const = 0;                             
    virtual void SetQuestCardRenownReward(const CCharString&, int) const = 0;                           
    virtual void RemoveAllAvailableQuestCardsFromGuild() const = 0;                                      
    virtual void FailAllActiveQuests() const = 0;                                                        
    virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; 
    virtual void AddFeatCard(int, const CCharString&, const CCharString&) const = 0;                    
    virtual void AddBoast(const CCharString&, int, int, int, bool, const CCharString&, int) const = 0; 
    virtual void RemoveBoast(int, const CCharString&) const = 0;                                        
    virtual void SetBoastAsFailed(int, const CCharString&) const = 0;                                   
    virtual void SetBoastAsCompleted(int, const CCharString&) const = 0;                                
    virtual bool IsBoastTaken(int, const CCharString&) const = 0;                                       
#ifdef FABLE_RETAIL_WASP
    virtual void AddLogBookEntry(const CWideString&,const CWideString&,const CWideString&,CHeroLogBook::CHeroLogBookEntry::ECategory) const = 0;
#else
    virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, int) const = 0;
#endif 
    virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const = 0;                      
    virtual void KickOffDeathScreen() const = 0;                                                         
    virtual void KickOffCreditsScreen(CCharString&) const = 0;                                           
    virtual void SetPreferredQuickAccessItem(const CCharString&, int, int) const = 0;                  
    virtual CCharString GetDeathRecoveryMarkerName() const = 0;                                          
    virtual void SetDeathRecoveryMarkerName(const CCharString&) const = 0;                               
    virtual void ResetDeathRecoveryMarkerNameToDefault() const = 0;                                      
    virtual bool IsToFailQuestOnDeath() const = 0;                                                       
    virtual void SetWhetherToFailQuestOnDeath(bool) const = 0;                                           
    virtual void ResetWhetherToFailQuestOnDeathToDefault() const = 0;                                    
    virtual CScriptThing GetMostRecentValidUsedTarget() const = 0;                                       
    virtual CCharString GetMostRecentValidUsedTargetName() const = 0;                                    
    virtual void DisplayQuestInfo(bool) const = 0;                                                       
    virtual void SetQuestInfoName(const char*) const = 0;                                                
    virtual void SetQuestInfoText(const char*) const = 0;                                                
    virtual int AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const = 0; 
    virtual int AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const = 0; 
    virtual int AddQuestInfoTimer(const CTimer&, const CCharString&, float) const = 0;                  
    virtual int AddQuestInfoCounter(const CCounter&, const CCharString&, float) const = 0;              
    virtual int AddQuestInfoCounter(const CCharString&, int, float) const = 0;                         
    virtual int AddQuestInfoCounterList(const CCharString&, int, float) const = 0;                     
    virtual int AddQuestInfoTick(const CCharString&, bool, float) const = 0;                            
    virtual int AddQuestInfoTick(EGameAction, bool, float) const = 0;                                   
    virtual void UpdateQuestInfoBar(int, float, float, float) const = 0;                                
    virtual void ChangeQuestInfoBarColour(int, const CRGBColour&, const CRGBColour&) const = 0;         
    virtual void UpdateQuestInfoTimer(int, float) const = 0;                                            
    virtual void UpdateQuestInfoCounter(int, int, int) const = 0;                                     
    virtual void UpdateQuestInfoCounterList(int, int, int) const = 0;                                 
    virtual void UpdateQuestInfoTick(int, bool) const = 0;                                              
    virtual void RemoveQuestInfoElement(int) const = 0;                                                 
    virtual void RemoveAllQuestInfoElements() const = 0;                                                 
    virtual void DisplayTime(bool) const = 0;                                                            
    virtual void DisplayMoneyBag(bool) const = 0;                                                        
    virtual void DisplayMiniGameInfo(bool, EMiniGameType) const = 0;                                     
    virtual void UpdateMiniGameInfoBar(float) const = 0;                                                 
    virtual bool IsEntityPickPocketable(const CScriptThing&) const = 0;                                  
    virtual bool IsEntityPickLockable(const CScriptThing&) const = 0;                                    
    virtual bool IsEntityStealable(const CScriptThing&) const = 0;                                       
    virtual void EntitySetAsPickPocketed(const CScriptThing&) const = 0;                                 
    virtual void EntitySetAsPickLocked(const CScriptThing&) const = 0;                                   
    virtual void EntitySetAsStolen(const CScriptThing&) const = 0;                                       
    virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const = 0;                    
    virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const = 0;             
    virtual void MiniMapRemoveMarker(const CScriptThing&) const = 0;                                     
    virtual void MiniMapRemoveAllMarkers() const = 0;                                                    
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const = 0; 
    virtual void MiniMapSetAsEnabled(bool) const = 0;                                                    
    virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const = 0;                        
    virtual void SetHUDEnabled(bool) const = 0;                                                          
    virtual bool TextEntryExists(const CCharString&) const = 0;                                          
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const = 0; 
    virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const = 0;                
    virtual void ClearThingHasInformation(const CScriptThing&) const = 0;                                
    virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntityResetAsPureAINarrator(const CScriptThing&) const = 0;                             
    virtual int AddNewConversation(const CScriptThing&, bool, bool) const = 0;                          
    virtual void AddPersonToConversation(int, const CScriptThing&) const = 0;                           
    virtual void AddLineToConversation(int, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const = 0; 
    virtual void RemoveConversation(int, bool) const = 0;                                               
    virtual bool IsConversationActive(int) const = 0;                                                   
    virtual void PlayAVIMovie(const CCharString&) const = 0;                                             
    virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) = 0;        
    virtual void FixMovieSequenceCamera(bool) const = 0;                                                 
    virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) = 0;                             
    virtual bool FadeScreenOut(float, float, CRGBColour) = 0;                                            
    virtual void FadeScreenIn() = 0;                                                                     
    virtual bool IsScreenFadingOut() const = 0;                                                          
    virtual void Pause(float) const = 0;                                                                 
    virtual void EndCutFade() = 0;                                                                       
    protected: virtual void EndLetterBox() = 0; public:                                                  
    virtual void PauseAllNonScriptedEntities(bool) = 0;                                                  
    virtual void PauseAllEntities(bool) = 0;                                                             
    virtual void SetAllowScreenFadingOnNextRegionChange(bool) = 0;                                       
    virtual void SetAllowScreenFadingIfAlreadyFaded(bool) = 0;                                           
    virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const = 0;                                  
    virtual void SetToDisplayTutorialsDuringCutscenes(bool) const = 0;                                   
    virtual void SetCutsceneMode(bool, bool) const = 0;                                                  
    virtual bool IsInCutscene() const = 0;                                                               
    virtual void SetCutsceneSkippable(bool) const = 0;                                                   
    virtual void SetCutsceneSkippableWhilePaused(bool) const = 0;                                        
    virtual void SetCutsceneActionMode(bool, const CCharString&) const = 0;                              
    virtual void PreloadNewScene(float) const = 0;                                                       
    virtual void StartProgressDisplay() const = 0;                                                       
    virtual void StopProgressDisplay() const = 0;                                                        
    virtual void SetScreenMessagesEnabled(bool) const = 0;                                               
    virtual bool IsHeroControlledByPlayer() const = 0;                                                   
    virtual bool IsInMovieSequence() const = 0;                                                          
    virtual void CancelAbilitiesForCutscene() const = 0;                                                 
    virtual void ResumeAbilitiesForCutscene() const = 0;                                                 
    virtual void CancelUsingAbility(EHeroAbility) const = 0;                                             
    virtual void SetAbilityAvailability(EHeroAbility, bool) const = 0;                                   
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const = 0;                                    
    virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const = 0;                   
    virtual void RemoveDeadCreature(const CCharString&) const = 0;                                       
    virtual void CameraSetCameraPreloadFlag(bool) const = 0;                                             
    virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const = 0;        
    virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const = 0;             
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const = 0;       
    virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const = 0;  
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const = 0; 
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const = 0; 
    virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const = 0; 
    virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, int, int) const = 0; 
    virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, int, int) const = 0; 
    virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, int, int) const = 0; 
    virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, int, int) const = 0; 
    virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const = 0; 
    virtual void CameraDefault() = 0;                                                                    
    virtual void CameraResetToViewBehindHero(float) = 0;                                                 
    virtual bool IsCameraInScriptedMode() const = 0;                                                     
    virtual void CameraUseScreenEffect(float, float, float) const = 0;                                   
    virtual void CameraCancelScreenEffect() const = 0;                                                   
    virtual bool IsCameraPosOnScreen(const C3DVector&) const = 0;                                        
    virtual float GetGameAngleXY() const = 0;                                                            
    virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const = 0;               
    virtual void CameraShake(float, float) const = 0;                                                    
    virtual void OpenDoor(const CScriptThing&) const = 0;                                                
    virtual void CloseDoor(const CScriptThing&) const = 0;                                               
    virtual void OpenHouseDoors(const CScriptThing&) const = 0;                                          
    virtual void CloseHouseDoors(const CScriptThing&) const = 0;                                         
    virtual void JamDoor(const CScriptThing&) const = 0;                                                 
    virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const = 0;                    
    virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const = 0;                     
    virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const = 0;                       
    virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const = 0;                         
    virtual bool IsChestOpen(const CScriptThing&) const = 0;                                             
    virtual bool OpenChest(const CScriptThing&, bool) const = 0;                                         
    virtual void CloseChest(const CScriptThing&) const = 0;                                              
    virtual int GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const = 0;        
    virtual void DisplayLockedChestMessage(const CScriptThing&) const = 0;                               
    virtual void SetTrophyAsMountable(const CScriptThing&, bool) const = 0;                              
    virtual void SetVillageLimbo(const CScriptThing&, bool) const = 0;                                   
    virtual void SetCreatureNotReload(const CScriptThing&) const = 0;                                    
    virtual bool IsSleepingTime(const CScriptThing&) const = 0;                                          
    virtual void EnableGuards(const CScriptThing&, bool) const = 0;                                      
    virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const = 0;        
    virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const = 0; 
    virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const = 0;                                  
    virtual void SetThingAsUsable(const CScriptThing&, bool) const = 0;                                  
    virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const = 0;               
    virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const = 0;              
    virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const = 0; 
    virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const = 0;           
    virtual void ClearThingBestEnemyTarget(const CScriptThing&) const = 0;                               
    virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const = 0;                            
    virtual bool IsEntityInLimbo(const CScriptThing&) const = 0;                                         
    virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const = 0;                      
    virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const = 0;                            
    virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const = 0;                  
    virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const = 0; 
    virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const = 0; 
    virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const = 0;                 
    virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const = 0;                          
    virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, int) const = 0;                     
    virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const = 0;                         
    virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const = 0;                
    virtual void EntitySetCombatAbility(const CScriptThing&, float) const = 0;                           
    virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const = 0;              
    virtual void EntityClearRangedTarget(const CScriptThing&) const = 0;                                 
    virtual void EntitySetTargetable(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const = 0;                  
    virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const = 0;           
    virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const = 0;        
    virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const = 0; 
    virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const = 0;                       
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const = 0; 
    virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const = 0;       
    virtual void SetThingPersistent(const CScriptThing&, bool) const = 0;                                
    virtual void EntitySetThingAsWantingMoney(const CScriptThing&, int) const = 0;                      
    virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, int) const = 0;                      
    virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const = 0;   
    virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const = 0;        
    virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned int) const = 0;                  
    virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned int&) const = 0;                 
    virtual void EntitySetAsForSale(const CScriptThing&, bool) const = 0;                                
    virtual void EntitySetStockItemPrice(const CScriptThing&, int) const = 0;                           
    virtual int EntityGetStockItemPrice(const CScriptThing&) const = 0;                                 
    virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const = 0;     
    virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const = 0;                         
    virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const = 0;                         
    virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const = 0;                              
    virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const = 0;                              
    virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const = 0;              
    virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const = 0;                         
    virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const = 0;                     
    virtual void EntitySetAsScared(const CScriptThing&, bool) const = 0;                                 
    virtual void EntitySetAsDrunk(CScriptThing&, bool) const = 0;                                        
    virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const = 0;           
    virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const = 0;                         
    virtual void EntityForceToLookAtNothing(const CScriptThing&) const = 0;                              
    virtual void EntityResetForceToLookAt(const CScriptThing&) const = 0;                                
    virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, int) const = 0;                   
    virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const = 0;                        
    virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const = 0;                 
    virtual void EntityDropGenericBox(const CScriptThing&) const = 0;                                    
    virtual void EntitySheatheWeapons(const CScriptThing&, bool) const = 0;                              
    virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const = 0;                            
    virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const = 0;                        
    virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const = 0;                       
    virtual void EntitySetAlpha(const CScriptThing&, float, bool) const = 0;                             
    virtual void EntitySetAsDrawable(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const = 0;          
    virtual ESex EntityGetSex(const CScriptThing&) const = 0;                                            
    virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const = 0;          
    virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const = 0;                           
    virtual void EntitySetAsDamageable(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const = 0;                         
    virtual void EntitySetAsLocked(const CScriptThing&, bool) const = 0;                                 
    virtual void EntityDecapitate(const CScriptThing&) const = 0;                                        
    virtual void EntityGiveGold(const CScriptThing&, int) const = 0;                                    
    virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const = 0;                    
    virtual int EntityGetBossPhase(const CScriptThing&) const = 0;                                      
    virtual void EntitySetBossPhase(const CScriptThing&, int) const = 0;                                
    virtual void EntityResetCreatureMode(const CScriptThing&) const = 0;                                 
    virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const = 0;                        
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const = 0;          
    virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const = 0;              
    virtual void EntityLeaveCombatStance(const CScriptThing&) const = 0;                                 
    virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const = 0;                   
    virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const = 0;                     
    virtual void EntityEnableGravity(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetLightAsOn(const CScriptThing&, bool) const = 0;                                
    virtual void EntityFadeOut(const CScriptThing&, float) const = 0;                                    
    virtual void EntityFadeIn(const CScriptThing&, float) const = 0;                                     
    virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const = 0;         
    virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const = 0;                       
    virtual bool EntityCastForcePush(const CScriptThing&, bool) const = 0;                               
    virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const = 0;        
    virtual void BeginLoadingMesh(const CCharString&) const = 0;                                         
    virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const = 0;       
    virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const = 0; 
    virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const = 0;                       
    virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) = 0;                                     
    virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const = 0;                       
    virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const = 0;          
    virtual void CreatureGeneratorTrigger(const CScriptThing&) const = 0;                                
    virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const = 0; 
    virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const = 0;                             
    virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const = 0;                            
    virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const = 0; 
    virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, int) const = 0;                   
    virtual int CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const = 0;               
    virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const = 0;                   
    virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const = 0;   
    virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const = 0;                       
    virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const = 0;                       
    virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const = 0;           
    virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const = 0;            
    virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const = 0;                     
    virtual bool IsCreatureFlying(const CScriptThing&) const = 0;                                        
    virtual void SetTeleporterAsActive(const CScriptThing&, bool) const = 0;                             
    virtual bool IsTeleporterActive(const CScriptThing&) const = 0;                                      
    virtual void SetTeleportingAsActive(bool) const = 0;                                                 
    virtual bool IsTeleportingActive() const = 0;                                                        
    virtual void SetRegionExitAsActive(const CScriptThing&, bool) const = 0;                             
    virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const = 0;                         
    virtual void SetRegionTextDisplayAsActive(bool) const = 0;                                           
    virtual void SetHeroSleepingAsEnabled(bool) const = 0;                                               
    virtual bool IsHeroSleepingEnabled() const = 0;                                                      
    virtual void SetExperienceSpendingAsEnabled(bool) const = 0;                                         
    virtual void SetMoralityChangingAsEnabled(bool) const = 0;                                           
    virtual void SetSummonerDeathExplosionAffectsHero(bool) const = 0;                                   
    virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const = 0;                    
    virtual bool IsDiggingSpotEnabled(const CScriptThing&) const = 0;                                    
    virtual bool IsDiggingSpotHidden(const CScriptThing&) const = 0;                                     
    virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const = 0;                            
    virtual bool CheckForCameraMessage(const CCharString&) const = 0;                                    
    virtual void WaitForCameraMessage(const CCharString&) const = 0;                                     
    virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const = 0;           
    virtual void SetFireToThing(const CScriptThing&) const = 0;                                          
    virtual void ExtinguishFiresOnThing(const CScriptThing&) const = 0;                                  
    virtual bool IsThingOnFire(const CScriptThing&) const = 0;                                           
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const = 0;                  
    virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const = 0;             
    virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const = 0;                  
    virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; 
    virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const = 0;                    
    virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const = 0;    
    virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const = 0;                            
    virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const = 0;                  
    virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const = 0;                     
    virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned int,std::less<unsigned int>,std::allocator<unsigned int> >&) const = 0; 
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;       
    virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;     
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;        
    virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const = 0;                  
    virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const = 0;          
    virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const = 0;         
    virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const = 0;           
    virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const = 0;                 
    virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const = 0;          
    virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const = 0;               
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const = 0;        
    virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const = 0;  
    virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const = 0;                     
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, int) const = 0;                      
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, int) const = 0;                          
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const = 0;            
    virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const = 0; 
    virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const = 0; 
    virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const = 0; 
    virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntitySetPersonalityOverride(const CScriptThing&, int) const = 0;                      
    virtual void EntityClearPersonalityOverride(const CScriptThing&) const = 0;                          
    virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const = 0;            
    virtual void EntitySetAsOpinionSource(const CScriptThing&, int) const = 0;                          
    virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const = 0;                        
    virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const = 0;                       
    virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const = 0;               
    virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const = 0;            
    virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual int EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, int) const = 0;       
    virtual bool IsDeedWitnessed(int) const = 0;                                                        
    virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;       
    virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0; 
    virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;    
    virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const = 0;   
    virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const = 0;            
    virtual void EntitySetSoundRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetSmellRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetSightRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const = 0;                     
    virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const = 0;                       
    virtual float EntityGetHearingRadius(const CScriptThing&) const = 0;                                 
    virtual bool ManuallyTriggerTrap(const CScriptThing&) const = 0;                                     
    virtual bool ManuallyResetTrap(const CScriptThing&) const = 0;                                       
    virtual void SetTimeOfDay(float) const = 0;                                                          
    virtual int GetTimeOfDay() const = 0;                                                               
    virtual void SetTimeAsStopped(bool, int&) const = 0;                                                
    virtual void FastForwardTimeTo(float, float) const = 0;                                              
    virtual bool IsTimeOfDayBetween(int, int) const = 0;                                               
    virtual int GetDayOfWeek() const = 0;                                                               
    virtual int GetDayCount() const = 0;                                                                
    virtual int GetWorldFrame() const = 0;                                                              
    virtual int GetConstantFPS() const = 0;                                                             
    virtual CCharString GetActiveQuestName() const = 0;                                                  
    virtual void TransitionToTheme(const CCharString&, float) const = 0;                                 
    virtual void ResetToDefaultTheme(float) const = 0;                                                   
    virtual void TransitionToThemeAllInternals(const CCharString&, float) const = 0;                     
    virtual void ResetToDefaultThemeAllInternals(float) const = 0;                                       
    virtual void TransitionToThemeExternals(const CCharString&, float) const = 0;                        
    virtual void ResetToDefaultThemeExternals(float) const = 0;                                          
    virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const = 0;              
    virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const = 0;             
    virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const = 0;                
    virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const = 0;                   
    virtual void SetAllSoundsAsMuted(bool) const = 0;                                                    
    virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const = 0;   
    virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const = 0;           
    virtual void RadialBlurFadeOut(float, void*) const = 0;                                              
    virtual bool IsRadialBlurFadeActive() const = 0;                                                     
    virtual void CancelRadialBlurFade() const = 0;                                                       
    virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const = 0;                         
    virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const = 0;     
    virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const = 0;                      
    virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const = 0; 
    virtual void ScreenFilterFadeOut(float, void*) const = 0;                                            
    virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const = 0;       
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const = 0;            
    virtual bool IsGiftRomantic(const CCharString&) const = 0;                                           
    virtual bool IsGiftFriendly(const CCharString&) const = 0;                                           
    virtual bool IsGiftOffensive(const CCharString&) const = 0;                                          
    virtual bool IsThingABed(const CScriptThing&) const = 0;                                             
    virtual bool IsThingAChest(const CScriptThing&) const = 0;                                           
    virtual bool IsThingADoor(const CScriptThing&) const = 0;                                            
    virtual bool IsThingSmashable(const CScriptThing&) const = 0;                                        
    virtual bool IsThingSearchable(const CScriptThing&) const = 0;                                       
    virtual void ApplyScriptBrush(const CCharString&) const = 0;                                         
    virtual void EnableDecals(bool) const = 0;                                                           
    virtual unsigned int PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const = 0;   
    virtual unsigned int PlaySoundOnThing(const CScriptThing&, const CCharString&) const = 0;           
    virtual unsigned int PlaySoundAtPos(const C3DVector&, const CCharString&) const = 0;                
    virtual unsigned int Play2DSound(const CCharString&) const = 0;                                     
    virtual bool IsSoundPlaying(unsigned int) const = 0;                                                
    virtual void EnableSounds(bool) const = 0;                                                           
    virtual void StopSound(unsigned int) const = 0;                                                     
    virtual void OverrideMusic(EMusicSetType, bool, bool) const = 0;                                     
    virtual void StopOverrideMusic(bool) const = 0;                                                      
    virtual void CacheMusicSet(EMusicSetType) const = 0;                                                 
    virtual void EnableDangerMusic(bool) const = 0;                                                      
    virtual bool IsDangerMusicEnabled() const = 0;                                                       
    virtual void StartCountdownTimer(float) const = 0;                                                   
    virtual float GetCountdownTimer() const = 0;                                                         
    virtual void AutoSaveCheckPoint() const = 0;                                                         
    virtual void AutoSaveQuestStart() const = 0;                                                         
    virtual void AutoSave() const = 0;                                                                   
    virtual void SetSavingAsEnabled(bool) const = 0;                                                     
    virtual bool IsSavingEnabled() const = 0;                                                            
    virtual void SetSaveGameMarkerPos(const C3DVector&) const = 0;                                       
    virtual void ResetToFrontEnd() const = 0;                                                            
    virtual void SetGuildSealRecallLocation(const C3DVector&, float) const = 0;                          
    virtual C3DVector GetGuildSealRecallPos() const = 0;                                                 
    virtual float GetGuildSealRecallAngleXY() const = 0;                                                 
    virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const = 0;               
    virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const = 0;            
    virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const = 0; 
    virtual CWideString GetTextString(const CCharString&) const = 0;                                     
    virtual void AddRumourCategory(const CCharString&) const = 0;                                        
    virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const = 0;               
    virtual void RemoveRumourCategory(const CCharString&) const = 0;                                     
    virtual void SetCategoryActivity(const CCharString&, bool) const = 0;                                
    virtual void AddGossipVillage(const CCharString&, const CCharString&) const = 0;                     
    virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const = 0;           
    virtual void SetIsGossipForPlayer(const CCharString&, bool) const = 0;                               
    virtual void SetIsGossipForPlayer(CCharString, bool) = 0;                                            
    virtual void UpdateOnlineScore_Archery(float) const = 0;                                             
    virtual void UpdateOnlineScore_ChickenKick(float) const = 0;                                         
    virtual void UpdateOnlineScore_ChapelOrTemple(float) const = 0;                                      
    virtual void UpdateOnlineScore_FishingCompo(float) const = 0;                                        
    virtual void UpdateScore_FishingCompetition(float) const = 0;                                        
    virtual float GetBestTimePairs() const = 0;                                                          
    virtual float GetBestTimeSorting() const = 0;                                                        
    virtual int GetBestScoreBlackjack() const = 0;                                                      
    virtual int GetBestScoreCoinGolfOakVale() const = 0;                                                
    virtual int GetBestScoreCoinGolfSnowSpire() const = 0;                                              
    virtual int GetBestScoreShoveHaPenny() const = 0;                                                   
    virtual float GetBestTimeGuessTheAddition() const = 0;                                               
    virtual bool IsHeroInTavernGame() const = 0;                                                         
    virtual int GetNumHousesOwned() const = 0;                                                          
    virtual void StartSneaking() const = 0;                                                              
    virtual int GetStealDuration(const CScriptThing&) const = 0;                                        
    virtual void SetUseableByHero(const CScriptThing&, bool) const = 0;                                  
    virtual void SetOwnedByHero(const CScriptThing&, bool) const = 0;                                    
    virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const = 0;                     
    virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const = 0;                            
    virtual void SetIsThingForcePushable(const CScriptThing&, bool) const = 0;                           
    virtual void SetIsThingLightningable(const CScriptThing&, bool) const = 0;                           
    virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const = 0;                           
    virtual bool IsThingTurncoated(const CScriptThing&) const = 0;                                       
    virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const = 0;             
    virtual void RemoveCreatureScriptedMode(const CScriptThing&) const = 0;                              
    virtual void ForceShipsVisible() const = 0;                                                          
    virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) = 0; 
    virtual void SetBedAvailability(const CScriptThing&, bool) = 0;                                      
    virtual void RepopulateVillage(const CScriptThing&) const = 0;                                       
    virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) = 0;                        
    virtual CScriptThing SetResidency(const CScriptThing&, bool) = 0;                                    
    virtual void SetThankingPhrase(CScriptThing, unsigned int) = 0;                                     
    virtual unsigned int GetThankingPhrase(CScriptThing) = 0;                                           
    virtual void ResetThankingPhrase(CScriptThing) = 0;                                                  
    virtual void SetIgnoringPhrase(CScriptThing, unsigned int) = 0;                                     
    virtual unsigned int GetIgnoringPhrase(CScriptThing) = 0;                                           
    virtual void ResetIgnoringPhrase(CScriptThing) = 0;                                                  
    virtual void SetWanderCentrePoint(CScriptThing, C3DVector) = 0;                                      
    virtual C3DVector GetWanderCentrePoint(CScriptThing) = 0;                                            
    virtual void ResetWanderCentrePoint(CScriptThing) = 0;                                               
    virtual void SetWanderMinDistance(CScriptThing, float) = 0;                                          
    virtual float GetWanderMinDistance(CScriptThing) = 0;                                                
    virtual void ResetWanderMinDistance(CScriptThing) = 0;                                               
    virtual void SetWanderMaxDistance(CScriptThing, float) = 0;                                          
    virtual float GetWanderMaxDistance(CScriptThing) = 0;                                                
    virtual void ResetWanderMaxDistance(CScriptThing) = 0;                                               
    virtual void SetGossipCounter(CScriptThing, int) = 0;                                               
    virtual int GetGossipCounter(CScriptThing) = 0;                                                     
    virtual void ResetGossipCounter(CScriptThing) = 0;                                                   
    virtual void SetMaxGossipPhrase(CScriptThing, int) = 0;                                             
    virtual int GetMaxGossipPhrase(CScriptThing) = 0;                                                   
    virtual void ResetMaxGossipPhrase(CScriptThing) = 0;                                                 
    virtual void SetWarningPhrase(CScriptThing, unsigned int) = 0;                                      
    virtual unsigned int GetWarningPhrase(CScriptThing) = 0;                                            
    virtual void ResetWarningPhrase(CScriptThing) = 0;                                                   
    virtual void SetBeerRequestPhrase(CScriptThing, unsigned int) = 0;                                  
    virtual unsigned int GetBeerRequestPhrase(CScriptThing) = 0;                                        
    virtual void ResetBeerRequestPhrase(CScriptThing) = 0;                                               
    virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) = 0;                        
    virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) = 0;                              
    virtual void ResetScriptingStateGroup(CScriptThing) = 0;                                             
    virtual void SetMaxHeroReactionDistance(CScriptThing, float) = 0;                                    
    virtual float GetMaxHeroReactionDistance(CScriptThing) = 0;                                          
    virtual void ResetMaxHeroReactionDistance(CScriptThing) = 0;                                         
    virtual void SetActionFrequency(CScriptThing, int) = 0;                                             
    virtual int GetActionFrequency(CScriptThing) = 0;                                                   
    virtual void ResetActionFrequency(CScriptThing) = 0;                                                 
    virtual void SetActionFrequencyVariation(CScriptThing, float) = 0;                                   
    virtual float GetActionFrequencyVariation(CScriptThing) = 0;                                         
    virtual void ResetActionFrequencyVariation(CScriptThing) = 0;                                        
    virtual void SetAction(CScriptThing, CCharString) = 0;                                               
    virtual CCharString GetAction(CScriptThing) = 0;                                                     
    virtual void ResetAction(CScriptThing) = 0;                                                          
    virtual void SetFaceHeroForAction(CScriptThing, bool) = 0;                                           
    virtual bool GetFaceHeroForAction(CScriptThing) = 0;                                                 
    virtual void ResetFaceHeroForAction(CScriptThing) = 0;                                               
    virtual void SetTargetName(CScriptThing, CCharString) = 0;                                           
    virtual CCharString GetTargetName(CScriptThing) = 0;                                                 
    virtual void ResetTargetName(CScriptThing) = 0;                                                      
    virtual void SetFollowDistance(CScriptThing, float) = 0;                                             
    virtual float GetFollowDistance(CScriptThing) = 0;                                                   
    virtual void ResetFollowDistance(CScriptThing) = 0;                                                  
    virtual void SetAttackHeroOnSight(CScriptThing, bool) = 0;                                           
    virtual bool GetAttackHeroOnSight(CScriptThing) = 0;                                                 
    virtual void ResetAttackHeroOnSight(CScriptThing) = 0;                                               
    virtual void SetTimeToSpendHarassingHero(CScriptThing, int) = 0;                                    
    virtual int GetTimeToSpendHarassingHero(CScriptThing) = 0;                                          
    virtual void ResetTimeToSpendHarassingHero(CScriptThing) = 0;                                        
    virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) = 0;                      
    virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                            
    virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                           
    virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) = 0;                                  
    virtual float GetCombatNearbyBreakOffRange(CScriptThing) = 0;                                        
    virtual void ResetCombatNearbyBreakOffRange(CScriptThing) = 0;                                       
    virtual void SetStealStealableItems(CScriptThing, bool) = 0;                                         
    virtual bool GetStealStealableItems(CScriptThing) = 0;                                               
    virtual void ResetStealStealableItems(CScriptThing) = 0;                                             
    virtual void SetRecoverStealableItems(CScriptThing, bool) = 0;                                       
    virtual bool GetRecoverStealableItems(CScriptThing) = 0;                                             
    virtual void ResetRecoverStealableItems(CScriptThing) = 0;                                           
    virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) = 0;                        
    virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) = 0;                              
    virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) = 0;                            
    virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) = 0;            
    virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                  
    virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                
    virtual void SetAllowedToFollow(CScriptThing, bool) = 0;                                             
    virtual bool GetAllowedToFollow(CScriptThing) = 0;                                                   
    virtual void ResetAllowedToFollow(CScriptThing) = 0;                                                 
    virtual void SetTableName(CScriptThing, CCharString) = 0;                                            
    virtual CCharString GetTableName(CScriptThing) = 0;                                                  
    virtual void ResetTableName(CScriptThing) = 0;                                                       
    virtual void SetSeatName(CScriptThing, CCharString) = 0;                                             
    virtual CCharString GetSeatName(CScriptThing) = 0;                                                   
    virtual void ResetSeatName(CScriptThing) = 0;                                                        
    virtual void SetDisableHeadLooking(CScriptThing, bool) = 0;                                          
    virtual bool GetDisableHeadLooking(CScriptThing) = 0;                                                
    virtual void ResetDisableHeadLooking(CScriptThing) = 0;                                              
    virtual void SetIsPushableByHero(CScriptThing, bool) = 0;                                            
    virtual bool GetIsPushableByHero(CScriptThing) = 0;                                                  
    virtual void ResetIsPushableByHero(CScriptThing) = 0;                                                
    virtual void SetLookForFiniteTime(CScriptThing, bool) = 0;                                           
    virtual bool GetLookForFiniteTime(CScriptThing) = 0;                                                 
    virtual void ResetLookForFiniteTime(CScriptThing) = 0;                                               
    virtual void SetAvoidRegionExits(CScriptThing, bool) = 0;                                            
    virtual bool GetAvoidRegionExits(CScriptThing) = 0;                                                  
    virtual void ResetAvoidRegionExits(CScriptThing) = 0;                                                
    virtual void SetTargetingDistanceOffset(CScriptThing, float) = 0;                                    
    virtual float GetTargetingDistanceOffset(CScriptThing) = 0;                                          
    virtual void ResetTargetingDistanceOffset(CScriptThing) = 0;                                         
    virtual void SetPlayerUsingMeleeDummies(bool) = 0;                                                   
    virtual bool GetPlayerUsingMeleeDummies() = 0;                                                       
    virtual void SetPlayerUsingRangedDummies(bool) = 0;                                                  
    virtual bool GetPlayerUsingRangedDummies() = 0;                                                      
    virtual void SetPlayerUsingWillDummies(bool) = 0;                                                    
    virtual bool GetPlayerUsingWillDummies() = 0;                                                        
    virtual void SetCheapHeadLooking(bool) = 0;                                                          
    virtual bool GetCheapHeadLooking() = 0;                                                              
    virtual void SetQuitTavernGame(bool) = 0;                                                            
    virtual bool GetQuitTavernGame() = 0;                                                                
    virtual void SetPrizeTavernTable(bool) = 0;                                                          
    virtual bool GetPrizeTavernTable() = 0;                                                              
    virtual void SetBettingActive(bool) = 0;                                                             
    virtual bool GetBettingActive() = 0;                                                                 
    virtual void SetBettingAccept(bool) = 0;                                                             
    virtual bool GetBettingAccept() = 0;                                                                 
    virtual void SetBettingAmount(int) = 0;                                                             
    virtual int GetBettingAmount() = 0;                                                                 
    virtual void SetCountBetMoneyDown(bool) = 0;                                                         
    virtual bool GetCountBetMoneyDown() = 0;                                                             
    virtual void SetSpotTheAdditionBeaten(bool) = 0;                                                     
    virtual bool GetSpotTheAdditionBeaten() = 0;                                                         
    virtual void SetGlobalTargetingDistanceOffset(float) = 0;                                            
    virtual float GetGlobalTargetingDistanceOffset() = 0;                                                
    virtual void SetTradingPriceMult(float) = 0;                                                         
    virtual float GetTradingPriceMult() = 0;                                                             
    virtual void SetBoastingEnabled(bool) = 0;                                                           
    virtual bool GetBoastingEnabled() = 0;                                                               
    virtual void SetActiveGossipCategories(CCharString, bool) = 0;                                       
    virtual bool* GetActiveGossipCategories(CCharString) = 0;                                            
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() = 0; 
    virtual int GetActiveGossipCategoriesSize() = 0;                                                     
    virtual void ClearActiveGossipCategories() = 0;                                                      
    virtual bool* GetIsGossipForPlayer(CCharString) = 0;                                                 
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() = 0; 
    virtual int GetIsGossipForPlayerSize() = 0;                                                          
    virtual void ClearIsGossipForPlayer() = 0;                                                           
    virtual void SetGossip(CCharString, CCharString, int) = 0;                                          
    virtual CCharString GetGossip(CCharString, int) = 0;                                                
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) = 0;     
    virtual int GetGossipSize(CCharString) = 0;                                                          
    virtual void ClearGossip(CCharString) = 0;                                                           
    virtual void RemoveGossip(CCharString) = 0;                                                          
    virtual void AddGossip(CCharString, CCharString) = 0;                                                
    virtual void AddGossip(CCharString) = 0;                                                             
    virtual void SetGossipVillages(CCharString, CCharString, int) = 0;                                  
    virtual CCharString GetGossipVillages(CCharString, int) = 0;                                        
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) = 0; 
    virtual int GetGossipVillagesSize(CCharString) = 0;                                                  
    virtual void ClearGossipVillages(CCharString) = 0;                                                   
    virtual void RemoveGossipVillages(CCharString) = 0;                                                  
    virtual void AddGossipVillages(CCharString, CCharString) = 0;                                        
    virtual void AddGossipVillages(CCharString) = 0;                                                     
    virtual void SetGossipFactions(CCharString, CCharString, int) = 0;                                  
    virtual CCharString GetGossipFactions(CCharString, int) = 0;                                        
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) = 0; 
    virtual int GetGossipFactionsSize(CCharString) = 0;                                                  
    virtual void ClearGossipFactions(CCharString) = 0;                                                   
    virtual void RemoveGossipFactions(CCharString) = 0;                                                  
    virtual void AddGossipFactions(CCharString, CCharString) = 0;                                        
    virtual void AddGossipFactions(CCharString) = 0;                                                     
};

#endif 
#endif 

#ifndef FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H
#define FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H


#ifndef FABLE_ENGINE_CLEVELSCRIPTBASE_H
#define FABLE_ENGINE_CLEVELSCRIPTBASE_H











#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CLevelScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CSCRIPTBASE_H
#define FABLE_ENGINE_CSCRIPTBASE_H












#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};

#endif 


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#endif 

#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H







class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef FableU64 FableNScriptUInt64;                           
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority;
#endif
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef FableU64 FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
#ifndef FABLE_RETAIL_WASP
enum EScriptAIPriority { };
#endif
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif 


class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();                                                       
    virtual ~CScriptBase();                                              
protected:
    virtual void RegisterMain();                                         
    virtual void Main();                                                 
    virtual void Init();                                                 
public:
    virtual void OnPersist(CPersistContext&);                            
    void Activate();
    void SetPredicate(const NScript::CPredicateBase&);                   
    void RemovePredicate();
    bool IsActiveThreadTerminating();                                    
    CSpawnedFuncBase* GetPActiveThread();
    unsigned int GetThreadID();
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);     
    void KillSpawnedFunction(const CCharString&, unsigned int);
    void KillAllThreads();
    void RunSubThreads();
    void AddEntityScriptBinding(CEntityScriptBindingBase*);              
    bool IsEntityScriptBound(const CCharString&) const;
    void PostAddScriptedEntities();                                      
    void OnRegionLoaded(const CCharString&);
    void OnRegionUnloaded(const CCharString&);
    void OnScriptedEntityActivated(FableNScriptUInt64, CScriptThing&);
    void OnScriptedEntityDeactivated(FableNScriptUInt64, CScriptThing&, bool);
    void OnScriptedEntityInterrupted(FableNScriptUInt64);
    void LoadGameState(const CCharString&);
    void SaveGameState(CCharString&);
protected:
    bool UpdateSpawnedFunction(CSpawnedFuncBase*);
    void RunSpawnedFunctions();
    void RunEntityThreads();
    void DoScriptDataPersist(CCharString&, bool);
    void DoEntityScriptPersist(CActiveEntityScriptBase&, CCharString&, bool);
    void LoadEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
    void SaveEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
public:
#ifdef FABLE_RETAIL_WASP
private:
#endif
    unsigned char     Threads[0x4];                                      
    unsigned char     EntityScripts[0x10];                               
    unsigned char     EntityScriptBindings[0x10];                        
    unsigned int     LastWorldUpdateFrame;                              
    CSpawnedFuncBase* PActiveThread;                                     
    unsigned char     RuntimeScriptedEntityPersist[0x10];                
};

#endif 
#endif 

class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(game),MasterScriptData((T*)data) {}
#else
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
#endif
    virtual ~CLevelScriptBase();
    T* GetMasterData();
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
    CGameScriptInterfaceBase& Game;
    T* MasterScriptData;
#else
    CGameScriptInterfaceBase* Game;                                      
    T*                        MasterScriptData;                          
#endif
};

#endif 
#endif 

#ifndef FABLE_ENGINE_CTIMER_H
#define FABLE_ENGINE_CTIMER_H








#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CTimer.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


class CTimer {
public:
    CTimer();                                                            
    ~CTimer();                                                           
    CTimer& operator=(int);
    bool operator==(int) const;
    bool operator!=(int) const;
    bool operator<(int) const;
    bool operator>(int) const;
    bool operator<=(int) const;
    bool operator>=(int) const;
    int GetTime() const;
    int GetIndex() const;
    void Pause();
    void Unpause();
    int TimerIndex;                                                     
};

#endif 
#endif 

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>

#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL


#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
protected:
    T* PData;
    unsigned int StringLength;
    unsigned int DataLength : 31;
    bool UseFastExtend : 1;
#if defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH)
    static int __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_RELATIVE_PATH
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    unsigned int GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    int NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    int GetLength() const;
    int FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(int length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();

protected:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        int length);

private:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, int length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        int length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];


#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, int length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
#ifdef FABLE_RETAIL_SUBTITLE
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned int GetCRC() const; 
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    int GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { int offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_RELATIVE_PATH
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned int> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_HERO_LOGBOOK
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_RETAIL_SHOP
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_RELATIVE_PATH
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BountyHuntScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    struct TeleportData {
        CCharString defName; unsigned int appearanceSeed; float health;
        CCharString scriptName, teleportTo, teleportMkr;
    };
    CQ_BountyHuntScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_BountyHuntScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void SpawnBanditGangGrunts(CCharString spawnName);
    bool QuestFailed;
    bool QuestSucceeded;
    bool QuestOver;
    bool QuestStarted;
    bool FishermanDeactivated;
    bool VillagerKilled;
    bool VillagerIntroDone;
    bool LeaderKilled;
    bool Deputy1Killed;
    bool Deputy2Killed;
    bool Deputy1IntroComplete;
    bool Deputy2IntroComplete;
    bool Hostage1Killed;
    bool Hostage2Killed;
    bool Hostage1Safe;
    bool Hostage2Safe;
    bool Deputy1Escaped;
    bool LeaderEscaped;
    bool SpawnDeputy1Grunts;
    bool SpawnDeputy2Grunts;
    bool SpawnLeaderGrunts;
    bool AllDeputy1GruntsKilled;
    bool AllDeputy2GruntsKilled;
    bool AllLeaderGruntsKilled;
    bool AllLeaderChargeGruntsKilled;
    bool AllGruntsKilled;
    bool FishComplete;
    bool LeaderIntro1Complete;
    bool LeaderIntro2Complete;
    bool LeaderAttackingHostage;
    bool LeaderAttackingHero;
    bool LeaderRansomPaid;
    bool LeaderRansomDenied;
    bool LeaderLeftHostage;
    bool LeaderReachedMoney;
    bool LeaderAttackedAfterRansomPaid;
    bool LeaderChargeComplete;
    bool LeaderCutsceneComplete;
    bool LakeComplete;
    bool BanditCutsceneDone;
    bool BountyHuntIntroComplete;
    int BanditGruntTextCounter;
    bool BoastActiveTimeLimit;
    bool BoastActiveDecapitation;
    int BoastTimerID;
    CTimer BoastTime;
    bool BoastQuestInfoDisplayed;
    std::vector<TeleportData> TeleportInfo;
};
}
#endif

inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline void CScriptThing::SetDataString(const CCharString& data) { if(!PImp.IsNull()) PImp->SetDataString(data); }
void NScript::CQ_BountyHuntScript::SpawnBanditGangGrunts(CCharString spawnName)
{
    std::vector<CScriptThing> spawnPoints;
    int numPoints=Game.GetAllThingsWithScriptName(spawnName,spawnPoints);
    CScriptThing spawnMkr;
    CScriptThing grunt;
    for(int idx=0;idx<numPoints;++idx) {
        spawnMkr=spawnPoints[idx];
        if(spawnMkr.IsAlive()) {
            if(IsActiveThreadTerminating()) return;
            int choice=fixture_rand()%4;
            if(choice==0) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game.CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_A",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else if(choice==1) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game.CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_B",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else if(choice==2) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game.CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_C",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else {
                if(IsActiveThreadTerminating()) return;
                grunt=Game.CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_D",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            }
            while(!grunt.IsAlive()) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            CCharString num=spawnMkr.GetDataString();
            grunt.SetDataString(spawnMkr.GetDataString());
            Game.EntityAttachToScript(grunt,Game.GetActiveQuestName());
        }
    }
}

class FixtureScriptInterface:public CGameScriptInterfaceBase { public:
 virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned int) const { assert(!"unexpected interface call: Error"); throw 0; }
 virtual void TraceMessage(const CCharString&) const { assert(!"unexpected interface call: TraceMessage"); throw 0; }
 virtual void Validate() const { assert(!"unexpected interface call: Validate"); throw 0; }
 virtual void SetDebugCameraType(int) const { assert(!"unexpected interface call: SetDebugCameraType"); throw 0; }
 virtual void DeactivateBoastUi() const { assert(!"unexpected interface call: DeactivateBoastUi"); throw 0; }
 virtual bool IsXbox() const { assert(!"unexpected interface call: IsXbox"); throw 0; }
 virtual void NewScriptFrame() const { assert(!"unexpected interface call: NewScriptFrame"); throw 0; }
 virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) { assert(!"unexpected interface call: StartScriptingEntity"); throw 0; }
 virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityUnderScriptedControl"); throw 0; }
 virtual bool IsActiveThreadTerminating() const { assert(!"unexpected interface call: IsActiveThreadTerminating"); throw 0; }
 virtual bool IsLevelLoaded(const CCharString&) const { assert(!"unexpected interface call: IsLevelLoaded"); throw 0; }
 virtual bool IsRegionLoaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionLoaded"); throw 0; }
 virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionLoadedAndPreloaded"); throw 0; }
 virtual bool IsRegionDefLoaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionDefLoaded"); throw 0; }
 virtual const CCharString& GetRegionName() const { assert(!"unexpected interface call: GetRegionName"); throw 0; }
 virtual bool MsgIsLevelLoaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsLevelLoaded"); throw 0; }
 virtual bool MsgIsLevelUnloaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsLevelUnloaded"); throw 0; }
 virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: MsgOnLevelLoaded"); throw 0; }
 virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: MsgOnLevelUnloaded"); throw 0; }
 virtual bool MsgIsRegionLoaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsRegionLoaded"); throw 0; }
 virtual bool MsgIsRegionUnloaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsRegionUnloaded"); throw 0; }
 virtual bool MsgOnRegionLoaded(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionLoaded"); throw 0; }
 virtual bool MsgOnRegionUnloaded(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionUnloaded"); throw 0; }
 virtual bool MsgOnRegionPreunload(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionPreunload"); throw 0; }
 virtual bool MsgOnQuestCompleted(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestCompleted"); throw 0; }
 virtual bool MsgOnAnyQuestCompleted(CCharString&) const { assert(!"unexpected interface call: MsgOnAnyQuestCompleted"); throw 0; }
 virtual bool MsgOnQuestFailed(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestFailed"); throw 0; }
 virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestCompletedBeforeScreenShown"); throw 0; }
 virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestFailedBeforeScreenShown"); throw 0; }
 virtual bool MsgOnQuestAccepted(CCharString&) const { assert(!"unexpected interface call: MsgOnQuestAccepted"); throw 0; }
 virtual bool MsgOnFeatAccepted(int&) const { assert(!"unexpected interface call: MsgOnFeatAccepted"); throw 0; }
 virtual bool MsgIsBoastMade(int) const { assert(!"unexpected interface call: MsgIsBoastMade"); throw 0; }
 virtual bool MsgOnBoastMade(int&, CCharString&) const { assert(!"unexpected interface call: MsgOnBoastMade"); throw 0; }
 virtual bool MsgOnBoastsMade(std::vector<std::pair<int,CCharString>,std::allocator<std::pair<int,CCharString> > >&) const { assert(!"unexpected interface call: MsgOnBoastsMade"); throw 0; }
 virtual void RemoveBoastMessage() const { assert(!"unexpected interface call: RemoveBoastMessage"); throw 0; }
 virtual bool IsQuestStartScreenActive(CCharString&) const { assert(!"unexpected interface call: IsQuestStartScreenActive"); throw 0; }
 virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const { assert(!"unexpected interface call: MsgOnLeavingQuestStartScreen"); throw 0; }
 virtual bool MsgOnLeavingExperienceSpendingScreen() const { assert(!"unexpected interface call: MsgOnLeavingExperienceSpendingScreen"); throw 0; }
 virtual int MsgIsQuestionAnsweredYesOrNo() const { assert(!"unexpected interface call: MsgIsQuestionAnsweredYesOrNo"); throw 0; }
 virtual bool MsgIsGameInfoClickedPast() const { assert(!"unexpected interface call: MsgIsGameInfoClickedPast"); throw 0; }
 virtual bool MsgIsTutorialClickedPast() const { assert(!"unexpected interface call: MsgIsTutorialClickedPast"); throw 0; }
 virtual bool MsgIsActionModeButtonPressed() const { assert(!"unexpected interface call: MsgIsActionModeButtonPressed"); throw 0; }
 virtual bool MsgOnExpressionPerformed(CCharString&) const { assert(!"unexpected interface call: MsgOnExpressionPerformed"); throw 0; }
 virtual bool MsgIsCutSceneSkipped() const { assert(!"unexpected interface call: MsgIsCutSceneSkipped"); throw 0; }
 virtual void RemoveAllCutSceneSkippedMessages() const { assert(!"unexpected interface call: RemoveAllCutSceneSkippedMessages"); throw 0; }
 virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const { assert(!"unexpected interface call: MsgOnHeroHairTypeChanged"); throw 0; }
 virtual bool MsgOnHeroUsedTeleporter(CCharString&) const { assert(!"unexpected interface call: MsgOnHeroUsedTeleporter"); throw 0; }
 virtual bool MsgOnHeroUsedGuildSeal() const { assert(!"unexpected interface call: MsgOnHeroUsedGuildSeal"); throw 0; }
 virtual bool MsgOnGameSavedManually() const { assert(!"unexpected interface call: MsgOnGameSavedManually"); throw 0; }
 virtual bool MsgOnHeroSlept(bool&) const { assert(!"unexpected interface call: MsgOnHeroSlept"); throw 0; }
 virtual bool MsgOnHeroFiredRangedWeapon() const { assert(!"unexpected interface call: MsgOnHeroFiredRangedWeapon"); throw 0; }
 virtual bool MsgOnHeroCastSpell(EHeroAbility&) const { assert(!"unexpected interface call: MsgOnHeroCastSpell"); throw 0; }
 virtual bool MsgOnHeroPickedPocket(CScriptThing&) const { assert(!"unexpected interface call: MsgOnHeroPickedPocket"); throw 0; }
 virtual bool MsgOnHeroPickedLock(CScriptThing&) const { assert(!"unexpected interface call: MsgOnHeroPickedLock"); throw 0; }
 virtual bool MsgOnFishingGameFinished(CScriptThing&) const { assert(!"unexpected interface call: MsgOnFishingGameFinished"); throw 0; }
 virtual bool MsgOnTavernGameFinished(const CScriptThing&) const { assert(!"unexpected interface call: MsgOnTavernGameFinished"); throw 0; }
 virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const { assert(!"unexpected interface call: MsgOnHeroRewardedWithItemsFrom"); throw 0; }
 virtual bool MsgOnChestOpeningCancelled() const { assert(!"unexpected interface call: MsgOnChestOpeningCancelled"); throw 0; }
 virtual void DontPopulateNextLoadedRegion() const { assert(!"unexpected interface call: DontPopulateNextLoadedRegion"); throw 0; }
 virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const { assert(!"unexpected interface call: GetWanderingPopulationScriptDefNameInCurrentRegion"); throw 0; }
 virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const { assert(!"unexpected interface call: GetWanderingPopulationScriptDefNameInRegion"); throw 0; }
 virtual bool IsHeroAllowedHenchmenInCurrentRegion() const { assert(!"unexpected interface call: IsHeroAllowedHenchmenInCurrentRegion"); throw 0; }
 virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const { assert(!"unexpected interface call: IsHeroAllowedHenchmenInRegion"); throw 0; }
 virtual void PostAddScriptedEntities() const { assert(!"unexpected interface call: PostAddScriptedEntities"); throw 0; }
 virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const { assert(!"unexpected interface call: IsPlayerHoldingUnsheatheRangedWeaponButton"); throw 0; }
 virtual bool IsPlayerHoldingLockTargetButton() const { assert(!"unexpected interface call: IsPlayerHoldingLockTargetButton"); throw 0; }
 virtual bool IsPlayerHoldingFireRangedWeaponButton() const { assert(!"unexpected interface call: IsPlayerHoldingFireRangedWeaponButton"); throw 0; }
 virtual bool IsPlayerHoldingFirstPersonTargetingButton() const { assert(!"unexpected interface call: IsPlayerHoldingFirstPersonTargetingButton"); throw 0; }
 virtual bool IsHeroInProjectileWeaponMode() const { assert(!"unexpected interface call: IsHeroInProjectileWeaponMode"); throw 0; }
 virtual CScriptThing& GetHero() { assert(!"unexpected interface call: GetHero"); throw 0; }
 virtual CScriptThing GetHeroTargetedThing() const { assert(!"unexpected interface call: GetHeroTargetedThing"); throw 0; }
 virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GetThingWithScriptName"); throw 0; }
 virtual CScriptThing GetThingWithScriptName(const CCharString&) const { assert(!"unexpected interface call: GetThingWithScriptName"); throw 0; }
 virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const { assert(!"unexpected interface call: GetRandomThingWithScriptName"); throw 0; }
 virtual int GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithScriptName"); throw 0; }
 virtual int GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllCreaturesInAreaWithScriptName"); throw 0; }
 virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetNearestWithScriptName"); throw 0; }
 virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetFurthestWithScriptName"); throw 0; }
 virtual int GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithDefNameByDistanceFrom"); throw 0; }
 virtual int GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithDefName"); throw 0; }
 virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetNearestWithDefName"); throw 0; }
 virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetFurthestWithDefName"); throw 0; }
 virtual CScriptThing GetThingWithUID(FableU64) const { assert(!"unexpected interface call: GetThingWithUID"); throw 0; }
 virtual int GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllCreaturesExcludingHero"); throw 0; }
 virtual int GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsInLevel"); throw 0; }
 virtual bool IsThingWithThisUIDAlive(FableU64) const { assert(!"unexpected interface call: IsThingWithThisUIDAlive"); throw 0; }
 virtual int RegisterTimer() { assert(!"unexpected interface call: RegisterTimer"); throw 0; }
 virtual void DeregisterTimer(int) { assert(!"unexpected interface call: DeregisterTimer"); throw 0; }
 virtual void SetTimer(int, int) { assert(!"unexpected interface call: SetTimer"); throw 0; }
 virtual int GetTimer(int) const { assert(!"unexpected interface call: GetTimer"); throw 0; }
 virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const { assert(!"unexpected interface call: CreateCreature"); throw 0; }
 virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const { assert(!"unexpected interface call: CreateCreatureNearby"); throw 0; }
 virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreateCreatureOnEntity"); throw 0; }
 virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: TurnCreatureInto"); throw 0; }
 virtual void SetCreatureCreationDelayFrames(int) { assert(!"unexpected interface call: SetCreatureCreationDelayFrames"); throw 0; }
 virtual void ResetCreatureCreationDelayFrames() { assert(!"unexpected interface call: ResetCreatureCreationDelayFrames"); throw 0; }
 virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const { assert(!"unexpected interface call: CreateObject"); throw 0; }
 virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const { assert(!"unexpected interface call: CreateObject"); throw 0; }
 virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreateObjectOnEntity"); throw 0; }
 virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: CreateEffect"); throw 0; }
 virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const { assert(!"unexpected interface call: CreateEffect"); throw 0; }
 virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const { assert(!"unexpected interface call: CreateLight"); throw 0; }
 virtual CScriptThing CreateExperienceOrb(const C3DVector&, int) const { assert(!"unexpected interface call: CreateExperienceOrb"); throw 0; }
 virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const { assert(!"unexpected interface call: CreateExplosion"); throw 0; }
 virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const { assert(!"unexpected interface call: CreatePhysicalBarrier"); throw 0; }
 virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const { assert(!"unexpected interface call: CreateRumble"); throw 0; }
 virtual void ClearAllRumbles() const { assert(!"unexpected interface call: ClearAllRumbles"); throw 0; }
 virtual void RemoveThing(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: RemoveThing"); throw 0; }
 virtual void ShowOnScreenMessage(const CCharString&, float) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const { assert(!"unexpected interface call: AddScreenMessage"); throw 0; }
 virtual void AddScreenTitleMessage(const CCharString&, float, bool) const { assert(!"unexpected interface call: AddScreenTitleMessage"); throw 0; }
 virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroYesNoQuestion"); throw 0; }
 virtual void DisplayGameInfo(const CCharString&) const { assert(!"unexpected interface call: DisplayGameInfo"); throw 0; }
 virtual void DisplayGameInfoText(const CWideString&) const { assert(!"unexpected interface call: DisplayGameInfoText"); throw 0; }
 virtual bool IsSafeToDisplayGameInfo() const { assert(!"unexpected interface call: IsSafeToDisplayGameInfo"); throw 0; }
 virtual bool DisplayTutorial(ETutorialCategory) const { assert(!"unexpected interface call: DisplayTutorial"); throw 0; }
 virtual bool IsTutorialSystemEnabled() const { assert(!"unexpected interface call: IsTutorialSystemEnabled"); throw 0; }
 virtual void GiveHeroWeapon(const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroWeapon"); throw 0; }
 virtual void GiveHeroObject(const CCharString&, int, bool) const { assert(!"unexpected interface call: GiveHeroObject"); throw 0; }
 virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const { assert(!"unexpected interface call: SetWeaponAsHerosActiveWeapon"); throw 0; }
 virtual void GiveHeroItem(const CScriptThing&) const { assert(!"unexpected interface call: GiveHeroItem"); throw 0; }
 virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const { assert(!"unexpected interface call: GiveHeroItemsFromContainer"); throw 0; }
 virtual void TakeObjectFromHero(const CCharString&) const { assert(!"unexpected interface call: TakeObjectFromHero"); throw 0; }
 virtual void GiveHeroGold(int) const { assert(!"unexpected interface call: GiveHeroGold"); throw 0; }
 virtual int GetHeroGold() const { assert(!"unexpected interface call: GetHeroGold"); throw 0; }
 virtual void GiveHeroExperience(int) const { assert(!"unexpected interface call: GiveHeroExperience"); throw 0; }
 virtual void SetHeroAbleToGainExperience(bool) const { assert(!"unexpected interface call: SetHeroAbleToGainExperience"); throw 0; }
 virtual void SheatheHeroWeapons() const { assert(!"unexpected interface call: SheatheHeroWeapons"); throw 0; }
 virtual void SetHeroWillAsUsable(bool) const { assert(!"unexpected interface call: SetHeroWillAsUsable"); throw 0; }
 virtual void SetHeroWeaponsAsUsable(bool) const { assert(!"unexpected interface call: SetHeroWeaponsAsUsable"); throw 0; }
 virtual void SetWeaponOutCrimeEnabled(bool) const { assert(!"unexpected interface call: SetWeaponOutCrimeEnabled"); throw 0; }
 virtual void SetGuardsIgnoreCrimes(bool) const { assert(!"unexpected interface call: SetGuardsIgnoreCrimes"); throw 0; }
 virtual void RemoveAllHeroWeapons() const { assert(!"unexpected interface call: RemoveAllHeroWeapons"); throw 0; }
 virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const { assert(!"unexpected interface call: IsReportedOrUnreportedCrimeKnown"); throw 0; }
 virtual void ConfiscateAllHeroItems() const { assert(!"unexpected interface call: ConfiscateAllHeroItems"); throw 0; }
 virtual void ConfiscateAllHeroWeapons() const { assert(!"unexpected interface call: ConfiscateAllHeroWeapons"); throw 0; }
 virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const { assert(!"unexpected interface call: ConfiscateItemsOfTypeFromHero"); throw 0; }
 virtual void ReturnAllConfiscatedItemsToHero() const { assert(!"unexpected interface call: ReturnAllConfiscatedItemsToHero"); throw 0; }
 virtual bool GiveHeroTutorial(ETutorialCategory) const { assert(!"unexpected interface call: GiveHeroTutorial"); throw 0; }
 virtual void MakeHeroCarryItemInHand(const CCharString&) const { assert(!"unexpected interface call: MakeHeroCarryItemInHand"); throw 0; }
 virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: MakeHeroCarryItemInHand"); throw 0; }
 virtual void AddTattooToHero(const CCharString&) const { assert(!"unexpected interface call: AddTattooToHero"); throw 0; }
 virtual void GiveHeroAbility(EHeroAbility, bool) const { assert(!"unexpected interface call: GiveHeroAbility"); throw 0; }
 virtual bool IsPlayerZTargetingThing(const CScriptThing&) const { assert(!"unexpected interface call: IsPlayerZTargetingThing"); throw 0; }
 virtual bool IsPlayerCreatureBlocking() const { assert(!"unexpected interface call: IsPlayerCreatureBlocking"); throw 0; }
 virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const { assert(!"unexpected interface call: IsPlayerCreatureReadyToFireProjectileWeapon"); throw 0; }
 virtual int GetPlayerCreatureCombatMultiplier() const { assert(!"unexpected interface call: GetPlayerCreatureCombatMultiplier"); throw 0; }
 virtual int GetPlayerCreatureCombatMultiplierRunningNumHits() const { assert(!"unexpected interface call: GetPlayerCreatureCombatMultiplierRunningNumHits"); throw 0; }
 virtual void ResetPlayerCreatureCombatMultiplier() const { assert(!"unexpected interface call: ResetPlayerCreatureCombatMultiplier"); throw 0; }
 virtual bool IsPlayerCreatureFlourishEnabled() const { assert(!"unexpected interface call: IsPlayerCreatureFlourishEnabled"); throw 0; }
 virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const { assert(!"unexpected interface call: SetPlayerCreatureOnlyTarget"); throw 0; }
 virtual void ResetPlayerCreatureOnlyTarget() const { assert(!"unexpected interface call: ResetPlayerCreatureOnlyTarget"); throw 0; }
 virtual void RespawnHero(bool) const { assert(!"unexpected interface call: RespawnHero"); throw 0; }
 virtual void GiveHeroMorality(float) const { assert(!"unexpected interface call: GiveHeroMorality"); throw 0; }
 virtual float GetHeroMorality() const { assert(!"unexpected interface call: GetHeroMorality"); throw 0; }
 virtual EMorality GetHeroMoralityCategory() const { assert(!"unexpected interface call: GetHeroMoralityCategory"); throw 0; }
 virtual void ChangeHeroMoralityDueToTheft() const { assert(!"unexpected interface call: ChangeHeroMoralityDueToTheft"); throw 0; }
 virtual void ChangeHeroMoralityDueToPicklock() const { assert(!"unexpected interface call: ChangeHeroMoralityDueToPicklock"); throw 0; }
 virtual void GiveHeroRenownPoints(int) const { assert(!"unexpected interface call: GiveHeroRenownPoints"); throw 0; }
 virtual int GetHeroRenownLevel() const { assert(!"unexpected interface call: GetHeroRenownLevel"); throw 0; }
 virtual bool IsHeroRenownLevelFull() const { assert(!"unexpected interface call: IsHeroRenownLevelFull"); throw 0; }
 virtual void IncreaseHeroRenownLevel() const { assert(!"unexpected interface call: IncreaseHeroRenownLevel"); throw 0; }
 virtual int GetHeroStrengthLevel() const { assert(!"unexpected interface call: GetHeroStrengthLevel"); throw 0; }
 virtual int GetHeroSkillLevel() const { assert(!"unexpected interface call: GetHeroSkillLevel"); throw 0; }
 virtual int GetHeroWillLevel() const { assert(!"unexpected interface call: GetHeroWillLevel"); throw 0; }
 virtual int GetHeroStatLevel(EHeroTrainableStatType) const { assert(!"unexpected interface call: GetHeroStatLevel"); throw 0; }
 virtual int GetHeroStatMax(EHeroTrainableStatType) const { assert(!"unexpected interface call: GetHeroStatMax"); throw 0; }
 virtual void SetHeroAge(float) const { assert(!"unexpected interface call: SetHeroAge"); throw 0; }
 virtual float GetHeroAge() const { assert(!"unexpected interface call: GetHeroAge"); throw 0; }
 virtual void SetHeroAsTeenager(bool) const { assert(!"unexpected interface call: SetHeroAsTeenager"); throw 0; }
 virtual void SetHeroAsApprentice(bool) const { assert(!"unexpected interface call: SetHeroAsApprentice"); throw 0; }
 virtual float GetDistanceHeroCanBeHeardFrom() const { assert(!"unexpected interface call: GetDistanceHeroCanBeHeardFrom"); throw 0; }
 virtual int GetHeroRoughExperienceLevel() const { assert(!"unexpected interface call: GetHeroRoughExperienceLevel"); throw 0; }
 virtual int GetHeroExperienceAvailableToSpend() const { assert(!"unexpected interface call: GetHeroExperienceAvailableToSpend"); throw 0; }
 virtual float GetHeroFatness() const { assert(!"unexpected interface call: GetHeroFatness"); throw 0; }
 virtual float GetHeroScariness() const { assert(!"unexpected interface call: GetHeroScariness"); throw 0; }
 virtual float GetHeroAttractiveness() const { assert(!"unexpected interface call: GetHeroAttractiveness"); throw 0; }
 virtual float GetHeroWillEnergyLevel() const { assert(!"unexpected interface call: GetHeroWillEnergyLevel"); throw 0; }
 virtual void SetHeroWillEnergyLevel(float) const { assert(!"unexpected interface call: SetHeroWillEnergyLevel"); throw 0; }
 virtual void SetHeroWillEnergyAsAbleToRefill(bool) const { assert(!"unexpected interface call: SetHeroWillEnergyAsAbleToRefill"); throw 0; }
 virtual int GetNumberOfItemsOfTypeInInventory(const CCharString&) const { assert(!"unexpected interface call: GetNumberOfItemsOfTypeInInventory"); throw 0; }
 virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const { assert(!"unexpected interface call: IsObjectInThingsPossession"); throw 0; }
 virtual bool IsHeroHandLampLit() const { assert(!"unexpected interface call: IsHeroHandLampLit"); throw 0; }
 virtual void SetHeroHandLampAsLit(bool) const { assert(!"unexpected interface call: SetHeroHandLampAsLit"); throw 0; }
 virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: IsWearingClothingItem"); throw 0; }
 virtual bool IsHeroNaked() const { assert(!"unexpected interface call: IsHeroNaked"); throw 0; }
 virtual void RemoveHeroClothing() const { assert(!"unexpected interface call: RemoveHeroClothing"); throw 0; }
 virtual void SetHeroAsWearing(const CCharString&) const { assert(!"unexpected interface call: SetHeroAsWearing"); throw 0; }
 virtual void ChangeHeroHairstyle(const CCharString&) const { assert(!"unexpected interface call: ChangeHeroHairstyle"); throw 0; }
 virtual void RemoveHeroHairstyle() const { assert(!"unexpected interface call: RemoveHeroHairstyle"); throw 0; }
 virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: IsWearingHairstyle"); throw 0; }
 virtual bool IsPlayerCarryingItemOfType(const CCharString&) const { assert(!"unexpected interface call: IsPlayerCarryingItemOfType"); throw 0; }
 virtual bool IsPlayerWieldingWeapon() const { assert(!"unexpected interface call: IsPlayerWieldingWeapon"); throw 0; }
 virtual bool IsEntityWieldingWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingWeapon"); throw 0; }
 virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingMeleeWeapon"); throw 0; }
 virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingRangedWeapon"); throw 0; }
 virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const { assert(!"unexpected interface call: GetPreviouslyWieldedMeleeWeaponName"); throw 0; }
 virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const { assert(!"unexpected interface call: GetPreviouslyWieldedRangedWeaponName"); throw 0; }
 virtual bool IsEntityAbleToAttack(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityAbleToAttack"); throw 0; }
 virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetThingInPrimarySlot"); throw 0; }
 virtual void ApplyHeroPenaltyForDeath() const { assert(!"unexpected interface call: ApplyHeroPenaltyForDeath"); throw 0; }
 virtual void GiveHeroTitle(const CCharString&) const { assert(!"unexpected interface call: GiveHeroTitle"); throw 0; }
 virtual EHeroTitle GetHeroTitle() const { assert(!"unexpected interface call: GetHeroTitle"); throw 0; }
 virtual void EntitySetAsMarryable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsMarryable"); throw 0; }
 virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAbleToRegionFollowWhenMarried"); throw 0; }
 virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityForceMarriageToHero"); throw 0; }
 virtual bool IsEntityMarriedToHero(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityMarriedToHero"); throw 0; }
 virtual bool IsEntityMarriable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityMarriable"); throw 0; }
 virtual bool GetHeroHasMarried() const { assert(!"unexpected interface call: GetHeroHasMarried"); throw 0; }
 virtual bool GetHeroHasCurrentMarriage() const { assert(!"unexpected interface call: GetHeroHasCurrentMarriage"); throw 0; }
 virtual bool GetHeroHasDivorcedMarriage() const { assert(!"unexpected interface call: GetHeroHasDivorcedMarriage"); throw 0; }
 virtual bool GetHeroHasChildren() const { assert(!"unexpected interface call: GetHeroHasChildren"); throw 0; }
 virtual bool GetHeroHasMurderedWife() const { assert(!"unexpected interface call: GetHeroHasMurderedWife"); throw 0; }
 virtual bool IsHeroChild() const { assert(!"unexpected interface call: IsHeroChild"); throw 0; }
 virtual void CancelHeroTeleportEffects() const { assert(!"unexpected interface call: CancelHeroTeleportEffects"); throw 0; }
 virtual int GetNumberOfTimesHeroHasHadSex() const { assert(!"unexpected interface call: GetNumberOfTimesHeroHasHadSex"); throw 0; }
 virtual void SetNumberOfTimesHeroHasHadSex(int) const { assert(!"unexpected interface call: SetNumberOfTimesHeroHasHadSex"); throw 0; }
 virtual void SetHeroAsHavingHadSex(bool) const { assert(!"unexpected interface call: SetHeroAsHavingHadSex"); throw 0; }
 virtual void SetHeroAsHavingHadGaySex(bool) const { assert(!"unexpected interface call: SetHeroAsHavingHadGaySex"); throw 0; }
 virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GiveThingHeroRewardItem"); throw 0; }
 virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveThingItemInHand"); throw 0; }
 virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GiveThingItemInSlot"); throw 0; }
 virtual void GiveHeroExpression(const CCharString&, int, bool) const { assert(!"unexpected interface call: GiveHeroExpression"); throw 0; }
 virtual bool HeroHasExpression(const CCharString&) const { assert(!"unexpected interface call: HeroHasExpression"); throw 0; }
 virtual bool IsHeroPerformingExpression(const CCharString&) const { assert(!"unexpected interface call: IsHeroPerformingExpression"); throw 0; }
 virtual bool IsDPadButtonHeldForExpression(const CCharString&) const { assert(!"unexpected interface call: IsDPadButtonHeldForExpression"); throw 0; }
 virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntityFollowThing"); throw 0; }
 virtual void EntityStopFollowing(const CScriptThing&) const { assert(!"unexpected interface call: EntityStopFollowing"); throw 0; }
 virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetFollowingEntityList"); throw 0; }
 virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetHeroSummonedCreaturesList"); throw 0; }
 virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetPerceivingHeroEntityList"); throw 0; }
 virtual bool IsEntityFollowingHero(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityFollowingHero"); throw 0; }
 virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAllowedToFollowHero"); throw 0; }
 virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAllowedToChangeRegionFollowingState"); throw 0; }
 virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsRespondingToFollowAndWaitExpressions"); throw 0; }
 virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsMirroringHeroEnemyRelationsWhileFollowing"); throw 0; }
 virtual void TeleportAllFollowersToHeroPosition() const { assert(!"unexpected interface call: TeleportAllFollowersToHeroPosition"); throw 0; }
 virtual void EntityTeleportToHeroPosition(const CScriptThing&) const { assert(!"unexpected interface call: EntityTeleportToHeroPosition"); throw 0; }
 virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const { assert(!"unexpected interface call: SendEntityEvent"); throw 0; }
 virtual float GetWaterHeightAtPosition(const C3DVector&) const { assert(!"unexpected interface call: GetWaterHeightAtPosition"); throw 0; }
 virtual bool IsFishingSpotEnabled(const CScriptThing&) const { assert(!"unexpected interface call: IsFishingSpotEnabled"); throw 0; }
 virtual void DisableFishingSpot(const CScriptThing&) const { assert(!"unexpected interface call: DisableFishingSpot"); throw 0; }
 virtual void UpdateFishWeight(const CScriptThing&) const { assert(!"unexpected interface call: UpdateFishWeight"); throw 0; }
 virtual float GetBestFishWeight(const CCharString&) const { assert(!"unexpected interface call: GetBestFishWeight"); throw 0; }
 virtual void HeroGoFishing(bool) const { assert(!"unexpected interface call: HeroGoFishing"); throw 0; }
 virtual int GetHeroFishingLevel() const { assert(!"unexpected interface call: GetHeroFishingLevel"); throw 0; }
 virtual void HeroGoDigging() const { assert(!"unexpected interface call: HeroGoDigging"); throw 0; }
 virtual void HeroStopDigging() const { assert(!"unexpected interface call: HeroStopDigging"); throw 0; }
 virtual void HeroPlayOracleMinigame() const { assert(!"unexpected interface call: HeroPlayOracleMinigame"); throw 0; }
 virtual bool IsHeroPlayingOracleMinigame() const { assert(!"unexpected interface call: IsHeroPlayingOracleMinigame"); throw 0; }
 virtual bool HasHeroWonOracleMinigame() const { assert(!"unexpected interface call: HasHeroWonOracleMinigame"); throw 0; }
 virtual void HeroPlayFireheartMinigame() const { assert(!"unexpected interface call: HeroPlayFireheartMinigame"); throw 0; }
 virtual void HeroQuitFireheartMinigame() const { assert(!"unexpected interface call: HeroQuitFireheartMinigame"); throw 0; }
 virtual bool HasHeroForceQuitFireheartMinigame() const { assert(!"unexpected interface call: HasHeroForceQuitFireheartMinigame"); throw 0; }
 virtual float GetHeroHealth() const { assert(!"unexpected interface call: GetHeroHealth"); throw 0; }
 virtual float GetHeroHealthMax() const { assert(!"unexpected interface call: GetHeroHealthMax"); throw 0; }
 virtual float GetHeroHealthPercentage() const { assert(!"unexpected interface call: GetHeroHealthPercentage"); throw 0; }
 virtual int GetHeroWillEnergy() const { assert(!"unexpected interface call: GetHeroWillEnergy"); throw 0; }
 virtual int GetHeroWillEnergyMax() const { assert(!"unexpected interface call: GetHeroWillEnergyMax"); throw 0; }
 virtual int GetHeroWillEnergyPercentage() const { assert(!"unexpected interface call: GetHeroWillEnergyPercentage"); throw 0; }
 virtual void ChangeHeroHealthBy(float, bool, bool) const { assert(!"unexpected interface call: ChangeHeroHealthBy"); throw 0; }
 virtual float GetHealth(const CScriptThing&) const { assert(!"unexpected interface call: GetHealth"); throw 0; }
 virtual void ModifyThingHealth(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: ModifyThingHealth"); throw 0; }
 virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetMaxHealth"); throw 0; }
 virtual void SetThingAsKilled(const CScriptThing&) const { assert(!"unexpected interface call: SetThingAsKilled"); throw 0; }
 virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned int) const { assert(!"unexpected interface call: GiveHeroNewQuestObjective"); throw 0; }
 virtual void TellHeroQuestObjectiveCompleted(unsigned int) const { assert(!"unexpected interface call: TellHeroQuestObjectiveCompleted"); throw 0; }
 virtual void TellHeroQuestObjectiveFailed(unsigned int) const { assert(!"unexpected interface call: TellHeroQuestObjectiveFailed"); throw 0; }
 virtual void AddQuestRegion(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddQuestRegion"); throw 0; }
 virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const { assert(!"unexpected interface call: SetQuestWorldMapOffset"); throw 0; }
 virtual bool IsHeroOnQuest() const { assert(!"unexpected interface call: IsHeroOnQuest"); throw 0; }
 virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: HeroReceiveMessageFromGuildMaster"); throw 0; }
 virtual void SetGuildMasterMessages(bool) const { assert(!"unexpected interface call: SetGuildMasterMessages"); throw 0; }
 virtual void ActivateQuest(const CCharString&) const { assert(!"unexpected interface call: ActivateQuest"); throw 0; }
 virtual void ActivateMultipleQuests(const CArray<CCharString>&) const { assert(!"unexpected interface call: ActivateMultipleQuests"); throw 0; }
 virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const { assert(!"unexpected interface call: ActivateQuestWithoutLoadingResources"); throw 0; }
 virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const { assert(!"unexpected interface call: ActivateMultipleQuestsWithoutLoadingResources"); throw 0; }
 virtual void DeactivateQuest(const CCharString&, unsigned int) const { assert(!"unexpected interface call: DeactivateQuest"); throw 0; }
 virtual void DeactivateQuestLater(const CCharString&, unsigned int) const { assert(!"unexpected interface call: DeactivateQuestLater"); throw 0; }
 virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const { assert(!"unexpected interface call: PrepareQuestsWhenFinalQuestIsActivated"); throw 0; }
 virtual void PrepareQuestsWhenFinalQuestIsCompleted() const { assert(!"unexpected interface call: PrepareQuestsWhenFinalQuestIsCompleted"); throw 0; }
 virtual bool IsQuestActive(const CCharString&) const { assert(!"unexpected interface call: IsQuestActive"); throw 0; }
 virtual bool IsQuestRegistered(const CCharString&) const { assert(!"unexpected interface call: IsQuestRegistered"); throw 0; }
 virtual bool IsQuestCompleted(const CCharString&) const { assert(!"unexpected interface call: IsQuestCompleted"); throw 0; }
 virtual bool IsQuestFailed(const CCharString&) const { assert(!"unexpected interface call: IsQuestFailed"); throw 0; }
 virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const { assert(!"unexpected interface call: SetQuestAsCompleted"); throw 0; }
 virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const { assert(!"unexpected interface call: SetQuestAsFailed"); throw 0; }
 virtual void SetQuestAsPersistent(const CCharString&, bool) const { assert(!"unexpected interface call: SetQuestAsPersistent"); throw 0; }
 virtual const CCharString& GetExclusiveQuestScriptName() const { assert(!"unexpected interface call: GetExclusiveQuestScriptName"); throw 0; }
 virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: AddQuestCard"); throw 0; }
 virtual void RemoveQuestCardFromGuild(const CCharString&) const { assert(!"unexpected interface call: RemoveQuestCardFromGuild"); throw 0; }
 virtual void RemoveQuestCardFromHero(const CCharString&) const { assert(!"unexpected interface call: RemoveQuestCardFromHero"); throw 0; }
 virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroQuestCardDirectly"); throw 0; }
 virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetQuestCardObjective"); throw 0; }
 virtual void SetQuestCardGoldReward(const CCharString&, int) const { assert(!"unexpected interface call: SetQuestCardGoldReward"); throw 0; }
 virtual void SetQuestCardRenownReward(const CCharString&, int) const { assert(!"unexpected interface call: SetQuestCardRenownReward"); throw 0; }
 virtual void RemoveAllAvailableQuestCardsFromGuild() const { assert(!"unexpected interface call: RemoveAllAvailableQuestCardsFromGuild"); throw 0; }
 virtual void FailAllActiveQuests() const { assert(!"unexpected interface call: FailAllActiveQuests"); throw 0; }
 virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: GetAllActiveQuestInfo"); throw 0; }
 virtual void AddFeatCard(int, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddFeatCard"); throw 0; }
 virtual void AddBoast(const CCharString&, int, int, int, bool, const CCharString&, int) const { assert(!"unexpected interface call: AddBoast"); throw 0; }
 virtual void RemoveBoast(int, const CCharString&) const { assert(!"unexpected interface call: RemoveBoast"); throw 0; }
 virtual void SetBoastAsFailed(int, const CCharString&) const { assert(!"unexpected interface call: SetBoastAsFailed"); throw 0; }
 virtual void SetBoastAsCompleted(int, const CCharString&) const { assert(!"unexpected interface call: SetBoastAsCompleted"); throw 0; }
 virtual bool IsBoastTaken(int, const CCharString&) const { assert(!"unexpected interface call: IsBoastTaken"); throw 0; }
 virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, int) const { assert(!"unexpected interface call: AddLogBookEntry"); throw 0; }
 virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const { assert(!"unexpected interface call: KickOffQuestStartScreen"); throw 0; }
 virtual void KickOffDeathScreen() const { assert(!"unexpected interface call: KickOffDeathScreen"); throw 0; }
 virtual void KickOffCreditsScreen(CCharString&) const { assert(!"unexpected interface call: KickOffCreditsScreen"); throw 0; }
 virtual void SetPreferredQuickAccessItem(const CCharString&, int, int) const { assert(!"unexpected interface call: SetPreferredQuickAccessItem"); throw 0; }
 virtual CCharString GetDeathRecoveryMarkerName() const { assert(!"unexpected interface call: GetDeathRecoveryMarkerName"); throw 0; }
 virtual void SetDeathRecoveryMarkerName(const CCharString&) const { assert(!"unexpected interface call: SetDeathRecoveryMarkerName"); throw 0; }
 virtual void ResetDeathRecoveryMarkerNameToDefault() const { assert(!"unexpected interface call: ResetDeathRecoveryMarkerNameToDefault"); throw 0; }
 virtual bool IsToFailQuestOnDeath() const { assert(!"unexpected interface call: IsToFailQuestOnDeath"); throw 0; }
 virtual void SetWhetherToFailQuestOnDeath(bool) const { assert(!"unexpected interface call: SetWhetherToFailQuestOnDeath"); throw 0; }
 virtual void ResetWhetherToFailQuestOnDeathToDefault() const { assert(!"unexpected interface call: ResetWhetherToFailQuestOnDeathToDefault"); throw 0; }
 virtual CScriptThing GetMostRecentValidUsedTarget() const { assert(!"unexpected interface call: GetMostRecentValidUsedTarget"); throw 0; }
 virtual CCharString GetMostRecentValidUsedTargetName() const { assert(!"unexpected interface call: GetMostRecentValidUsedTargetName"); throw 0; }
 virtual void DisplayQuestInfo(bool) const { assert(!"unexpected interface call: DisplayQuestInfo"); throw 0; }
 virtual void SetQuestInfoName(const char*) const { assert(!"unexpected interface call: SetQuestInfoName"); throw 0; }
 virtual void SetQuestInfoText(const char*) const { assert(!"unexpected interface call: SetQuestInfoText"); throw 0; }
 virtual int AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoBar"); throw 0; }
 virtual int AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoBarHealth"); throw 0; }
 virtual int AddQuestInfoTimer(const CTimer&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoTimer"); throw 0; }
 virtual int AddQuestInfoCounter(const CCounter&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoCounter"); throw 0; }
 virtual int AddQuestInfoCounter(const CCharString&, int, float) const { assert(!"unexpected interface call: AddQuestInfoCounter"); throw 0; }
 virtual int AddQuestInfoCounterList(const CCharString&, int, float) const { assert(!"unexpected interface call: AddQuestInfoCounterList"); throw 0; }
 virtual int AddQuestInfoTick(const CCharString&, bool, float) const { assert(!"unexpected interface call: AddQuestInfoTick"); throw 0; }
 virtual int AddQuestInfoTick(EGameAction, bool, float) const { assert(!"unexpected interface call: AddQuestInfoTick"); throw 0; }
 virtual void UpdateQuestInfoBar(int, float, float, float) const { assert(!"unexpected interface call: UpdateQuestInfoBar"); throw 0; }
 virtual void ChangeQuestInfoBarColour(int, const CRGBColour&, const CRGBColour&) const { assert(!"unexpected interface call: ChangeQuestInfoBarColour"); throw 0; }
 virtual void UpdateQuestInfoTimer(int, float) const { assert(!"unexpected interface call: UpdateQuestInfoTimer"); throw 0; }
 virtual void UpdateQuestInfoCounter(int, int, int) const { assert(!"unexpected interface call: UpdateQuestInfoCounter"); throw 0; }
 virtual void UpdateQuestInfoCounterList(int, int, int) const { assert(!"unexpected interface call: UpdateQuestInfoCounterList"); throw 0; }
 virtual void UpdateQuestInfoTick(int, bool) const { assert(!"unexpected interface call: UpdateQuestInfoTick"); throw 0; }
 virtual void RemoveQuestInfoElement(int) const { assert(!"unexpected interface call: RemoveQuestInfoElement"); throw 0; }
 virtual void RemoveAllQuestInfoElements() const { assert(!"unexpected interface call: RemoveAllQuestInfoElements"); throw 0; }
 virtual void DisplayTime(bool) const { assert(!"unexpected interface call: DisplayTime"); throw 0; }
 virtual void DisplayMoneyBag(bool) const { assert(!"unexpected interface call: DisplayMoneyBag"); throw 0; }
 virtual void DisplayMiniGameInfo(bool, EMiniGameType) const { assert(!"unexpected interface call: DisplayMiniGameInfo"); throw 0; }
 virtual void UpdateMiniGameInfoBar(float) const { assert(!"unexpected interface call: UpdateMiniGameInfoBar"); throw 0; }
 virtual bool IsEntityPickPocketable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityPickPocketable"); throw 0; }
 virtual bool IsEntityPickLockable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityPickLockable"); throw 0; }
 virtual bool IsEntityStealable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityStealable"); throw 0; }
 virtual void EntitySetAsPickPocketed(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsPickPocketed"); throw 0; }
 virtual void EntitySetAsPickLocked(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsPickLocked"); throw 0; }
 virtual void EntitySetAsStolen(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsStolen"); throw 0; }
 virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: MiniMapAddMarker"); throw 0; }
 virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: MiniMapSetMarkerGraphic"); throw 0; }
 virtual void MiniMapRemoveMarker(const CScriptThing&) const { assert(!"unexpected interface call: MiniMapRemoveMarker"); throw 0; }
 virtual void MiniMapRemoveAllMarkers() const { assert(!"unexpected interface call: MiniMapRemoveAllMarkers"); throw 0; }
 virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: MiniMapAllowRouteBetweenRegions"); throw 0; }
 virtual void MiniMapSetAsEnabled(bool) const { assert(!"unexpected interface call: MiniMapSetAsEnabled"); throw 0; }
 virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsHiddenOnMiniMap"); throw 0; }
 virtual void SetHUDEnabled(bool) const { assert(!"unexpected interface call: SetHUDEnabled"); throw 0; }
 virtual bool TextEntryExists(const CCharString&) const { assert(!"unexpected interface call: TextEntryExists"); throw 0; }
 virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetValidTextEntryNameWithAttitude"); throw 0; }
 virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const { assert(!"unexpected interface call: SetThingHasInformation"); throw 0; }
 virtual void ClearThingHasInformation(const CScriptThing&) const { assert(!"unexpected interface call: ClearThingHasInformation"); throw 0; }
 virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetWillBeUsingNarrator"); throw 0; }
 virtual void EntityResetAsPureAINarrator(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetAsPureAINarrator"); throw 0; }
 virtual int AddNewConversation(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: AddNewConversation"); throw 0; }
 virtual void AddPersonToConversation(int, const CScriptThing&) const { assert(!"unexpected interface call: AddPersonToConversation"); throw 0; }
 virtual void AddLineToConversation(int, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: AddLineToConversation"); throw 0; }
 virtual void RemoveConversation(int, bool) const { assert(!"unexpected interface call: RemoveConversation"); throw 0; }
 virtual bool IsConversationActive(int) const { assert(!"unexpected interface call: IsConversationActive"); throw 0; }
 virtual void PlayAVIMovie(const CCharString&) const { assert(!"unexpected interface call: PlayAVIMovie"); throw 0; }
 virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) { assert(!"unexpected interface call: StartMovieSequence"); throw 0; }
 virtual void FixMovieSequenceCamera(bool) const { assert(!"unexpected interface call: FixMovieSequenceCamera"); throw 0; }
 virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { assert(!"unexpected interface call: FadeScreenOutUntilNextCallToFadeScreenIn"); throw 0; }
 virtual bool FadeScreenOut(float, float, CRGBColour) { assert(!"unexpected interface call: FadeScreenOut"); throw 0; }
 virtual void FadeScreenIn() { assert(!"unexpected interface call: FadeScreenIn"); throw 0; }
 virtual bool IsScreenFadingOut() const { assert(!"unexpected interface call: IsScreenFadingOut"); throw 0; }
 virtual void Pause(float) const { assert(!"unexpected interface call: Pause"); throw 0; }
 virtual void EndCutFade() { assert(!"unexpected interface call: EndCutFade"); throw 0; }
 virtual void EndLetterBox() { assert(!"unexpected interface call: EndLetterBox"); throw 0; }
 virtual void PauseAllNonScriptedEntities(bool) { assert(!"unexpected interface call: PauseAllNonScriptedEntities"); throw 0; }
 virtual void PauseAllEntities(bool) { assert(!"unexpected interface call: PauseAllEntities"); throw 0; }
 virtual void SetAllowScreenFadingOnNextRegionChange(bool) { assert(!"unexpected interface call: SetAllowScreenFadingOnNextRegionChange"); throw 0; }
 virtual void SetAllowScreenFadingIfAlreadyFaded(bool) { assert(!"unexpected interface call: SetAllowScreenFadingIfAlreadyFaded"); throw 0; }
 virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const { assert(!"unexpected interface call: SetToKeepHeroAbilitiesDuringCutscenes"); throw 0; }
 virtual void SetToDisplayTutorialsDuringCutscenes(bool) const { assert(!"unexpected interface call: SetToDisplayTutorialsDuringCutscenes"); throw 0; }
 virtual void SetCutsceneMode(bool, bool) const { assert(!"unexpected interface call: SetCutsceneMode"); throw 0; }
 virtual bool IsInCutscene() const { assert(!"unexpected interface call: IsInCutscene"); throw 0; }
 virtual void SetCutsceneSkippable(bool) const { assert(!"unexpected interface call: SetCutsceneSkippable"); throw 0; }
 virtual void SetCutsceneSkippableWhilePaused(bool) const { assert(!"unexpected interface call: SetCutsceneSkippableWhilePaused"); throw 0; }
 virtual void SetCutsceneActionMode(bool, const CCharString&) const { assert(!"unexpected interface call: SetCutsceneActionMode"); throw 0; }
 virtual void PreloadNewScene(float) const { assert(!"unexpected interface call: PreloadNewScene"); throw 0; }
 virtual void StartProgressDisplay() const { assert(!"unexpected interface call: StartProgressDisplay"); throw 0; }
 virtual void StopProgressDisplay() const { assert(!"unexpected interface call: StopProgressDisplay"); throw 0; }
 virtual void SetScreenMessagesEnabled(bool) const { assert(!"unexpected interface call: SetScreenMessagesEnabled"); throw 0; }
 virtual bool IsHeroControlledByPlayer() const { assert(!"unexpected interface call: IsHeroControlledByPlayer"); throw 0; }
 virtual bool IsInMovieSequence() const { assert(!"unexpected interface call: IsInMovieSequence"); throw 0; }
 virtual void CancelAbilitiesForCutscene() const { assert(!"unexpected interface call: CancelAbilitiesForCutscene"); throw 0; }
 virtual void ResumeAbilitiesForCutscene() const { assert(!"unexpected interface call: ResumeAbilitiesForCutscene"); throw 0; }
 virtual void CancelUsingAbility(EHeroAbility) const { assert(!"unexpected interface call: CancelUsingAbility"); throw 0; }
 virtual void SetAbilityAvailability(EHeroAbility, bool) const { assert(!"unexpected interface call: SetAbilityAvailability"); throw 0; }
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const { assert(!"unexpected interface call: SetEnvironmentalEffectsAlwaysUpdate"); throw 0; }
 virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const { assert(!"unexpected interface call: SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden"); throw 0; }
 virtual void RemoveDeadCreature(const CCharString&) const { assert(!"unexpected interface call: RemoveDeadCreature"); throw 0; }
 virtual void CameraSetCameraPreloadFlag(bool) const { assert(!"unexpected interface call: CameraSetCameraPreloadFlag"); throw 0; }
 virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraCircleAroundThing"); throw 0; }
 virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraCircleAroundPos"); throw 0; }
 virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraMoveToPosAndLookAtPos"); throw 0; }
 virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const { assert(!"unexpected interface call: CameraMoveToPosAndLookAtThing"); throw 0; }
 virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookingAt"); throw 0; }
 virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookingAt"); throw 0; }
 virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookFromAndLookTo"); throw 0; }
 virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, int, int) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, int, int) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, int, int) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, int, int) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const { assert(!"unexpected interface call: CameraDoConversation"); throw 0; }
 virtual void CameraDefault() { assert(!"unexpected interface call: CameraDefault"); throw 0; }
 virtual void CameraResetToViewBehindHero(float) { assert(!"unexpected interface call: CameraResetToViewBehindHero"); throw 0; }
 virtual bool IsCameraInScriptedMode() const { assert(!"unexpected interface call: IsCameraInScriptedMode"); throw 0; }
 virtual void CameraUseScreenEffect(float, float, float) const { assert(!"unexpected interface call: CameraUseScreenEffect"); throw 0; }
 virtual void CameraCancelScreenEffect() const { assert(!"unexpected interface call: CameraCancelScreenEffect"); throw 0; }
 virtual bool IsCameraPosOnScreen(const C3DVector&) const { assert(!"unexpected interface call: IsCameraPosOnScreen"); throw 0; }
 virtual float GetGameAngleXY() const { assert(!"unexpected interface call: GetGameAngleXY"); throw 0; }
 virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const { assert(!"unexpected interface call: CameraEarthquakeIntensityAtPos"); throw 0; }
 virtual void CameraShake(float, float) const { assert(!"unexpected interface call: CameraShake"); throw 0; }
 virtual void OpenDoor(const CScriptThing&) const { assert(!"unexpected interface call: OpenDoor"); throw 0; }
 virtual void CloseDoor(const CScriptThing&) const { assert(!"unexpected interface call: CloseDoor"); throw 0; }
 virtual void OpenHouseDoors(const CScriptThing&) const { assert(!"unexpected interface call: OpenHouseDoors"); throw 0; }
 virtual void CloseHouseDoors(const CScriptThing&) const { assert(!"unexpected interface call: CloseHouseDoors"); throw 0; }
 virtual void JamDoor(const CScriptThing&) const { assert(!"unexpected interface call: JamDoor"); throw 0; }
 virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const { assert(!"unexpected interface call: SetDoorTriggerType"); throw 0; }
 virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const { assert(!"unexpected interface call: OverrideAutomaticHouseLocking"); throw 0; }
 virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: SetHouseOwnedByPlayer"); throw 0; }
 virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetBuyableHouseAsScripted"); throw 0; }
 virtual bool IsChestOpen(const CScriptThing&) const { assert(!"unexpected interface call: IsChestOpen"); throw 0; }
 virtual bool OpenChest(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpenChest"); throw 0; }
 virtual void CloseChest(const CScriptThing&) const { assert(!"unexpected interface call: CloseChest"); throw 0; }
 virtual int GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const { assert(!"unexpected interface call: GetNumberOfKeysNeededToUnlockChest"); throw 0; }
 virtual void DisplayLockedChestMessage(const CScriptThing&) const { assert(!"unexpected interface call: DisplayLockedChestMessage"); throw 0; }
 virtual void SetTrophyAsMountable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTrophyAsMountable"); throw 0; }
 virtual void SetVillageLimbo(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetVillageLimbo"); throw 0; }
 virtual void SetCreatureNotReload(const CScriptThing&) const { assert(!"unexpected interface call: SetCreatureNotReload"); throw 0; }
 virtual bool IsSleepingTime(const CScriptThing&) const { assert(!"unexpected interface call: IsSleepingTime"); throw 0; }
 virtual void EnableGuards(const CScriptThing&, bool) const { assert(!"unexpected interface call: EnableGuards"); throw 0; }
 virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const { assert(!"unexpected interface call: EnableVillagerDefTypes"); throw 0; }
 virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const { assert(!"unexpected interface call: TryToRespawnDefNamed"); throw 0; }
 virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const { assert(!"unexpected interface call: ClearHeroEnemyOfGuards"); throw 0; }
 virtual void SetThingAsUsable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetThingAsUsable"); throw 0; }
 virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: SetThingHomeBuilding"); throw 0; }
 virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const { assert(!"unexpected interface call: SetVillageAttitude"); throw 0; }
 virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const { assert(!"unexpected interface call: AddCrimeCommitted"); throw 0; }
 virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: GiveThingBestEnemyTarget"); throw 0; }
 virtual void ClearThingBestEnemyTarget(const CScriptThing&) const { assert(!"unexpected interface call: ClearThingBestEnemyTarget"); throw 0; }
 virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetInLimbo"); throw 0; }
 virtual bool IsEntityInLimbo(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityInLimbo"); throw 0; }
 virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const { assert(!"unexpected interface call: EntityGetShotStrikePos"); throw 0; }
 virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetNegateAllHits"); throw 0; }
 virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetEvadeAllHits"); throw 0; }
 virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAbleToBeEngagedInCombat"); throw 0; }
 virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAlwaysBlockAttacksFromThing"); throw 0; }
 virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetAttackThingImmediately"); throw 0; }
 virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetCombatType"); throw 0; }
 virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetCombatTypeToDefault"); throw 0; }
 virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetMaxNumberOfAttackers"); throw 0; }
 virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearMaxNumberOfAttackers"); throw 0; }
 virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntityAttachToScript"); throw 0; }
 virtual void EntitySetCombatAbility(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetCombatAbility"); throw 0; }
 virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetRangedTarget"); throw 0; }
 virtual void EntityClearRangedTarget(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearRangedTarget"); throw 0; }
 virtual void EntitySetTargetable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetTargetable"); throw 0; }
 virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const { assert(!"unexpected interface call: EntitySetTargetingType"); throw 0; }
 virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetTargetingValidTargetWithoutLOS"); throw 0; }
 virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityTeleportToThing"); throw 0; }
 virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { assert(!"unexpected interface call: EntityTeleportToPosition"); throw 0; }
 virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetFacingAngle"); throw 0; }
 virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetFacingAngleTowardsThing"); throw 0; }
 virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { assert(!"unexpected interface call: EntitySetPerceptionVariables"); throw 0; }
 virtual void SetThingPersistent(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetThingPersistent"); throw 0; }
 virtual void EntitySetThingAsWantingMoney(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetThingAsWantingMoney"); throw 0; }
 virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetAppearanceMorphSeed"); throw 0; }
 virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: SetEntityAsRegionFollowing"); throw 0; }
 virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetEntityAsFollowingHeroThroughTeleporters"); throw 0; }
 virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned int) const { assert(!"unexpected interface call: EntitySetAppearanceSeed"); throw 0; }
 virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned int&) const { assert(!"unexpected interface call: EntityGetAppearanceSeed"); throw 0; }
 virtual void EntitySetAsForSale(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsForSale"); throw 0; }
 virtual void EntitySetStockItemPrice(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetStockItemPrice"); throw 0; }
 virtual int EntityGetStockItemPrice(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStockItemPrice"); throw 0; }
 virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: EntityPlayObjectAnimation"); throw 0; }
 virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetMaxRunningSpeed"); throw 0; }
 virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetMaxWalkingSpeed"); throw 0; }
 virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetMaxRunningSpeed"); throw 0; }
 virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetMaxWalkingSpeed"); throw 0; }
 virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityAttachToVillage"); throw 0; }
 virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsSittingOnFloor"); throw 0; }
 virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsHavingBoundHands"); throw 0; }
 virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsRemoveAllMovementBlockingModes"); throw 0; }
 virtual void EntitySetAsScared(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsScared"); throw 0; }
 virtual void EntitySetAsDrunk(CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDrunk"); throw 0; }
 virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityForceToLookAtThing"); throw 0; }
 virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityForceToLookAtCamera"); throw 0; }
 virtual void EntityForceToLookAtNothing(const CScriptThing&) const { assert(!"unexpected interface call: EntityForceToLookAtNothing"); throw 0; }
 virtual void EntityResetForceToLookAt(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetForceToLookAt"); throw 0; }
 virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetShotAccuracyPercentage"); throw 0; }
 virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStandingOnThing"); throw 0; }
 virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStandingInsideBuilding"); throw 0; }
 virtual void EntityDropGenericBox(const CScriptThing&) const { assert(!"unexpected interface call: EntityDropGenericBox"); throw 0; }
 virtual void EntitySheatheWeapons(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySheatheWeapons"); throw 0; }
 virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheWeapons"); throw 0; }
 virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheMeleeWeapon"); throw 0; }
 virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheRangedWeapon"); throw 0; }
 virtual void EntitySetAlpha(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetAlpha"); throw 0; }
 virtual void EntitySetAsDrawable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDrawable"); throw 0; }
 virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const { assert(!"unexpected interface call: EntitySetCutsceneBehaviour"); throw 0; }
 virtual ESex EntityGetSex(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetSex"); throw 0; }
 virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAbleToWalkThroughSolidObjects"); throw 0; }
 virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsRespondToHit"); throw 0; }
 virtual void EntitySetAsDamageable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDamageable"); throw 0; }
 virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetAsKillable"); throw 0; }
 virtual void EntitySetAsLocked(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsLocked"); throw 0; }
 virtual void EntityDecapitate(const CScriptThing&) const { assert(!"unexpected interface call: EntityDecapitate"); throw 0; }
 virtual void EntityGiveGold(const CScriptThing&, int) const { assert(!"unexpected interface call: EntityGiveGold"); throw 0; }
 virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAllowBossPhaseChanges"); throw 0; }
 virtual int EntityGetBossPhase(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetBossPhase"); throw 0; }
 virtual void EntitySetBossPhase(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetBossPhase"); throw 0; }
 virtual void EntityResetCreatureMode(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetCreatureMode"); throw 0; }
 virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsReceivingEvents"); throw 0; }
 virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsToAddToComboMultiplierWhenHit"); throw 0; }
 virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsToAddToStatChangesWhenHit"); throw 0; }
 virtual void EntityLeaveCombatStance(const CScriptThing&) const { assert(!"unexpected interface call: EntityLeaveCombatStance"); throw 0; }
 virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsUseMovementInActions"); throw 0; }
 virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDisplayingEmoteIcon"); throw 0; }
 virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsCollidableToThings"); throw 0; }
 virtual void EntityEnableGravity(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityEnableGravity"); throw 0; }
 virtual void EntitySetLightAsOn(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetLightAsOn"); throw 0; }
 virtual void EntityFadeOut(const CScriptThing&, float) const { assert(!"unexpected interface call: EntityFadeOut"); throw 0; }
 virtual void EntityFadeIn(const CScriptThing&, float) const { assert(!"unexpected interface call: EntityFadeIn"); throw 0; }
 virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntityBeginLoadingAnimation"); throw 0; }
 virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const { assert(!"unexpected interface call: EntityBeginLoadingBasicAnimations"); throw 0; }
 virtual bool EntityCastForcePush(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityCastForcePush"); throw 0; }
 virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityCastLightningAtTarget"); throw 0; }
 virtual void BeginLoadingMesh(const CCharString&) const { assert(!"unexpected interface call: BeginLoadingMesh"); throw 0; }
 virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const { assert(!"unexpected interface call: EntityWillTeleportToArea"); throw 0; }
 virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityStartScreamerSuperAttackThing"); throw 0; }
 virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const { assert(!"unexpected interface call: EntityEndScreamerSuperAttackThing"); throw 0; }
 virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) { assert(!"unexpected interface call: SetHeroGuideToShowQuestCardsWhenSpokenTo"); throw 0; }
 virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const { assert(!"unexpected interface call: SetLightColour"); throw 0; }
 virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreatureGeneratorSetFamily"); throw 0; }
 virtual void CreatureGeneratorTrigger(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorTrigger"); throw 0; }
 virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const { assert(!"unexpected interface call: CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger"); throw 0; }
 virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorIsDepleted"); throw 0; }
 virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorIsDestroyed"); throw 0; }
 virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreatureGeneratorSetGeneratedCreatureScriptName"); throw 0; }
 virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, int) const { assert(!"unexpected interface call: CreatureGeneratorSetNumTriggers"); throw 0; }
 virtual int CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorGetNumGeneratedCreatures"); throw 0; }
 virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorAreAllCreaturesAlive"); throw 0; }
 virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorAddTriggerer"); throw 0; }
 virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorRemoveTriggerer"); throw 0; }
 virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorEnabled"); throw 0; }
 virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsEnabled"); throw 0; }
 virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsEnabledDuringScript"); throw 0; }
 virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsCreatureGroupAsEnabled"); throw 0; }
 virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const { assert(!"unexpected interface call: IsCreatureGenerationEnabledForRegion"); throw 0; }
 virtual bool IsCreatureFlying(const CScriptThing&) const { assert(!"unexpected interface call: IsCreatureFlying"); throw 0; }
 virtual void SetTeleporterAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTeleporterAsActive"); throw 0; }
 virtual bool IsTeleporterActive(const CScriptThing&) const { assert(!"unexpected interface call: IsTeleporterActive"); throw 0; }
 virtual void SetTeleportingAsActive(bool) const { assert(!"unexpected interface call: SetTeleportingAsActive"); throw 0; }
 virtual bool IsTeleportingActive() const { assert(!"unexpected interface call: IsTeleportingActive"); throw 0; }
 virtual void SetRegionExitAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetRegionExitAsActive"); throw 0; }
 virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetRegionEntranceAsActive"); throw 0; }
 virtual void SetRegionTextDisplayAsActive(bool) const { assert(!"unexpected interface call: SetRegionTextDisplayAsActive"); throw 0; }
 virtual void SetHeroSleepingAsEnabled(bool) const { assert(!"unexpected interface call: SetHeroSleepingAsEnabled"); throw 0; }
 virtual bool IsHeroSleepingEnabled() const { assert(!"unexpected interface call: IsHeroSleepingEnabled"); throw 0; }
 virtual void SetExperienceSpendingAsEnabled(bool) const { assert(!"unexpected interface call: SetExperienceSpendingAsEnabled"); throw 0; }
 virtual void SetMoralityChangingAsEnabled(bool) const { assert(!"unexpected interface call: SetMoralityChangingAsEnabled"); throw 0; }
 virtual void SetSummonerDeathExplosionAffectsHero(bool) const { assert(!"unexpected interface call: SetSummonerDeathExplosionAffectsHero"); throw 0; }
 virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const { assert(!"unexpected interface call: GetNearestEnabledDiggingSpot"); throw 0; }
 virtual bool IsDiggingSpotEnabled(const CScriptThing&) const { assert(!"unexpected interface call: IsDiggingSpotEnabled"); throw 0; }
 virtual bool IsDiggingSpotHidden(const CScriptThing&) const { assert(!"unexpected interface call: IsDiggingSpotHidden"); throw 0; }
 virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetDiggingSpotAsHidden"); throw 0; }
 virtual bool CheckForCameraMessage(const CCharString&) const { assert(!"unexpected interface call: CheckForCameraMessage"); throw 0; }
 virtual void WaitForCameraMessage(const CCharString&) const { assert(!"unexpected interface call: WaitForCameraMessage"); throw 0; }
 virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const { assert(!"unexpected interface call: SetThingAsConscious"); throw 0; }
 virtual void SetFireToThing(const CScriptThing&) const { assert(!"unexpected interface call: SetFireToThing"); throw 0; }
 virtual void ExtinguishFiresOnThing(const CScriptThing&) const { assert(!"unexpected interface call: ExtinguishFiresOnThing"); throw 0; }
 virtual bool IsThingOnFire(const CScriptThing&) const { assert(!"unexpected interface call: IsThingOnFire"); throw 0; }
 virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: AddItemToContainer"); throw 0; }
 virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: RemoveItemFromContainer"); throw 0; }
 virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetDeathContainerAsEnabled"); throw 0; }
 virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: GetItemDefNamesFromContainer"); throw 0; }
 virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: SetCreatureBrain"); throw 0; }
 virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: EntitySetStategroupEnabled"); throw 0; }
 virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAllStategroupsEnabled"); throw 0; }
 virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetCombatEnabled"); throw 0; }
 virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetSleepEnabled"); throw 0; }
 virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetOpinionReactionsEnabled"); throw 0; }
 virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetDeedReactionsEnabled"); throw 0; }
 virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned int,std::less<unsigned int>,std::allocator<unsigned int> >&) const { assert(!"unexpected interface call: DebugGetAllTextEntriesForTargetedThing"); throw 0; }
 virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetThingAsEnemyOfThing"); throw 0; }
 virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityUnsetThingAsEnemyOfThing"); throw 0; }
 virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetThingAsAllyOfThing"); throw 0; }
 virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityUnsetThingAsAllyOfThing"); throw 0; }
 virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetInFaction"); throw 0; }
 virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsAlliedToFaction"); throw 0; }
 virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsNeutralToFaction"); throw 0; }
 virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsEnemyToFaction"); throw 0; }
 virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: AreEntitiesEnemies"); throw 0; }
 virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const { assert(!"unexpected interface call: GetNextInOpinionAttitudeGraph"); throw 0; }
 virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const { assert(!"unexpected interface call: GetOpinionAttitudeAsString"); throw 0; }
 virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetOpinionAttitudeToPlayer"); throw 0; }
 virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const { assert(!"unexpected interface call: EntityGetOpinionAttitudeToPlayerAsString"); throw 0; }
 virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const { assert(!"unexpected interface call: EntityGetOpinionOfPlayer"); throw 0; }
 virtual void EntitySetOpinionReactionMask(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetOpinionReactionMask"); throw 0; }
 virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetOpinionReactionMask"); throw 0; }
 virtual void EntitySetOpinionDeedMask(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetOpinionDeedMask"); throw 0; }
 virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetOpinionDeedMask"); throw 0; }
 virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const { assert(!"unexpected interface call: EntitySetOpinionDeedTypeEnabled"); throw 0; }
 virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const { assert(!"unexpected interface call: EntitySetOpinionAttitudeEnabled"); throw 0; }
 virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const { assert(!"unexpected interface call: EntitySetOpinionReactionEnabled"); throw 0; }
 virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetPersonalityOverride"); throw 0; }
 virtual void EntitySetPersonalityOverride(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetPersonalityOverride"); throw 0; }
 virtual void EntityClearPersonalityOverride(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearPersonalityOverride"); throw 0; }
 virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetAsOpinionSource"); throw 0; }
 virtual void EntitySetAsOpinionSource(const CScriptThing&, int) const { assert(!"unexpected interface call: EntitySetAsOpinionSource"); throw 0; }
 virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsetAsOpinionSource"); throw 0; }
 virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpinionSourceSetAsExclusive"); throw 0; }
 virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpinionSourceSetAsAttentionGrabbing"); throw 0; }
 virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const { assert(!"unexpected interface call: EntityPostOpinionDeedToAll"); throw 0; }
 virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedToRecipient"); throw 0; }
 virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedToRecipientVillage"); throw 0; }
 virtual int EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedKeepSearchingForWitnesses"); throw 0; }
 virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, int) const { assert(!"unexpected interface call: RemoveOpinionDeedStillSearchingForWitnesses"); throw 0; }
 virtual bool IsDeedWitnessed(int) const { assert(!"unexpected interface call: IsDeedWitnessed"); throw 0; }
 virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Seen_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_NearlySeen_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Smelled_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Heard_ByOtherThing"); throw 0; }
 virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: IsThingAwareOfOtherThingInAnyWay"); throw 0; }
 virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsAwareOfThing"); throw 0; }
 virtual void EntitySetSoundRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSoundRadius"); throw 0; }
 virtual void EntitySetSmellRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSmellRadius"); throw 0; }
 virtual void EntitySetSightRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSightRadius"); throw 0; }
 virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetExtendedSightRadius"); throw 0; }
 virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetGiveUpChaseRadius"); throw 0; }
 virtual float EntityGetHearingRadius(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetHearingRadius"); throw 0; }
 virtual bool ManuallyTriggerTrap(const CScriptThing&) const { assert(!"unexpected interface call: ManuallyTriggerTrap"); throw 0; }
 virtual bool ManuallyResetTrap(const CScriptThing&) const { assert(!"unexpected interface call: ManuallyResetTrap"); throw 0; }
 virtual void SetTimeOfDay(float) const { assert(!"unexpected interface call: SetTimeOfDay"); throw 0; }
 virtual int GetTimeOfDay() const { assert(!"unexpected interface call: GetTimeOfDay"); throw 0; }
 virtual void SetTimeAsStopped(bool, int&) const { assert(!"unexpected interface call: SetTimeAsStopped"); throw 0; }
 virtual void FastForwardTimeTo(float, float) const { assert(!"unexpected interface call: FastForwardTimeTo"); throw 0; }
 virtual bool IsTimeOfDayBetween(int, int) const { assert(!"unexpected interface call: IsTimeOfDayBetween"); throw 0; }
 virtual int GetDayOfWeek() const { assert(!"unexpected interface call: GetDayOfWeek"); throw 0; }
 virtual int GetDayCount() const { assert(!"unexpected interface call: GetDayCount"); throw 0; }
 virtual int GetWorldFrame() const { assert(!"unexpected interface call: GetWorldFrame"); throw 0; }
 virtual int GetConstantFPS() const { assert(!"unexpected interface call: GetConstantFPS"); throw 0; }
 virtual CCharString GetActiveQuestName() const { assert(!"unexpected interface call: GetActiveQuestName"); throw 0; }
 virtual void TransitionToTheme(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToTheme"); throw 0; }
 virtual void ResetToDefaultTheme(float) const { assert(!"unexpected interface call: ResetToDefaultTheme"); throw 0; }
 virtual void TransitionToThemeAllInternals(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToThemeAllInternals"); throw 0; }
 virtual void ResetToDefaultThemeAllInternals(float) const { assert(!"unexpected interface call: ResetToDefaultThemeAllInternals"); throw 0; }
 virtual void TransitionToThemeExternals(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToThemeExternals"); throw 0; }
 virtual void ResetToDefaultThemeExternals(float) const { assert(!"unexpected interface call: ResetToDefaultThemeExternals"); throw 0; }
 virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightAllChannels"); throw 0; }
 virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightAllInternals"); throw 0; }
 virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightExternals"); throw 0; }
 virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const { assert(!"unexpected interface call: SetSoundThemesAsEnabledForRegion"); throw 0; }
 virtual void SetAllSoundsAsMuted(bool) const { assert(!"unexpected interface call: SetAllSoundsAsMuted"); throw 0; }
 virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const { assert(!"unexpected interface call: RadialBlurFadeTo"); throw 0; }
 virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const { assert(!"unexpected interface call: RadialBlurFadeTo"); throw 0; }
 virtual void RadialBlurFadeOut(float, void*) const { assert(!"unexpected interface call: RadialBlurFadeOut"); throw 0; }
 virtual bool IsRadialBlurFadeActive() const { assert(!"unexpected interface call: IsRadialBlurFadeActive"); throw 0; }
 virtual void CancelRadialBlurFade() const { assert(!"unexpected interface call: CancelRadialBlurFade"); throw 0; }
 virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const { assert(!"unexpected interface call: RadialBlurSetCenterWorldPos"); throw 0; }
 virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const { assert(!"unexpected interface call: DisplacementMonochromeEffectColourFadeTo"); throw 0; }
 virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const { assert(!"unexpected interface call: DisplacementMonochromeEffectColourFadeOut"); throw 0; }
 virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const { assert(!"unexpected interface call: ScreenFilterFadeTo"); throw 0; }
 virtual void ScreenFilterFadeOut(float, void*) const { assert(!"unexpected interface call: ScreenFilterFadeOut"); throw 0; }
 virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const { assert(!"unexpected interface call: SetThingAndCarriedItemsNotAffectedByScreenFilter"); throw 0; }
 virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const { assert(!"unexpected interface call: UnSetThingAndCarriedItemsNotAffectedByScreenFilter"); throw 0; }
 virtual bool IsGiftRomantic(const CCharString&) const { assert(!"unexpected interface call: IsGiftRomantic"); throw 0; }
 virtual bool IsGiftFriendly(const CCharString&) const { assert(!"unexpected interface call: IsGiftFriendly"); throw 0; }
 virtual bool IsGiftOffensive(const CCharString&) const { assert(!"unexpected interface call: IsGiftOffensive"); throw 0; }
 virtual bool IsThingABed(const CScriptThing&) const { assert(!"unexpected interface call: IsThingABed"); throw 0; }
 virtual bool IsThingAChest(const CScriptThing&) const { assert(!"unexpected interface call: IsThingAChest"); throw 0; }
 virtual bool IsThingADoor(const CScriptThing&) const { assert(!"unexpected interface call: IsThingADoor"); throw 0; }
 virtual bool IsThingSmashable(const CScriptThing&) const { assert(!"unexpected interface call: IsThingSmashable"); throw 0; }
 virtual bool IsThingSearchable(const CScriptThing&) const { assert(!"unexpected interface call: IsThingSearchable"); throw 0; }
 virtual void ApplyScriptBrush(const CCharString&) const { assert(!"unexpected interface call: ApplyScriptBrush"); throw 0; }
 virtual void EnableDecals(bool) const { assert(!"unexpected interface call: EnableDecals"); throw 0; }
 virtual unsigned int PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: PlayCriteriaSoundOnThing"); throw 0; }
 virtual unsigned int PlaySoundOnThing(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: PlaySoundOnThing"); throw 0; }
 virtual unsigned int PlaySoundAtPos(const C3DVector&, const CCharString&) const { assert(!"unexpected interface call: PlaySoundAtPos"); throw 0; }
 virtual unsigned int Play2DSound(const CCharString&) const { assert(!"unexpected interface call: Play2DSound"); throw 0; }
 virtual bool IsSoundPlaying(unsigned int) const { assert(!"unexpected interface call: IsSoundPlaying"); throw 0; }
 virtual void EnableSounds(bool) const { assert(!"unexpected interface call: EnableSounds"); throw 0; }
 virtual void StopSound(unsigned int) const { assert(!"unexpected interface call: StopSound"); throw 0; }
 virtual void OverrideMusic(EMusicSetType, bool, bool) const { assert(!"unexpected interface call: OverrideMusic"); throw 0; }
 virtual void StopOverrideMusic(bool) const { assert(!"unexpected interface call: StopOverrideMusic"); throw 0; }
 virtual void CacheMusicSet(EMusicSetType) const { assert(!"unexpected interface call: CacheMusicSet"); throw 0; }
 virtual void EnableDangerMusic(bool) const { assert(!"unexpected interface call: EnableDangerMusic"); throw 0; }
 virtual bool IsDangerMusicEnabled() const { assert(!"unexpected interface call: IsDangerMusicEnabled"); throw 0; }
 virtual void StartCountdownTimer(float) const { assert(!"unexpected interface call: StartCountdownTimer"); throw 0; }
 virtual float GetCountdownTimer() const { assert(!"unexpected interface call: GetCountdownTimer"); throw 0; }
 virtual void AutoSaveCheckPoint() const { assert(!"unexpected interface call: AutoSaveCheckPoint"); throw 0; }
 virtual void AutoSaveQuestStart() const { assert(!"unexpected interface call: AutoSaveQuestStart"); throw 0; }
 virtual void AutoSave() const { assert(!"unexpected interface call: AutoSave"); throw 0; }
 virtual void SetSavingAsEnabled(bool) const { assert(!"unexpected interface call: SetSavingAsEnabled"); throw 0; }
 virtual bool IsSavingEnabled() const { assert(!"unexpected interface call: IsSavingEnabled"); throw 0; }
 virtual void SetSaveGameMarkerPos(const C3DVector&) const { assert(!"unexpected interface call: SetSaveGameMarkerPos"); throw 0; }
 virtual void ResetToFrontEnd() const { assert(!"unexpected interface call: ResetToFrontEnd"); throw 0; }
 virtual void SetGuildSealRecallLocation(const C3DVector&, float) const { assert(!"unexpected interface call: SetGuildSealRecallLocation"); throw 0; }
 virtual C3DVector GetGuildSealRecallPos() const { assert(!"unexpected interface call: GetGuildSealRecallPos"); throw 0; }
 virtual float GetGuildSealRecallAngleXY() const { assert(!"unexpected interface call: GetGuildSealRecallAngleXY"); throw 0; }
 virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const { assert(!"unexpected interface call: SetReadableObjectText"); throw 0; }
 virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: SetReadableObjectTextTag"); throw 0; }
 virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const { assert(!"unexpected interface call: GetFormattedString"); throw 0; }
 virtual CWideString GetTextString(const CCharString&) const { assert(!"unexpected interface call: GetTextString"); throw 0; }
 virtual void AddRumourCategory(const CCharString&) const { assert(!"unexpected interface call: AddRumourCategory"); throw 0; }
 virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddNewRumourToCategory"); throw 0; }
 virtual void RemoveRumourCategory(const CCharString&) const { assert(!"unexpected interface call: RemoveRumourCategory"); throw 0; }
 virtual void SetCategoryActivity(const CCharString&, bool) const { assert(!"unexpected interface call: SetCategoryActivity"); throw 0; }
 virtual void AddGossipVillage(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddGossipVillage"); throw 0; }
 virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddGossipFactionToCategory"); throw 0; }
 virtual void SetIsGossipForPlayer(const CCharString&, bool) const { assert(!"unexpected interface call: SetIsGossipForPlayer"); throw 0; }
 virtual void SetIsGossipForPlayer(CCharString, bool) { assert(!"unexpected interface call: SetIsGossipForPlayer"); throw 0; }
 virtual void UpdateOnlineScore_Archery(float) const { assert(!"unexpected interface call: UpdateOnlineScore_Archery"); throw 0; }
 virtual void UpdateOnlineScore_ChickenKick(float) const { assert(!"unexpected interface call: UpdateOnlineScore_ChickenKick"); throw 0; }
 virtual void UpdateOnlineScore_ChapelOrTemple(float) const { assert(!"unexpected interface call: UpdateOnlineScore_ChapelOrTemple"); throw 0; }
 virtual void UpdateOnlineScore_FishingCompo(float) const { assert(!"unexpected interface call: UpdateOnlineScore_FishingCompo"); throw 0; }
 virtual void UpdateScore_FishingCompetition(float) const { assert(!"unexpected interface call: UpdateScore_FishingCompetition"); throw 0; }
 virtual float GetBestTimePairs() const { assert(!"unexpected interface call: GetBestTimePairs"); throw 0; }
 virtual float GetBestTimeSorting() const { assert(!"unexpected interface call: GetBestTimeSorting"); throw 0; }
 virtual int GetBestScoreBlackjack() const { assert(!"unexpected interface call: GetBestScoreBlackjack"); throw 0; }
 virtual int GetBestScoreCoinGolfOakVale() const { assert(!"unexpected interface call: GetBestScoreCoinGolfOakVale"); throw 0; }
 virtual int GetBestScoreCoinGolfSnowSpire() const { assert(!"unexpected interface call: GetBestScoreCoinGolfSnowSpire"); throw 0; }
 virtual int GetBestScoreShoveHaPenny() const { assert(!"unexpected interface call: GetBestScoreShoveHaPenny"); throw 0; }
 virtual float GetBestTimeGuessTheAddition() const { assert(!"unexpected interface call: GetBestTimeGuessTheAddition"); throw 0; }
 virtual bool IsHeroInTavernGame() const { assert(!"unexpected interface call: IsHeroInTavernGame"); throw 0; }
 virtual int GetNumHousesOwned() const { assert(!"unexpected interface call: GetNumHousesOwned"); throw 0; }
 virtual void StartSneaking() const { assert(!"unexpected interface call: StartSneaking"); throw 0; }
 virtual int GetStealDuration(const CScriptThing&) const { assert(!"unexpected interface call: GetStealDuration"); throw 0; }
 virtual void SetUseableByHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetUseableByHero"); throw 0; }
 virtual void SetOwnedByHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetOwnedByHero"); throw 0; }
 virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTavernTableAvailableForUse"); throw 0; }
 virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingTurncoatable"); throw 0; }
 virtual void SetIsThingForcePushable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingForcePushable"); throw 0; }
 virtual void SetIsThingLightningable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingLightningable"); throw 0; }
 virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingEpicSpellable"); throw 0; }
 virtual bool IsThingTurncoated(const CScriptThing&) const { assert(!"unexpected interface call: IsThingTurncoated"); throw 0; }
 virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: AddCreatureScriptedMode"); throw 0; }
 virtual void RemoveCreatureScriptedMode(const CScriptThing&) const { assert(!"unexpected interface call: RemoveCreatureScriptedMode"); throw 0; }
 virtual void ForceShipsVisible() const { assert(!"unexpected interface call: ForceShipsVisible"); throw 0; }
 virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) { assert(!"unexpected interface call: GetSleepingPositionAndOrientationFromBed"); throw 0; }
 virtual void SetBedAvailability(const CScriptThing&, bool) { assert(!"unexpected interface call: SetBedAvailability"); throw 0; }
 virtual void RepopulateVillage(const CScriptThing&) const { assert(!"unexpected interface call: RepopulateVillage"); throw 0; }
 virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) { assert(!"unexpected interface call: SmashAllWindowsWithinRadiusOfPoint"); throw 0; }
 virtual CScriptThing SetResidency(const CScriptThing&, bool) { assert(!"unexpected interface call: SetResidency"); throw 0; }
 virtual void SetThankingPhrase(CScriptThing, unsigned int) { assert(!"unexpected interface call: SetThankingPhrase"); throw 0; }
 virtual unsigned int GetThankingPhrase(CScriptThing) { assert(!"unexpected interface call: GetThankingPhrase"); throw 0; }
 virtual void ResetThankingPhrase(CScriptThing) { assert(!"unexpected interface call: ResetThankingPhrase"); throw 0; }
 virtual void SetIgnoringPhrase(CScriptThing, unsigned int) { assert(!"unexpected interface call: SetIgnoringPhrase"); throw 0; }
 virtual unsigned int GetIgnoringPhrase(CScriptThing) { assert(!"unexpected interface call: GetIgnoringPhrase"); throw 0; }
 virtual void ResetIgnoringPhrase(CScriptThing) { assert(!"unexpected interface call: ResetIgnoringPhrase"); throw 0; }
 virtual void SetWanderCentrePoint(CScriptThing, C3DVector) { assert(!"unexpected interface call: SetWanderCentrePoint"); throw 0; }
 virtual C3DVector GetWanderCentrePoint(CScriptThing) { assert(!"unexpected interface call: GetWanderCentrePoint"); throw 0; }
 virtual void ResetWanderCentrePoint(CScriptThing) { assert(!"unexpected interface call: ResetWanderCentrePoint"); throw 0; }
 virtual void SetWanderMinDistance(CScriptThing, float) { assert(!"unexpected interface call: SetWanderMinDistance"); throw 0; }
 virtual float GetWanderMinDistance(CScriptThing) { assert(!"unexpected interface call: GetWanderMinDistance"); throw 0; }
 virtual void ResetWanderMinDistance(CScriptThing) { assert(!"unexpected interface call: ResetWanderMinDistance"); throw 0; }
 virtual void SetWanderMaxDistance(CScriptThing, float) { assert(!"unexpected interface call: SetWanderMaxDistance"); throw 0; }
 virtual float GetWanderMaxDistance(CScriptThing) { assert(!"unexpected interface call: GetWanderMaxDistance"); throw 0; }
 virtual void ResetWanderMaxDistance(CScriptThing) { assert(!"unexpected interface call: ResetWanderMaxDistance"); throw 0; }
 virtual void SetGossipCounter(CScriptThing, int) { assert(!"unexpected interface call: SetGossipCounter"); throw 0; }
 virtual int GetGossipCounter(CScriptThing) { assert(!"unexpected interface call: GetGossipCounter"); throw 0; }
 virtual void ResetGossipCounter(CScriptThing) { assert(!"unexpected interface call: ResetGossipCounter"); throw 0; }
 virtual void SetMaxGossipPhrase(CScriptThing, int) { assert(!"unexpected interface call: SetMaxGossipPhrase"); throw 0; }
 virtual int GetMaxGossipPhrase(CScriptThing) { assert(!"unexpected interface call: GetMaxGossipPhrase"); throw 0; }
 virtual void ResetMaxGossipPhrase(CScriptThing) { assert(!"unexpected interface call: ResetMaxGossipPhrase"); throw 0; }
 virtual void SetWarningPhrase(CScriptThing, unsigned int) { assert(!"unexpected interface call: SetWarningPhrase"); throw 0; }
 virtual unsigned int GetWarningPhrase(CScriptThing) { assert(!"unexpected interface call: GetWarningPhrase"); throw 0; }
 virtual void ResetWarningPhrase(CScriptThing) { assert(!"unexpected interface call: ResetWarningPhrase"); throw 0; }
 virtual void SetBeerRequestPhrase(CScriptThing, unsigned int) { assert(!"unexpected interface call: SetBeerRequestPhrase"); throw 0; }
 virtual unsigned int GetBeerRequestPhrase(CScriptThing) { assert(!"unexpected interface call: GetBeerRequestPhrase"); throw 0; }
 virtual void ResetBeerRequestPhrase(CScriptThing) { assert(!"unexpected interface call: ResetBeerRequestPhrase"); throw 0; }
 virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) { assert(!"unexpected interface call: SetScriptingStateGroup"); throw 0; }
 virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) { assert(!"unexpected interface call: GetScriptingStateGroup"); throw 0; }
 virtual void ResetScriptingStateGroup(CScriptThing) { assert(!"unexpected interface call: ResetScriptingStateGroup"); throw 0; }
 virtual void SetMaxHeroReactionDistance(CScriptThing, float) { assert(!"unexpected interface call: SetMaxHeroReactionDistance"); throw 0; }
 virtual float GetMaxHeroReactionDistance(CScriptThing) { assert(!"unexpected interface call: GetMaxHeroReactionDistance"); throw 0; }
 virtual void ResetMaxHeroReactionDistance(CScriptThing) { assert(!"unexpected interface call: ResetMaxHeroReactionDistance"); throw 0; }
 virtual void SetActionFrequency(CScriptThing, int) { assert(!"unexpected interface call: SetActionFrequency"); throw 0; }
 virtual int GetActionFrequency(CScriptThing) { assert(!"unexpected interface call: GetActionFrequency"); throw 0; }
 virtual void ResetActionFrequency(CScriptThing) { assert(!"unexpected interface call: ResetActionFrequency"); throw 0; }
 virtual void SetActionFrequencyVariation(CScriptThing, float) { assert(!"unexpected interface call: SetActionFrequencyVariation"); throw 0; }
 virtual float GetActionFrequencyVariation(CScriptThing) { assert(!"unexpected interface call: GetActionFrequencyVariation"); throw 0; }
 virtual void ResetActionFrequencyVariation(CScriptThing) { assert(!"unexpected interface call: ResetActionFrequencyVariation"); throw 0; }
 virtual void SetAction(CScriptThing, CCharString) { assert(!"unexpected interface call: SetAction"); throw 0; }
 virtual CCharString GetAction(CScriptThing) { assert(!"unexpected interface call: GetAction"); throw 0; }
 virtual void ResetAction(CScriptThing) { assert(!"unexpected interface call: ResetAction"); throw 0; }
 virtual void SetFaceHeroForAction(CScriptThing, bool) { assert(!"unexpected interface call: SetFaceHeroForAction"); throw 0; }
 virtual bool GetFaceHeroForAction(CScriptThing) { assert(!"unexpected interface call: GetFaceHeroForAction"); throw 0; }
 virtual void ResetFaceHeroForAction(CScriptThing) { assert(!"unexpected interface call: ResetFaceHeroForAction"); throw 0; }
 virtual void SetTargetName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetTargetName"); throw 0; }
 virtual CCharString GetTargetName(CScriptThing) { assert(!"unexpected interface call: GetTargetName"); throw 0; }
 virtual void ResetTargetName(CScriptThing) { assert(!"unexpected interface call: ResetTargetName"); throw 0; }
 virtual void SetFollowDistance(CScriptThing, float) { assert(!"unexpected interface call: SetFollowDistance"); throw 0; }
 virtual float GetFollowDistance(CScriptThing) { assert(!"unexpected interface call: GetFollowDistance"); throw 0; }
 virtual void ResetFollowDistance(CScriptThing) { assert(!"unexpected interface call: ResetFollowDistance"); throw 0; }
 virtual void SetAttackHeroOnSight(CScriptThing, bool) { assert(!"unexpected interface call: SetAttackHeroOnSight"); throw 0; }
 virtual bool GetAttackHeroOnSight(CScriptThing) { assert(!"unexpected interface call: GetAttackHeroOnSight"); throw 0; }
 virtual void ResetAttackHeroOnSight(CScriptThing) { assert(!"unexpected interface call: ResetAttackHeroOnSight"); throw 0; }
 virtual void SetTimeToSpendHarassingHero(CScriptThing, int) { assert(!"unexpected interface call: SetTimeToSpendHarassingHero"); throw 0; }
 virtual int GetTimeToSpendHarassingHero(CScriptThing) { assert(!"unexpected interface call: GetTimeToSpendHarassingHero"); throw 0; }
 virtual void ResetTimeToSpendHarassingHero(CScriptThing) { assert(!"unexpected interface call: ResetTimeToSpendHarassingHero"); throw 0; }
 virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) { assert(!"unexpected interface call: SetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { assert(!"unexpected interface call: GetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { assert(!"unexpected interface call: ResetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) { assert(!"unexpected interface call: SetCombatNearbyBreakOffRange"); throw 0; }
 virtual float GetCombatNearbyBreakOffRange(CScriptThing) { assert(!"unexpected interface call: GetCombatNearbyBreakOffRange"); throw 0; }
 virtual void ResetCombatNearbyBreakOffRange(CScriptThing) { assert(!"unexpected interface call: ResetCombatNearbyBreakOffRange"); throw 0; }
 virtual void SetStealStealableItems(CScriptThing, bool) { assert(!"unexpected interface call: SetStealStealableItems"); throw 0; }
 virtual bool GetStealStealableItems(CScriptThing) { assert(!"unexpected interface call: GetStealStealableItems"); throw 0; }
 virtual void ResetStealStealableItems(CScriptThing) { assert(!"unexpected interface call: ResetStealStealableItems"); throw 0; }
 virtual void SetRecoverStealableItems(CScriptThing, bool) { assert(!"unexpected interface call: SetRecoverStealableItems"); throw 0; }
 virtual bool GetRecoverStealableItems(CScriptThing) { assert(!"unexpected interface call: GetRecoverStealableItems"); throw 0; }
 virtual void ResetRecoverStealableItems(CScriptThing) { assert(!"unexpected interface call: ResetRecoverStealableItems"); throw 0; }
 virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) { assert(!"unexpected interface call: SetTakeStealableItemToRandomDestination"); throw 0; }
 virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) { assert(!"unexpected interface call: GetTakeStealableItemToRandomDestination"); throw 0; }
 virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) { assert(!"unexpected interface call: ResetTakeStealableItemToRandomDestination"); throw 0; }
 virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) { assert(!"unexpected interface call: SetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { assert(!"unexpected interface call: GetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { assert(!"unexpected interface call: ResetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual void SetAllowedToFollow(CScriptThing, bool) { assert(!"unexpected interface call: SetAllowedToFollow"); throw 0; }
 virtual bool GetAllowedToFollow(CScriptThing) { assert(!"unexpected interface call: GetAllowedToFollow"); throw 0; }
 virtual void ResetAllowedToFollow(CScriptThing) { assert(!"unexpected interface call: ResetAllowedToFollow"); throw 0; }
 virtual void SetTableName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetTableName"); throw 0; }
 virtual CCharString GetTableName(CScriptThing) { assert(!"unexpected interface call: GetTableName"); throw 0; }
 virtual void ResetTableName(CScriptThing) { assert(!"unexpected interface call: ResetTableName"); throw 0; }
 virtual void SetSeatName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetSeatName"); throw 0; }
 virtual CCharString GetSeatName(CScriptThing) { assert(!"unexpected interface call: GetSeatName"); throw 0; }
 virtual void ResetSeatName(CScriptThing) { assert(!"unexpected interface call: ResetSeatName"); throw 0; }
 virtual void SetDisableHeadLooking(CScriptThing, bool) { assert(!"unexpected interface call: SetDisableHeadLooking"); throw 0; }
 virtual bool GetDisableHeadLooking(CScriptThing) { assert(!"unexpected interface call: GetDisableHeadLooking"); throw 0; }
 virtual void ResetDisableHeadLooking(CScriptThing) { assert(!"unexpected interface call: ResetDisableHeadLooking"); throw 0; }
 virtual void SetIsPushableByHero(CScriptThing, bool) { assert(!"unexpected interface call: SetIsPushableByHero"); throw 0; }
 virtual bool GetIsPushableByHero(CScriptThing) { assert(!"unexpected interface call: GetIsPushableByHero"); throw 0; }
 virtual void ResetIsPushableByHero(CScriptThing) { assert(!"unexpected interface call: ResetIsPushableByHero"); throw 0; }
 virtual void SetLookForFiniteTime(CScriptThing, bool) { assert(!"unexpected interface call: SetLookForFiniteTime"); throw 0; }
 virtual bool GetLookForFiniteTime(CScriptThing) { assert(!"unexpected interface call: GetLookForFiniteTime"); throw 0; }
 virtual void ResetLookForFiniteTime(CScriptThing) { assert(!"unexpected interface call: ResetLookForFiniteTime"); throw 0; }
 virtual void SetAvoidRegionExits(CScriptThing, bool) { assert(!"unexpected interface call: SetAvoidRegionExits"); throw 0; }
 virtual bool GetAvoidRegionExits(CScriptThing) { assert(!"unexpected interface call: GetAvoidRegionExits"); throw 0; }
 virtual void ResetAvoidRegionExits(CScriptThing) { assert(!"unexpected interface call: ResetAvoidRegionExits"); throw 0; }
 virtual void SetTargetingDistanceOffset(CScriptThing, float) { assert(!"unexpected interface call: SetTargetingDistanceOffset"); throw 0; }
 virtual float GetTargetingDistanceOffset(CScriptThing) { assert(!"unexpected interface call: GetTargetingDistanceOffset"); throw 0; }
 virtual void ResetTargetingDistanceOffset(CScriptThing) { assert(!"unexpected interface call: ResetTargetingDistanceOffset"); throw 0; }
 virtual void SetPlayerUsingMeleeDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingMeleeDummies"); throw 0; }
 virtual bool GetPlayerUsingMeleeDummies() { assert(!"unexpected interface call: GetPlayerUsingMeleeDummies"); throw 0; }
 virtual void SetPlayerUsingRangedDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingRangedDummies"); throw 0; }
 virtual bool GetPlayerUsingRangedDummies() { assert(!"unexpected interface call: GetPlayerUsingRangedDummies"); throw 0; }
 virtual void SetPlayerUsingWillDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingWillDummies"); throw 0; }
 virtual bool GetPlayerUsingWillDummies() { assert(!"unexpected interface call: GetPlayerUsingWillDummies"); throw 0; }
 virtual void SetCheapHeadLooking(bool) { assert(!"unexpected interface call: SetCheapHeadLooking"); throw 0; }
 virtual bool GetCheapHeadLooking() { assert(!"unexpected interface call: GetCheapHeadLooking"); throw 0; }
 virtual void SetQuitTavernGame(bool) { assert(!"unexpected interface call: SetQuitTavernGame"); throw 0; }
 virtual bool GetQuitTavernGame() { assert(!"unexpected interface call: GetQuitTavernGame"); throw 0; }
 virtual void SetPrizeTavernTable(bool) { assert(!"unexpected interface call: SetPrizeTavernTable"); throw 0; }
 virtual bool GetPrizeTavernTable() { assert(!"unexpected interface call: GetPrizeTavernTable"); throw 0; }
 virtual void SetBettingActive(bool) { assert(!"unexpected interface call: SetBettingActive"); throw 0; }
 virtual bool GetBettingActive() { assert(!"unexpected interface call: GetBettingActive"); throw 0; }
 virtual void SetBettingAccept(bool) { assert(!"unexpected interface call: SetBettingAccept"); throw 0; }
 virtual bool GetBettingAccept() { assert(!"unexpected interface call: GetBettingAccept"); throw 0; }
 virtual void SetBettingAmount(int) { assert(!"unexpected interface call: SetBettingAmount"); throw 0; }
 virtual int GetBettingAmount() { assert(!"unexpected interface call: GetBettingAmount"); throw 0; }
 virtual void SetCountBetMoneyDown(bool) { assert(!"unexpected interface call: SetCountBetMoneyDown"); throw 0; }
 virtual bool GetCountBetMoneyDown() { assert(!"unexpected interface call: GetCountBetMoneyDown"); throw 0; }
 virtual void SetSpotTheAdditionBeaten(bool) { assert(!"unexpected interface call: SetSpotTheAdditionBeaten"); throw 0; }
 virtual bool GetSpotTheAdditionBeaten() { assert(!"unexpected interface call: GetSpotTheAdditionBeaten"); throw 0; }
 virtual void SetGlobalTargetingDistanceOffset(float) { assert(!"unexpected interface call: SetGlobalTargetingDistanceOffset"); throw 0; }
 virtual float GetGlobalTargetingDistanceOffset() { assert(!"unexpected interface call: GetGlobalTargetingDistanceOffset"); throw 0; }
 virtual void SetTradingPriceMult(float) { assert(!"unexpected interface call: SetTradingPriceMult"); throw 0; }
 virtual float GetTradingPriceMult() { assert(!"unexpected interface call: GetTradingPriceMult"); throw 0; }
 virtual void SetBoastingEnabled(bool) { assert(!"unexpected interface call: SetBoastingEnabled"); throw 0; }
 virtual bool GetBoastingEnabled() { assert(!"unexpected interface call: GetBoastingEnabled"); throw 0; }
 virtual void SetActiveGossipCategories(CCharString, bool) { assert(!"unexpected interface call: SetActiveGossipCategories"); throw 0; }
 virtual bool* GetActiveGossipCategories(CCharString) { assert(!"unexpected interface call: GetActiveGossipCategories"); throw 0; }
 virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() { assert(!"unexpected interface call: GetActiveGossipCategories"); throw 0; }
 virtual int GetActiveGossipCategoriesSize() { assert(!"unexpected interface call: GetActiveGossipCategoriesSize"); throw 0; }
 virtual void ClearActiveGossipCategories() { assert(!"unexpected interface call: ClearActiveGossipCategories"); throw 0; }
 virtual bool* GetIsGossipForPlayer(CCharString) { assert(!"unexpected interface call: GetIsGossipForPlayer"); throw 0; }
 virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() { assert(!"unexpected interface call: GetIsGossipForPlayer"); throw 0; }
 virtual int GetIsGossipForPlayerSize() { assert(!"unexpected interface call: GetIsGossipForPlayerSize"); throw 0; }
 virtual void ClearIsGossipForPlayer() { assert(!"unexpected interface call: ClearIsGossipForPlayer"); throw 0; }
 virtual void SetGossip(CCharString, CCharString, int) { assert(!"unexpected interface call: SetGossip"); throw 0; }
 virtual CCharString GetGossip(CCharString, int) { assert(!"unexpected interface call: GetGossip"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) { assert(!"unexpected interface call: GetGossip"); throw 0; }
 virtual int GetGossipSize(CCharString) { assert(!"unexpected interface call: GetGossipSize"); throw 0; }
 virtual void ClearGossip(CCharString) { assert(!"unexpected interface call: ClearGossip"); throw 0; }
 virtual void RemoveGossip(CCharString) { assert(!"unexpected interface call: RemoveGossip"); throw 0; }
 virtual void AddGossip(CCharString, CCharString) { assert(!"unexpected interface call: AddGossip"); throw 0; }
 virtual void AddGossip(CCharString) { assert(!"unexpected interface call: AddGossip"); throw 0; }
 virtual void SetGossipVillages(CCharString, CCharString, int) { assert(!"unexpected interface call: SetGossipVillages"); throw 0; }
 virtual CCharString GetGossipVillages(CCharString, int) { assert(!"unexpected interface call: GetGossipVillages"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) { assert(!"unexpected interface call: GetGossipVillages"); throw 0; }
 virtual int GetGossipVillagesSize(CCharString) { assert(!"unexpected interface call: GetGossipVillagesSize"); throw 0; }
 virtual void ClearGossipVillages(CCharString) { assert(!"unexpected interface call: ClearGossipVillages"); throw 0; }
 virtual void RemoveGossipVillages(CCharString) { assert(!"unexpected interface call: RemoveGossipVillages"); throw 0; }
 virtual void AddGossipVillages(CCharString, CCharString) { assert(!"unexpected interface call: AddGossipVillages"); throw 0; }
 virtual void AddGossipVillages(CCharString) { assert(!"unexpected interface call: AddGossipVillages"); throw 0; }
 virtual void SetGossipFactions(CCharString, CCharString, int) { assert(!"unexpected interface call: SetGossipFactions"); throw 0; }
 virtual CCharString GetGossipFactions(CCharString, int) { assert(!"unexpected interface call: GetGossipFactions"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) { assert(!"unexpected interface call: GetGossipFactions"); throw 0; }
 virtual int GetGossipFactionsSize(CCharString) { assert(!"unexpected interface call: GetGossipFactionsSize"); throw 0; }
 virtual void ClearGossipFactions(CCharString) { assert(!"unexpected interface call: ClearGossipFactions"); throw 0; }
 virtual void RemoveGossipFactions(CCharString) { assert(!"unexpected interface call: RemoveGossipFactions"); throw 0; }
 virtual void AddGossipFactions(CCharString, CCharString) { assert(!"unexpected interface call: AddGossipFactions"); throw 0; }
 virtual void AddGossipFactions(CCharString) { assert(!"unexpected interface call: AddGossipFactions"); throw 0; }
};
const CCharString& CScriptThing::GetName() const { assert(!"unexpected CScriptThing call: GetName"); throw 0; }
CCharString CScriptThing::GetDefName() const { assert(!"unexpected CScriptThing call: GetDefName"); throw 0; }
C3DVector CScriptThing::GetFocalPos() const { assert(!"unexpected CScriptThing call: GetFocalPos"); throw 0; }
C3DVector CScriptThing::GetHomePos() const { assert(!"unexpected CScriptThing call: GetHomePos"); throw 0; }
CCharString CScriptThing::GetCurrentMapName() const { assert(!"unexpected CScriptThing call: GetCurrentMapName"); throw 0; }
CCharString CScriptThing::GetHomeMapName() const { assert(!"unexpected CScriptThing call: GetHomeMapName"); throw 0; }
float CScriptThing::GetAngleXY() const { assert(!"unexpected CScriptThing call: GetAngleXY"); throw 0; }
CThing* CScriptThing::GetPThing() const { assert(!"unexpected CScriptThing call: GetPThing"); throw 0; }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { assert(!"unexpected CScriptThing call: GetPThingUniqueID"); throw 0; }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { assert(!"unexpected CScriptThing call: GetCurrentStateGroupType"); throw 0; }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { assert(!"unexpected CScriptThing call: GetCurrentScriptPriority"); throw 0; }
bool CScriptThing::IsSneaking() const { assert(!"unexpected CScriptThing call: IsSneaking"); throw 0; }
bool CScriptThing::IsAwareOfHero() const { assert(!"unexpected CScriptThing call: IsAwareOfHero"); throw 0; }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsTriggeredBy"); throw 0; }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsKilledBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoKilledMe() const { assert(!"unexpected CScriptThing call: MsgWhoKilledMe"); throw 0; }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsUsedBy"); throw 0; }
bool CScriptThing::MsgIsHitBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitBy"); throw 0; }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithFlourish"); throw 0; }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithDecapitate"); throw 0; }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithWeapon"); throw 0; }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithProjectileWeapon"); throw 0; }
CScriptThing CScriptThing::MsgWhoHitMe() const { assert(!"unexpected CScriptThing call: MsgWhoHitMe"); throw 0; }
bool CScriptThing::MsgIsTalkedToBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsTalkedToBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { assert(!"unexpected CScriptThing call: MsgWhoTalkedToMe"); throw 0; }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { assert(!"unexpected CScriptThing call: MsgExpressionPerformedTo"); throw 0; }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { assert(!"unexpected CScriptThing call: MsgWhoExpressedToMe"); throw 0; }
int CScriptThing::MsgHowLongWasExpressionPerformed() const { assert(!"unexpected CScriptThing call: MsgHowLongWasExpressionPerformed"); throw 0; }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsKnockedOutBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { assert(!"unexpected CScriptThing call: MsgWhoKnockedMeOut"); throw 0; }
bool CScriptThing::MsgReceivedMoney(int&) const { assert(!"unexpected CScriptThing call: MsgReceivedMoney"); throw 0; }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsPresentedWithItem"); throw 0; }
bool CScriptThing::MsgReceivedInventoryItem() const { assert(!"unexpected CScriptThing call: MsgReceivedInventoryItem"); throw 0; }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { assert(!"unexpected CScriptThing call: MsgPerformedSpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnySpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnyAggressiveSpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnyNonAggressiveSpecialAbility"); throw 0; }
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitBySpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnySpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnyAggressiveSpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnyNonAggressiveSpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgOpenedChest() const { assert(!"unexpected CScriptThing call: MsgOpenedChest"); throw 0; }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithMeleeWeapon"); throw 0; }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithMeleeWeapon"); throw 0; }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithRangedWeapon"); throw 0; }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithRangedWeapon"); throw 0; }
bool CScriptThing::MsgHitEnemyWithBareHands() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithBareHands"); throw 0; }
bool CScriptThing::MsgHitFriendWithBareHands() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithBareHands"); throw 0; }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { assert(!"unexpected CScriptThing call: MsgAttackedWithMeleeWeaponWithoutHittingAnything"); throw 0; }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { assert(!"unexpected CScriptThing call: MsgAttackedWithBareHandsWithoutHittingAnything"); throw 0; }
bool CScriptThing::MsgTalkedToAnyone() const { assert(!"unexpected CScriptThing call: MsgTalkedToAnyone"); throw 0; }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned int,std::allocator<unsigned int> >&) const { assert(!"unexpected CScriptThing call: MsgGetThingsKilled"); throw 0; }
bool CScriptThing::MsgPerformedFlourish() const { assert(!"unexpected CScriptThing call: MsgPerformedFlourish"); throw 0; }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { assert(!"unexpected CScriptThing call: MsgPerformedSuccessfulFlourish"); throw 0; }
bool CScriptThing::MsgOnMoralityChanged(int&) const { assert(!"unexpected CScriptThing call: MsgOnMoralityChanged"); throw 0; }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { assert(!"unexpected CScriptThing call: MsgOnCutSceneAnimEvent"); throw 0; }
bool CScriptThing::MsgIsKicked() const { assert(!"unexpected CScriptThing call: MsgIsKicked"); throw 0; }
bool CScriptThing::IsUnconscious() const { assert(!"unexpected CScriptThing call: IsUnconscious"); throw 0; }
bool CScriptThing::IsUsable() const { assert(!"unexpected CScriptThing call: IsUsable"); throw 0; }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { assert(!"unexpected CScriptThing call: IsBeingCarriedBy"); throw 0; }
bool CScriptThing::IsOpenDoor() const { assert(!"unexpected CScriptThing call: IsOpenDoor"); throw 0; }
bool CScriptThing::IsSummonedCreature() const { assert(!"unexpected CScriptThing call: IsSummonedCreature"); throw 0; }
void CScriptThing::SetAsUsable(bool) { assert(!"unexpected CScriptThing call: SetAsUsable"); throw 0; }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { assert(!"unexpected CScriptThing call: SetFriendsWithEverythingFlag"); throw 0; }
bool CScriptThing::GetActivationTriggerStatus() const { assert(!"unexpected CScriptThing call: GetActivationTriggerStatus"); throw 0; }
void CScriptThing::SetActivationTriggerStatus(bool) { assert(!"unexpected CScriptThing call: SetActivationTriggerStatus"); throw 0; }
void CScriptThing::SetToKillOnLevelUnload(bool) { assert(!"unexpected CScriptThing call: SetToKillOnLevelUnload"); throw 0; }
void CScriptThing::UpdateThingAttachment() { assert(!"unexpected CScriptThing call: UpdateThingAttachment"); throw 0; }
void CScriptThing::IncrementScriptCounter() { assert(!"unexpected CScriptThing call: IncrementScriptCounter"); throw 0; }
void CScriptThing::DecrementScriptCounter() { assert(!"unexpected CScriptThing call: DecrementScriptCounter"); throw 0; }
int CScriptThing::GetScriptCounter() { assert(!"unexpected CScriptThing call: GetScriptCounter"); throw 0; }
bool CScriptThing::IsDead() const { assert(!"unexpected CScriptThing call: IsDead"); throw 0; }
bool CScriptThing::IsNull() const { assert(!"unexpected CScriptThing call: IsNull"); throw 0; }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { assert(!"unexpected CScriptThing call: IsEqualTo"); throw 0; }

static int live_strings=0,live_things=0;
static const char* str(const CCharString& value){const char* p;memcpy(&p,&value,sizeof(p));return p?p:"";}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* p,int n){if(n<0)n=(int)strlen(p);char* out=new char[n+1];memcpy(out,p,n);out[n]=0;PStringData=(CCharStringData*)out;++live_strings;}
CCharString::CCharString(const CCharString& other){const char* p=str(other);int n=(int)strlen(p);char* out=new char[n+1];memcpy(out,p,n+1);PStringData=(CCharStringData*)out;++live_strings;}
CCharString::~CCharString(){if(PStringData){delete[] (char*)PStringData;--live_strings;}}
CWideString::~CWideString(){}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase(){}
CScriptBase::CScriptBase(){}
CScriptBase::~CScriptBase(){}
void CScriptBase::RegisterMain(){assert(false);}
void CScriptBase::Main(){assert(false);}
void CScriptBase::Init(){assert(false);}
void CScriptBase::OnPersist(CPersistContext&){assert(false);}
template<class T> CLevelScriptBase<T>::~CLevelScriptBase(){}
CTimer::CTimer():TimerIndex(0){}
CTimer::~CTimer(){}
NScript::CQ_BountyHuntScript::CQ_BountyHuntScript(CScriptDataBase* d,CGameScriptInterfaceBase& g):CLevelScriptBase<CQ_SunnyvaleMasterData>(d,g){}
NScript::CQ_BountyHuntScript::~CQ_BountyHuntScript(){}
void NScript::CQ_BountyHuntScript::RegisterMain(){assert(false);}
void NScript::CQ_BountyHuntScript::Main(){assert(false);}
void NScript::CQ_BountyHuntScript::Init(){assert(false);}
void NScript::CQ_BountyHuntScript::OnPersist(CPersistContext&){assert(false);}
CCharString CScriptThing::NullString;
C3DVector CScriptThing::NullCoord(0.0f,0.0f,0.0f);
void CScriptThing::SetImplementation(CCountedPointer<CScriptThing> p){PImp=p;}
CCharString CScriptThing::GetDataString()const {if(PImp.IsNull())return NullString;return PImp->GetDataString();}
struct Trace {int values[256];int count,frames,checks,randoms;Trace():count(0),frames(0),checks(0),randoms(0){} void add(int x){assert(count<256);values[count++]=x;}};
static Trace actual;
static int stop_after,seed,wait_delay,fail_creation;
static bool terminating(){++actual.checks;actual.add(100+actual.checks);return stop_after && actual.checks>=stop_after;}
bool CScriptBase::IsActiveThreadTerminating(){return terminating();}
static int fixture_rand(){int value=(seed+actual.randoms)%4;++actual.randoms;actual.add(200+value);return value;}
class ScenarioThing:public CScriptThing {
public:
 int index,ready_frame;bool marker,initial_alive;C3DVector position;
 ScenarioThing(int i,bool m,bool alive,int ready):index(i),ready_frame(ready),marker(m),initial_alive(alive),position((float)(i*2),4.0f,8.0f){++live_things;}
 ~ScenarioThing(){--live_things;}
 bool IsAlive()const {return initial_alive && actual.frames>=ready_frame;}
 const C3DVector& GetPos()const{return position;}
 CCharString GetDataString()const {assert(marker);actual.add(400+index);char b[24];sprintf(b,"marker%d",index);return CCharString(b);}
 void SetDataString(const CCharString& value){assert(!marker);char b[24];sprintf(b,"marker%d",index);assert(strcmp(str(value),b)==0);actual.add(500+index);}
};
class ScenarioGame:public FixtureScriptInterface {
public:
 std::vector<CScriptThing> input;
 ScenarioGame(int n,int pattern){for(int i=0;i<n;++i){bool alive=pattern==0 || (pattern==1 && i%2==0);CScriptThing h;h.SetImplementation(CCountedPointer<CScriptThing>(new ScenarioThing(i,true,alive,0)));CScriptThing explicit_copy(h);input.push_back(explicit_copy);}}
 int GetAllThingsWithScriptName(const CCharString& name,std::vector<CScriptThing,std::allocator<CScriptThing> >& output)const {assert(strcmp(str(name),"assigned_spawn")==0);for(unsigned int i=0;i<input.size();++i)output.push_back(input[i]);return (int)input.size();}
 CScriptThing CreateCreature(const CCharString& name,const C3DVector& pos,const CCharString& script,bool delayed)const {
  assert(!delayed&&strcmp(str(script),"BH_GangGruntSpawn")==0);const char* text=str(name);assert(strlen(text)==30&&memcmp(text,"CREATURE_BANDIT_GRUNT_LEVEL3_",29)==0);
  int kind=text[29]-'A';assert(kind>=0&&kind<4);int i=(int)(pos.X/2.0f);assert(pos.Y==4.0f&&pos.Z==8.0f);actual.add(300+i*4+kind);
  CScriptThing result;if(!fail_creation)result.SetImplementation(CCountedPointer<CScriptThing>(new ScenarioThing(i,false,true,actual.frames+wait_delay)));return result;
 }
 void NewScriptFrame()const {++actual.frames;actual.add(600);assert(actual.frames<50);}
 CCharString GetActiveQuestName()const {actual.add(700);return CCharString("BountyHunt");}
 void EntityAttachToScript(const CScriptThing& grunt,const CCharString& name)const {assert(grunt.IsAlive()&&strcmp(str(name),"BountyHunt")==0);actual.add(800);}
};
static Trace oracle(int n,int pattern){
 Trace t;
 for(int i=0;i<n;++i){
  bool alive=pattern==0||(pattern==1&&i%2==0);if(!alive)continue;
  ++t.checks;t.add(100+t.checks);if(stop_after&&t.checks>=stop_after)break;
  int kind=(seed+t.randoms)%4;++t.randoms;t.add(200+kind);
  ++t.checks;t.add(100+t.checks);if(stop_after&&t.checks>=stop_after)break;
  t.add(300+i*4+kind);
  int ready=t.frames+wait_delay;bool quit=false;
  while(fail_creation||t.frames<ready){++t.frames;t.add(600);++t.checks;t.add(100+t.checks);if(stop_after&&t.checks>=stop_after){quit=true;break;}}
  if(quit)break;
  ++t.checks;t.add(100+t.checks);if(stop_after&&t.checks>=stop_after)break;
  t.add(400+i);t.add(400+i);t.add(500+i);t.add(700);t.add(800);
 }
 return t;
}
static void run_case(int n,int pattern){
 actual=Trace();Trace expected=oracle(n,pattern);
 {ScenarioGame game(n,pattern);NScript::CQ_BountyHuntScript script(0,game);script.SpawnBanditGangGrunts(CCharString("assigned_spawn"));}
 assert(actual.count==expected.count&&actual.frames==expected.frames&&actual.checks==expected.checks&&actual.randoms==expected.randoms);
 for(int i=0;i<actual.count;++i)assert(actual.values[i]==expected.values[i]);
 assert(live_strings==0&&live_things==0);
}
int main(){
 const int counts[3]={0,1,4},stops[6]={0,1,2,3,5,9};int cases=0;
 fail_creation=0;
 for(int n=0;n<3;++n)for(int pattern=0;pattern<3;++pattern)for(seed=0;seed<4;++seed)for(int delay=0;delay<2;++delay)for(int stop=0;stop<6;++stop){wait_delay=delay*2;stop_after=stops[stop];run_case(counts[n],pattern);++cases;}
 fail_creation=1;wait_delay=0;
 for(seed=0;seed<4;++seed)for(stop_after=3;stop_after<=5;++stop_after){run_case(4,0);++cases;}
 assert(cases==444);puts("00d17470_TEST PASS");return 0;
}
