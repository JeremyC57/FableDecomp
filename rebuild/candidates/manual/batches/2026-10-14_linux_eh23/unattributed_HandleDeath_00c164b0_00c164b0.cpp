// CTCMapwhoHost_00c164b0::HandleDeath(CDeathEvent*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, -1 for a null event or when the event's entry at +4 does not belong to the mapwho at +8; else the mapwho is told the event (@ 0x00C0EAD0) and its entry cleared (@ 0x00C0EC10); returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C164B0, oracle length 199 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+1Ch] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne ok ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; or eax,-1 ; ... ret 4 ; ok: mov ecx,[edi+8] ; push esi ; mov esi,[eax+4] ; cmp [esi+4],ecx ; je same ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; pop esi ; or eax,-1 ; ... ret 4 ; same: push eax ; call CTCMapwho::OnDie ; mov ecx,[edi+8] ; push esi ; call CTCMapwho::ClearEntry ; ... xor eax,eax ; ... ret 4
// The mapwho pointer loaded for the compare is reused as `this` for the first call; the second
// reloads it. The manifest label is a description; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTCMapwho_00c164b0;
struct CMapwhoEntry_00c164b0 {
    long m_0;
    CTCMapwho_00c164b0* m_pMapwho;         // +4
};

struct CDeathEvent_00c164b0 {
    long m_0;
    CMapwhoEntry_00c164b0* m_pEntry;       // +4
};

struct CTCMapwho_00c164b0 {
    void OnDie(CDeathEvent_00c164b0* event);          // 0x00C0EAD0
    void ClearEntry(CMapwhoEntry_00c164b0* entry);    // 0x00C0EC10
};

struct CTCMapwhoHost_00c164b0 {
    long m_0;
    long m_4;
    CTCMapwho_00c164b0* m_pMapwho;         // +8
    long HandleDeath(CDeathEvent_00c164b0* event);
};

long CTCMapwhoHost_00c164b0::HandleDeath(CDeathEvent_00c164b0* event)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (!event)
        return -1;
    CMapwhoEntry_00c164b0* entry = event->m_pEntry;
    if (entry->m_pMapwho != m_pMapwho)
        return -1;
    m_pMapwho->OnDie(event);
    m_pMapwho->ClearEntry(entry);
    return 0;
}
