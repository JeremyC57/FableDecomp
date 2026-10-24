// CTCInventoryItem::GetContextSensitiveType(long defIndex) -- static: cache the defs for defIndex and return the Type of the thing def's CContextSensitiveItemDef sub-definition, or 0 when the def cannot be cached or has no such sub-def
// Leaf: ?GetContextSensitiveType@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D9B90, 81 bytes (+15 int3). Ego_r.pdb `?GetContextSensitiveType@CTCInventoryItem@@SI?AW4EContextSensitiveType@@J@Z`
// (static __fastcall, defIndex in ecx straight on to CacheDef; `ret`).
// Callees: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool),
// CDefClassBase::GetSubDef<CContextSensitiveItemDef>(CDefPointer<const CContextSensitiveItemDef>*) const @ 0x005933C8
// on the cached thing def (global 0x013B8C14), the sub-def's virtual slot 1 on the last release.
// Layout: engine/CContextSensitiveItemDef.h (Type +0x28, an EContextSensitiveType kept as bytes in the header).
// Shape: speed TU; the out slot is the `push ecx` word, zeroed after its address is pushed; the hit path reads Type
// (esi) between the count's load and its decremented store, the destructor's null test dropped after the dereference; the
// miss path releases under the null test (`dec [ecx+4]`) and returns 0.
#include "engine/CContextSensitiveItemDef.h"
enum EContextSensitiveType { };                         // enumerator names not recovered
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
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
    ~CDefPointer() { if (Object) Object->Release(); }
    const T* operator->() const { return reinterpret_cast<const T*>(Object); }
};
class CDefClassBase : public CDefPointeeBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005933C8 for CContextSensitiveItemDef
};
class CThingDef : public CDefClassBase { };
const CDefPointeeBase* FindDef_5d9b90(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
// Same-TU body (retail instantiates the template in this TU): it only fills the caller's out slot.
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = FindDef_5d9b90(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
template <class T> struct CThingDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static bool __fastcall CacheDef(long defIndex);                     // 0x005D94B0
    static EContextSensitiveType __fastcall GetContextSensitiveType(long defIndex);
    static CThingDefPointer<const CThingDef> s_CachedThingDef;        // 0x013B8C14
};
EContextSensitiveType __fastcall CTCInventoryItem::GetContextSensitiveType(long defIndex)
{
    if (CacheDef(defIndex)) {
        CDefPointer<const CContextSensitiveItemDef> sub;
        if (s_CachedThingDef->GetSubDef(&sub))
            return reinterpret_cast<const EContextSensitiveType&>(sub->Type);
    }
    return EContextSensitiveType(0);
}