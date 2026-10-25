// CItemList::~CItemList() -- EH-framed: own vtable, the CListImpl member at +0x8 destroyed out of line, inlined empty CTypedListBase destructor
// Leaf: ??1CItemList@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C09B70, oracle length 73 bytes. Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as CVtable_012b1b78::~CVtable_012b1b78() over CBase_012b1ab0 /
//   CMember_00c17b10; descriptive (no PDB covers this library). FINDINGS chunk 431 asks for one name set for this typed
//   list: the landed CItemDispatcher::RunAllItems 0x00C10400 names it CItemList : CTypedListBase (vtable 0x012B1B78
//   over 0x012B1AB0) with the CListImpl member (destructor 0x00C17B10); the constructor 0x00C0A610 is renamed alike.
//   Callers: 0x00C09DC3 and three EH unwind funclets (0x0121136B / 0x01211395 / 0x012113BF).
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset CItemList::`vftable' ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; call CListImpl::~CListImpl ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CTypedListBase::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Plain destructor: installs this class's vtable (0x012B1B78), destroys the member at +0x8 through its out-of-line
// destructor @ 0x00C17B10 under EH state 0, then the inline empty base destructor stores the base vtable (0x012B1AB0).
// Non-virtual destructors keep the lander on `??1`.
struct CListImpl {
    void* m_data;
    ~CListImpl();                                    // 0x00C17B10
};

struct CTypedListBase {                              // vtable 0x012B1AB0
    virtual void Slot0();
    ~CTypedListBase() { }
};

struct CItemList : CTypedListBase {                  // vtable 0x012B1B78
    CListImpl* m_pImpl;                              // +0x4 (the base's impl pointer)
    CListImpl m_impl;                                // +0x8
    virtual void Slot0();
    ~CItemList();
};

CItemList::~CItemList()
{
}
