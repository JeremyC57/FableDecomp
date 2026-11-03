#ifdef FABLE_RETAIL_BANK_SYMBOLS
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/bank_symbols.h"
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return std::vector<T>::operator[](n);}const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
#endif
#else
#if defined(FABLE_RETAIL_PARTICLE_SPRINGS)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/particle_springs.h"
template<class T>class CArray:public std::vector<T>{public:CArray(){}explicit CArray(unsigned int n):std::vector<T>(n){}CArray(unsigned int n,const T&x):std::vector<T>(n,x){}CArray(const CArray&x):std::vector<T>(x){} T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,x);++this->_M_finish;}else std::vector<T>::push_back(x);} };
template<>__declspec(noinline) CArray<std::set<unsigned long> >::CArray(unsigned int n):std::vector<std::set<unsigned long> >(n){}
#endif
#else
#ifdef FABLE_RETAIL_STIPPLE
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/stipple.h"
// Original public vector base; original CArray constructor overloads confirmed in PDB.
template<class T>class CArray:public std::vector<T>{public:CArray(){} CArray(unsigned int n,const T&v=T()):std::vector<T>(n,v){} T&operator[](unsigned int i){return std::vector<T>::operator[](i);}const T&operator[](unsigned int i)const{return std::vector<T>::operator[](i);}};
#endif
#else
#if defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/animation.h"
// Original CArray public vector base and constructors, raw PDB plus native animation.
template<class T>class CArray:public std::vector<T>{public:
 CArray():std::vector<T>(){} explicit CArray(unsigned int n):std::vector<T>(n){}
 CArray(unsigned int n,const T& x):std::vector<T>(n,x){}
 void push_back(const T& _Val){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);}
#ifdef FABLE_RETAIL_ANIMATION_RETRY
 unsigned int GetByteSize()const{return this->size()*sizeof(T);}
#endif
 T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport/fable_containers.h"
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}const T*GetAsCArray()const{return this->size()>0?this->_M_start:(const T*)this;}};
#endif
#else
#ifdef FABLE_RETAIL_NAV_REGIONS
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/navigation.h"
// Original public vector base; native node arrays use four-byte entries.
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_STRIPIFIER)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
#include "stlport46/stripifier.h"
// Original CArray public vector base, constructors and index accessors (PDB).
// Retail uses STLport's 12-byte base; the donor's later vector base is 16 bytes.
template<class T>class CArray:public std::vector<T>{
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
template<class T> class CArray:public std::vector<T> {
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
#include "stlport46/geometry.h"
#ifdef FABLE_RETAIL_MESH_PARTITION
template<class T>class CArray:public std::vector<T>{
public:CArray(){} void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,x);++this->_M_finish;}else std::vector<T>::push_back(x);} T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};
#else
template<class T>class CArray:public std::vector<T>{
public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};
#endif
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_DISPLAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Original CArray adds no fields to its public STLport vector base.
template<class T> class CArray : public std::vector<T> {};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// Original CArray<char> methods from FableWin PDB. Retail zero-size arms
// at 00409905 and 004099dc return the address of the CArray object itself.
// This branch is included after the selected genuine STLport vector definition.
template<class T> class CArray : public std::vector<T> {
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
#include "stlport/fable_containers.h"
// Original CArray public vector base, no added members. Retail STLport size12.
// PDB CArray<C3DVector> confirms unsigned-int index and both reference overloads.
template<class T> class CArray : public std::vector<T> {
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

#endif

#endif

#endif

#endif
