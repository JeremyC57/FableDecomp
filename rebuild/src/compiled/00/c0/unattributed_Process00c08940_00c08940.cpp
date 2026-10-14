// CEventQueue_00c08940::Process(CEvent*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the event goes to the optional handler at +0x64 (@ 0x00C14AD0), the handler at +0x38 (@ 0x00C12140) and the member handler at +0x3c (@ 0x00C104F0); the event's payload at +4 is released (virtual slot 3) and nulled, then the event deleted through its virtual destructor (slot 6)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08940, oracle length 149 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call CScopedLock::CScopedLock ; mov ecx,[edi+64h] ; test ecx,ecx ; mov esi,[event] ; mov dword ptr [esp+18h],0 ; je skip ; push esi ; call COptional::Handle ; skip: mov ecx,[edi+38h] ; push esi ; call CHandler::Handle ; push esi ; lea ecx,[edi+3Ch] ; call CMemberHandler::Handle ; mov ecx,[esi+4] ; test ecx,ecx ; je del ; mov eax,[ecx] ; call [eax+0Ch] ; mov dword ptr [esi+4],0 ; del: mov edx,[esi] ; push 1 ; mov ecx,esi ; call [edx+18h] ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ...
// The item processor called by the drain loop landed at 0x00C08A00. The manifest label is
// propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CPayload_00c08940 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Release();                // slot 3
};

struct CEvent_00c08940 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual ~CEvent_00c08940();            // slot 6
    CPayload_00c08940* m_pPayload;         // +4
};

struct COptionalHandler_00c08940 { void Handle(CEvent_00c08940* event); };   // 0x00C14AD0
struct CHandler_00c08940 { void Handle(CEvent_00c08940* event); };           // 0x00C12140
struct CMemberHandler_00c08940 {                                             // +0x3c
    unsigned char _body[0x28];
    void Handle(CEvent_00c08940* event);   // 0x00C104F0
};

struct CEventQueue_00c08940 {
    unsigned char _pad[0x38];
    CHandler_00c08940* m_pHandler;         // +0x38
    CMemberHandler_00c08940 m_handler;     // +0x3c
    COptionalHandler_00c08940* m_pOptional;   // +0x64
    void Process(CEvent_00c08940* event);
};

void CEventQueue_00c08940::Process(CEvent_00c08940* event)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (m_pOptional)
        m_pOptional->Handle(event);
    m_pHandler->Handle(event);
    m_handler.Handle(event);
    if (event->m_pPayload) {
        event->m_pPayload->Release();
        event->m_pPayload = 0;
    }
    delete event;
}