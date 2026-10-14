// SPair_00c2e140::~SPair_00c2e140() -- EH-framed destructor of a map node value: the second at +0x3c (@ 0x00C3B1D0), then the first's member at +0xc (@ 0x00C3AC40, the first's own destructor is implicit and inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E140, oracle length 77 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+3Ch] ; mov dword ptr [esp+10h],0 ; call ~CSecond ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+10h],-1 ; call ~CFirstMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The manifest label is Dinkumware's `_Dest_val<allocator<pair<CCharString,CScriptInfo>>>`, i.e.
// the out-of-line destructor of a map node value (a different instantiation from 0x00C1ADB0: here
// the first is a 0x3c-byte object whose only non-trivial part is a member at +0xc). Members are
// destroyed in reverse order: the second at +0x3c (EH state 0 while the first is alive), then the
// first's member at +0xc (state -1). The first is modelled flat (its head bytes and the member as
// pair members) because the state-0 unwind funclet jumps straight to the member's destructor; a
// first class with an implicit destructor would route the funclet through an out-of-line `??1`.
// The user-declared pair destructor is the landed unit.
struct CFirstMember_00c2e140 {             // +0xc inside the first: the only non-trivially destructible part
    unsigned char _body[0x30];
    ~CFirstMember_00c2e140();              // 0x00C3AC40
};

struct CSecond_00c2e140 {                  // +0x3c
    unsigned char _body[4];
    ~CSecond_00c2e140();                   // 0x00C3B1D0
};

struct SPair_00c2e140 {
    unsigned char first_head[0xc];         // +0: the trivially destructible part of the first
    CFirstMember_00c2e140 first_member;    // +0xc: the first's only non-trivial part, destroyed directly (its funclet jumps straight to the member destructor)
    CSecond_00c2e140 second;               // +0x3c
    ~SPair_00c2e140();
};

SPair_00c2e140::~SPair_00c2e140()
{
}
