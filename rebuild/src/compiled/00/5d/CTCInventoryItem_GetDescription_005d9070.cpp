// Retry 10b (B-12 sweep): the text id is a const REFERENCE to the def's member (`const unsigned long& textIndex = def->ItemDescription;`), which keeps it in edx across the global's load and the bank deref (retail order); a value local is loaded after the bank chain.
// CTCInventoryItem::GetDescription() -- the wide description text of the item's inventory def (its ItemDescription text id looked up in the main component's text bank), or the empty wide string when the def has no description
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9070, length 172 bytes; speed TU (no frame pointer, `mov dword ptr [..],0`).
// Callees: NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 on g_pMainGameComponent(0x013B86A0)->TextBank (+0x14)
// (CIVCountedPointer<const CGroupEntry> by hidden pointer), NGameText::CGroupEntry::GetText() const @ 0x009C7A40 (CWideString by hidden
// pointer -- the function's own result pointer is passed straight through), CWideString::CWideString(const CWideString&) @ 0x0099B720 on
// the empty wide string 0x013BCA24, the defs' virtual slot 1 on release.
// Layout: engine/CTCInventoryItem.h PDef_Object +0x0C; CInventoryItemDef::ItemDescription at +0x34 (unsigned long, PDB offsets).
// Shape: the text id is a const-reference local (loaded into edx before the bank chain, which VC evaluates after the arguments); the def is read twice through an inline by-value accessor (two add-ref/release pairs; the first release load/dec/stores the
// count around the `seta bl` test of the UNSIGNED text id, the second is `dec [esi+4] ; jne`), the counted entry temporary is released
// with the inlined DecRef/GetRefCount pair and nulled afterwards (the store survives because the def release follows).
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
};
extern CWideString g_emptyWideString;              // 0x013BCA24
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CInventoryItemDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    unsigned long ItemDescription;                 // +0x34
};
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) const;               // slot 0
    virtual void Delete() const;                         // slot 1
    mutable unsigned long IVRefCount;                      // +4
    void DecRef() const { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:      // 4 bytes
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
class CTextEntry : public CIVCountedPointeeBase { public:       // NGameText::CGroupEntry
    CWideString GetText() const;                   // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:                                 // NGameText::CDataBank
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                        // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CTCInventoryItem { public:
    void*   __vftable;                             // +0x00
    void*   ParentThing;                           // +0x04
    long    field8;                                // +0x08
    CDefPointer<CInventoryItemDef> PDef;           // +0x0c
    CDefPointer<CInventoryItemDef> GetPDef() const { return PDef; }
    CWideString GetDescription();
};
CWideString CTCInventoryItem::GetDescription()
{
    bool hasDescription = GetPDef()->ItemDescription > 0;
    if (hasDescription) {
        CDefPointer<CInventoryItemDef> def(PDef);
        CDataBank* bank = g_pMainGameComponent->TextBank;
        const unsigned long& textIndex = def->ItemDescription;
        return bank->GetPTextEntry(textIndex, true).Object->GetText();
    }
    return g_emptyWideString;
}