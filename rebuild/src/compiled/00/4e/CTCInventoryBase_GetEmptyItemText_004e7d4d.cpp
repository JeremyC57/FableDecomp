// CTCInventoryBase::GetEmptyItemText(long itemType) -- base implementation of virtual slot 76: returns a copy of the shared empty CWideString whatever the argument
// Leaf: GetEmptyItemText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7D4D, length 21 bytes; size TU. The catalog label "CTCInventoryClothing::vfunc_76"
// is a slot name: the body sits in slot 76 of ten inventory-screen vtables, NInventory::CTCInventoryBase's own (0x01253DBC + 0x130)
// among them, so it is the base class's default. The real method name is unknown; the leaf name is descriptive.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 on the global empty CWideString @ 0x013BCA24 (default-
// constructed by its own dynamic initialiser).
// Shape: a by-value class result: the hidden result pointer is the first stack argument (`mov ecx,[esp+4]`), re-read for the
// return value (`mov eax,[esp+4]`), `ret 8` for it plus the unused argument; `this` is never read.
#pragma optimize("s", on)
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);                       // 0x0099B720
    ~CWideString();                                              // 0x0099B510
};
extern CWideString g_EmptyWideString;                            // 0x013BCA24
struct CTCInventoryBase_Retail {
    CWideString GetEmptyItemText(long itemType);
};
CWideString CTCInventoryBase_Retail::GetEmptyItemText(long itemType)
{
    return g_EmptyWideString;
}