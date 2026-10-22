// CTCStockItem::GetSellersBanter() -- the seller's banter text id of the stock item's CStockItemDef (fetched through the parent thing's def, released by hand)
// Leaf: GetSellersBanter
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F4F40, 52 bytes, `ret`. Catalogued FUN_007f4f40 (unreferenced); named from
//   Ego_r.pdb `?GetSellersBanter@CTCStockItem@@QAEKXZ` (unsigned long result) and CStockItemDef::SellersBanter +0x30
//   (its sibling GetBuyersPhrase would read +0x34). It sits among the CTCStockItem methods (OnSerialise 0x007F4ED0,
//   GetShopTC 0x007F4FC0).
// Callees: CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer*) @ 0x0059350C (manifest "GetSubDef<CAbilityDef>", a
//   byte-twin template; the result is ignored), the def's virtual slot 1 on the last release.
// Layout: engine/CTCStockItem.h (Ego_r.pdb); the header pads the CTCBase prefix, so the parent thing (+0x04) is read
//   through an accessor. CThing's definition pointer at +0x70 (retail, as the landed CTCStockItem::OnCreate 0x007F4E80).
// Shape: the landed OnCreate idiom: the def pointer is zeroed after the argument push, the def is read once and released
//   by hand WITHOUT a null test (`dec [ecx+4] ; jne`), the banter loaded before the release.
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
    bool          CanOnlyBeSoldToPlayer;                    // +0x2d
    unsigned long SellersBanter;                            // +0x30
    unsigned long BuyersPhrase;                             // +0x34
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CStockItemDef>* out);        // 0x0059350C
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x007F4E80 idiom).
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
    unsigned long GetSellersBanter();
};

unsigned long CTCStockItem_Methods::GetSellersBanter()
{
    CDefPointer<CStockItemDef> def;
    GetThing()->PDef->GetSubDef(&def);
    CStockItemDef* object = def.Object;
    unsigned long banter = object->SellersBanter;
    if (--object->RefCount == 0)
        object->DestroySelf();
    return banter;
}