// NInventory::CCategory::GetCategoryIdentifier() const -- returns the category definition's CategoryIdentifier
// Retail: Fable.exe @ 0x005BD1B3, 28 bytes. Body: a CDefPointer copy of PDef (add-ref `inc [ecx+4]` when non-null), read
// CategoryIdentifier (+0x50), then release the copy (`dec [ecx+4]`, slot 1 Delete on zero).
// Layout: retail CCategory PDef at +0x14 (engine/CCategory.h has the Ego_r.pdb +0x18; the STLport vector ahead of it is
// 12 bytes, not 16), CInventoryCategoryDef CategoryIdentifier at +0x50 (Ego_r.pdb +0x54; the retail CDefPointeeBase
// prefix is 4 bytes smaller, as in landed 0x008C4680), so both are modelled locally.
// Opt: size TU (`dec dword ptr [ecx+4]` in place).
#pragma optimize("s", on)
enum EInventoryCategoryIdentifier { INVENTORY_CATEGORY_NONE };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryCategoryDef : CDefPointeeBase {
    unsigned char                _pad08[0x48];            // +0x08
    EInventoryCategoryIdentifier CategoryIdentifier;      // +0x50
};
namespace NInventory {
class CCategory {
public:
    unsigned char               PAssociatedThing[0x8];    // +0x00 CIntelligentPointer<CThing>
    unsigned char               ItemsInCategory[0xc];     // +0x08 STLport vector<CItem>
    CDefPointer<CInventoryCategoryDef> PDef;              // +0x14
    EInventoryCategoryIdentifier GetCategoryIdentifier() const;
};
EInventoryCategoryIdentifier CCategory::GetCategoryIdentifier() const
{
    CDefPointer<CInventoryCategoryDef> def(PDef);
    return def->CategoryIdentifier;
}
}