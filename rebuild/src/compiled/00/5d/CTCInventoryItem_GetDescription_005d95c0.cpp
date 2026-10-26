// CTCInventoryItem::GetDescription(long defIndex) -- static: cache the item def for defIndex and return its description text (ItemDescription looked up in the main component's text bank), or the empty wide string when the def is missing or has no description
// Leaf: ?GetDescription@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D95C0, 112 bytes (+2 int3). Ego_r.pdb `?GetDescription@CTCInventoryItem@@SI?AVCWideString@@J@Z`
// (static __fastcall: the hidden CWideString result in ecx, defIndex in edx; `ret`). Twin: GetDetails 0x005D9630 (+0x38).
// Callees: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool: loads the thing def and its
// CInventoryItemDef into the static cache), NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 on
// g_pMainGameComponent (0x013B86A0)->TextBank (+0x14) (a CIVCountedPointer<const CGroupEntry> by hidden pointer, in the
// `push ecx` frame word), NGameText::CGroupEntry::GetText() const @ 0x009C7A40 (straight into this function's result),
// CWideString::CWideString(const CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24, the entry's slot 1 on release.
// Globals: 0x013B8C18 the cached CInventoryItemDef (engine/CInventoryItemDef.h: ItemDescription +0x34, ItemDetails +0x38).
// Shape: speed TU; the cached def is reached through CDefPointer::operator-> at both uses, so the global load is shared
// (eax) but the ItemDescription field is loaded twice (`test ecx,ecx`/`jbe`, then `mov edx,[eax+34h]`) -- a plain
// `def` local or a bool flag CSEs the field into one load (107v110). The entry temporary's release is the
// DecRef()/GetRefCount() pair (load/dec/store, `test edx,edx`), its dead null store dropped.
#include "engine/CInventoryItemDef.h"
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
};
extern CWideString g_emptyWideString;                   // 0x013BCA24
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) const;                    // slot 0
    virtual void Delete() const;                              // slot 1
    mutable unsigned long IVRefCount;                           // +0x04
    void DecRef() const { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:           // 4 bytes
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public:            // NGameText::CGroupEntry
    CWideString GetText() const;                        // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:                                      // NGameText::CDataBank
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CDefPointeeBase;
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static bool __fastcall CacheDef(long defIndex);                     // 0x005D94B0
    static CWideString __fastcall GetDescription(long defIndex);
    static CDefPointer<const CInventoryItemDef> s_CachedItemDef;      // 0x013B8C18
};
CWideString __fastcall CTCInventoryItem::GetDescription(long defIndex)
{
    if (CacheDef(defIndex)) {
        if (s_CachedItemDef->ItemDescription > 0) {
            CDataBank* bank = g_pMainGameComponent->TextBank;
            const unsigned long& textIndex = s_CachedItemDef->ItemDescription;
            return bank->GetPTextEntry(textIndex, true).Object->GetText();
        }
    }
    return g_emptyWideString;
}