// CConstraintOwner_00c1bed0::AddConstraintRepeat(void*) -- under the profile lock at 0x0143BAFC, hands the argument to the global manager's notify routine (@ 0x00C336F0); `this` is not used
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BED0, 41 bytes (`ret 4`).
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock),
// the `__stdcall` notify @ 0x00C336F0 (labelled "Constraint_NotifyGlobalManager"; it reads the manager singleton
// at 0x0143BB5C itself).
// Label: "CParticleProgram::AddConstraintRepeat" is propagated (the landed 0x00C11710/0x00C11780 relatives are
// named by address too); the only caller (0x00C10B30) calls it on the object at its +0x7bc, so it is a member
// whose `this` is dead. Same shape as the landed CRenderQueue_00c279a0::Flush: no EH frame around the lock.
// Shape: `sub esp,8` frame for the lock, the argument re-read from its slot after the lock constructor, no
// `add esp` after the `__stdcall` call.
struct CProfileLock {                                // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                     // 0x00C0FFC0
    ~CProfileLock();                                 // 0x00C0FFE0
};

extern unsigned char g_ProfileSection_0143bafc[1];   // 0x0143BAFC
void __stdcall NotifyGlobalManager_00c336f0(void* constraint);   // 0x00C336F0

struct CConstraintOwner_00c1bed0 {
    void AddConstraintRepeat(void* constraint);
};

void CConstraintOwner_00c1bed0::AddConstraintRepeat(void* constraint)
{
    CProfileLock lock(g_ProfileSection_0143bafc);
    NotifyGlobalManager_00c336f0(constraint);
}
