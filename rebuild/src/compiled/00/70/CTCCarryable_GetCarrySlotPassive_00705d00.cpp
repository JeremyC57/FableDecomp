// CTCCarryable::GetCarrySlotPassive() const -- fetch the parent thing's CCarryableDef through GetSubDef, read its PassiveCarrySlot (+0x30), release the def by hand and return the value
// Leaf: ?GetCarrySlotPassive@CTCCarryable@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00705d00, 52 bytes, thiscall, plain `ret`.
// Identity (proven): bound ?GetCarrySlotPassive@CTCCarryable@@QBEJXZ (Ego_r.pdb); GetSubDef<CCarryableDef> callee reading Ego_r.pdb CCarryableDef::PassiveCarrySlot +0x30; callers PutSheatheObjectInCarrySlot 0x005C36C2, CThingCreatureBase::CreateDefaultWeapons 0x00664680, CAIStateGroup_TavernKeeper::OnReplace 0x00968BC0.
// Callees: CDefClassBase::GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*) const 0x005CC10E (Ego_r.pdb template member); the def's
//   Destroy() (vtable slot 1) on the last release.
// Layout: engine/CTCBase.h (ParentThing +0x04); retail CThing holds its definition pointer at +0x70 (as the landed
//   CTCStockItem::OnCreate 0x007F4E80; the Ego_r.pdb CThing layout differs there).
// Shape (family L018): the def pointer is zeroed after its address is pushed, the field is read before the hand-written
//   release (load / dec / store count, Destroy at zero, no null test) -- the landed 0x007F4F40 idiom.
#include "engine/CTCBase.h"
template <class T> class CDefPointer {
public:
    T* Object;                                            // +0x00
    CDefPointer() : Object(0) { }
};
class CCarryableDef {
public:
    virtual ~CCarryableDef();
    virtual void Destroy() const;                         // slot 1
    mutable long RefCount;                                // +0x04
    unsigned char _pad08[0x30 - 0x08];
    long PassiveCarrySlot;                                        // +0x30
};
class CDefClassBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005CC10E for CCarryableDef
};
// Same-TU stand-in body (not graded; the real lookup is out of line in retail): it only writes the out slot, which tells
// VC7.1 the def pointer's address does not escape the call, so the zero store follows the argument push as in retail.
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = (const T*)(const void*)this;
    return true;
}
#pragma auto_inline(on)
class CThing {                                             // Layout: retail definition pointer at +0x70
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                  // +0x70
};
class CTCCarryable : public CTCBase {
public:
    long GetCarrySlotPassive() const;
};
long CTCCarryable::GetCarrySlotPassive() const
{
    CDefPointer<const CCarryableDef> def;
    ParentThing->PDef->GetSubDef(&def);
    const CCarryableDef* object = def.Object;
    long value = object->PassiveCarrySlot;
    if (--object->RefCount == 0)
        object->Destroy();
    return value;
}