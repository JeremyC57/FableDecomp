// CTCStockItem::GetBuyersPhrase() -- fetch the parent thing's CStockItemDef through GetSubDef, read its BuyersPhrase (+0x34), release the def by hand and return the value
// Leaf: ?GetBuyersPhrase@CTCStockItem@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007f4f80, 52 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?GetBuyersPhrase@CTCStockItem@@QAEKXZ; GetSubDef<CStockItemDef> callee; CStockItemDef::BuyersPhrase +0x34 (Ego_r.pdb; the landed sibling CTCStockItem::GetSellersBanter 0x007F4F40 reads SellersBanter +0x30); caller CAIStateGroup_BuyFromSeller::Update 0x00919AE0.
// Callees: CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>*) const 0x0059350C (Ego_r.pdb template member); the def's
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
class CStockItemDef {
public:
    virtual ~CStockItemDef();
    virtual void Destroy() const;                         // slot 1
    mutable long RefCount;                                // +0x04
    unsigned char _pad08[0x34 - 0x08];
    unsigned long BuyersPhrase;                                        // +0x34
};
class CDefClassBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0059350C for CStockItemDef
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
class CTCStockItem : public CTCBase {
public:
    unsigned long GetBuyersPhrase();
};
unsigned long CTCStockItem::GetBuyersPhrase()
{
    CDefPointer<const CStockItemDef> def;
    ParentThing->PDef->GetSubDef(&def);
    const CStockItemDef* object = def.Object;
    unsigned long value = object->BuyersPhrase;
    if (--object->RefCount == 0)
        object->Destroy();
    return value;
}