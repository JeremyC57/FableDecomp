// NInventory::CCategory::SetCategoryThingAssociation(CThing* thing) -- point the inventory category's associated-thing pointer at `thing` (build an intelligent pointer temporary, assign it, release the temporary)
// Leaf: ?SetCategoryThingAssociation@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BD183, 43 bytes (a /Os module: unaligned entry, ebp frame, `leave`),
//   `__fastcall`, `ret 4`.
// Identity: Ego_r.pdb ?SetCategoryThingAssociation@CCategory@NInventory@@QAEXPAVCThing@@@Z; the caller
//   CTCInventoryBase::AddCategory (0x005C02A6) calls it on `Categories.back()` (`[edi+24h] - 2Ch`: retail CCategory is
//   0x2c with the 12-byte STLport vector) with its CThing* argument. PAssociatedThing (CIntelligentPointer<CThing>,
//   vtable 0x01238C6C) is the category's first member (+0), so the member assignment runs on `this`. The catalog's
//   "swap<CTCBase::CRegisteredTCInfo>" is propagated from an unrelated body.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 and ::operator=(const
//   CIntelligentPointer&) 0x004A9760 (both out of line in this module), ~CBaseIntelligentPointer 0x00A01C10 (the
//   temporary; the derived destructor is the generated one, no vtable reset).
// Shape: a NAMED pointer local (`lea eax,[ebp-8] ; push eax` for the assignment); an unnamed temporary passes the
//   constructor's returned `this` (`push eax`, 40v43).
// Layout: engine/CCategory.h carries the Ego_r.pdb layout (16-byte vector) with the pointer as opaque bytes, so the
//   category is modelled locally (only +0 is touched).
#pragma optimize("s", on)
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* PItem;                                 // +0x04
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    virtual void v01();
    CIntelligentPointer(T* item);                                   // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};

namespace NInventory {
class CCategory {                                       // retail 0x2c bytes
public:
    CIntelligentPointer<CThing> PAssociatedThing;       // +0x00
    unsigned char               _rest[0x24];            // +0x08 ItemsInCategory, PDef, PInventory, indices, flags
    void SetCategoryThingAssociation(CThing* thing);
};

void CCategory::SetCategoryThingAssociation(CThing* thing)
{
    CIntelligentPointer<CThing> associated(thing);
    PAssociatedThing = associated;
}
}