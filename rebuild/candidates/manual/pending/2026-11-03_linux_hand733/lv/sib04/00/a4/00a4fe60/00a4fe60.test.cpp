#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
typedef long FableI64;typedef unsigned long FableU64;
#else
typedef __int64 FableI64;typedef unsigned __int64 FableU64;
#endif
#define FABLE_FASTCALL __fastcall
#define FABLE_STATIC_ASSERT(x)
struct AnimationFixture;
#define FABLE_RETAIL_ANIMATION_SHARED
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGMANAGER_BINARY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H
// STLport 4.6.2 excerpts: stl/_alloc.h, _vector.h, _list.h, _bitset.h,
// _pair.h and _function_base.h. Original inheritance, field names and keys.
// Only declarations and operations needed by these translation units appear.
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
struct __true_type { friend struct ::AnimationFixture; };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { friend struct ::AnimationFixture; };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { friend struct ::AnimationFixture; };
template<class T> struct __type_traits { friend struct ::AnimationFixture; typedef __false_type is_POD_type; };
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
struct __false_type { friend struct ::AnimationFixture; };
#else
struct __false_type { friend struct ::AnimationFixture; __false_type() { } };
#endif
struct input_iterator_tag { friend struct ::AnimationFixture; };
struct forward_iterator_tag : public input_iterator_tag { friend struct ::AnimationFixture; forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { friend struct ::AnimationFixture; };
struct random_access_iterator_tag : public bidirectional_iterator_tag { friend struct ::AnimationFixture; };
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
template<class T> struct __type_traits { friend struct ::AnimationFixture; typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
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
struct __false_type { friend struct ::AnimationFixture; };
#else
struct __false_type { friend struct ::AnimationFixture; __false_type() { } };
#endif
struct input_iterator_tag { friend struct ::AnimationFixture; };
struct forward_iterator_tag : public input_iterator_tag { friend struct ::AnimationFixture; };
struct bidirectional_iterator_tag : public forward_iterator_tag { friend struct ::AnimationFixture; };
struct random_access_iterator_tag : public bidirectional_iterator_tag { friend struct ::AnimationFixture; random_access_iterator_tag() { } };
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
template<class _Tp> class allocator { friend struct ::AnimationFixture;
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
class _STLP_alloc_proxy : public _MaybeReboundAlloc { friend struct ::AnimationFixture;
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base { friend struct ::AnimationFixture;
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
class vector : public _Vector_base<_Tp,_Alloc> { friend struct ::AnimationFixture;
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
    // Original STLport4.6 _vector.h erase/clear and non-trivial push_back.
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
    // STLport _vector.c non-trivial growth: copy prefix, fill, copy suffix, release.
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
    // Original STLport _vector.h true-tag overflow path: trivial copy then fill.
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
template<class _T1,class _T2> struct pair { friend struct ::AnimationFixture;
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
struct _List_node_base { friend struct ::AnimationFixture;
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { friend struct ::AnimationFixture; _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { friend struct ::AnimationFixture; typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { friend struct ::AnimationFixture; typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base { friend struct ::AnimationFixture;
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base { friend struct ::AnimationFixture;
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
template<class _Tp,class _Alloc> class _List_base { friend struct ::AnimationFixture;
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
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> { friend struct ::AnimationFixture;
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
template<size_t _Nw> struct _Base_bitset { friend struct ::AnimationFixture;
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP
// Original STLport _range_errors.h no-exceptions branch.
inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> { friend struct ::AnimationFixture;
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function { friend struct ::AnimationFixture;
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif
#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING
// Original PDB CWideString, class key V, one protected pointer at +0.
#ifdef FABLE_RETAIL_HERO_LOGBOOK
#ifndef FABLE_ENGINE_CWIDESTRINGDATA_H
#define FABLE_ENGINE_CWIDESTRINGDATA_H
#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H
// STLport 4.6.2 _string.h/_string.c excerpts, adapted to this header subset.
// Copyright (c) 1994 Hewlett-Packard Company; 1996,1997 Silicon Graphics
// Computer Systems, Inc.; 1997 Moscow Center for SPARC Technology;
// 1999 Boris Fomitchev. Permission to use/copy/modify is granted without
// fee provided these notices are retained. Provided as is, without warranty.

#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
#ifdef FABLE_RETAIL_HERO_LOGBOOK
// Authentic STLport4.6.2 char_traits.h125..150,207..209; native403c60.
template<class C,class I> class __char_traits_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
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
template<class T,class A> class _String_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
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
struct _String_reserve_t { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; };
template<class C,class Traits=char_traits<C>,class A=allocator<C> >
class basic_string:protected _String_base<C,A> { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
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

// Ego_r TPI d45a/d454: protected Data, public NoRefs and const equality.
// Native47aa32..44 loads both start/finish pairs; native refcount is +12.
class CWideStringData { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
    bool operator==(const CWideStringData& other)const { const std::basic_string<unsigned short>& data=Data;return data==other.Data; }
};
#endif

#endif
class CCharString;
class CWideStringData;
class CWideString { friend struct ::AnimationFixture;
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
    static long NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;
};
#endif
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
// PDB C3DVector complete TPI 0x2783: public X/Y/Z at 0/4/8, size 12.
// Embedded alignment is under review; this header introduces no packing directive.
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
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned long*)&n &= 0x7fffffffUL;
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
 typedef unsigned long word_type;
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

// Ego_r TPI0xa59e, public long X/Y, native coordinate callers at0074a8a8/8e3.
class C2DCoordI { friend struct ::AnimationFixture; friend struct ::AnimationFixture; public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif

#endif
class C3DVector { friend struct ::AnimationFixture;
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
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

// Inert test-only return type for an unused virtual that always throws.
class CQuaternion{};
// C3DAnimation2::InitialiseSharedAnimation(C3DAnimFile const &,EAnimationType) -- initialize shared animation data from the supplied animation file.
// Leaf: ?InitialiseSharedAnimation@C3DAnimation2@@QAE_NABVC3DAnimFile@@W4EAnimationType@@@Z
#define FABLE_RETAIL_ANIMATION_SHARED
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGMANAGER_BINARY
#define FABLE_RETAIL_CDATAINPUTSTREAM

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMATION2_H
#define FABLE_ENGINE_C3DANIMATION2_H

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).


class CBaseClass { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};

#endif // FABLE_ENGINE_CBASECLASS_H


class CBaseClassNonCopyable : public CBaseClass { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#endif // FABLE_ENGINE_CBASECLASSNONCOPYABLE_H

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>



#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA
// Retail opt-in (Codex pass11): original Ego_r TPI fields and native
// CCharString memory accounting confirm13byte data and17byte refcount owner.
// This branch reuses the existing packed scope; default declarations stay intact.
template<class T> class CBasicString { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
#if defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH)
    static long __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_RELATIVE_PATH
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    unsigned long GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool operator==(const T* text) const;
#endif
};
class CCharStringData { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#else
class CCharStringData
{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; public:
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
{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     // 0x0099AF10 ?GetAsCharArray@CWideString@@QBEPBGXZ
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         // 0x0099B070 ?FindCharPos@CWideString@@QBEJG@Z
    void __cdecl Sprintf(const wchar_t* format, ...);          // 0x0099BA70 ?Sprintf@CWideString@@QAAXPBGZZ
    void SetLength(long length);                               // 0x0099BC50 ?SetLength@CWideString@@QAEXJ@Z
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
    CWideStringData* AllocStringData(                          // 0x0099B3C0 protected (Ego_r / FableWin: IAE)
        const wchar_t* text,
        long length);

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
{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CCharString();
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);                     // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z (Ego_r / FableWin)
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     // 0x0099E9B0 protected (Ego_r / FableWin: IAE)
    CCharStringData* AllocStringData(                          // 0x0099EA60 protected (Ego_r / FableWin: IAE)
        const char* text,
        long length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];


#endif

#endif
class CCharStringData;
class CCharString { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CCharString();
    CCharString(const char* text, long length = -1);
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
    unsigned long GetCRC() const; // Ego_r00403c00; native00404310 calls the original CRC kernel.
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
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

#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).


class CCPPointerInfo { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};

template <class T> class CCountedPointer { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

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
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
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
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

#ifndef FABLE_ENGINE_C3DANIMATIONSTATS_H
#define FABLE_ENGINE_C3DANIMATIONSTATS_H

#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Retail opt-in (2026-10-30, class-model migration wave 5): define FABLE_RETAIL_BASE_OBJECT for the polymorphic retail class.
// RTTI .?AVCBaseObject@@ : CBaseClass (ghidra_out/rtti_bases.tsv, 1 base). Vtable 0x0129A7D8, 2 slots (the next dword
// is string data): slot 0 scalar deleting dtor 0x0099A4D0, slot 1 Validate 0x0099A400 (`ret`). Members and spellings are
// the Ego_r.pdb and FableWin.pdb names, identical in both (Ego_r 0x00B01FD0-0x00B020F0, FableWin 0x03044AF0-0x03044E40):
// ??0CBaseObject@@IAE@XZ, ??0CBaseObject@@IAE@ABV0@@Z, ??4CBaseObject@@IAEAAV0@ABV0@@Z (protected), ??1CBaseObject@@MAE@XZ
// (protected virtual), ?Validate@CBaseObject@@UBEXXZ, ?ClearIntelligentPointers@CBaseObject@@QAEXXZ,
// ?InternalValidate@CBaseObject@@IBEXXZ. Retail bodies (CBaseClass.cpp's TU): ctor 0x0099A380 and copy ctor 0x0099A390
// both `mov [eax],0x0129A7D8; mov [eax+4],0` (the copy does not copy the pointer); operator= 0x0099A3B0 `mov eax,ecx;
// ret 4`; ClearIntelligentPointers 0x0099A3C0 (zeroes [p+0], deletes p when [p+4]==0, zeroes +4); ~CBaseObject 0x0099A430
// (the same body inline, then the CBaseClass vtable store: same-TU auto-inlining, wave 2 LESSONS item 2). All of them are out
// of line: no landed row outside that TU stores 0x0129A7D8, and the four spellings above are bound in reloc_bindings.tsv.
// Layout (retail bytes): IntelligentPointer +0x04 (read/zeroed at [ecx+4] by 0x0099A3C0); sizeof 8 (Ego_r.pdb agrees).
// unproven: InternalValidate has no retail body (debug only: Validate is `ret`); the class key of CBaseObjectPointer
// (no PDB mangled name carries it; 1,839 landed rows say `class`, 72 `struct`).
// The CBaseClass.cpp TU rows (0x0099A2D0-0x0099A4D0) model this class row-locally by design (auto-inlined bodies).
#ifdef FABLE_RETAIL_BASE_OBJECT


class CBaseObjectPointer;
class CBaseObject : public CBaseClass { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
protected:
    CBaseObject();                                                       // 0x0099A380
    CBaseObject(const CBaseObject&);                                     // 0x0099A390
    CBaseObject& operator=(const CBaseObject&);                          // 0x0099A3B0
public:
    void ClearIntelligentPointers();                                     // 0x0099A3C0
    virtual void Validate() const;                                       // slot 1  0x0099A400
protected:
    virtual ~CBaseObject();                                              // slot 0  0x0099A4D0 (scalar deleting); ~ 0x0099A430
    void InternalValidate() const;                                       // unproven: no retail body
public:
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
private:
#endif
    CBaseObjectPointer* IntelligentPointer;                              // +0x04
};
#else // flattened Ego_r layout (the landed default)
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CBaseObject = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
private:
#endif
    CBaseObjectPointer* IntelligentPointer;                  // +0x04
};
#pragma pack(pop)


#endif // FABLE_RETAIL_BASE_OBJECT
#endif // FABLE_ENGINE_CBASEOBJECT_H

#ifndef FABLE_ENGINE_EANIMATIONTYPE_H
#define FABLE_ENGINE_EANIMATIONTYPE_H

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
enum EAnimationType{ANIMTYPE_BASE=0,ANIMTYPE_BASE_RELATIVE=4,ANIMTYPE_COMBINATION_PARTIAL=3,ANIMTYPE_COMBINATION_DELTA=5};
#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationStats:public CBaseObject{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:
C3DAnimationStats(const C3DAnimationStats&);
C3DAnimationStats();
virtual ~C3DAnimationStats();
virtual C3DVector GetMovementOffsetAtTime(float) const=0;
virtual C3DVector GetMovementAtTime(float) const=0;
virtual bool HasRotation() const=0;
virtual CQuaternion GetRotationAtTime(float) const=0;
virtual void GetDeltaMovement(CMatrix3x4&,float,float) const=0;
void InitialiseStats(const C3DAnimFile&,EAnimationType);
private:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;unsigned char MovementVector[12];unsigned char _pad_1e[2];unsigned char TimingEvents[12];};
#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CMOVABLERESOURCE_H
#define FABLE_ENGINE_CMOVABLERESOURCE_H

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CMovableResource{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;private:
CMovableResource(const CMovableResource&);
public:
CMovableResource();
virtual ~CMovableResource();
virtual void RelocateData(long,void *,void *,long)=0;
virtual bool CanRelocateData(long) const;
virtual unsigned long Verify() const;
};
#endif
#else
#ifndef FABLE_ENGINE_CMOVABLERESOURCE_H
#define FABLE_ENGINE_CMOVABLERESOURCE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Retail opt-in (chunk 708): define FABLE_RETAIL_CMOVABLERESOURCE before the include for the polymorphic retail class.
// RTTI .?AVCMovableResource@@ has no base (ghidra_out/rtti_bases.tsv). Vtable 0x0129B860, 4 slots (slot 4 is string data):
// slot 0 the scalar deleting dtor 0x009D5110 (`mov [esi],0x0129B860` then the null-tested delete: the empty dtor body is inline),
// slot 1 _purecall 0x00BFEA08 = RelocateData (pure here; CIndexBuffer 0x0129EF2C slot 1 = ?RelocateData@CIndexBuffer@@UAEXJPAX0J@Z
// 0x00AC1F50), slot 2 CanRelocateData 0x009D50F0 (`mov al,1 ; ret 4`, FableWin ?CanRelocateData@CMovableResource@@UBE_NJ@Z),
// slot 3 Verify 0x009D5100 (`mov eax,7Bh ; ret`, Ego_r / FableWin ?Verify@CMovableResource@@UBEKXZ). Ego_r has no out-of-line
// ??0 / ??1 (only ??_E 0x009AD1F0); FableWin (debug) lists ??0CMovableResource@@QAE@XZ / ??1CMovableResource@@UAE@XZ; 59 retail
// sites store 0x0129B860 inline. Sizeof 4 (vptr only, Ego_r agrees).
#ifdef FABLE_RETAIL_CMOVABLERESOURCE

class CMovableResource { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    CMovableResource() { }                                               // FableWin ??0CMovableResource@@QAE@XZ (inline in retail)
    virtual ~CMovableResource() { }                                      // slot 0  0x009D5110 (scalar deleting, body inline)
    virtual void RelocateData(long, void*, void*, long) = 0;             // slot 1  _purecall
    virtual bool CanRelocateData(long) const;                            // slot 2  0x009D50F0
    virtual unsigned long Verify() const;                                // slot 3  0x009D5100
};
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CMovableResource = 0x4.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CMovableResource { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void* __vftable;                                         // +0x00 vptr, or first dword of a flattened base subobject
};
#pragma pack(pop)


#endif // FABLE_RETAIL_CMOVABLERESOURCE
#endif // FABLE_ENGINE_CMOVABLERESOURCE_H

#endif

#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCE_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCE_H

#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequenceData{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:class C3DVectorWord;
 C3DAnimationSequenceData():BoneName(0),RotationTrack(0),RotationTrackSize(0),PositionTrackSize(0),PalettedRotationTrackSize(0),PalettedPositionTrackSize(0),PositionTrack(0),PalettedRotationTrack(0),PalettedPositionTrack(0){FrameCount=0;IdentityFlag=false;}
 unsigned long GetFrameCount()const{return FrameCount;}float GetSamplesPerSecond()const{return SamplesPerSecond;}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,CSimpleMemoryPool&);
private:char* BoneName;float SamplesPerSecond;unsigned short FrameCount;
 unsigned char BoneType:5;unsigned char EnabledFlag:1;unsigned char AnimatedFlag:1;unsigned char IdentityFlag:1;
 unsigned char RotationTrackMode:2;unsigned char PositionTrackMode:2;unsigned char ScalingTrackMode:2;
 float PositionFactor,ScalingFactor;CQuaternion* RotationTrack;
 unsigned short RotationTrackSize,PositionTrackSize,PalettedRotationTrackSize,PalettedPositionTrackSize;
 C3DVectorWord* PositionTrack;unsigned char* PalettedRotationTrack;unsigned char* PalettedPositionTrack;
};
#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequence{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:C3DAnimationSequence():Data(0),ParentIndex(-1),Shared(false){}
 unsigned long GetFrameCount()const{return Data->GetFrameCount();}float GetSamplesPerSecond()const{return Data->GetSamplesPerSecond();}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,C3DAnimationSequenceData*,CSimpleMemoryPool&);
 void ReadCompressedFromFile(CDataInputStream&,unsigned long,C3DAnimationSequenceData*,CSimpleMemoryPool&);
private:C3DAnimationSequenceData* Data;short ParentIndex;bool Shared;
};
#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimation2:public C3DAnimationStats,public CMovableResource{ friend struct ::AnimationFixture;
public:
C3DAnimation2(const C3DAnimation2&);
C3DAnimation2();
virtual ~C3DAnimation2();
bool InitialiseSharedAnimation(const C3DAnimFile&,EAnimationType);
virtual C3DVector GetMovementOffsetAtTime(float) const;
virtual C3DVector GetMovementAtTime(float) const;
virtual bool HasRotation() const;
virtual CQuaternion GetRotationAtTime(float) const;
virtual void GetDeltaMovement(CMatrix3x4&,float,float) const;
virtual void RelocateData(long,void *,void *,long);
virtual bool CanRelocateData(long) const;
private:
void ReadCompressedFileData(const C3DAnimFileObjectChunk&,CSimpleMemoryPool&);

private:CCountedPointer<CSimpleMemoryPool> Allocator;CMovableResourceMemoryPool* MemoryPool;long MemoryHandle;
 CCountedPointer<C3DAnimationSequence> MovementTrack;CCountedPointer<C3DAnimationSequenceData> MovementTrackData;
 float SamplesPerSecond;unsigned long FrameCount,SequenceCount;C3DAnimationSequence* Sequences;
 unsigned long SequenceDataBufferSize;C3DAnimationSequenceData* SequenceDataBuffer;bool* BoneMask;bool* RigidBoneMask;
 C3DVector* RigidBonePositions;bool LockedFromMemMemoryRelocation;unsigned char _pad_75[3];unsigned char InstanceList[4];
};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMATION2_H
#define FABLE_ENGINE_C3DANIMATION2_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimation2 = 0x88.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimation2 = 0x7c.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class C3DAnimationSequence;
class C3DAnimationSequenceData;
class C3DVector;
class CCPPointerInfo;
class CMovableResourceMemoryPool;
class CSimpleMemoryPool;

#pragma pack(push, 1)
class C3DAnimation2 { friend struct ::AnimationFixture;
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _pad_0x04[0x4];              // +0x04
    unsigned char               _pad_0x08[0x4];              // +0x08
    unsigned char               _pad_0x0c[0x4];              // +0x0c
    unsigned char               _pad_0x10[0x4];              // +0x10
    unsigned char               _pad_0x14[0x4];              // +0x14
    unsigned char               _pad_0x18[0x4];              // +0x18
    unsigned char               _pad_0x1c[0x2];              // +0x1c
    unsigned char               _pad_0x1e[0x12];             // +0x1e
    CSimpleMemoryPool*          Allocator_Data;              // +0x30 CCountedPointer<CSimpleMemoryPool>::Data
    CCPPointerInfo*             Allocator_Info;              // +0x34 CCountedPointer<CSimpleMemoryPool>::Info
    CMovableResourceMemoryPool* MemoryPool;                  // +0x38
    long                        MemoryHandle;                // +0x3c
    C3DAnimationSequence*       MovementTrack_Data;          // +0x40 CCountedPointer<C3DAnimationSequence>::Data
    CCPPointerInfo*             MovementTrack_Info;          // +0x44 CCountedPointer<C3DAnimationSequence>::Info
    C3DAnimationSequenceData*   MovementTrackData_Data;      // +0x48 CCountedPointer<C3DAnimationSequenceData>::Data
    CCPPointerInfo*             MovementTrackData_Info;      // +0x4c CCountedPointer<C3DAnimationSequenceData>::Info
    float                       SamplesPerSecond;            // +0x50
    unsigned long               FrameCount;                  // +0x54
    unsigned long               SequenceCount;               // +0x58
    C3DAnimationSequence*       Sequences;                   // +0x5c
    unsigned long               SequenceDataBufferSize;      // +0x60
    C3DAnimationSequenceData*   SequenceDataBuffer;          // +0x64
    void*                       BoneMask;                    // +0x68 bool *
    void*                       RigidBoneMask;               // +0x6c bool *
    C3DVector*                  RigidBonePositions;          // +0x70
    bool                        LockedFromMemMemoryRelocation; // +0x74
    unsigned char               _pad_0x75[0x1];              // +0x75
    unsigned char               _pad_0x76[0x2];              // +0x76
    unsigned char               InstanceList[0x4];           // +0x78 list<CIntelligentPointer<C3DAnimation2>,std::allocator<CIntelligentPointer<C3DAnimation2>_>_>
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMATION2_H

#endif

#ifndef FABLE_ENGINE_C3DANIMFILE_H
#define FABLE_ENGINE_C3DANIMFILE_H

#ifndef FABLE_ENGINE_CCHUNKEDFILE_H
#define FABLE_ENGINE_CCHUNKEDFILE_H




class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CChunkedFile:public CBaseClassNonCopyable{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:
CChunkedFile(const CChunkedFile&);
CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&);
virtual ~CChunkedFile();
private:CCountedPointer<CChunkedFileSuperChunk> RootChunk;unsigned long Version;CCharString FileComment;};
#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFile:public CChunkedFile{ friend struct ::AnimationFixture;public:
C3DAnimFile(const C3DAnimFile&);
private:
C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>&);
public:
C3DAnimFile();
CCountedPointer<C3DAnimFileRootChunk const > GetRoot() const;
CCountedPointer<C3DAnimFileRootChunk> GetRoot();
virtual ~C3DAnimFile();
};
#endif

#ifndef FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H
#define FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CCHUNKEDFILECHUNK_H
#define FABLE_ENGINE_CCHUNKEDFILECHUNK_H




class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CChunkedFileChunk:public CBaseClassNonCopyable{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:
CChunkedFileChunk(const CChunkedFileChunk&);
protected:
CChunkedFileChunk(const char*);
public:
virtual ~CChunkedFileChunk();
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
virtual int IsSuperChunk() const;
virtual int IsUnknownType() const;
virtual const CChunkedFileSuperChunk* GetAsSuperChunk() const;
virtual CChunkedFileSuperChunk* GetAsSuperChunk();
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&)=0;
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const=0;
virtual CChunkedFileChunk* CreateNewInstance() const=0;
protected:
virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*) const;
private:char ChunkType[5];CChunkedFileSuperChunk* Parent;};
#endif
#else
#ifndef FABLE_ENGINE_CCHUNKEDFILECHUNK_H
#define FABLE_ENGINE_CCHUNKEDFILECHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CChunkedFileChunk = 0x10.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CChunkedFileSuperChunk;

#pragma pack(push, 1)
class CChunkedFileChunk { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    char                    ChunkType[5];                    // +0x04
    unsigned char           _pad_0x09[0x3];                  // +0x09
    CChunkedFileSuperChunk* Parent;                          // +0x0c
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CCHUNKEDFILECHUNK_H

#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CChunkedFileSuperChunk:public CChunkedFileChunk{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;private:
CChunkedFileSuperChunk(const CChunkedFileSuperChunk&);
protected:
CChunkedFileSuperChunk(const char*);
public:
virtual ~CChunkedFileSuperChunk();
virtual int IsSuperChunk() const;
virtual const CChunkedFileSuperChunk* GetAsSuperChunk() const;
virtual CChunkedFileSuperChunk* GetAsSuperChunk();
CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char*);
const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char*) const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const=0;
protected:
virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*) const;
private:unsigned char SubChunks[12],RegisteredSubChunkTypes[12];unsigned long CurrentChunkIndex;};
#endif
#else
#ifndef FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H
#define FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CChunkedFileSuperChunk = 0x34.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CChunkedFileSuperChunk = 0x2c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CChunkedFileSuperChunk { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char SubChunks[0xc];                            // +0x10 CArray<CCountedPointer<CChunkedFileChunk>_>
    unsigned char RegisteredSubChunkTypes[0xc];              // +0x1c CArray<CCountedPointer<CChunkedFileChunk>_>
    unsigned long CurrentChunkIndex;                         // +0x28
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H

#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileRootChunk:public CChunkedFileSuperChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileRootChunk(const C3DAnimFileRootChunk&);
C3DAnimFileRootChunk();
virtual const CCharString GetChunkDescription() const;
CCountedPointer<C3DAnimFileObjectChunk const > FindFirstObject() const;
CCountedPointer<C3DAnimFileObjectChunk> FindFirstObject();
bool HasHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk const > GetHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileRootChunk();
private:bool LoopFlag;float Duration;};
#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMFILEHELPERSCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEHELPERSCHUNK_H


class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileHelpersChunk:public CChunkedFileSuperChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileHelpersChunk(const C3DAnimFileHelpersChunk&);
C3DAnimFileHelpersChunk();
virtual const CCharString GetChunkDescription() const;
bool HasMovementVector() const;
CCountedPointer<C3DAnimFileMovementVectorChunk const > GetMovementVector() const;
CCountedPointer<C3DAnimFileMovementVectorChunk> GetMovementVector();
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileHelpersChunk();
};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMFILEHELPERSCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEHELPERSCHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimFileHelpersChunk = 0x34.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimFileHelpersChunk = 0x2c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class C3DAnimFileHelpersChunk { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0xc];                           // +0x04 flattened base-class subobject(s)
    unsigned char _pad_0x10[0x18];                           // +0x10
    unsigned char _pad_0x28[0x4];                            // +0x28
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMFILEHELPERSCHUNK_H

#endif

#ifndef FABLE_ENGINE_C3DANIMFILEOBJECTCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEOBJECTCHUNK_H


class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileObjectChunk:public CChunkedFileSuperChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileObjectChunk(const C3DAnimFileObjectChunk&);
C3DAnimFileObjectChunk();
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
CCountedPointer<C3DAnimFileBoneMaskChunk const > PeekBoneMask() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileObjectChunk();
private:CCharString Name;long SubMeshIndex,ParentIndex,FirstChildIndex,NextSiblingIndex;};
#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMFILEXALLOCATIONSIZECHUNK_H
#define FABLE_ENGINE_C3DANIMFILEXALLOCATIONSIZECHUNK_H


class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileXAllocationSizeChunk:public CChunkedFileChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileXAllocationSizeChunk(const C3DAnimFileXAllocationSizeChunk&);
C3DAnimFileXAllocationSizeChunk();
virtual const CCharString GetChunkDescription() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileXAllocationSizeChunk();
public:unsigned long GetAllocSize()const{return AllocSize;}static const char* __fastcall GetStaticChunkType(){return "XALO";}private:unsigned long AllocSize;};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMFILEXALLOCATIONSIZECHUNK_H
#define FABLE_ENGINE_C3DANIMFILEXALLOCATIONSIZECHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimFileXAllocationSizeChunk = 0x14.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class C3DAnimFileXAllocationSizeChunk { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned long AllocSize;                                 // +0x10
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMFILEXALLOCATIONSIZECHUNK_H

#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMFILEXSEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILEXSEQUENCECHUNK_H


#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T>class CArray:public std::vector<T>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}const T*GetAsCArray()const{return this->size()>0?this->_M_start:(const T*)this;}};
#endif
#else
#ifdef FABLE_RETAIL_NAV_REGIONS
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

// Modified from genuine STLport4.6 _list.h/_list.c for this native ABI.
#ifndef FABLE_STLPORT46_NAVIGATION_H
#define FABLE_STLPORT46_NAVIGATION_H
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

// Modified for the documented Fable STLport instantiations; unused facilities omitted.
#ifndef FABLE_STLPORT46_VECTOR_ROOTS_H
#define FABLE_STLPORT46_VECTOR_ROOTS_H
// STLport 4.6 _vector.h, _alloc.h, _construct.h and _uninitialized.h.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t, void* p) { return p; }
#else
#include <new>
#endif
namespace std {
struct __true_type { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
struct __false_type { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
template<class T> struct __type_traits { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
};
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
template<class T> class allocator { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    allocator() {}
#ifdef FABLE_RETAIL_NAV_REGIONS
    allocator(const allocator&){}
    template<class U>allocator(const allocator<U>&){}
#endif
    T* allocate(size_t n,const void* = 0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,size_t) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public: V _M_data;
    _STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class T,class U> inline void _Construct(T* p,const U& x) { new(p) T(x); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> inline void __destroy_aux(T* first,T* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline void __destroy_aux(T*,T*,const __true_type&) {}
template<class T> inline void _Destroy(T* first,T* last) { __destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor()); }
inline void* __copy_trivial(const void* first,const void* last,void* out) {
 return last==first ? out : (char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __false_type&) {
 O cur=out; for(;first!=last;++first,++cur) _Construct(&*cur,*first); return cur;
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __true_type&) {
 return (O)__copy_trivial(first,last,out);
}
template<class T> inline T* __uninitialized_fill_n(T* first,size_t n,const T& value,const __false_type&) { T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;}
template<class T> inline T* fill_n(T* first,size_t n,const T& value) { for(;n>0;--n,++first)*first=value;return first; }
template<class T,class A> class _Vector_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public: typedef A allocator_type;
 _Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0) {
  _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
 }
 ~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 typedef T value_type;typedef T* pointer;typedef const T* const_pointer;typedef T* iterator;typedef const T* const_iterator;typedef size_t size_type;typedef A allocator_type;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 explicit vector(const A& a=A()):_Vector_base<T,A>(a){}
 vector(const vector& x):_Vector_base<T,A>(x.size(),x.get_allocator()){
 this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 A get_allocator()const{return (const A&)this->_M_end_of_storage;}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
#ifdef FABLE_STLPORT_CYLINDER
 void push_back(const T& x);
#else
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
#endif
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_REGIONS)
 vector& operator=(const vector&);
 iterator insert(iterator,const T&);
 T& operator[](size_type n){return this->_M_start[n];}
 const T& operator[](size_type n)const{return this->_M_start[n];}
#endif
 void clear(){erase(begin(),end());}
 iterator erase(iterator pos){if(pos+1!=end()) __copy_trivial(pos+1,this->_M_finish,pos);--this->_M_finish;_Destroy(this->_M_finish);return pos;}
 iterator erase(iterator first,iterator last){pointer i=first;for(;last!=this->_M_finish;++last,++i)*i=*last;_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
#ifdef FABLE_STLPORT_SPLINE
 void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);
#else
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
 void _M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
 }
};
#ifdef FABLE_STLPORT_CYLINDER
template<class A,class B,class R>struct binary_function{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;bool operator()(const T&a,const T&b)const{return a<b;}};
template<class I,class C>void sort(I,I,C);
#endif
}
#endif

namespace std {
template<class T>struct less;
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map;
struct _List_node_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;T _M_data;};
template<class T>struct _Nonconst_traits{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef T* pointer;typedef T& reference;};
template<class T>struct _Const_traits{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef const T* pointer;typedef const T& reference;};
struct _List_iterator_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 _List_node_base* _M_node;
 _List_iterator_base(_List_node_base* x):_M_node(x){} _List_iterator_base(){}
 void _M_incr(){_M_node=_M_node->_M_next;} void _M_decr(){_M_node=_M_node->_M_prev;}
 bool operator==(const _List_iterator_base& x)const{return _M_node==x._M_node;}
 bool operator!=(const _List_iterator_base& x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 _List_iterator(_List_node<T>* x):_List_iterator_base(x){} _List_iterator(){}
 _List_iterator(const iterator& x):_List_iterator_base(x._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 typename Traits::pointer operator->()const{return &operator*();}
 _List_iterator& operator++(){_M_incr();return *this;}
 _List_iterator& operator--(){_M_decr();return *this;}
};
template<class T,class A>class _List_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 typedef _List_node<T> _Node;
 _List_base(const A& a):_M_node(allocator<_Node>(a),(_Node*)0){_Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear();
 _STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;
};
template<class T,class A>void _List_base<T,A>::clear(){_List_node<T>* cur=(_List_node<T>*)this->_M_node._M_data->_M_next;while(cur!=this->_M_node._M_data){_List_node<T>* tmp=cur;cur=(_List_node<T>*)cur->_M_next;_Destroy(&tmp->_M_data);this->_M_node.deallocate(tmp,1);}this->_M_node._M_data->_M_next=this->_M_node._M_data;this->_M_node._M_data->_M_prev=this->_M_node._M_data;}
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 typedef _List_node<T> _Node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
 explicit list(const A& a=A()):_List_base<T,A>(a){}
 list(const list& x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}
 ~list(){}
 A get_allocator()const{return A((const allocator<_Node>&)this->_M_node);}
 iterator begin(){return (_Node*)this->_M_node._M_data->_M_next;}const_iterator begin()const{return (_Node*)this->_M_node._M_data->_M_next;}
 iterator end(){return this->_M_node._M_data;}const_iterator end()const{return this->_M_node._M_data;}
 T& back(){return *--end();}const T& back()const{return *--end();}
protected:
 _Node* _M_create_node(const T& x){_Node* p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}
public:
 iterator insert(iterator position,const T& x){_Node* tmp=_M_create_node(x);_List_node_base* n=position._M_node;_List_node_base* p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}
 void push_back(const T& x){insert(end(),x);}
 template<class I>void insert(iterator pos,I first,I last){_M_insert_dispatch(pos,first,last,__false_type());}
 template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}
};
}
#endif

// Original public vector base; native node arrays use four-byte entries.
template<class T>class CArray:public std::vector<T>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_STRIPIFIER)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
#ifndef FABLE_STLPORT46_STRIPIFIER_H
#define FABLE_STLPORT46_STRIPIFIER_H
// Modified STLport 4.6 vector/allocator excerpts for the retail triangle TU.
// Original inheritance and POD dispatch; exception configuration disabled.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};struct __false_type{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
template<class T>struct __type_traits{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef __false_type is_POD_type;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<long>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned short>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<bool>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<class T>class allocator{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:allocator(){};T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
template<class T>inline T*__copy_ptrs(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*__uninitialized_copy(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class T>inline T*__uninitialized_fill_n(T*p,size_t n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T,class A>class _Vector_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&back(){return *(end()-1);}const T&back()const{return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,_IsPODType());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void resize(size_type,const T&);
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=(T*)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(T*)__copy_trivial(position,this->_M_finish,new_finish);
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
};
}
#endif

// Original CArray public vector base, constructors and index accessors (PDB).
// Retail uses STLport's 12-byte base; the donor's later vector base is 16 bytes.
template<class T>class CArray:public std::vector<T>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 CArray():std::vector<T>(){}
 explicit CArray(unsigned int n):std::vector<T>(n,T()){}
 CArray(unsigned int n,const T&x):std::vector<T>(n,x){}
 T&operator[](unsigned int n){return this->_M_start[n];}
 const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_FONT)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Retail opt-in (Codex pass11): original public vector base and accessors.
template<class T> class CArray:public std::vector<T> { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 T& operator[](unsigned int i){return this->_M_start[i];}
 T* GetAsCArray(){return this->size()>0?&this->front():GetZeroSizeArray();}
private:T* GetZeroSizeArray(){return (T*)this;}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_GEOMETRY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Retail opt-in (Codex pass11): original public vector base, no added storage.
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT46_GEOMETRY_H
// Adapted STLport 4.6 container declarations and inline bodies for this TU.
#include <stddef.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __false_type { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
template<class T>class allocator { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
#endif
};
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
#endif
struct _Rb_tree_node_base { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;T _M_value_field;};
template<class Dummy>class _Rb_global{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{ friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}
#endif

template<class T>class CArray:public std::vector<T>{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_DISPLAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Original CArray adds no fields to its public STLport vector base.
template<class T> class CArray : public std::vector<T> { friend struct ::AnimationFixture; friend struct ::AnimationFixture;};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Original CArray<char> methods from FableWin PDB. Retail zero-size arms
// at 00409905 and 004099dc return the address of the CArray object itself.
// This branch is included after the selected genuine STLport vector definition.
template<class T> class CArray : public std::vector<T> { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    explicit CArray(unsigned int count) : std::vector<T>(count) {}
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    CArray() : std::vector<T>() { }
    T& operator[](unsigned int n) { return this->_M_start[n]; }
    const T& operator[](unsigned int n) const { return this->_M_start[n]; }
    const T* GetAsCArray() const { return this->size()>0 ? &*this->begin() : (const T*)this; }
#endif
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
#else
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

// Original CArray public vector base, no added members. Retail STLport size12.
// PDB CArray<C3DVector> confirms unsigned-int index and both reference overloads.
template<class T> class CArray : public std::vector<T> { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
};
#endif

#endif

#endif

#endif

#endif

#endif

#endif

#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileXSequenceChunk:public CChunkedFileChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileXSequenceChunk(const C3DAnimFileXSequenceChunk&);
C3DAnimFileXSequenceChunk();
virtual const CCharString GetChunkDescription() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileXSequenceChunk();
public:unsigned long GetDataSize()const{return CompressedData.size();}const void* PeekData()const{return CompressedData.GetAsCArray();}static const char* __fastcall GetStaticChunkType(){return "XSEQ";}private:CArray<unsigned char> CompressedData;};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMFILEXSEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILEXSEQUENCECHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimFileXSequenceChunk = 0x20.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimFileXSequenceChunk = 0x1c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class C3DAnimFileXSequenceChunk { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char CompressedData[0xc];                       // +0x10 CArray<unsigned_char>
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMFILEXSEQUENCECHUNK_H

#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMFILEBONEMASKCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEBONEMASKCHUNK_H



class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileBoneMaskChunk:public CChunkedFileChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileBoneMaskChunk(const C3DAnimFileBoneMaskChunk&);
C3DAnimFileBoneMaskChunk();
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileBoneMaskChunk();
public:bool GetBoneEnable(unsigned long i)const{return EnableMask[i];}private:CArray<bool> EnableMask;};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMFILEBONEMASKCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEBONEMASKCHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimFileBoneMaskChunk = 0x24.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimFileBoneMaskChunk = 0x1c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class C3DAnimFileBoneMaskChunk { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char EnableMask[0xc];                           // +0x10 CArray<bool>
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMFILEBONEMASKCHUNK_H

#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_C3DANIMFILEMOVEMENTVECTORCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEMOVEMENTVECTORCHUNK_H


class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileMovementVectorChunk:public CChunkedFileSuperChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileMovementVectorChunk(const C3DAnimFileMovementVectorChunk&);
C3DAnimFileMovementVectorChunk();
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
bool HasMovementTrack() const;
CCountedPointer<C3DAnimFileSequenceChunk const > GetMovementTrack() const;
CCountedPointer<C3DAnimFileSequenceChunk> GetMovementTrack();
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileMovementVectorChunk();
private:unsigned char Vector[12];};
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMFILEMOVEMENTVECTORCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEMOVEMENTVECTORCHUNK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimFileMovementVectorChunk = 0x40.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimFileMovementVectorChunk = 0x38.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class C3DAnimFileMovementVectorChunk { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x18];                           // +0x10
    unsigned char _pad_0x28[0x4];                            // +0x28
    float         Vector_X;                                  // +0x2c C3DVector::X
    float         Vector_Y;                                  // +0x30 C3DVector::Y
    float         Vector_Z;                                  // +0x34 C3DVector::Z
};
#pragma pack(pop)


#endif // FABLE_ENGINE_C3DANIMFILEMOVEMENTVECTORCHUNK_H

#endif

#ifndef FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H


#ifndef FABLE_ENGINE_EBONETYPE_H
#define FABLE_ENGINE_EBONETYPE_H
// Original EBoneType values from raw PDB; native sequence field occupies four bytes.
enum EBoneType{BONETYPE_STANDARD=0,BONETYPE_DUMMY=1,BONETYPE_MOVEMENT_DUMMY=2};
#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileSequenceChunk:public CChunkedFileSuperChunk{ friend struct ::AnimationFixture;public:
C3DAnimFileSequenceChunk(const C3DAnimFileSequenceChunk&);
C3DAnimFileSequenceChunk();
C3DAnimFileSequenceChunk(long,const CCharString&);
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileSequenceChunk();
public:long GetParentIndex()const{return ParentIndex;}private:long BoneIndex,ParentIndex;CCharString BoneName;float SamplesPerSecond;EBoneType BoneType;unsigned char Matrices[12];};
#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CSIMPLEMEMORYPOOL_H
#define FABLE_ENGINE_CSIMPLEMEMORYPOOL_H

#include <new>



class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CSimpleMemoryPool:public CBaseClassNonCopyable{ friend struct ::AnimationFixture;public:
CSimpleMemoryPool(const CSimpleMemoryPool&);
CSimpleMemoryPool(unsigned long,unsigned long);
virtual ~CSimpleMemoryPool();
void * Allocate(unsigned long);
void SetCapacity(unsigned long);
void SetMemoryPool(void *,unsigned long);
public:
 template<class T>T* New(unsigned long count){T* data=count!=0?((FableU64)count*sizeof(T)>0xffffffffUL?0:(T*)Allocate(count*sizeof(T))):0;for(unsigned long i=0;i<count;++i)new(data+i)T;return data;}
private:class CMemoryBlock;CMemoryBlock* FirstMemoryBlock;unsigned long Capacity,GrowthGranularity;unsigned char Root[12];};
#endif
#else
#ifndef FABLE_ENGINE_CSIMPLEMEMORYPOOL_H
#define FABLE_ENGINE_CSIMPLEMEMORYPOOL_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CSimpleMemoryPool = 0x1c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


struct CArea;
struct CMemoryBlock;

#pragma pack(push, 1)
class CSimpleMemoryPool { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    CMemoryBlock* FirstMemoryBlock;                          // +0x04
    unsigned long Capacity;                                  // +0x08
    unsigned long GrowthGranularity;                         // +0x0c
    unsigned char Root[0xc];                                 // +0x10 CArea
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CSIMPLEMEMORYPOOL_H

#endif

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CMOVABLERESOURCEMEMORYPOOL_H
#define FABLE_ENGINE_CMOVABLERESOURCEMEMORYPOOL_H

#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#define FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H




class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CHandleBasedMemoryPool:public CBaseClassNonCopyable{ friend struct ::AnimationFixture; friend struct ::AnimationFixture;public:
CHandleBasedMemoryPool(const CHandleBasedMemoryPool&);
CHandleBasedMemoryPool();
virtual ~CHandleBasedMemoryPool();
void * GetPointerFromHandle(long);
protected:
virtual void MoveResource(void *,long,void *,void *,unsigned long);
virtual bool CanMoveResource(void *,long);
virtual void ValidateResource(void *);
private:unsigned char Buffer[0x460];};
#endif
#else
#ifndef FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#define FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CHandleBasedMemoryPool = 0x468.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CHandleBasedMemoryPool = 0x464.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CCharStringData;
class CFailedAllocationHandler;
struct CHeader;
struct _RTL_CRITICAL_SECTION_DEBUG;

#pragma pack(push, 1)
class CHandleBasedMemoryPool { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void*                        __vftable;                  // +0x00 vptr, or first dword of a flattened base subobject
    void*                        Buffer;                     // +0x04
    void*                        UnalignedBuffer;            // +0x08
    long                         PoolSize;                   // +0x0c
    long                         AllocatedSize;              // +0x10
    long                         NumAllocations;             // +0x14
    long                         LockCount;                  // +0x18
    long                         LargestFreeArea;            // +0x1c
    unsigned long                MemoryFlags;                // +0x20
    bool                         MultiThreaded;              // +0x24
    bool                         ShutdownFlag;               // +0x25
    bool                         EnableDefrag;               // +0x26
    bool                         EnableFullDefragIfPartialFails; // +0x27
    unsigned long                MaxFreeSizeBeforeDefrag;    // +0x28
    unsigned long                MaxFreeSizeBeforeFailure;   // +0x2c
    long                         MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler*    FailureHandler;             // +0x34
    CHeader*                     FreeLists[256];             // +0x38
    unsigned char                Handles[0xc];               // +0x438 CArray<CHandleBasedMemoryPool::CHandleInfo>
    long                         FirstUnusedHandle;          // +0x444
    CCharStringData*             Name_PStringData;           // +0x448 CCharString::PStringData
    _RTL_CRITICAL_SECTION_DEBUG* CriticalSection_DebugInfo;  // +0x44c _RTL_CRITICAL_SECTION::DebugInfo
    long                         CriticalSection_LockCount;  // +0x450 _RTL_CRITICAL_SECTION::LockCount
    long                         CriticalSection_RecursionCount; // +0x454 _RTL_CRITICAL_SECTION::RecursionCount
    void*                        CriticalSection_OwningThread; // +0x458 _RTL_CRITICAL_SECTION::OwningThread
    void*                        CriticalSection_LockSemaphore; // +0x45c _RTL_CRITICAL_SECTION::LockSemaphore
    unsigned long                CriticalSection_SpinCount;  // +0x460 _RTL_CRITICAL_SECTION::SpinCount
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H

#endif

class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CMovableResourceMemoryPool:public CHandleBasedMemoryPool{ friend struct ::AnimationFixture;public:
CMovableResourceMemoryPool(const CMovableResourceMemoryPool&);
CMovableResourceMemoryPool();
bool AllocateResource(long *,CMovableResource*,unsigned long,unsigned long);
protected:
virtual void MoveResource(void *,long,void *,void *,unsigned long);
virtual bool CanMoveResource(void *,long);
virtual void ValidateResource(void *);
public:
virtual ~CMovableResourceMemoryPool();
};
#endif
#else
#ifndef FABLE_ENGINE_CMOVABLERESOURCEMEMORYPOOL_H
#define FABLE_ENGINE_CMOVABLERESOURCEMEMORYPOOL_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CMovableResourceMemoryPool = 0x468.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CMovableResourceMemoryPool = 0x464.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CMovableResourceMemoryPool { friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x434];                         // +0x04 flattened base-class subobject(s)
    unsigned char _pad_0x438[0xc];                           // +0x438
    unsigned char _pad_0x444[0x20];                          // +0x444
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CMOVABLERESOURCEMEMORYPOOL_H

#endif

#if defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#ifndef FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
#define FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
// Retail opt-in (Codex pass 11): raw PDB CDataInputStream base24,
// StreamSource+18; original inline default constructor lib_data_stream.hpp246.
#if defined(FABLE_RETAIL_CDATAINPUTSTREAM)
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#ifndef FABLE_RETAIL_HEADER_CDATASTREAM
#define FABLE_RETAIL_HEADER_CDATASTREAM

class CDataStream : public CBaseClassNonCopyable { friend struct ::AnimationFixture; friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
};
#endif

#include <string.h>
#pragma intrinsic(memcpy)
class CCharString;
class CWideString;
// Layout: original CDataInputStream fields +4,+8,+c,+10,+14, size0x18.
class CDataInputStream : public CDataStream { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    CDataInputStream();
#else
    virtual ~CDataInputStream();
#endif
    virtual void Close();
    unsigned long GetLength() const { return StreamSize; }
    virtual unsigned long GetPosition() const { return StreamPos; }
    virtual void SetPosition(unsigned long);
    inline void Read(void* buffer,long size) {
        if(size<=0)return;
        FableI64 newPosition=(FableI64)(StreamPos+size);
#ifdef FABLE_RETAIL_CENGINEMAP_READ
        bool valid=newPosition>=0 && newPosition<=0x7fffffff;
        if(!valid)return;
#else
        if(newPosition<0 || newPosition>0x7fffffff)return;
#endif
        if(size<=SourceChunkBytesRemaining) {
            memcpy(buffer,CurrentSourcePtr,size);
            CurrentSourcePtr+=size;
            SourceChunkBytesRemaining-=size;
            StreamPos+=size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
    }
    template<class T> T ReadType() { T val; Read(&val,sizeof(T)); return val; }
    inline unsigned long ReadULONG() { return ReadType<unsigned long>(); }
    inline long ReadSLONG() { return ReadType<long>(); }
    virtual void ReadCompressed(void*,long);
    virtual void ReadZlibCompressed(void*,long);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
protected:
    void SetPositionAssumeWithinSourceChunk(unsigned long);
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char*,long);
    virtual void InternalReadNextSourceChunk(const unsigned char*&,long&);
    virtual bool NeedsBufferedRead(long);
    virtual void InternalReadUnbuffered(unsigned char*,long);
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
};
#endif
#else
#ifndef FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDataInputStream = 0x18.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CDataInputStream { friend struct ::AnimationFixture; friend struct ::AnimationFixture;
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long StreamPos;                                 // +0x04
    long          StreamSize;                                // +0x08
    void*         CurrentSourcePtr;                          // +0x0c uchar *
    unsigned long SourceChunkStreamPos;                      // +0x10
    long          SourceChunkBytesRemaining;                 // +0x14
#ifdef FABLE_CDATAINPUTSTREAM_FRIEND
    friend FABLE_CDATAINPUTSTREAM_FRIEND;                   // the including row's inline-helper class
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size); // 0x00993CA0 ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CDATAINPUTSTREAM_H

#endif

class CMemoryDataInputStream : public CDataInputStream { friend struct ::AnimationFixture;
public:
    CMemoryDataInputStream() : StreamSource(0) { }
    CMemoryDataInputStream(const void*,unsigned long);
    void OpenMemorySource(const void*,unsigned long);
    const unsigned char* ReadConstData(long size) {
        if(size<=0) return 0;
        unsigned long position=GetPosition();
        FableI64 new_position=(FableI64)(position+size);
        if(new_position<0 || new_position>0x7fffffff) return 0;
        const unsigned char* result=StreamSource+position;
        SetPositionAssumeWithinSourceChunk((unsigned long)new_position);
        return result;
    }
    virtual void ReadCompressed(void*,long);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
private:
    const unsigned char* StreamSource;
};
#endif
#else
#ifndef FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
#define FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CMemoryDataInputStream = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CMemoryDataInputStream { friend struct ::AnimationFixture;
public:
    void* __vftable;                                         // +0x00 vptr, or first dword of a flattened base subobject
    long  position;                                          // +0x04 retail-only long (previous header)
    long  sourceLength;                                      // +0x08 retail-only long (previous header)
    long  chunkBase;                                         // +0x0c retail-only long (previous header)
    long  chunkPosition;                                     // +0x10 retail-only long (previous header)
    long  chunkLength;                                       // +0x14 retail-only long (previous header)
    void* StreamSource;                                      // +0x18 uchar *
};
#pragma pack(pop)


#endif // FABLE_ENGINE_CMEMORYDATAINPUTSTREAM_H

#endif

#ifndef FABLE_ENGINE_CSCOPEDLOCKFLAGBOOL_H
#define FABLE_ENGINE_CSCOPEDLOCKFLAGBOOL_H

// Layout: original FableWin PDB class, four-byte bool reference Flag; native scoped true/false stores.
class CScopedLockFlagBool { friend struct ::AnimationFixture;
public:
 CScopedLockFlagBool(bool& flag):Flag(flag){Flag=true;}
 ~CScopedLockFlagBool(){Flag=false;}
private:bool& Flag;
};
#endif


inline bool C3DAnimationSequence::InitialiseFromFileData(const C3DAnimFileSequenceChunk& sequence,C3DAnimationSequenceData* data,CSimpleMemoryPool& allocator)
{
 ParentIndex=(short)sequence.GetParentIndex();Data=data;Shared=false;
 return Data->InitialiseFromFileData(sequence,allocator);
}

bool C3DAnimation2::InitialiseSharedAnimation(const C3DAnimFile& file_data,EAnimationType anim_type)
{
 CScopedLockFlagBool lock(LockedFromMemMemoryRelocation);
 InitialiseStats(file_data,anim_type);
 Allocator->SetCapacity(16384);
 if(file_data.GetRoot()->HasHelpers()) {
  CCountedPointer<C3DAnimFileXAllocationSizeChunk> alloc_size_chunk;
  alloc_size_chunk.SetStaticCast(file_data.GetRoot()->GetHelpers()->FindFirstSubChunkOfType(C3DAnimFileXAllocationSizeChunk::GetStaticChunkType()));
  if(!alloc_size_chunk.IsNull()) {
   unsigned long capacity=alloc_size_chunk->GetAllocSize();
   if(capacity) {
    if(MemoryPool) {
     if(!MemoryPool->AllocateResource(&MemoryHandle,this,capacity,0)) {return false;}
     Allocator->SetMemoryPool(MemoryPool->GetPointerFromHandle(MemoryHandle),capacity);
    }
   }else Allocator->SetCapacity(0);
  }
 }
 CCountedPointer<const C3DAnimFileObjectChunk> anim_data=file_data.GetRoot()->FindFirstObject();
 ReadCompressedFileData(*anim_data,*Allocator);
 FrameCount=Sequences[0].GetFrameCount();SamplesPerSecond=Sequences[0].GetSamplesPerSecond();
 if(anim_type&2) {
  CCountedPointer<const C3DAnimFileBoneMaskChunk> bone_mask_data=anim_data->PeekBoneMask();
  BoneMask=Allocator->New<bool>(SequenceCount);
  for(unsigned long c_bone=0;c_bone<SequenceCount;++c_bone)BoneMask[c_bone]=bone_mask_data->GetBoneEnable(c_bone);
 }
 MovementTrack.ReleaseData();
 if(file_data.GetRoot()->HasHelpers()&&file_data.GetRoot()->GetHelpers()->HasMovementVector()) {
  CCountedPointer<const C3DAnimFileMovementVectorChunk> vector_chunk=file_data.GetRoot()->GetHelpers()->GetMovementVector();
  CCountedPointer<const C3DAnimFileXSequenceChunk> compressed_track;
  compressed_track.SetStaticCast(vector_chunk->FindFirstSubChunkOfType(C3DAnimFileXSequenceChunk::GetStaticChunkType()));
  if(!compressed_track.IsNull()) {
   MovementTrack.Reset(new C3DAnimationSequence);
   MovementTrackData.Reset(new C3DAnimationSequenceData);
   CMemoryDataInputStream datastream(compressed_track->PeekData(),compressed_track->GetDataSize());
   MovementTrack->ReadCompressedFromFile(datastream,compressed_track->GetDataSize(),MovementTrackData.Get(),*Allocator);
  }else if(vector_chunk->HasMovementTrack()) {
   MovementTrack.Reset(new C3DAnimationSequence);
   MovementTrackData.Reset(new C3DAnimationSequenceData);
   MovementTrack->InitialiseFromFileData(*vector_chunk->GetMovementTrack(),MovementTrackData.Get(),*Allocator);
  }
 }
 return true;
}

static void require(bool condition,int code){if(!condition){fprintf(stderr,"animation failure %d\n",code);exit(code);}}
static bool has_helpers,has_alloc,has_movement,has_compressed,has_track,allocation_success;
static unsigned long allocation_size,sequence_count,compressed_size;static int set_capacity_calls,set_pool_calls,resource_calls,read_calls,compressed_calls,plain_calls,mask_calls,stream_dtors;
static unsigned long last_capacity;static unsigned char memory_area[64];static C3DAnimation2* active_animation;
static CCountedPointer<C3DAnimFileRootChunk> root_chunk;
static CCountedPointer<C3DAnimFileHelpersChunk> helpers_chunk;
static CCountedPointer<C3DAnimFileObjectChunk> object_chunk;
static CCountedPointer<C3DAnimFileXAllocationSizeChunk> allocation_chunk;
static CCountedPointer<C3DAnimFileMovementVectorChunk> movement_chunk;
static CCountedPointer<C3DAnimFileXSequenceChunk> compressed_chunk;
static CCountedPointer<C3DAnimFileSequenceChunk> sequence_chunk;
static CCountedPointer<C3DAnimFileBoneMaskChunk> mask_chunk;
static C3DAnimationSequence* initial_sequences;static CSimpleMemoryPool* active_pool;static const C3DAnimFile* active_file;static EAnimationType active_type;
struct AnimationFixture {
 template<class T>static void vector_data(CArray<T>& a,unsigned n){a._M_start=(T*)malloc(n*sizeof(T));a._M_finish=a._M_start+n;a._M_end_of_storage._M_data=a._M_finish;}
 static void configure_chunks(){allocation_chunk->AllocSize=allocation_size;vector_data(mask_chunk->EnableMask,sequence_count);for(unsigned i=0;i<sequence_count;++i)mask_chunk->EnableMask[i]=(i&1)!=0;vector_data(compressed_chunk->CompressedData,compressed_size);for(unsigned i=0;i<compressed_size;++i)compressed_chunk->CompressedData[i]=(unsigned char)(i+1);sequence_chunk->ParentIndex=5;}
 static void configure(C3DAnimation2& a,CSimpleMemoryPool& pool,CMovableResourceMemoryPool* resource,C3DAnimationSequenceData& sd,C3DAnimationSequence& seq){active_animation=&a;active_pool=&pool;a.Allocator.Data=&pool;a.Allocator.Info=0;a.MemoryPool=resource;a.MemoryHandle=0;a.SequenceCount=sequence_count;a.Sequences=&seq;a.BoneMask=0;a.LockedFromMemMemoryRelocation=false;sd.FrameCount=7;sd.SamplesPerSecond=16;seq.Data=&sd;initial_sequences=&seq;a.MovementTrack.Reset(new C3DAnimationSequence);a.MovementTrackData.Reset(new C3DAnimationSequenceData);}
 static void stream(CDataInputStream& stream,const unsigned char* p,unsigned long n){stream.StreamSize=n;stream.CurrentSourcePtr=p;stream.SourceChunkBytesRemaining=n;}
 static bool locked(){return active_animation->LockedFromMemMemoryRelocation;}
 static void stats(C3DAnimationStats& stats,const C3DAnimFile& file,EAnimationType type){require(&file==active_file&&type==active_type&&locked(),50);stats.AnimType=type;}
 static void read(C3DAnimation2& a,const C3DAnimFileObjectChunk& obj,CSimpleMemoryPool& pool){require(&obj==object_chunk.Get()&&&pool==active_pool&&locked(),51);a.SequenceCount=sequence_count;a.Sequences=initial_sequences;++read_calls;}
 static void check(bool success,bool partial,bool expect_movement,bool compressed){C3DAnimation2& a=*active_animation;require(!a.LockedFromMemMemoryRelocation,52);if(!success){require(read_calls==0&&set_pool_calls==0,53);return;}require(read_calls==1&&a.FrameCount==7&&a.SamplesPerSecond==16,54);if(partial){require(mask_calls==(sequence_count?1:0),55);for(unsigned i=0;i<sequence_count;++i)require(a.BoneMask[i]==((i&1)!=0),56);}else require(a.BoneMask==0&&mask_calls==0,57);require((a.MovementTrack.Get()!=0)==expect_movement,58);if(expect_movement){require(a.MovementTrackData.Get()!=0&&a.MovementTrack->Data==a.MovementTrackData.Get(),59);require(a.MovementTrack->ParentIndex==(compressed?9:5)&&!a.MovementTrack->Shared,60);}require(compressed_calls==(expect_movement&&compressed?1:0)&&plain_calls==(expect_movement&&!compressed?1:0),61);require(stream_dtors==(expect_movement&&compressed?1:0),62);}
 static void compressed(C3DAnimationSequence& seq,CDataInputStream& stream,unsigned long length,C3DAnimationSequenceData* data,CSimpleMemoryPool& pool){require(locked()&&length==compressed_size&&&pool==active_pool,63);require(stream.StreamSize==(long)compressed_size&&stream.CurrentSourcePtr==compressed_chunk->PeekData(),64);seq.Data=data;seq.ParentIndex=9;seq.Shared=false;++compressed_calls;}
 static void clear(C3DAnimation2& a){if(a.BoneMask)free(a.BoneMask);a.BoneMask=0;a.Allocator.Data=0;active_animation=0;}
};
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}void CBaseObject::Validate()const{}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){}
C3DAnimationStats::C3DAnimationStats(){}C3DAnimationStats::~C3DAnimationStats(){}
void C3DAnimationStats::InitialiseStats(const C3DAnimFile& f,EAnimationType t){AnimationFixture::stats(*this,f,t);}
CMovableResource::CMovableResource(){}CMovableResource::~CMovableResource(){}
C3DAnimation2::C3DAnimation2(){}C3DAnimation2::~C3DAnimation2(){}
void C3DAnimation2::ReadCompressedFileData(const C3DAnimFileObjectChunk& obj,CSimpleMemoryPool& pool){AnimationFixture::read(*this,obj,pool);}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long,unsigned long){}CSimpleMemoryPool::~CSimpleMemoryPool(){}
void CSimpleMemoryPool::SetCapacity(unsigned long n){require(AnimationFixture::locked(),70);++set_capacity_calls;last_capacity=n;}
void CSimpleMemoryPool::SetMemoryPool(void* p,unsigned long size){require(AnimationFixture::locked()&&p==memory_area&&size==allocation_size,71);++set_pool_calls;}
void* CSimpleMemoryPool::Allocate(unsigned long n){require(n==sequence_count&&AnimationFixture::locked(),72);++mask_calls;return malloc(n);}
CHandleBasedMemoryPool::CHandleBasedMemoryPool(){}CHandleBasedMemoryPool::~CHandleBasedMemoryPool(){}
void* CHandleBasedMemoryPool::GetPointerFromHandle(long h){require(h==12&&AnimationFixture::locked(),73);return memory_area;}
CMovableResourceMemoryPool::CMovableResourceMemoryPool(){}CMovableResourceMemoryPool::~CMovableResourceMemoryPool(){}
bool CMovableResourceMemoryPool::AllocateResource(long* handle,CMovableResource* r,unsigned long n,unsigned long flags){require(r==static_cast<CMovableResource*>(active_animation)&&n==allocation_size&&flags==0&&AnimationFixture::locked(),74);++resource_calls;*handle=12;return allocation_success;}
CChunkedFileChunk::CChunkedFileChunk(const char* tag):Parent(0){memset(ChunkType,0,5);memcpy(ChunkType,tag,4);}CChunkedFileChunk::~CChunkedFileChunk(){}
CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* tag):CChunkedFileChunk(tag){}CChunkedFileSuperChunk::~CChunkedFileSuperChunk(){}
CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root):RootChunk(root),Version(0){}CChunkedFile::~CChunkedFile(){}
C3DAnimFile::C3DAnimFile():CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>()){}C3DAnimFile::~C3DAnimFile(){}
CCountedPointer<const C3DAnimFileRootChunk> C3DAnimFile::GetRoot()const{require(this==active_file&&AnimationFixture::locked(),75);return root_chunk;}
bool C3DAnimFileRootChunk::HasHelpers()const{require(this==root_chunk.Get(),76);return has_helpers;}
CCountedPointer<const C3DAnimFileHelpersChunk> C3DAnimFileRootChunk::GetHelpers()const{require(has_helpers,77);return helpers_chunk;}
CCountedPointer<const C3DAnimFileObjectChunk> C3DAnimFileRootChunk::FindFirstObject()const{return object_chunk;}
bool C3DAnimFileHelpersChunk::HasMovementVector()const{return has_movement;}
CCountedPointer<const C3DAnimFileMovementVectorChunk> C3DAnimFileHelpersChunk::GetMovementVector()const{require(has_movement,78);return movement_chunk;}
CCountedPointer<const C3DAnimFileBoneMaskChunk> C3DAnimFileObjectChunk::PeekBoneMask()const{return mask_chunk;}
bool C3DAnimFileMovementVectorChunk::HasMovementTrack()const{return has_track;}
CCountedPointer<const C3DAnimFileSequenceChunk> C3DAnimFileMovementVectorChunk::GetMovementTrack()const{require(has_track,79);return sequence_chunk;}
const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* tag)const{require(AnimationFixture::locked(),80);if(strcmp(tag,"XALO")==0){require(this==static_cast<const CChunkedFileSuperChunk*>(helpers_chunk.Get()),81);return has_alloc?CCountedPointer<CChunkedFileChunk>(allocation_chunk):CCountedPointer<CChunkedFileChunk>();}require(strcmp(tag,"XSEQ")==0&&this==static_cast<const CChunkedFileSuperChunk*>(movement_chunk.Get()),82);return has_compressed?CCountedPointer<CChunkedFileChunk>(compressed_chunk):CCountedPointer<CChunkedFileChunk>();}
void C3DAnimationSequence::ReadCompressedFromFile(CDataInputStream& stream,unsigned long n,C3DAnimationSequenceData* data,CSimpleMemoryPool& pool){AnimationFixture::compressed(*this,stream,n,data,pool);}
bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& c,CSimpleMemoryPool& pool){require(&c==sequence_chunk.Get()&&&pool==active_pool&&AnimationFixture::locked(),83);++plain_calls;return true;}
CDataInputStream::CDataInputStream():StreamPos(0),StreamSize(0),CurrentSourcePtr(0),SourceChunkStreamPos(0),SourceChunkBytesRemaining(0){}
CDataStream::~CDataStream(){++stream_dtors;}
CMemoryDataInputStream::CMemoryDataInputStream(const void* p,unsigned long n):StreamSource((const unsigned char*)p){AnimationFixture::stream(*this,StreamSource,n);}
void CDataInputStream::Close(){}void CDataInputStream::SetPosition(unsigned long){throw 1;}void CDataInputStream::ReadCompressed(void*,long){throw 1;}void CDataInputStream::ReadZlibCompressed(void*,long){throw 1;}CCharString CDataInputStream::ReadNullTerminatedString(){throw 1;}CWideString CDataInputStream::ReadNullTerminatedWString(){throw 1;}void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&){throw 1;}bool CDataInputStream::NeedsBufferedRead(long){throw 1;}void CDataInputStream::InternalReadUnbuffered(unsigned char*,long){throw 1;}
void CMemoryDataInputStream::ReadCompressed(void*,long){throw 1;}CCharString CMemoryDataInputStream::ReadNullTerminatedString(){throw 1;}CWideString CMemoryDataInputStream::ReadNullTerminatedWString(){throw 1;}
bool CMovableResource::CanRelocateData(long) const{throw 1;}
unsigned long CMovableResource::Verify() const{throw 1;}
void CHandleBasedMemoryPool::MoveResource(void *,long,void *,void *,unsigned long){throw 1;}
bool CHandleBasedMemoryPool::CanMoveResource(void *,long){throw 1;}
void CHandleBasedMemoryPool::ValidateResource(void *){throw 1;}
void CMovableResourceMemoryPool::MoveResource(void *,long,void *,void *,unsigned long){throw 1;}
bool CMovableResourceMemoryPool::CanMoveResource(void *,long){throw 1;}
void CMovableResourceMemoryPool::ValidateResource(void *){throw 1;}
C3DVector C3DAnimation2::GetMovementOffsetAtTime(float) const{throw 1;}
C3DVector C3DAnimation2::GetMovementAtTime(float) const{throw 1;}
bool C3DAnimation2::HasRotation() const{throw 1;}
CQuaternion C3DAnimation2::GetRotationAtTime(float) const{throw 1;}
void C3DAnimation2::GetDeltaMovement(CMatrix3x4&,float,float) const{throw 1;}
void C3DAnimation2::RelocateData(long,void *,void *,long){throw 1;}
bool C3DAnimation2::CanRelocateData(long) const{throw 1;}
const CCharString CChunkedFileChunk::GetChunkDescription() const{throw 1;}
const CCharString CChunkedFileChunk::GetChunkInfo() const{throw 1;}
int CChunkedFileChunk::IsSuperChunk() const{throw 1;}
int CChunkedFileChunk::IsUnknownType() const{throw 1;}
const CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk() const{throw 1;}
CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk(){throw 1;}
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk*) const{throw 1;}
int CChunkedFileSuperChunk::IsSuperChunk() const{throw 1;}
const CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk() const{throw 1;}
CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk(){throw 1;}
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk*) const{throw 1;}
const CCharString C3DAnimFileRootChunk::GetChunkDescription() const{throw 1;}
void C3DAnimFileRootChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileRootChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileRootChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileHelpersChunk::GetChunkDescription() const{throw 1;}
CChunkedFileChunk* C3DAnimFileHelpersChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileObjectChunk::GetChunkDescription() const{throw 1;}
const CCharString C3DAnimFileObjectChunk::GetChunkInfo() const{throw 1;}
void C3DAnimFileObjectChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileObjectChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileObjectChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileMovementVectorChunk::GetChunkDescription() const{throw 1;}
const CCharString C3DAnimFileMovementVectorChunk::GetChunkInfo() const{throw 1;}
void C3DAnimFileMovementVectorChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileMovementVectorChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileMovementVectorChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileSequenceChunk::GetChunkDescription() const{throw 1;}
const CCharString C3DAnimFileSequenceChunk::GetChunkInfo() const{throw 1;}
void C3DAnimFileSequenceChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileSequenceChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileXSequenceChunk::GetChunkDescription() const{throw 1;}
void C3DAnimFileXSequenceChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileXSequenceChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileXSequenceChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileXAllocationSizeChunk::GetChunkDescription() const{throw 1;}
void C3DAnimFileXAllocationSizeChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileXAllocationSizeChunk::CreateNewInstance() const{throw 1;}
const CCharString C3DAnimFileBoneMaskChunk::GetChunkDescription() const{throw 1;}
const CCharString C3DAnimFileBoneMaskChunk::GetChunkInfo() const{throw 1;}
void C3DAnimFileBoneMaskChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){throw 1;}
void C3DAnimFileBoneMaskChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{throw 1;}
CChunkedFileChunk* C3DAnimFileBoneMaskChunk::CreateNewInstance() const{throw 1;}
C3DAnimFileRootChunk::C3DAnimFileRootChunk():CChunkedFileSuperChunk("TEST"){}C3DAnimFileRootChunk::~C3DAnimFileRootChunk(){}
C3DAnimFileHelpersChunk::C3DAnimFileHelpersChunk():CChunkedFileSuperChunk("TEST"){}C3DAnimFileHelpersChunk::~C3DAnimFileHelpersChunk(){}
C3DAnimFileObjectChunk::C3DAnimFileObjectChunk():CChunkedFileSuperChunk("TEST"){}C3DAnimFileObjectChunk::~C3DAnimFileObjectChunk(){}
C3DAnimFileMovementVectorChunk::C3DAnimFileMovementVectorChunk():CChunkedFileSuperChunk("TEST"){}C3DAnimFileMovementVectorChunk::~C3DAnimFileMovementVectorChunk(){}
C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk():CChunkedFileSuperChunk("TEST"){}C3DAnimFileSequenceChunk::~C3DAnimFileSequenceChunk(){}
C3DAnimFileXSequenceChunk::C3DAnimFileXSequenceChunk():CChunkedFileChunk("TEST"){}C3DAnimFileXSequenceChunk::~C3DAnimFileXSequenceChunk(){}
C3DAnimFileXAllocationSizeChunk::C3DAnimFileXAllocationSizeChunk():CChunkedFileChunk("TEST"){}C3DAnimFileXAllocationSizeChunk::~C3DAnimFileXAllocationSizeChunk(){}
C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk():CChunkedFileChunk("TEST"){}C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk(){}

static void release_chunks(){root_chunk.ReleaseData();helpers_chunk.ReleaseData();object_chunk.ReleaseData();allocation_chunk.ReleaseData();movement_chunk.ReleaseData();compressed_chunk.ReleaseData();sequence_chunk.ReleaseData();mask_chunk.ReleaseData();}
static void create_chunks(){root_chunk.Reset(new C3DAnimFileRootChunk);helpers_chunk.Reset(new C3DAnimFileHelpersChunk);object_chunk.Reset(new C3DAnimFileObjectChunk);allocation_chunk.Reset(new C3DAnimFileXAllocationSizeChunk);movement_chunk.Reset(new C3DAnimFileMovementVectorChunk);compressed_chunk.Reset(new C3DAnimFileXSequenceChunk);sequence_chunk.Reset(new C3DAnimFileSequenceChunk);mask_chunk.Reset(new C3DAnimFileBoneMaskChunk);AnimationFixture::configure_chunks();}
static void check_counts(){require(root_chunk.GetReferenceCount()==1&&helpers_chunk.GetReferenceCount()==1&&object_chunk.GetReferenceCount()==1&&allocation_chunk.GetReferenceCount()==1&&movement_chunk.GetReferenceCount()==1&&compressed_chunk.GetReferenceCount()==1&&sequence_chunk.GetReferenceCount()==1&&mask_chunk.GetReferenceCount()==1,90);}
int main(){unsigned cases=0;for(int helper=0;helper<2;++helper)for(int alloc=0;alloc<3;++alloc)for(int resource=0;resource<3;++resource)for(int movement=0;movement<2;++movement)for(int compressed=0;compressed<2;++compressed)for(int track=0;track<2;++track)for(int partial=0;partial<2;++partial)for(int bones=0;bones<2;++bones)for(int bytes=0;bytes<2;++bytes){has_helpers=helper!=0;has_alloc=alloc!=0;allocation_size=alloc==2?32:0;has_movement=movement!=0;has_compressed=compressed!=0;has_track=track!=0;allocation_success=resource!=2;sequence_count=bones?3:0;compressed_size=bytes?4:0;create_chunks();{C3DAnimFile file;CSimpleMemoryPool pool(0,0);CMovableResourceMemoryPool resource_pool;C3DAnimationSequenceData data;C3DAnimationSequence sequence;C3DAnimation2 animation;active_file=&file;active_type=partial?ANIMTYPE_COMBINATION_PARTIAL:ANIMTYPE_BASE;AnimationFixture::configure(animation,pool,resource?&resource_pool:0,data,sequence);set_capacity_calls=set_pool_calls=resource_calls=read_calls=compressed_calls=plain_calls=mask_calls=stream_dtors=0;bool result=animation.InitialiseSharedAnimation(file,active_type);bool allocate=has_helpers&&has_alloc&&allocation_size!=0&&resource!=0;bool success=!(allocate&&!allocation_success);require(result==success,91);require(resource_calls==(allocate?1:0),92);require(set_pool_calls==(allocate&&success?1:0),93);require(set_capacity_calls==(has_helpers&&has_alloc&&allocation_size==0?2:1),94);require(last_capacity==(has_helpers&&has_alloc&&allocation_size==0?0:16384),95);AnimationFixture::check(success,partial!=0,has_helpers&&has_movement&&(has_compressed||has_track),has_compressed);check_counts();AnimationFixture::clear(animation);}release_chunks();++cases;}require(cases==1152,96);puts("00a4fe60_TEST PASS");return 0;}