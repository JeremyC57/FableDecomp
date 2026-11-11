// NInventory::CItem::GetInventoryItemDef() const -- the counted CInventoryItemDef of a non-empty slot: the item type's def from the global definition manager, narrowed with GetSubDef; a null pointer for an empty slot
// Leaf: ?GetInventoryItemDef@CItem@NInventory@@ABE?AV?$CDefPointer@$$CBVCInventoryItemDef@@@@XZ
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD82E, length 114 bytes.
// Callees: CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const @ 0x0042AF3C (the manager at
// CMainGameComponent::Get()(0x013B86A0) +0x24 -> +0x10), CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const ..>*) const @ 0x005BADE1
// (both landed); the defs' virtual slot 1 (Delete) on the last release.
// Model: NInventory::CItem over engine/CItem.h (ItemType +0, NumberOfItems +4; the landed 0x005BD931 pattern); the counted defs as the landed CItem::GetMaximumNumberOfItems
// 0x005BD931 (vptr, IVRefCount +4). Pragmas: the frame-pointer size TU of the CItem module (`optimize("y", off)` +
// `("s", on)`).
// Shape: both def pointers are zeroed frame words (`and [ebp-4],0`); `CDefPtr sub; ...; return sub;` copies out with the
// merged `inc ; dec ; jne` (VC7.1 has no NRVO); the base def is read once into esi and released by hand after the copy.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
class CDefClassBase;
template <class T> class CDefPointer { public:      // 4 bytes: the counted def pointer (the pointee as its counted base)
    CIVCountedPointeeBase* Object;
    FABLE_FORCEINLINE CDefPointer() : Object(0) {}
    FABLE_FORCEINLINE CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CIVCountedPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005BADE1 GetSubDef<CInventoryItemDef> (Ego_r / FableWin template)
};
class CInventoryItemDef : public CDefClassBase { public:
};
class CThingBaseDef : public CDefClassBase { public:
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CGameComponentDefs {
    unsigned char       _pad00[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char       _pad00[0x24];
    CGameComponentDefs* Defs;                      // +0x24
};
namespace NInventory {
class CItem : public ::CItem {                     // NInventory::CItem (Ego_r.pdb / FableWin.pdb) over the shared engine/CItem.h layout
private:
    CDefPointer<const CInventoryItemDef> GetInventoryItemDef() const;   // Ego_r.pdb ?GetInventoryItemDef@CItem@NInventory@@ABE?AV?$CDefPointer@$$CBVCInventoryItemDef@@@@XZ
};
}

CDefPointer<const CInventoryItemDef> NInventory::CItem::GetInventoryItemDef() const
{
    if (NumberOfItems > 0) {
        CDefPointer<const CThingBaseDef> def;
        CMainGameComponent::Get()->Defs->DefinitionManager->GetDef(ItemType, &def);
        CDefPointer<const CInventoryItemDef> sub;
        def->GetSubDef(&sub);
        return sub;
    }
    return CDefPointer<const CInventoryItemDef>();
}
