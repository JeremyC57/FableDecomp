// CTarget_00c10b30::Add(void*) -- under the profile lock at 0x0143BAFC, hands the argument to the global manager's notify routine (@ 0x00C336F0); `this` is not used
// Vendor naming wave V1 (one address, one name): 0x00C336F0 ?AudioSystem_SetListenerTarget@@YGXPAX@Z (was ?NotifyGlobalManager_00c336f0@@YGXPAX@Z); 0x00C1BED0 ?Add@CTarget_00c10b30@@QAEXPAX@Z (was ?AddConstraintRepeat@CConstraintOwner_00c1bed0@@QAEXPAX@Z)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BED0, 41 bytes (`ret 4`).
// Callees: PKCProtectionInstance::PKCProtectionInstance(void*) @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (the 8-byte scoped lock),
// the `__stdcall` notify @ 0x00C336F0 (labelled "Constraint_NotifyGlobalManager"; it reads the manager singleton
// at 0x0143BB5C itself).
// Label: "CParticleProgram::AddConstraintRepeat" is propagated (the landed 0x00C11710/0x00C11780 relatives are
// named by address too); the only caller (0x00C10B30) calls it on the object at its +0x7bc, so it is a member
// whose `this` is dead. Same shape as the landed CRenderQueue_00c279a0::Flush: no EH frame around the lock.
// Shape: `sub esp,8` frame for the lock, the argument re-read from its slot after the lock constructor, no
// `add esp` after the `__stdcall` call.
class PKCCriticalSection;
class PKCProtectionInstance { public:                                // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    PKCProtectionInstance(PKCCriticalSection* section);                     // 0x00C0FFC0
    ~PKCProtectionInstance();                                 // 0x00C0FFE0
};

extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
void __stdcall AudioSystem_SetListenerTarget(void* constraint);   // 0x00C336F0

struct CTarget_00c10b30 {
    void Add(void* constraint);
};

void CTarget_00c10b30::Add(void* constraint)
{
    PKCProtectionInstance lock(&g_soundLock);
    AudioSystem_SetListenerTarget(constraint);
}