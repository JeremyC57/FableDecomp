// CAIStateGroupBase::CAIStateGroupBase() -- EH-framed constructor: out-of-line base constructor (@ 0x00C5B1A0), own vtable, the member at +0x1f0 built out of line (@ 0x00C5D0A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F760, oracle length 77 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; lea ecx,[esi+1F0h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The polymorphic base is constructed out of line on `this` (its destructor is non-trivial: EH
// state 0 once it exists), the derived vtable (0x012B5D70) is stored and the member at +0x1f0 is
// default-constructed out of line. `this` is kept in the frame for the unwind funclet. The class
// comes from the manifest label; the base and member are descriptive.
struct CAIStateGroupRoot {                 // out-of-line constructor and destructor
    virtual void v00();
    unsigned char _body[0x1ec];
    CAIStateGroupRoot();                   // 0x00C5B1A0
    ~CAIStateGroupRoot();
};

struct CAIStateGroupMember {               // +0x1f0
    unsigned char _body[4];
    CAIStateGroupMember();                 // 0x00C5D0A0
    ~CAIStateGroupMember();
};

struct CAIStateGroupBase : CAIStateGroupRoot {   // vtable 0x012B5D70
    CAIStateGroupMember m_member;          // +0x1f0
    CAIStateGroupBase();
};

CAIStateGroupBase::CAIStateGroupBase()
    : m_member()
{
}