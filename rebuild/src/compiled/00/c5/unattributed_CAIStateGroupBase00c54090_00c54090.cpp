// CAIStateGroupBase_00c54090::CAIStateGroupBase_00c54090() -- EH-framed constructor: out-of-line base constructor (@ 0x00C6AD70), own vtable, the member at +0x2f8 built out of line (@ 0x00C6AFC0)
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C6AD70 ??0CAudioContextBase@@QAE@XZ (was ??0CAIStateGroupRoot_00c54090@@QAE@XZ); 0x00C6AFC0 ??0CSoundOutput00c6afc0@@QAE@XZ (was ??0CAIStateGroupMember_00c54090@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54090, oracle length 77 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; lea ecx,[esi+2F8h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The polymorphic base is constructed out of line on `this` (its destructor is non-trivial: EH
// state 0 once it exists), the derived vtable (0x012B89C8) is stored and the member at +0x2f8 is
// default-constructed out of line. `this` is kept in the frame for the unwind funclet. The class
// comes from the manifest label; the base and member are descriptive.
struct CAudioContextBase {                 // out-of-line constructor and destructor
    virtual void v00();
    unsigned char _body[0x2f4];
    CAudioContextBase();                   // 0x00C6AD70
    ~CAudioContextBase();
};

struct CSoundOutput00c6afc0 {               // +0x2f8
    unsigned char _body[4];
    CSoundOutput00c6afc0();                 // 0x00C6AFC0
    ~CSoundOutput00c6afc0();
};

struct CAIStateGroupBase_00c54090 : CAudioContextBase {   // vtable 0x012B89C8
    CSoundOutput00c6afc0 m_member;          // +0x2f8
    CAIStateGroupBase_00c54090();
};

CAIStateGroupBase_00c54090::CAIStateGroupBase_00c54090()
    : m_member()
{
}