// CBaseObject::ClearIntelligentPointers() -- detach the object from its intelligent-pointer block: null the block's object, free the block when no pointer references it, forget it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A3C0, length 44 bytes.
// Leaf: ClearIntelligentPointers
// Label correction: the catalog "_Dest_val<..., std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>" is
// propagated. The row sits in the CBaseObject / CBaseIntelligentPointer TU (0x0099A300..0x0099A4D0) and its body is
// exactly the middle of CBaseObject's deleting destructor 0x0099A4D0 (vtable 0x0129A7D8 slot 0) without the vtable
// stores; Ego_r.pdb lists the public non-virtual ?ClearIntelligentPointers@CBaseObject@@QAEXXZ next to the CBaseObject
// constructors, which is this body. No call reference was found (the destructor inlines its own copy).
// Callees: operator delete @ 0x00BFE9BC (the pointer block).
// Layout: engine/CBaseObject.h (IntelligentPointer +0x04); Ego_r.pdb CBaseObjectPointer = {object +0, refcount +4}.
// Shape: the member is re-read after the store through it (the store may alias), then the block's count tested.
#include "engine/CBaseObject.h"
void __cdecl operator delete(void* block);  // 0x00BFE9BC
struct CBaseObjectPointer {                 // Ego_r.pdb, 8 bytes
    CBaseObject*  object;                   // +0x00
    unsigned long refcount;                 // +0x04
};
struct CBaseObject_Methods : CBaseObject { void ClearIntelligentPointers(); };
void CBaseObject_Methods::ClearIntelligentPointers()
{
    if (IntelligentPointer) {
        IntelligentPointer->object = 0;
        if (IntelligentPointer->refcount == 0)
            delete IntelligentPointer;
        IntelligentPointer = 0;
    }
}