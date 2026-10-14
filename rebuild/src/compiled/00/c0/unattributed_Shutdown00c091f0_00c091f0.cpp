// CPixelShader_00c091f0::Shutdown() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, an out-of-line release on `this` (@ 0x00C091A0), the objects at +0x34 (@ 0x00C0AF20) and +0x38 (@ 0x00C12880) released, and the optional object at +0x64 advanced (@ 0x00C14DD0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C091F0, oracle length 116 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; call Release ; mov ecx,[esi+34h] ; call CFirst::Release ; mov ecx,[esi+38h] ; call CSecond::Reset ; mov ecx,[esi+64h] ; test ecx,ecx ; pop esi ; je skip ; call CThird::Advance ; skip: lea ecx,[esp] ; mov dword ptr [esp+10h],-1 ; call ~CScopedLock ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret
// No vtable store and no return value: a shutdown under the lock, not the destructor the manifest
// label claims. The member pointers are called, not deleted. The class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CFirst_00c091f0 { void Release(); };     // 0x00C0AF20
struct CSecond_00c091f0 { void Reset(); };      // 0x00C12880
struct CThird_00c091f0 { void Advance(); };     // 0x00C14DD0

struct CPixelShader_00c091f0 {
    unsigned char _pad[0x34];
    CFirst_00c091f0* m_pFirst;             // +0x34
    CSecond_00c091f0* m_pSecond;           // +0x38
    unsigned char _pad2[0x28];
    CThird_00c091f0* m_pThird;             // +0x64
    void Release();                        // 0x00C091A0
    void Shutdown();
};

void CPixelShader_00c091f0::Shutdown()
{
    CScopedLock lock(&g_lock_0143bb14);
    Release();
    m_pFirst->Release();
    m_pSecond->Reset();
    if (m_pThird)
        m_pThird->Advance();
}