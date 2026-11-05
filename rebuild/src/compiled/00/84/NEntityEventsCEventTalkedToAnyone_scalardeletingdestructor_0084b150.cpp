// NEntityEvents::CEventTalkedToAnyone::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor (PThingTalkedTo destroyed, then the inline NEntityEvents::CEventBase destructor's vtable store) and the flag-tested operator delete
// Leaf: ??_GCEventTalkedToAnyone@NEntityEvents@@UAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B150, 39 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity (proven): NEntityEvents::CEventTalkedToAnyone::`scalar deleting destructor'; evidence: it is slot 0 of vtable 0x01274270 (RTTI .?AVCEventTalkedToAnyone@NEntityEvents@@,
//   bases .?AVCEventBase@NEntityEvents@@(+0)), it destroys PThingTalkedTo at +0x8 (Ego_r.pdb CIntelligentPointer<CThing> PThingTalkedTo +0x8) and stores the
//   NEntityEvents::CEventBase vtable 0x0124F2C4 (RTTI .?AVCEventBase@NEntityEvents@@).
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H085: six deleting destructors of classes whose only non-trivial
//   member is one object at +8 / +0x94; retail has no identical-code folding, so each address is its own class's ??_G).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 on PThingTalkedTo (the implicit ~CIntelligentPointer<CThing> inline), operator delete 0x00BFE9BC.
// Shape: NEntityEvents::CEventTalkedToAnyone has no user-declared destructor (no own vtable store); NEntityEvents::CEventBase's `virtual ~CEventBase() { }` is inline
//   and stores its vtable; the vtable (and so this ??_G) is emitted by the construction helper below. Local classes, Review at landing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NEntityEvents {
class CEventBase {                                         // vtable 0x0124F2C4
public:
    virtual ~CEventBase() { }
    long EventType;                                         // +0x04
};
class CEventTalkedToAnyone : public CEventBase {
public:
    CIntelligentPointer<CThing> PThingTalkedTo; // +0x08
};
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_0084b150()
{
    return new NEntityEvents::CEventTalkedToAnyone;
}