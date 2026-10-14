// CCombat_00c2ca40::CreateParryBlockEvent(void*, void*, void*, void*, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the factory at +8 of the owner at +4 creates the event from `this` and the six arguments in the order (a, f, b, c, d, e) (@ 0x00C2A490); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CA40, oracle length 130 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[e] ; mov ecx,[d] ; mov edx,[c] ; push eax ; mov eax,[b] ; push ecx ; mov ecx,[f] ; push edx ; mov edx,[a] ; push eax ; mov eax,[esi+4] ; push ecx ; mov ecx,[eax+8] ; push edx ; push esi ; mov dword ptr [esp+30h],0 ; call CFactory::Create ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 18h
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CCombat_00c2ca40;
struct CEventFactory_00c2ca40 {
    void* Create(CCombat_00c2ca40* combat, void* a, void* f, void* b, void* c, void* d, void* e);   // 0x00C2A490
};

struct COwner_00c2ca40 {
    unsigned char _pad[8];
    CEventFactory_00c2ca40* m_pFactory;    // +8
};

struct CCombat_00c2ca40 {
    unsigned char _pad[4];
    COwner_00c2ca40* m_pOwner;             // +4
    void* CreateParryBlockEvent(void* a, void* b, void* c, void* d, void* e, void* f);
};

void* CCombat_00c2ca40::CreateParryBlockEvent(void* a, void* b, void* c, void* d, void* e, void* f)
{
    CScopedLock lock(&g_lock_0143bb14);
    return m_pOwner->m_pFactory->Create(this, a, f, b, c, d, e);
}
