// CPair_00c387e0::~CPair_00c387e0() -- EH-framed empty destructor: member at +0x1c (@ 0x00C29FB0) then base @ 0x00C1C820
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C387E0, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+0x1c] ; mov dword ptr [esp+10h],0 ; call ~member ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~base ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). Non-polymorphic aggregate (no vtable stores): the
// member at +0x1c is destroyed out of line @ 0x00C29FB0 under EH state 0, then the base
// destructor @ 0x00C1C820 runs under state -1.
// The manifest label (`_Dest_val<...pair<CCharString,CScriptInfo>>`) names a map pair; classes are
// placeholders named by their destructor addresses. One of 9 bodies.
struct CBase_00c1c820 {
    void* m_data;
    ~CBase_00c1c820();                  // 0x00C1C820
};

struct CMember_00c29fb0 {
    void* m_data;
    ~CMember_00c29fb0();                // 0x00C29FB0
};

struct CPair_00c387e0 : CBase_00c1c820 {
    unsigned char _pad[0x18];
    CMember_00c29fb0 m_member;          // +0x1c
    ~CPair_00c387e0();
};

CPair_00c387e0::~CPair_00c387e0()
{
}
