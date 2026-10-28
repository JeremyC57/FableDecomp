#include <stdio.h>
class CThing {}; template<class T> class CIntelligentPointer;
class CThing;
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace std {
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; vector():_M_start(0),_M_finish(0) { _M_end_of_storage._M_data=0; } };
}
class CTCSolidToThing:public CTCBase { public: std::vector<CIntelligentPointer<const CThing> > NotSolidToThings; CTCSolidToThing(CThing&); };
CTCSolidToThing::CTCSolidToThing(CThing& thing):CTCBase(thing) {}
static int calls,bad; static CThing* expected;
CTCBase::CTCBase(CThing& thing) { if(&thing!=expected) bad=1; ++calls; ParentThing=&thing; ToBeRemoved=IsInGlobalUpdateTCs=false; }
int main() { CThing thing; expected=&thing; CTCSolidToThing object(thing); if(bad || calls!=1 || object.ParentThing!=&thing || object.NotSolidToThings._M_start || object.NotSolidToThings._M_finish || object.NotSolidToThings._M_end_of_storage._M_data) { puts("00770e50_TEST FAIL"); return 1; } puts("00770e50_TEST PASS"); return 0; }