// CItem::GetItemDescription() const -- the wide description text of a non-empty slot's item (the inventory def's text index at +0x34, looked up in the game text bank); the empty wide string for an empty slot or a def without text
// Leaf: GetItemDescription
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD8A0, length 145 bytes.
// Callees: CItem::GetInventoryItemDef @ 0x005BD82E (counted def by value), NGameText::CDataBank::GetPGroupEntry(unsigned long,
// bool) @ 0x009C9070 on the bank at g_pMainGameComponent(0x013B86A0) +0x14 (CIVCountedPointer<const CGroupEntry> by value),
// CGroupEntry's text getter @ 0x009C7A40 (`return m_text;` at +0x28; manifest label "CBankFile::GetBankHandle" is
// propagated), CWideString::CWideString(const CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24.
// Model: engine/CItem.h; the counted temporaries and their inlined release (`mov ecx,eax ; add eax,4 ; dec [eax] ;
// cmp [eax],0`) exactly as the landed CItem::GetMaximumNumberOfItems 0x005BD931; both temporaries share the frame word
// [ebp-4]; the text bank is read into a local before the argument pushes. Pragmas: the CItem module's frame-pointer size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
};
extern CWideString g_emptyWideString;              // 0x013BCA24
struct CIVCountedPointeeVirtual {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
};
struct CIVRefCounted {                             // second base: the count at +4 behind the vptr
    unsigned long IVRefCount;
    __forceinline void DecRef() { --IVRefCount; }
    __forceinline unsigned long GetRefCount() const { return IVRefCount; }
};
struct CIVCountedPointeeBase : CIVCountedPointeeVirtual, CIVRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> struct CIVCountedPointer {      // 4 bytes
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CInventoryItemDef_Retail : CIVCountedPointeeBase {
    unsigned char _pad[0x34 - 8];
    long          DescriptionTextIndex;            // +0x34
};
struct CGroupEntry : CIVCountedPointeeBase {       // NGameText::CGroupEntry
    CWideString GetText() const;                   // 0x009C7A40
};
struct CDataBank {                                 // NGameText::CDataBank
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                        // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CItem_Methods : CItem {
    CIVCountedPointer<CInventoryItemDef_Retail> GetInventoryItemDef() const;   // 0x005BD82E
    CWideString GetItemDescription() const;
};

CWideString CItem_Methods::GetItemDescription() const
{
    if (NumberOfItems == 0)
        return g_emptyWideString;
    long textIndex = GetInventoryItemDef().Object->DescriptionTextIndex;
    if (textIndex > 0) {
        CDataBank* bank = g_pMainGameComponent->TextBank;
        return bank->GetPGroupEntry(textIndex, true).Object->GetText();
    }
    return g_emptyWideString;
}