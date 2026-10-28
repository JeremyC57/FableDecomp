#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
class CThing {}; class CThingCreatureBase:public CThing {}; class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer(T* item=0):CBaseIntelligentPointer() { SetPItem(item); } T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); } bool operator==(const CIntelligentPointer& other) const { return GetPItem()==other.GetPItem(); } CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; } };
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag:input_iterator_tag {}; struct bidirectional_iterator_tag:forward_iterator_tag {}; struct random_access_iterator_tag:bidirectional_iterator_tag { random_access_iterator_tag() {} };
template<class Input,class T> NOINLINE Input __fastcall __find(Input first,Input last,const T& value,const random_access_iterator_tag&) { int n=(last-first)>>2; for(;n>0;--n) { if(*first==value) return first; ++first; if(*first==value) return first; ++first; if(*first==value) return first; ++first; if(*first==value) return first; ++first; } switch(last-first) { case 3: if(*first==value) return first; ++first; case 2: if(*first==value) return first; ++first; case 1: if(*first==value) return first; default: return last; } }
template<class Input,class Output,class T> NOINLINE Output __fastcall remove_copy(Input first,Input last,Output result,const T& value) { for(;first!=last;++first) if(!(*first==value)) { *result=*first; ++result; } return result; }
template<class Input,class T> inline Input remove(Input first,Input last,const T& value) { first=__find(first,last,value,random_access_iterator_tag()); return first==last?first:remove_copy(first+1,last,first,value); }
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; T* begin() { return _M_start; } T* end() { return _M_finish; } T* erase(T*,T*); };
}
class CTCBase { public: virtual ~CTCBase() {} CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCSolidToThing:public CTCBase { public: std::vector<CIntelligentPointer<const CThing> > NotSolidToThings; void RemoveThingNotSolidTo(const CThing* thing); };
void CTCSolidToThing::RemoveThingNotSolidTo(const CThing* thing) { reinterpret_cast<std::vector<CIntelligentPointer<const CThing> >*>(&NotSolidToThings)->erase(std::remove(reinterpret_cast<std::vector<CIntelligentPointer<const CThing> >*>(&NotSolidToThings)->begin(),reinterpret_cast<std::vector<CIntelligentPointer<const CThing> >*>(&NotSolidToThings)->end(),CIntelligentPointer<const CThing>(thing)),reinterpret_cast<std::vector<CIntelligentPointer<const CThing> >*>(&NotSolidToThings)->end()); }
class CBaseObjectPointer { public: CBaseObject* object; unsigned long refcount; }; static CBaseObjectPointer pool[128]; static int nextBlock=0,destroyed=0,eraseCalls=0,bad=0; static void* expectedEnd;
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++destroyed; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData?PData->object:0; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* object) { if(!PData) PData=&pool[nextBlock++]; PData->object=object; }
namespace std { template<class T,class Alloc> T* vector<T,Alloc>::erase(T* first,T* last) { if(last!=expectedEnd || first<_M_start || first>last) bad=1; ++eraseCalls; _M_finish=first; return first; } }
int main() { CTCSolidToThing object; CIntelligentPointer<const CThing> data[5]; CThingCreatureBase things[3]; std::vector<CIntelligentPointer<const CThing> >& list=*reinterpret_cast<std::vector<CIntelligentPointer<const CThing> >*>(&object.NotSolidToThings); list._M_start=data; list._M_finish=data+5; list._M_end_of_storage._M_data=data+5; data[0].SetPItem((const CThing*)&things[0]); data[1].SetPItem((const CThing*)&things[1]); data[2].SetPItem((const CThing*)&things[0]); data[3].SetPItem((const CThing*)&things[2]); data[4].SetPItem((const CThing*)&things[0]); expectedEnd=data+5; destroyed=0;
 object.RemoveThingNotSolidTo(&things[0]); if(bad || eraseCalls!=1 || destroyed!=1 || list._M_finish!=data+2 || data[0].GetPItem()!=(const CThing*)&things[1] || data[1].GetPItem()!=(const CThing*)&things[2]) goto fail;
 expectedEnd=list._M_finish; object.RemoveThingNotSolidTo(&things[0]); if(bad || eraseCalls!=2 || destroyed!=2 || list._M_finish!=data+2) goto fail;
 list._M_finish=data; expectedEnd=data; object.RemoveThingNotSolidTo(&things[0]); if(bad || eraseCalls!=3 || destroyed!=3 || list._M_finish!=data) goto fail;
 puts("00770f20_TEST PASS"); return 0; fail: puts("00770f20_TEST FAIL"); return 1; }