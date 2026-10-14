// CVsyncCallbackTimer_00c6ae90::~CVsyncCallbackTimer_00c6ae90() -- EH-framed destructor: the member at +0x28c cleared (@ 0x00C6A3D0, state 1) and destroyed (@ 0x00C6A780, state 0), then the polymorphic member at +0x234 destroyed inline (its vtable 0x012B89C4 stored)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6AE90, oracle length 90 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea edi,[esi+28Ch] ; mov ecx,edi ; mov dword ptr [esp+14h],1 ; call CMember::Clear ; mov ecx,edi ; mov byte ptr [esp+14h],0 ; call ~CMember ; mov ecx,[esp+0Ch] ; mov dword ptr [esi+234h],offset vtable ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Two tracked members: during the body (state 1) both are alive, during the out-of-line member
// destructor (state 0, narrowed to a byte) only the polymorphic one, whose inline empty destructor
// then restores its vtable. The class itself stores no vtable. The manifest label is propagated
// garbage; the class is named by address.
struct CTimerSlot_00c6ae90 {               // +0x234: vtable 0x012B89C4
    virtual void v00();
    ~CTimerSlot_00c6ae90() { }
};

struct CTimerList_00c6ae90 {               // +0x28c
    unsigned char _body[4];
    void Clear();                          // 0x00C6A3D0
    ~CTimerList_00c6ae90();                // 0x00C6A780
};

struct CVsyncCallbackTimer_00c6ae90 {
    unsigned char _pad[0x234];
    CTimerSlot_00c6ae90 m_slot;            // +0x234
    unsigned char _pad2[0x54];
    CTimerList_00c6ae90 m_list;            // +0x28c
    ~CVsyncCallbackTimer_00c6ae90();
};

CVsyncCallbackTimer_00c6ae90::~CVsyncCallbackTimer_00c6ae90()
{
    m_list.Clear();
}