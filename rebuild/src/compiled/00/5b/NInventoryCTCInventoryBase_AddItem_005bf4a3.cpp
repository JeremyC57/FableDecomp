// NInventory::CTCInventoryBase::AddItem(const NInventory::CItem& item) -- find the category whose def index is the item's inventory category and add the item to it; on success notify the inventory (OnItemAdded(type, true, true)) and return true
// Leaf: ?AddItem@CTCInventoryBase@
// Retail: Fable.exe @ 0x005BF4A3, 74 bytes, `ret 4` (thiscall). No Ego_r.pdb symbol (inlined there): named by effect.
// Callers: CTCInventoryBase::InitialiseInventoryWithItems 0x005BF8CB, ReturnAllConfiscatedItems 0x005BF91A, Copy 0x005BFB96.
// Callees: GetInventoryCategory(long itemType) @ 0x005D97B0 (`__fastcall`: the item def's category def index or -1),
// std::find_if(first, last, CIsCategoryWithDefIndex) @ 0x005C08D3 (the `__fastcall` STLport forwarder over Categories,
// catalog "_Umove<EWeaponClass*>" propagated), NInventory::CCategory::AddItem(const CItem&) @ 0x005BF103 (bool),
// the inventory's virtual slot 84 (+0x150) OnItemAdded(long, bool, bool) (Ego_r `?OnItemAdded@CTCInventoryBase@NInventory@@MAEXJ_N0@Z`).
// Layout: retail CTCInventoryBase Categories {first, last, end} at +0x20 (as landed AddCategory 0x005C024A); CItem ItemType +0.
// Shape: size TU; the category end is read into a local AFTER the category lookup (edi, compared with the find result), the
// `&&` of the two tests shares the `xor al,al` tail with `mov al,1` after the virtual call.
#pragma optimize("s", on)
#include "engine/CItem.h"
long __fastcall GetInventoryCategory(long itemType);          // 0x005D97B0
namespace NInventory {
class CCategory {                                            // retail 0x2C bytes
public:
    unsigned char _body[0x2C];
    bool AddItem(const CItem& item);                         // 0x005BF103
};
}
struct CIsCategoryWithDefIndex {
    long DefIndex;
    CIsCategoryWithDefIndex(long defIndex) : DefIndex(defIndex) {}
};
namespace std {
NInventory::CCategory* __fastcall find_if(NInventory::CCategory* first, NInventory::CCategory* last, CIsCategoryWithDefIndex pred);   // 0x005C08D3
}
namespace NInventory {
class CTCInventoryBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void OnItemAdded(long itemType, bool a, bool b);  // slot 84 (+0x150)
    unsigned char _pad04[0x20 - 0x04];
    CCategory* CategoriesFirst;                              // +0x20 vector<CCategory> (retail 12 bytes)
    CCategory* CategoriesLast;                               // +0x24
    CCategory* CategoriesEnd;                                // +0x28
    bool AddItem(const CItem& item);
};
bool CTCInventoryBase::AddItem(const CItem& item)
{
    long categoryDefIndex = GetInventoryCategory(item.ItemType);
    CCategory* last = CategoriesLast;
    CCategory* category = std::find_if(CategoriesFirst, last, CIsCategoryWithDefIndex(categoryDefIndex));
    if (category != last && category->AddItem(item)) {
        OnItemAdded(item.ItemType, true, true);
        return true;
    }
    return false;
}
}