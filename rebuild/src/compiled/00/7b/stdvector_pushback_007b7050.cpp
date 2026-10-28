// std::vector::push_back(const T& value) -- Copy an intelligent pointer into spare capacity or grow the vector.
// Leaf: ?push_back@?$vector@V?$CIntelligentPointer@VCThingMarker@@@@V?$allocator@V?$CIntelligentPointer@VCThingMarker@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@VCThingMarker@@@@@Z
// Identity: full RTTI at0125a860 proves CIntelligentPointer<CThingMarker>; original base ctor/GetPItem/SetPItem and own overflow007b6ef0.
// Layout: CBaseIntelligentPointer is the original polymorphic8-byte wrapper, PData+4; vector is12bytes. No active wrapper header exists.
#ifndef _MSC_VER
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
class CBaseObject;class CBaseObjectPointer;
class CThingCreatureBase;class CThingAICreature;class CThingMarker;
class CBaseIntelligentPointer {public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(){}
    CIntelligentPointer(const CIntelligentPointer& other):CBaseIntelligentPointer(){SetPItem(other.GetPItem());}
    T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}
    void SetPItem(T* p){CBaseIntelligentPointer::SetPItem((CBaseObject*)p);}
};
namespace std {
struct __false_type {__false_type(){}};
template<class T> class allocator {};
template<class V,class T,class A> class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A=allocator<T> > class vector {
public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    void push_back(const T& value){if(_M_finish!=_M_end_of_storage._M_data){new(_M_finish) T(value);++_M_finish;}else _M_insert_overflow(_M_finish,value,__false_type(),1,true);}
protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
};
}
typedef CIntelligentPointer<CThingMarker> Element;
template void std::vector<Element>::push_back(const Element&);