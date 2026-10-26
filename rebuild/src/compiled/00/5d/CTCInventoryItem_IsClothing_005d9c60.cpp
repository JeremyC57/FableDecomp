// CTCInventoryItem::IsClothing(long defIndex) -- static: whether the item def for defIndex is a clothing item (its inventory interface type is 0x12); false when the def cannot be cached
// Leaf: ?IsClothing@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D9C60, 41 bytes (+7 int3). Ego_r.pdb `?IsClothing@CTCInventoryItem@@SI_NJ@Z` (static __fastcall,
// defIndex in ecx, passed straight on to CacheDef; `ret`).
// Callee: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool). Global: 0x013B8C18 the cached
// CInventoryItemDef (engine/CInventoryItemDef.h: InventoryType +0x50, an ETCInterfaceType kept as bytes in the header).
// Shape: speed TU; the static GetInventoryType(long) (out of line in Ego_r, 0x0055A9A0 there) is inlined with its two
// returns, and the `== 0x12` test is tail-duplicated into both without being folded on the miss path
// (`xor eax,eax ; xor ecx,ecx ; cmp eax,12h ; sete cl`).
#include "engine/CInventoryItemDef.h"
enum ETCInterfaceType { };                              // enumerator names not recovered; 0x12 is the clothing type
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static bool __fastcall CacheDef(long defIndex);                     // 0x005D94B0
    static ETCInterfaceType __fastcall GetInventoryType(long defIndex)
    {
        if (CacheDef(defIndex))
            return reinterpret_cast<const ETCInterfaceType&>(s_CachedItemDef->InventoryType);
        return ETCInterfaceType(0);
    }
    static bool __fastcall IsClothing(long defIndex);
    static CDefPointer<const CInventoryItemDef> s_CachedItemDef;      // 0x013B8C18
};
bool __fastcall CTCInventoryItem::IsClothing(long defIndex)
{
    return GetInventoryType(defIndex) == ETCInterfaceType(0x12);
}