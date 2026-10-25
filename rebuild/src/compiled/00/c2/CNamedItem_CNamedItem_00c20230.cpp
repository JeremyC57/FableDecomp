// CNamedItem::CNamedItem(const CString& name, unsigned long hash) -- EH-framed constructor: the item root (EH state 0), the hashed-item base (vtable 0x012B3474: hash at +4, an 8-byte member at +8 built out of line @ 0x00C35170), then this class's vtable (0x012B3600) and the name copy at +0x10 (CString copy constructor @ 0x00C0DAB0, state 1)
// Leaf: ??0CNamedItem@@QAE@ABVCString@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20230, oracle length 105 bytes (`ret 8`). Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as _unattributed::CIntelligentPointer_00c20230(void* source, void* item);
//   descriptive (no PDB covers this library). Its two callers are CItemAction::CItemAction 0x00C1EEE0 (landed), which
//   runs `new (0x14) CNamedItem(name, name.GetHash())` (push GetHash 0x00C0DBB0 result, push &name, ecx = new block)
//   and declares this constructor as CNamedItem(const CString& name, unsigned long hash), and 0x00C1F046. The same
//   caller builds the 0x10-byte hashed item through 0x00C1F640, which stores vtable 0x012B3474 / hash +4 / the +8
//   member exactly as this body's base part does. 0x00C0DAB0 is the landed CString::CString(const CString&).
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[hash] ; lea ecx,[esi+8] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset base vtable ; mov [esi+4],eax ; call member8 ctor ; mov ecx,[name] ; push ecx ; lea ecx,[esi+0x10] ; mov dword ptr [esp+14h],1 ; mov dword ptr [esi],offset vtable ; call CString copy ctor ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Two-level EH-framed constructor: the item root (a polymorphic root at +0 with an out-of-line destructor and no
// vtable store of its own (`novtable`), which is what puts the frame in EH state 0 before any member exists -- its
// funclet destroys it on `this` with no offset); the hashed-item base, whose own destructor is out of line too, so
// state 1's funclet destroys the whole base through it rather than the member; and the class itself (vtable,
// state 1, the name constructed from the first argument). `this` is kept in the frame for the unwind funclets.
class CString {
public:
    char* m_pchData;                       // +0
    CString(const CString& stringSrc);     // 0x00C0DAB0
    ~CString();                            // 0x00C0BA40
};

namespace N_00c35170 { struct CMember8 {  // 8 bytes, default-constructed out of line (the landed 0x00C35170 is labelled
    unsigned char _body[0x8];              // CBaseIntelligentPointer::CBaseIntelligentPointer, a name a game-side
    CMember8();                            // 0x00C35170 constructor (0x00A01B10) also emits, so the landed
    ~CMember8();                           // relocation name is kept)
}; } using N_00c35170::CMember8;

struct __declspec(novtable) CItem {        // polymorphic root at +0 (no vtable store of its own) with an out-of-line
    virtual void v00();                    // destructor: EH state 0 while the first member is built; its funclet
    ~CItem();                              // destroys it on `this` (+0), which is why the root owns the vptr
};

struct CHashedItem : CItem {               // 0x10 bytes, vtable 0x012B3474
    unsigned long m_hash;                  // +4
    CMember8 m_pointer;                    // +8
    CHashedItem(unsigned long hash) : m_hash(hash), m_pointer() { }
    ~CHashedItem();                        // out of line: the unwind funclet for state 1 destroys the whole base through it
};

struct CNamedItem : CHashedItem {          // 0x14 bytes, vtable 0x012B3600
    CString m_name;                        // +0x10
    CNamedItem(const CString& name, unsigned long hash);
};

CNamedItem::CNamedItem(const CString& name, unsigned long hash)
    : CHashedItem(hash), m_name(name)
{
}
