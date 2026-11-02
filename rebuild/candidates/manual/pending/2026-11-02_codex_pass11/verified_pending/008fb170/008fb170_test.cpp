#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif


#define FABLE_STLPORT_CAMERA_POINTS
#define FABLE_STLPORT_CONSTRUCTORS
#ifndef FABLE_ENGINE_CPHYSICSCOLLISIONPOINTSOBJECT_H
#define FABLE_ENGINE_CPHYSICSCOLLISIONPOINTSOBJECT_H


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
#pragma once



#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_static_assert_, __LINE__)[(expression) ? 1 : -1]


class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
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
    FABLE_VECTOR_INLINE C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
#include <new>
#endif
#include <stdlib.h>
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
struct __false_type { __false_type() { } };
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
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
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
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
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
struct __false_type { __false_type() { } };
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
    void deallocate(_Tp* p, size_t) { free(p); }
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
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
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
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_STLPORT_UI
    vector(const vector&);
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
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
public:
#endif
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    __forceinline
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
    void push_back(const _Tp&);
#endif
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#ifdef FABLE_STLPORT_CAMERA_POINTS
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
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
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

namespace NCameraModes {
class CPhysicsCollisionPointsObject {
public:
    std::list<C3DVector> Points;
    std::list<std::pair<float,std::list<C3DVector> > > SmartPointList;
    std::list<std::pair<float,long> > NumTestsPerTheta;
    long ScanDensity;
    float StartTheta;
    float DeltaTheta;
    long DebugGraphicsRed,DebugGraphicsGreen,DebugGraphicsBlue;
    CPhysicsCollisionPointsObject(long,float,float,long,long,long);
};
}
#ifdef _MSC_VER
typedef char FABLE_STATIC_ASSERT_CPhysicsCollisionPointsObject[(sizeof(NCameraModes::CPhysicsCollisionPointsObject)==36)?1:-1];
#endif
#endif

extern const float DELTA_NOTIONAL_ZERO;

NCameraModes::CPhysicsCollisionPointsObject::CPhysicsCollisionPointsObject(long scan_den,float s_thet,float d_thet,long red,long green,long blue)
: ScanDensity(scan_den),StartTheta(s_thet),DeltaTheta(d_thet),DebugGraphicsRed(red),DebugGraphicsGreen(green),DebugGraphicsBlue(blue)
{
    for(float theta=s_thet;theta<=0.5f+DELTA_NOTIONAL_ZERO;theta+=d_thet) {
        std::list<C3DVector> new_list;
        std::pair<float,std::list<C3DVector> > new_guy_to_add(theta,new_list);
        SmartPointList.push_back(new_guy_to_add);
    }
    std::list<std::pair<float,std::list<C3DVector> > >::iterator it=SmartPointList.begin();
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
}


#include <string.h>
static float native_epsilon(){unsigned int bits=0x38d1b717;float result;memcpy(&result,&bits,4);return result;}
const float DELTA_NOTIONAL_ZERO=native_epsilon();
static void require(bool condition,int code){if(!condition){fprintf(stderr,"points check %d\n",code);exit(code);}}
static void run(float start,float step,long density,long red,long green,long blue){
 NCameraModes::CPhysicsCollisionPointsObject object(density,start,step,red,green,blue);
 require(object.ScanDensity==density&&object.StartTheta==start&&object.DeltaTheta==step,1);
 require(object.DebugGraphicsRed==red&&object.DebugGraphicsGreen==green&&object.DebugGraphicsBlue==blue,2);
 require(object.Points.empty(),3);
 std::list<std::pair<float,std::list<C3DVector> > >::iterator iter=object.SmartPointList.begin();
 unsigned count=0;
 for(float expected=start;expected<=0.5f;expected+=step){
  require(iter!=object.SmartPointList.end(),4);
  require(iter->first==expected&&iter->second.empty(),5);
  std::_List_node_base* node=iter._M_node;
  require(node->_M_next->_M_prev==node&&node->_M_prev->_M_next==node,6);
  ++iter;++count;
 }
 require(iter==object.SmartPointList.end()&&count>=11,7);
 long values[11]={1,1,1,10,10,10,10,10,1,1,1};
 std::list<std::pair<float,long> >::iterator tests=object.NumTestsPerTheta.begin();
 for(unsigned i=0;i<11;++i){
  require(tests!=object.NumTestsPerTheta.end(),8);
  require(tests->first==start+step*(float)i&&tests->second==values[i],9);
  std::_List_node_base* node=tests._M_node;
  require(node->_M_next->_M_prev==node&&node->_M_prev->_M_next==node,10);
  ++tests;
 }
 require(tests==object.NumTestsPerTheta.end(),11);
}
int main(){
 float starts[4]={-0.25f,0.0f,0.125f,0.1875f};float steps[2]={0.03125f,0.015625f};
 for(unsigned i=0;i<4;++i)for(unsigned j=0;j<2;++j)for(long k=0;k<3;++k)
  run(starts[i],steps[j],k*7-3,255-k,k*64,127+k);
 for(unsigned boundary=0;boundary<2;++boundary){
  float start=boundary?0.0001220703125f:0.00006103515625f;
  NCameraModes::CPhysicsCollisionPointsObject object(1,start,0.03125f,1,2,3);
  unsigned count=0;float last=0;
  std::list<std::pair<float,std::list<C3DVector> > >::iterator item=object.SmartPointList.begin();
  for(;item!=object.SmartPointList.end();++item){++count;last=item->first;}
  require(count==(boundary?16u:17u),12);
  require(last==(boundary?0.4688720703125f:0.50006103515625f),13);
 }
 puts("008fb170_TEST PASS");return 0;
}
