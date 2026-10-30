// NEntityEvents::CEventDied::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CEventDied inlined (PDeadBody then PKilledBy, then CEventBase's inline destructor) and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEventDied@NEntityEvents@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00669360, 47 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): NEntityEvents::CEventDied::`scalar deleting destructor'; evidence: slot 0 of vtable 0x0125AAF4 (RTTI
//   .?AVCEventDied@NEntityEvents@@ : CEventBase@NEntityEvents), the vtable the J108 constructor 0x00669300 stores.
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family J109: deleting destructors of byte-identical shape; retail has no
//   identical-code folding).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (Ego_r.pdb ??1CBaseIntelligentPointer@@UAE@XZ;
//   each CIntelligentPointer<CThing> member's implicit destructor is its base's, called directly), operator delete 0x00BFE9BC.
// Shape: no vtable store for CEventDied (its destructor is implicit: VC7.1 resets the vptr only in user-written destructors);
//   the members are destroyed in reverse order, then CEventBase's user-written inline destructor stores 0x0124F2C4 (RTTI
//   .?AVCEventBase@NEntityEvents@@). The deleting destructor is emitted with the vtable in a TU that constructs the class:
//   MakeOne is that construction (a modelling device; the constructor is J108's).
// Model: local, as J108 (no engine headers for the event classes).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };                      // partial view
class CBaseIntelligentPointer {                             // 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};                                                          // implicit destructor (its vtable's one slot)
namespace NEntityEvents {
class CEventBase {                                          // Ego_r.pdb 8 bytes, vtable 0x0124F2C4
public:
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase() { }
    long WorldFrame;                                        // +0x04
};
class CEventDied : public CEventBase {   // Ego_r.pdb 24 bytes, vtable 0x0125AAF4
public:
    CEventDied(CThing* a, CThing* b) : PKilledBy(a), PDeadBody(b) { }   // J108 (0x00669300)
    CIntelligentPointer<CThing> PKilledBy;                  // +0x08
    CIntelligentPointer<CThing> PDeadBody;                  // +0x10
};
}   // namespace NEntityEvents
void* MakeOne(CThing* a, CThing* b)
{
    return new NEntityEvents::CEventDied(a, b);
}