// CTCInventoryItem::GetHairCardDef(long defIndex) -- static: fetch the thing def for defIndex from the game definition manager and return its CHairCardDef sub-definition as a counted pointer (null when either lookup fails)
// Leaf: ?GetHairCardDef@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D9A10, 137 bytes (+7 int3). Ego_r.pdb `?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z`
// (static __fastcall: the hidden CDefPointer result in ecx, defIndex in edx; `ret`). Twin: GetTattooCardDef 0x005D9AA0.
// Instance twin: GetHairCardDef() const 0x005D9430 (the owner thing's def instead of a manager lookup).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static `__fastcall`, Ego_r `SIAAV1@XZ`),
// CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const @ 0x0042AF3C (the folded instantiation shared by the
// thing-def lookups; T taken as CThingDef), CDefClassBase::GetSubDef<CHairCardDef>(CDefPointer<const CHairCardDef>*) const
// @ 0x005DA180 (landed), the defs' virtual slot 1 on the last release.
// Shape: speed TU; GetDef's arguments are pushed before the manager is fetched; the thing-def slot (one `push ecx` word)
// is read into esi after GetDef and reused for the sub-def out slot; `return out` folds the result's add-ref with the
// local's release (`inc edx ; mov eax,edx ; dec eax`), the miss paths store a null result, the thing def is released last.
// The slot reuse and the fold need the same-TU out-slot-only bodies of GetDef/GetSubDef (VC7.1 then knows the callees
// keep no pointer to the slots); with the two templates only declared it keeps two slots and grades 166v137.
#include "engine/CHairCardDef.h"
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void AddRef() const { ++RefCount; }
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer {                  // 4 bytes; the pointee seen through its counted base
public:
    const CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
};
class CDefClassBase : public CDefPointeeBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005DA180 for CHairCardDef
};
class CThingDef : public CDefClassBase { };
class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
const CDefPointeeBase* FindDef_5d9a10(const void* owner, long index);   // stand-in callee of the stand-in bodies below
#pragma auto_inline(off)
// Same-TU bodies (retail instantiates both templates in this TU): each only fills the caller's out slot.
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    out->Object = FindDef_5d9a10(this, index);
    return out->Object != 0;
}
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = FindDef_5d9a10(this, 0);
    return out->Object != 0;
}
#pragma auto_inline(on)
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static CDefPointer<const CHairCardDef> __fastcall GetHairCardDef(long defIndex);
};
CDefPointer<const CHairCardDef> __fastcall CTCInventoryItem::GetHairCardDef(long defIndex)
{
    CDefPointer<const CThingDef> thingDef;
    if (CGameDefinitionManager::Get().GetDef(defIndex, &thingDef)) {
        CDefPointer<const CHairCardDef> out;
        if (static_cast<const CDefClassBase*>(thingDef.Object)->GetSubDef(&out))
            return out;
    }
    return CDefPointer<const CHairCardDef>();
}