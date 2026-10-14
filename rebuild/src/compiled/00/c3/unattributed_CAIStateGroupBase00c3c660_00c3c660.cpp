// CAIStateGroupBase_00c3c660::CAIStateGroupBase_00c3c660() -- EH-framed constructor: out-of-line base constructor (@ 0x00C3C3B0), own vtable, the member at +0x30 built out of line (@ 0x00C4D850)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C660, oracle length 74 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; lea ecx,[esi+30h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The polymorphic base is constructed out of line on `this` (its destructor is non-trivial: EH
// state 0 once it exists), the derived vtable (0x012B4F84) is stored and the member at +0x30 is
// default-constructed out of line. `this` is kept in the frame for the unwind funclet. The class
// comes from the manifest label; the base and member are descriptive.
struct CAIStateGroupRoot_00c3c660 {                 // out-of-line constructor and destructor
    virtual void v00();
    unsigned char _body[0x2c];
    CAIStateGroupRoot_00c3c660();                   // 0x00C3C3B0
    ~CAIStateGroupRoot_00c3c660();
};

struct CAIStateGroupMember_00c3c660 {               // +0x30
    unsigned char _body[4];
    CAIStateGroupMember_00c3c660();                 // 0x00C4D850
    ~CAIStateGroupMember_00c3c660();
};

struct CAIStateGroupBase_00c3c660 : CAIStateGroupRoot_00c3c660 {   // vtable 0x012B4F84
    CAIStateGroupMember_00c3c660 m_member;          // +0x30
    CAIStateGroupBase_00c3c660();
};

CAIStateGroupBase_00c3c660::CAIStateGroupBase_00c3c660()
    : m_member()
{
}