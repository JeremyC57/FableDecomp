// CProfile::CProfile() -- EH-framed constructor: novtable destructible root (EH state 0), own vtable (0x012B4FBC), the member at +4 built out of line (@ 0x00C42A60)
// Leaf: ??0CProfile@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34AA0, oracle length 69 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+4] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 is entered before the first member exists, so the root base is already destructible:
// a polymorphic `novtable` root with an out-of-line destructor (no vtable store of its own). The
// class comes from the manifest label; the root and member are descriptive.
struct __declspec(novtable) CProfileRoot {
    virtual void v00();
    ~CProfileRoot();
};

struct CProfileMember {                    // +4
    unsigned char _body[4];
    CProfileMember();                      // 0x00C42A60
    ~CProfileMember();
};

struct CProfile : CProfileRoot {           // vtable 0x012B4FBC
    CProfileMember m_member;               // +4
    CProfile();
};

CProfile::CProfile()
    : m_member()
{
}