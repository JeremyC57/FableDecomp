// CTCCarryable::GetCarrySlotActive() const -- fetch the parent thing's CCarryableDef through GetSubDef, read its ActiveCarrySlot (+0x28), release the def by hand and return the value
// Leaf: ?GetCarrySlotActive@CTCCarryable@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00705cc0, 52 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?GetCarrySlotActive@CTCCarryable@@QBEJXZ; GetSubDef<CCarryableDef> callee reading Ego_r.pdb CCarryableDef::ActiveCarrySlot +0x28; callers put things in a creature's hand (CCreatureAction_Drink::FinishAction 0x00847C90, CCreatureAction_GetItemOutOfPocket::PerformAction 0x00849D90 / 0x00849E80, CGameScriptInterface::MakeHeroCarryItemInHand 0x00897FA0, CAIStateGroup_TavernServer::OnReplace 0x009673B0, CTCInventoryWeapons::OnInitialActivate 0x005CB974); its sibling 0x00705D00 reads PassiveCarrySlot +0x30 (the bound GetCarrySlotPassive).
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
    unsigned char _pad08[0x28 - 0x08];
    long ActiveCarrySlot;                                        // +0x28
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
    long GetCarrySlotActive() const;
};
long CTCCarryable::GetCarrySlotActive() const
{
    CDefPointer<const CCarryableDef> def;
    ParentThing->PDef->GetSubDef(&def);
    const CCarryableDef* object = def.Object;
    long value = object->ActiveCarrySlot;
    if (--object->RefCount == 0)
        object->Destroy();
    return value;
}