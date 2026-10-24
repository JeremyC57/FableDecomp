// CTCInventoryItem::ClearCachedDefs() -- forget the static def cache: zero the cached def index and release both cached def pointers (the thing def and its inventory-item def)
// Leaf: ?ClearCachedDefs@
// Retail: Fable.exe @ 0x005D9320, 96 bytes (+16 int3), `ret`. Ego_r.pdb `?ClearCachedDefs@CTCInventoryItem@@SIXXZ` (static
// __fastcall, no arguments).
// Globals: the CacheDef(long) @ 0x005D94B0 cache -- 0x013B8BC8 the cached def index, 0x013B8C14 the cached thing def
// (CDefPointer, filled by CDefinitionManager::GetDef), 0x013B8C18 the cached CInventoryItemDef (its sub-def; read by every
// static CTCInventoryItem getter: GetInventoryCategory +0x3C, GetDescription +0x34, ...).
// Body: s_CachedDefIndex = 0; then each def pointer `if (Object) Object->Release(); Object = 0;`.
// Shape: speed TU (16-byte aligned, int3 padding); a zero register (esi) for the three stores; each release is the pointee's
// out-of-line-shaped Release() over two inline ref-count accessors (`mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ;
// cmp edx,esi ; mov [eax],edx ; jne` then slot 1) -- the wave-18 DecRef()/GetRefCount() form; each global's zero store is
// scheduled after the next global's load.
// Layout: engine/CInventoryItemDef.h (retail offsets: ItemDescription +0x34, InventoryCategory +0x3C) names the cached def;
// the class itself is declared locally only for its static members (engine/CTCInventoryItem.h has the instance layout,
// which this static function never touches).
#include "engine/CInventoryItemDef.h"
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;                         // slot 1 (+0x04)
    mutable long RefCount;                                // +0x04
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CThingDef;                                         // no engine header
template <class T> struct CDefPointer {                   // CDefPointer<const T>: the pointee seen through its counted base
    CDefPointeeBase* Object;                              //   (the engine headers' `CDefPointeeBase* PDef_Object` convention)
    void Clear()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};
class CTCInventoryItem {
public:
    static void __fastcall ClearCachedDefs();
    static long                                  s_CachedDefIndex;      // 0x013B8BC8
    static CDefPointer<const CThingDef>          s_CachedThingDef;      // 0x013B8C14
    static CDefPointer<const CInventoryItemDef>  s_CachedItemDef;       // 0x013B8C18
};
void __fastcall CTCInventoryItem::ClearCachedDefs()
{
    s_CachedDefIndex = 0;
    s_CachedThingDef.Clear();
    s_CachedItemDef.Clear();
}