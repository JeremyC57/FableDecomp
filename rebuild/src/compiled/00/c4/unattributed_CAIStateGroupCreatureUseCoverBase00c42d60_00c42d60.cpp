// CAIStateGroup_CreatureUseCoverBase_00c42d60::CAIStateGroup_CreatureUseCoverBase_00c42d60() -- EH-framed constructor: out-of-line base constructor (@ 0x00C4CF20), own vtable (0x012B5A88), the cover member at +0x54 built out of line (@ 0x00C44250), then the index at +0xc, the pointer at +0x10 and the flag at +0xf4 set in the body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C42D60, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; lea ecx,[esi+54h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CCover::CCover ; mov ecx,[esp+8] ; mov dword ptr [esi+0Ch],-1 ; mov dword ptr [esi+10h],0 ; mov dword ptr [esi+0F4h],0 ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The constructor counterpart of the destructor at 0x00C3CF40 (same vtable). The three stores
// after the member constructor are body assignments (two of them to base members). The manifest
// label (`CIcicleData`) is propagated garbage; the names are descriptive.
struct CAIStateGroupBase_00c42d60 {
    virtual void v00();
    unsigned char _pad[8];
    long m_index;                          // +0xc
    void* m_pTarget;                       // +0x10
    unsigned char _body[0x40];
    CAIStateGroupBase_00c42d60();          // 0x00C4CF20
    ~CAIStateGroupBase_00c42d60();
};

struct CCoverMember_00c42d60 {             // +0x54
    unsigned char _body[0xa0];
    CCoverMember_00c42d60();               // 0x00C44250
    ~CCoverMember_00c42d60();
};

struct CAIStateGroup_CreatureUseCoverBase_00c42d60 : CAIStateGroupBase_00c42d60 {   // vtable 0x012B5A88
    CCoverMember_00c42d60 m_cover;         // +0x54
    long m_flag;                           // +0xf4
    CAIStateGroup_CreatureUseCoverBase_00c42d60();
};

CAIStateGroup_CreatureUseCoverBase_00c42d60::CAIStateGroup_CreatureUseCoverBase_00c42d60()
    : m_cover()
{
    m_index = -1;
    m_pTarget = 0;
    m_flag = 0;
}