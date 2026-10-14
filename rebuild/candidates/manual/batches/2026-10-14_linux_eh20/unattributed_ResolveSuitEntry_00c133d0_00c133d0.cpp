// CTCHeroExperience_00c133d0::ResolveSuitEntry(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, -1 when the suit map at +0x68 of the owner at +4 does not know the key (@ 0x00C196A0), else the entry resolved out of line with the second argument (@ 0x00C13310); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C133D0, oracle length 159 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[key] ; mov ecx,[esi+4] ; mov ecx,[ecx+68h] ; push eax ; mov dword ptr [esp+18h],0 ; call CClothingSuitMap::Find ; test eax,eax ; jne hit ; ... or eax,-1 ; ... ret 8 ; hit: mov edx,[arg] ; push edx ; push eax ; mov ecx,esi ; call ResolveEntry ; ...
// The map's find is the one used by the clothing-suit queries landed at 0x00C19B70 / 0x00C19C10.
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEntry_00c133d0;
struct CClothingSuitMap_00c133d0 {
    CEntry_00c133d0* Find(void* key);      // 0x00C196A0
};

struct COwner_00c133d0 {
    unsigned char _pad[0x68];
    CClothingSuitMap_00c133d0* m_pSuitMap; // +0x68
};

struct CTCHeroExperience_00c133d0 {
    unsigned char _pad[4];
    COwner_00c133d0* m_pOwner;             // +4
    long ResolveEntry(CEntry_00c133d0* entry, void* argument);   // 0x00C13310
    long ResolveSuitEntry(void* key, void* argument);
};

long CTCHeroExperience_00c133d0::ResolveSuitEntry(void* key, void* argument)
{
    CScopedLock lock(&g_lock_0143bb14);
    CEntry_00c133d0* entry = m_pOwner->m_pSuitMap->Find(key);
    if (!entry)
        return -1;
    return ResolveEntry(entry, argument);
}
