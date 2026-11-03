#ifdef FABLE_RETAIL_UI_ORDINARY_INLINE
#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY
#include "stlport46/geometry.h"
#else
#include "stlport/fable_containers.h"
#endif
// Ego_r TPI CArray derives std::vector; CVectorMap derives CArray.
// Retail LowerBound reads start+0/finish+4. Compare +12, Dirty +13.
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
#include "engine/CArray.h"
#else
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
public:T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}
#endif
#ifdef FABLE_RETAIL_FACTIONS
public:
    void push_back(const T& value){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,value);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,value,std::__false_type(),1,true);
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
public:void push_back(const T&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
public:
    // Original lib_array.hpp36-41: _Val parameter; direct capacity/construction path.
    void push_back(const T& _Val){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);
    }
#endif
};
#endif
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
#ifdef FABLE_RETAIL_FACTIONS
namespace std {template<class Iterator,class Compare>void __fastcall sort(Iterator,Iterator,Compare);}
#endif
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
#ifdef FABLE_RETAIL_FACTIONS
    CVectorMap():Dirty(false){}
#endif
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
#if defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_FACTIONS)
    void Insert(const K& key,const V& value){this->push_back(value_type(key,value));Dirty=true;}
    void Sort(){std::sort(this->begin(),this->end(),Compare);Dirty=false;}
    void Update(){if(Dirty)Sort();}
#endif
#if defined(FABLE_RETAIL_STATUS_MENU) && !defined(FABLE_RETAIL_UI_ORDINARY_INLINE)
    __forceinline
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#endif

#else
#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY
#include "stlport46/geometry.h"
#else
#include "stlport/fable_containers.h"
#endif
// Ego_r TPI CArray derives std::vector; CVectorMap derives CArray.
// Retail LowerBound reads start+0/finish+4. Compare +12, Dirty +13.
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MAP_BRUSHES)
#include "engine/CArray.h"
#else
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
public:T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}
#endif
#ifdef FABLE_RETAIL_FACTIONS
public:
    void push_back(const T& value){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,value);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,value,std::__false_type(),1,true);
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
public:void push_back(const T&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
public:
    // Original lib_array.hpp36-41: _Val parameter; direct capacity/construction path.
    void push_back(const T& _Val){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);
    }
#endif
};
#endif
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
#ifdef FABLE_RETAIL_FACTIONS
namespace std {template<class Iterator,class Compare>void __fastcall sort(Iterator,Iterator,Compare);}
#endif
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
#ifdef FABLE_RETAIL_FACTIONS
    CVectorMap():Dirty(false){}
#endif
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
#if defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_FACTIONS)
    void Insert(const K& key,const V& value){this->push_back(value_type(key,value));Dirty=true;}
    void Sort(){std::sort(this->begin(),this->end(),Compare);Dirty=false;}
    void Update(){if(Dirty)Sort();}
#endif
#ifdef FABLE_RETAIL_STATUS_MENU
    __forceinline
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#endif

#endif
