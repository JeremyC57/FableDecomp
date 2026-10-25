// CHashedItem::CHashedItem(unsigned long hash) -- EH-framed: vtable 0x012B3474, the hash kept at +0x4, the 8-byte member at +0x8 default-constructed out of line
// Leaf: ??0CHashedItem@@QAE@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1F640, oracle length 78 bytes (`ret 4`). Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as CVtable_012b3474::CVtable_012b3474(void*); descriptive (no PDB covers this
//   library). Its callers are CItemAction::CItemAction 0x00C1EEE0 (landed: `new (0x10) CHashedItem(name.GetHash())`,
//   push GetHash 0x00C0DBB0's result, ecx = new block; the source declares CHashedItem(unsigned long hash) 0x00C1F640)
//   and 0x00C1F07C. The named item 0x00C20230 (CNamedItem, 0x14 bytes) builds this same base part inline (vtable
//   0x012B3474, hash +4, member +8 via 0x00C35170) before its own vtable 0x012B3600.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esp+18h] ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset CHashedItem::`vftable' ; mov [esi+0x4],eax ; call CMember_00c35170::CMember_00c35170 ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The item root has an empty inline destructor (no code, but EH state 0 once it is constructed); this class installs
// its vtable (0x012B3474), stores the hash at +0x4 and default-constructs the member at +0x8 out of line @ 0x00C35170
// (labelled CBaseIntelligentPointer; the landed relocation name CMember_00c35170 is kept, since a game-side
// constructor 0x00A01B10 also emits that name).
struct CItem {
    virtual void v00();
    CItem() { }
    ~CItem() { }
};

struct CMember_00c35170 {
    void* m_data;
    CMember_00c35170();                    // 0x00C35170
    ~CMember_00c35170();
};

struct CHashedItem : CItem {
    unsigned long m_hash;                  // +0x4
    CMember_00c35170 m_member;             // +0x8
    virtual void v00();
    CHashedItem(unsigned long hash);
};

CHashedItem::CHashedItem(unsigned long hash)
    : CItem()
    , m_hash(hash)
    , m_member()
{
}
