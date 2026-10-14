// CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase() -- EH-framed destructor: own vtable (0x012B5A88), an out-of-line cleanup on `this` (@ 0x00C44170, state 1), the member at +0x54 destroyed (@ 0x00C44550, state 0), then the out-of-line base destructor (@ 0x00C4CF40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CF40, oracle length 92 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],1 ; call Cleanup ; lea ecx,[esi+54h] ; mov byte ptr [esp+10h],0 ; call ~CMember ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Two tracked objects (the member and the base) give EH state 1 during the body and state 0
// (narrowed to a byte) during the member destructor. The class comes from the manifest label.
struct CAIStateGroupBase_00c3cf40 {
    virtual void v00();
    ~CAIStateGroupBase_00c3cf40();         // 0x00C4CF40
};

struct CCoverMember_00c3cf40 {             // +0x54
    unsigned char _body[4];
    ~CCoverMember_00c3cf40();              // 0x00C44550
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase_00c3cf40 {   // vtable 0x012B5A88
    unsigned char _pad[0x50];
    CCoverMember_00c3cf40 m_cover;         // +0x54
    void Cleanup();                        // 0x00C44170
    ~CAIStateGroup_CreatureUseCoverBase();
};

CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase()
{
    Cleanup();
}