// CAIStateGroupBase::CAIStateGroupBase() -- EH-framed constructor: out-of-line base constructor (@ 0x00C5B1A0), own vtable, the member at +0x1f0 built out of line (@ 0x00C5D0A0)
// Leaf: ??0CAIStateGroupBase@
// Vendor naming wave V1 (one address, one name): 0x00C5B1A0 ??0CSoftwareVoice@@QAE@XZ (was ??0CAIStateGroupRoot@@QAE@XZ); 0x00C5D0A0 ??0CAudioRingBuffer@@QAE@XZ (was ??0CAIStateGroupMember@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F760, oracle length 77 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; lea ecx,[esi+1F0h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The polymorphic base is constructed out of line on `this` (its destructor is non-trivial: EH
// state 0 once it exists), the derived vtable (0x012B5D70) is stored and the member at +0x1f0 is
// default-constructed out of line. `this` is kept in the frame for the unwind funclet. The class
// comes from the manifest label; the base and member are descriptive.
struct CSoftwareVoice {                 // out-of-line constructor and destructor
    virtual void v00();
    unsigned char _body[0x1ec];
    CSoftwareVoice();                   // 0x00C5B1A0
    ~CSoftwareVoice();
};

struct CAudioRingBuffer {               // +0x1f0
    unsigned char _body[4];
    CAudioRingBuffer();                 // 0x00C5D0A0
    ~CAudioRingBuffer();
};

class CAIStateGroupBase : public CSoftwareVoice { public:   // vtable 0x012B5D70
    CAudioRingBuffer m_member;          // +0x1f0
    CAIStateGroupBase();
};

CAIStateGroupBase::CAIStateGroupBase()
    : m_member()
{
}