// CScriptEntry_00c1afd0::CScriptEntry_00c1afd0(void*, void*) -- EH-framed: base @ 0x00C1C810, first argument at +0x18, member at +0x1c constructed from (0, second argument)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AFD0, oracle length 84 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; mov ecx,[esp+1Ch] ; mov eax,[esp+18h] ; push ecx ; push 0 ; lea ecx,[esi+0x1c] ; mov dword ptr [esp+18h],0 ; mov [esi+0x18],eax ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Built with C++ exceptions enabled (/EHsc). The out-of-line base constructor @ 0x00C1C810 runs
// first (its destructor gives EH state 0), the first argument is stored at +0x18 and the
// member at +0x1c is constructed out of line @ 0x00C29FC0 from (0, second argument). No
// vtables are stored. Manifest labels here are propagated; classes are placeholders. One of 10 bodies.
struct CBase_00c1c810 {
    void* m_data;
    CBase_00c1c810();                    // 0x00C1C810
    ~CBase_00c1c810();
};

struct CMember_00c29fc0 {
    void* m_data;
    CMember_00c29fc0(long zero, void* value);   // 0x00C29FC0
};

struct CScriptEntry_00c1afd0 : CBase_00c1c810 {
    unsigned char _pad[0x14];
    void* m_pFirst;                        // +0x18
    CMember_00c29fc0 m_member;          // +0x1c
    CScriptEntry_00c1afd0(void* first, void* second);
};

CScriptEntry_00c1afd0::CScriptEntry_00c1afd0(void* first, void* second)
    : CBase_00c1c810()
    , m_pFirst(first)
    , m_member(0, second)
{
}