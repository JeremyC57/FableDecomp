// CItemList::CItemList(long mode) -- EH-framed: the typed-list base's impl pointer at +0x4 aimed at the CListImpl member at +0x8, which is constructed from the argument
// Leaf: ??0CItemList@@QAE@J@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A610, oracle length 79 bytes. Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as CVtable_012b1b78::CVtable_012b1b78(void*) over CVtable_012b1b78_Base /
//   CMember_00c185b0; descriptive (no PDB covers this library). FINDINGS chunk 431 asks for one name set for this typed
//   list: the landed CItemDispatcher::RunAllItems 0x00C10400 builds the same list inline (vtable 0x012B1B78 over the
//   base 0x012B1AB0, impl constructor 0x00C185B0 with argument 1, impl destructor 0x00C17B10) and names it CItemList
//   : CTypedListBase { CListImpl m_impl; }, which this source now uses (the destructor 0x00C09B70 is renamed alike).
//   This out-of-line copy has no code xrefs.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; mov [esp+4],esi ; mov [esi+0x4],ecx ; mov eax,[esp+18h] ; push eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset CItemList::`vftable' ; call CListImpl::CListImpl ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The impl pointer store precedes the vtable store, so it belongs to the inlined base constructor that receives the
// member's address; the base's empty inline destructor emits no code but gives the frame its EH state 0 (the base
// subobject is destroyed if the impl constructor throws); the base's own vtable store is dead and dropped.
struct CListImpl {                                   // the list's implementation object (+0x8 of the typed list)
    void* m_data;
    CListImpl(long mode);                            // 0x00C185B0
    ~CListImpl();                                    // 0x00C17B10
};

struct CTypedListBase {                              // vtable 0x012B1AB0
    virtual void Slot0();
    CListImpl* m_pImpl;                              // +0x4: the base is handed the derived member's address
    CTypedListBase(CListImpl* impl) : m_pImpl(impl) { }
    ~CTypedListBase() { }
};

struct CItemList : CTypedListBase {                  // vtable 0x012B1B78
    CListImpl m_impl;                                // +0x8
    virtual void Slot0();
    CItemList(long mode);
};

CItemList::CItemList(long mode)
    : CTypedListBase(&m_impl)
    , m_impl(mode)
{
}
