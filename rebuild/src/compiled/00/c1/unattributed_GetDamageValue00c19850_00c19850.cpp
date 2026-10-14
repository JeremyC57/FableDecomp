// GetDamageValue_00c19850(CHero*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, virtual slot 5 of the argument, then virtual slot 5 of its result, that result returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19850, oracle length 102 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,[hero] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+14h] ; mov edx,[eax] ; mov ecx,eax ; call [edx+14h] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// A member whose `this` is not read (ecx is overwritten by the argument). The manifest label is
// propagated; the names are descriptive.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CHero_00c19850 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CHero_00c19850* GetExperience();   // slot 5 (the result's slot 5 is the value)
};

struct CDamageQuery_00c19850 {
    long GetDamageValue(CHero_00c19850* hero);
};

long CDamageQuery_00c19850::GetDamageValue(CHero_00c19850* hero)
{
    CScopedLock lock(&g_lock_0143bb14);
    return (long)hero->GetExperience()->GetExperience();
}