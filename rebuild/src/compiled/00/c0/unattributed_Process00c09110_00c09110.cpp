// CInventorySetup_00c09110::Process(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, an 8-byte polymorphic request is built on the stack from the second argument (@ 0x00C08D20, state 1 while it lives), the `__fastcall` processor (@ 0x00C172F0) runs on the object at +0x5c with the first argument, the member at +0x80 and the request, the request's inline destructor stores its vtable (0x012B1AA0), and the result byte survives the lock's destructor in bl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C09110, oracle length 141 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,10h ; push ebx ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+14h] ; call CScopedLock::CScopedLock ; mov eax,[b] ; push eax ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+24h],0 ; call CRequest::CRequest ; mov edx,[esi+80h] ; lea ecx,[esp+8] ; push ecx ; mov ecx,[esi+5Ch] ; push edx ; mov edx,[a] ; mov byte ptr [esp+28h],1 ; call Process ; lea ecx,[esp+10h] ; mov bl,al ; mov dword ptr [esp+8],offset vtable ; mov dword ptr [esp+20h],-1 ; call ~CScopedLock ; mov ecx,[esp+18h] ; pop esi ; mov al,bl ; pop ebx ; mov fs:[0],ecx ; add esp,1Ch ; ret 8
// The processor takes the object and the first argument in ecx/edx (`__fastcall`), the rest on
// the stack. The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CRequest_00c09110 {                 // 8 bytes on the stack, vtable 0x012B1AA0
    virtual void v00();
    long m_value;
    CRequest_00c09110(void* source);       // 0x00C08D20
    ~CRequest_00c09110() { }
};

struct CProcessor_00c09110;
bool __fastcall Process_00c172f0(CProcessor_00c09110* processor, void* a, void* context, CRequest_00c09110* request);   // 0x00C172F0

struct CInventorySetup_00c09110 {
    unsigned char _pad[0x5c];
    CProcessor_00c09110* m_pProcessor;     // +0x5c
    unsigned char _pad2[0x20];
    void* m_pContext;                      // +0x80
    bool Process(void* a, void* b);
};

bool CInventorySetup_00c09110::Process(void* a, void* b)
{
    CScopedLock lock(&g_lock_0143bb14);
    CRequest_00c09110 request(b);
    return Process_00c172f0(m_pProcessor, a, m_pContext, &request);
}