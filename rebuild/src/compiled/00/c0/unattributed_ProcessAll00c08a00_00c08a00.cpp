// CEventQueue_00c08a00::ProcessAll() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every item produced by virtual slot 11 of the polymorphic member at +4 is processed out of line (@ 0x00C08940) until the member yields null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08A00, oracle length 118 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call CScopedLock::CScopedLock ; mov eax,[edi+4] ; lea esi,[edi+4] ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call [eax+2Ch] ; test eax,eax ; je done ; loop: push eax ; mov ecx,edi ; call Process ; mov edx,[esi] ; mov ecx,esi ; call [edx+2Ch] ; test eax,eax ; jne loop ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ...
// The member's address is kept in esi across the loop (lea once, the vtable reloaded per call).
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CItemSource_00c08a00 {              // polymorphic member at +4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Next();                  // slot 11
};

struct CEventQueue_00c08a00 {
    unsigned char _pad[4];
    CItemSource_00c08a00 m_source;         // +4
    void Process(void* item);              // 0x00C08940
    void ProcessAll();
};

void CEventQueue_00c08a00::ProcessAll()
{
    CScopedLock lock(&g_lock_0143bb14);
    for (void* item = m_source.Next(); item; item = m_source.Next())
        Process(item);
}