// CCombat_00c2c8f0::HandleBlockParryEvent(CEvent*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the event goes to the two agents at +8 (@ 0x00C2A840) and +0xc (@ 0x00C2BD40) of the owner at +4, then it is deleted through its virtual destructor (slot 3, `push 1`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C8F0, oracle length 127 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call CScopedLock::CScopedLock ; mov eax,[edi+4] ; mov esi,[event] ; mov ecx,[eax+8] ; push esi ; mov dword ptr [esp+1Ch],0 ; call CAgentA::Handle ; mov ecx,[edi+4] ; mov ecx,[ecx+0Ch] ; push esi ; call CAgentB::Handle ; test esi,esi ; je skip ; mov edx,[esi] ; push 1 ; mov ecx,esi ; call [edx+0Ch] ; skip: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ...
// `delete event` on a class with a virtual destructor is the null-checked deleting-destructor
// call. The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEvent_00c2c8f0 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual ~CEvent_00c2c8f0();            // slot 3
};

struct CAgentA_00c2c8f0 { void Handle(CEvent_00c2c8f0* event); };   // 0x00C2A840
struct CAgentB_00c2c8f0 { void Handle(CEvent_00c2c8f0* event); };   // 0x00C2BD40

struct COwner_00c2c8f0 {
    unsigned char _pad[8];
    CAgentA_00c2c8f0* m_pFirst;            // +8
    CAgentB_00c2c8f0* m_pSecond;           // +0xc
};

struct CCombat_00c2c8f0 {
    unsigned char _pad[4];
    COwner_00c2c8f0* m_pOwner;             // +4
    void HandleBlockParryEvent(CEvent_00c2c8f0* event);
};

void CCombat_00c2c8f0::HandleBlockParryEvent(CEvent_00c2c8f0* event)
{
    CScopedLock lock(&g_lock_0143bb14);
    m_pOwner->m_pFirst->Handle(event);
    m_pOwner->m_pSecond->Handle(event);
    delete event;
}