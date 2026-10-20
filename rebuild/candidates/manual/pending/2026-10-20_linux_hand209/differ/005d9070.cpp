// Grade: DIFFER(ours v retail) -- 172v172: scheduling/register only -- retail keeps the text id in edx across the global's load and the bank deref (mov edx,[esi+34h] ; mov ecx,[g] ; mov ecx,[ecx+14h] ; push edi ; push 1 ; push edx ; lea eax ; push eax), the lane loads it into eax and pushes it straight (push eax ; lea edx ; push edx) with the global read before the pushes
// CTCInventoryItem::GetDescription() -- the wide description text of the item's inventory def (its ItemDescription text id looked up in the main component's text bank), or the empty wide string when the def has no description
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9070, length 172 bytes; speed TU (no frame pointer, `mov dword ptr [..],0`).
// Callees: NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 on g_pMainGameComponent(0x013B86A0)->TextBank (+0x14)
// (CIVCountedPointer<const CGroupEntry> by hidden pointer), NGameText::CGroupEntry::GetText() const @ 0x009C7A40 (CWideString by hidden
// pointer -- the function's own result pointer is passed straight through), CWideString::CWideString(const CWideString&) @ 0x0099B720 on
// the empty wide string 0x013BCA24, the defs' virtual slot 1 on release.
// Layout: engine/CTCInventoryItem.h PDef_Object +0x0C; CInventoryItemDef::ItemDescription at +0x34 (unsigned long, PDB offsets).
// Shape: the text id is a named local (loaded into edx before the bank chain, which VC evaluates after the arguments); the def is read twice through an inline by-value accessor (two add-ref/release pairs; the first release load/dec/stores the
// count around the `seta bl` test of the UNSIGNED text id, the second is `dec [esi+4] ; jne`), the counted entry temporary is released
// with the inlined DecRef/GetRefCount pair and nulled afterwards (the store survives because the def release follows).
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
};
extern CWideString g_emptyWideString;              // 0x013BCA24
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryItemDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    unsigned long ItemDescription;                 // +0x34
};
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> struct CIVCountedPointer {      // 4 bytes
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
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
struct CTCInventoryItem {
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
        unsigned long textIndex = def->ItemDescription;
        return bank->GetPGroupEntry(textIndex, true).Object->GetText();
    }
    return g_emptyWideString;
}
