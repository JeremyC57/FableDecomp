// CTCStockItem::OnCreate() -- once: take the stock-item sub-def of the parent thing's definition and copy its default price and stealable flag
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F4E80, 68 bytes (plain `ret`). Vtable 0x01240C1C (CTCStockItem, rtti-vtables.tsv) slot 4.
// Callees: CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer*) @ 0x0059350C (manifest "GetSubDef<CAbilityDef>", a byte-twin
// template; the result is ignored), the def's virtual slot 1 on the last release.
// Layout: engine/CTCStockItem.h (Ego_r.pdb): Stealable +0x0d, Initialised +0x0e, Price +0x10; the header pads the CTCBase prefix,
// so the parent thing (+0x04) is read through an accessor. CThing's definition pointer at +0x70 (retail, as the landed
// CTCInventoryItem/CThing models); CStockItemDef (Ego_r.pdb): DefaultPrice +0x28, DefaultIsStealable +0x2c.
// Shape: the def pointer is zeroed after the argument push (declared, then the call); the object is read once and released
// by hand WITHOUT a null test (`dec [ecx+4] ; jne`), so retail trusts GetSubDef to fill it.
#include "engine/CTCStockItem.h"

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CStockItemDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long          DefaultPrice;                             // +0x28
    bool          DefaultIsStealable;                       // +0x2c
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CStockItemDef>* out);        // 0x0059350C
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CStockItemDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                    // +0x70
};

struct CTCStockItem_Methods : CTCStockItem {
    CThing* GetThing() const { return *(CThing* const*)&_pad_0x04[0]; }   // CTCBase::ParentThing (+0x04)
    void OnCreate();
};

void CTCStockItem_Methods::OnCreate()
{
    if (!Initialised) {
        CDefPointer<CStockItemDef> def;
        GetThing()->PDef->GetSubDef(&def);
        CStockItemDef* object = def.Object;
        Price = object->DefaultPrice;
        Stealable = object->DefaultIsStealable;
        Initialised = true;
        if (--object->RefCount == 0)
            object->DestroySelf();
    }
}
