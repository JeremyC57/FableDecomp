// CTCInventoryBase::GetInventoryText(long textIndex) -- the wide text of game-text entry `textIndex` (looked up in the main component's text bank), or the empty wide string when the index is not positive or the bank has no text entry for it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD273, length 181 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: NGameText::CDataBank::GetTextInfo(unsigned long) @ 0x009C7D00, NGameText::CDataBank::GetPGroupEntry(unsigned long, bool)
// const @ 0x009C9070 (CIVCountedPointer<const CGroupEntry> by hidden pointer), NGameText::CGroupEntry::GetText() const @ 0x009C7A40
// (CWideString by hidden pointer -- the function's own result pointer is passed straight through), CWideString::CWideString(const
// CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24; the bank is g_pMainGameComponent(0x013B86A0)->TextBank (+0x14).
// Shape: the result is a CWideString by value (hidden pointer at [ebp+8], `ret 8`); `this` is unused. The presence test is a named
// bool built from an `&&` over a counted temporary: its construction flag is the zeroed frame word [ebp-4] (`and [ebp-4],0` size
// store), the bool lands in the dead high byte of the index slot, the temporary in [ebp-8] and its release is the inlined long form
// (`mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`) of the CItem module. The second temporary is homed in the dead index
// slot ([ebp+0Ch], the index itself lives in edi).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
};
extern CWideString g_emptyWideString;              // 0x013BCA24
struct CIVRefCounted {                             // the count sits at +4 behind the vptr
    unsigned long IVRefCount;
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
};
struct CIVVirtual {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
};
struct CIVCountedPointeeBase : CIVVirtual, CIVRefCounted {
    void ReleaseTemporary()                        // inlined into ~CIVCountedPointer: the decrement, then the re-read test
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
struct CDataBankEntry;
struct CGroupEntry : CIVCountedPointeeBase {       // NGameText::CGroupEntry
    CWideString GetText() const;                   // 0x009C7A40
};
struct CDataBank {                                 // NGameText::CDataBank
    CDataBankEntry* GetTextInfo(unsigned long index) const;                                // 0x009C7D00
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                        // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

namespace NInventory {
struct CTCInventoryBase {
    CWideString GetInventoryText(long textIndex);
};
CWideString CTCInventoryBase::GetInventoryText(long textIndex)
{
    if (textIndex > 0) {
        CDataBank* bank = g_pMainGameComponent->TextBank;
        bool found = bank->GetTextInfo(textIndex) != 0 && bank->GetPGroupEntry(textIndex, true).Object != 0;
        if (found)
            return bank->GetPGroupEntry(textIndex, true).Object->GetText();
    }
    return g_emptyWideString;
}
}
