// CTCInventoryExperience_00c09270::Refresh() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the out-of-line rebuild @ 0x00C091F0 runs when the pointer at +0x74 is null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C09270, oracle length 96 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[esi+74h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne skip ; mov ecx,esi ; call Rebuild ; skip: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with a null-tested member. The manifest label
// is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTCInventoryExperience_00c09270 {
    unsigned char _pad[0x74];
    void* m_pCurrent;                      // +0x74
    void Rebuild();                        // 0x00C091F0
    void Refresh();
};

void CTCInventoryExperience_00c09270::Refresh()
{
    CScopedLock lock(&g_lock_0143bb14);
    if (!m_pCurrent)
        Rebuild();
}