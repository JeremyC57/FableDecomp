#include <stdio.h>
#include <stdlib.h>
// std::vector::push_back(const T& value) -- Copy an intelligent pointer into spare capacity or grow the vector.
// Leaf: ?push_back@?$vector@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@VCThingAICreature@@@@@Z
// Identity: full RTTI at0123e7e0 proves CIntelligentPointer<CThingAICreature>; original base ctor/GetPItem/SetPItem and own overflow007afd10.
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
typedef CIntelligentPointer<CThingAICreature> Element;
template void std::vector<Element>::push_back(const Element&);

class CBaseObject {};
class CBaseObjectPointer {public:CBaseObject* Item;};
static CBaseObjectPointer pool[8];static int created,getsCount,setsCount,grows,bad;
static const void* expectedValue;static const void* expectedPosition;static const void* expectedVector;
CBaseIntelligentPointer::CBaseIntelligentPointer(){PData=&pool[created++];PData->Item=0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{++getsCount;return PData->Item;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){++setsCount;PData->Item=p;}
namespace std {template<class T,class A> void vector<T,A>::_M_insert_overflow(T* pos,const T& value,const __false_type&,unsigned int n,bool atEnd){++grows;if(this!=expectedVector || pos!=expectedPosition || &value!=expectedValue || n!=1 || !atEnd)bad=1;}}
int main(){
    Element input;CBaseObject thing;input.PData->Item=&thing;
    Element* data=(Element*)malloc(2*sizeof(Element));if(!data)return 1;std::vector<Element> v;v._M_start=data;v._M_finish=data;v._M_end_of_storage._M_data=data+2;
    getsCount=setsCount=0;int initial=created;v.push_back(input);
    int ok=created==initial+1 && getsCount==1 && setsCount==1 && !grows && v._M_finish==data+1 && data[0].PData->Item==&thing;
    input.PData->Item=0;v.push_back(input);ok=ok && created==initial+2 && getsCount==2 && setsCount==2 && !grows && v._M_finish==data+2 && data[1].PData->Item==0;
    expectedValue=&input;expectedPosition=data+2;expectedVector=&v;v.push_back(input);
    ok=ok && !bad && grows==1 && created==initial+2 && getsCount==2 && setsCount==2;
    data[0].~Element();data[1].~Element();free(data);
    puts(ok ? "007b0000_TEST PASS" : "007b0000_TEST FAIL");return ok ? 0 : 1;
}