// CAudioOwner::Update() -- EH-framed per-tick update under a scoped lock over the global @ 0x0143BB14: an out-of-line step on `this` (@ 0x00C091A0), then the parameter block at +0x34 (CAudioParamBlock::Update @ 0x00C0AF20) and the sound-event manager at +0x38 (@ 0x00C12880) are updated, and the optional object at +0x64 advanced (@ 0x00C14DD0)
// Leaf: ?Update@CAudioOwner@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C091F0, oracle length 116 bytes. Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as _unattributed::CPixelShader_00c091f0::Shutdown (manifest "~CPixelShader",
//   propagated); descriptive (no PDB covers this library). `this` is the libpk audio owner (chunk 431's CAudioOwner,
//   destructor 0x00C09380): its two callers are the owner's own pumps, 0x00C092AA (under the lock, when the worker
//   thread handle +0x74 is null) and 0x00C09345 (in 0x00C092D0, when GetTickCount passes the next-tick time +0x78, which is then
//   advanced by 1000 / the tick rate +0x70), so the body is the periodic update, not a shutdown. +0x34 is the parameter
//   block (its landed Update 0x00C0AF20 cites this caller as m_pFirst) and +0x38 the CSoundEventManager (0x00C12880 walks
//   its event pool +0x5c / +0x60 and runs AssignPendingItems 0x00C10CD0). The other callees keep their landed names
//   (OnActivate 0x00C091A0, Advance 0x00C14DD0).
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; call OnActivate ; mov ecx,[esi+34h] ; call CAudioParamBlock::Update ; mov ecx,[esi+38h] ; call CSoundEventManager::Update ; mov ecx,[esi+64h] ; test ecx,ecx ; pop esi ; je skip ; call CThird::Advance ; skip: lea ecx,[esp] ; mov dword ptr [esp+10h],-1 ; call ~CScopedLock ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret
// No vtable store and no return value. The member pointers are called, not deleted.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CAudioParamBlock { void Update(); };       // 0x00C0AF20
struct CSoundEventManager { void Update(); };     // 0x00C12880
struct CThird_00c091f0 { void Advance(); };       // 0x00C14DD0

struct CAudioOwner {
    unsigned char _pad[0x34];
    CAudioParamBlock* m_pParams;           // +0x34
    CSoundEventManager* m_pEventManager;   // +0x38
    unsigned char _pad2[0x28];
    CThird_00c091f0* m_pThird;             // +0x64
    void OnActivate();                     // 0x00C091A0
    void Update();
};

void CAudioOwner::Update()
{
    CScopedLock lock(&g_lock_0143bb14);
    OnActivate();
    m_pParams->Update();
    m_pEventManager->Update();
    if (m_pThird)
        m_pThird->Advance();
}
