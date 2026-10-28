#include <stdio.h>
class CThing {}; class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer(T* item):CBaseIntelligentPointer() { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); } T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; void push_back(const T&); }; }
class CTCBoss { public: unsigned char _unmodeled00[60]; std::vector<CIntelligentPointer<CThing> > Effects; void AddEffect(CThing& thing); };
void CTCBoss::AddEffect(CThing& thing) { reinterpret_cast<std::vector<CIntelligentPointer<CThing> >*>(&Effects)->push_back(CIntelligentPointer<CThing>(&thing)); }
class CBaseObjectPointer { public: CBaseObject* object; }; static CBaseObjectPointer block; static int calls,bad; static void* expectedVector; static CThing* expectedThing;
CBaseIntelligentPointer::CBaseIntelligentPointer() { if(calls++) bad=1; PData=&block; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { if((CThing*)item!=expectedThing || calls++!=1) bad=1; PData->object=item; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData->object; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { if(calls++!=3) bad=1; }
namespace std { template<class T,class Alloc> void vector<T,Alloc>::push_back(const T& value) { if(this!=expectedVector || value.GetPItem()!=expectedThing || calls++!=2) bad=1; } }
int main() { CTCBoss owner; CThing thing; expectedVector=&owner.Effects; expectedThing=&thing; owner.AddEffect(thing); if(bad || calls!=4) { puts("007af6c0_TEST FAIL"); return 1; } puts("007af6c0_TEST PASS"); return 0; }