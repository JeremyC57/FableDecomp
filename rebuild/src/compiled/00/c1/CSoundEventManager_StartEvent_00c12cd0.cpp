// CSoundEventManager::StartEvent(CDriver* driver, long flags, const CPKVoiceParams* voice, long* eventId) -- EH-framed: under a scoped lock over the library lock @ 0x0143BB14, starts the driver's own sample (+4) with the driver's play parameters (+8) through the six-argument StartEvent @ 0x00C12940; the event survives the lock's destructor in esi
// Leaf: ?StartEvent@CSoundEventManager@@QAEPAUCSoundEvent@@PAUCDriver@@JPBUCPKVoiceParams@@PAJ@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12CD0, oracle length 123 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[d] ; mov ecx,[c] ; mov edx,[b] ; push eax ; mov eax,[a] ; push ecx ; push eax ; push edx ; mov edx,[eax+4] ; lea ecx,[eax+8] ; push ecx ; push edx ; mov ecx,esi ; mov dword ptr [esp+2Ch],0 ; call StartEvent ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp
// Vendor naming V2 (plan phase 1, one address, one name; libpk has no PDB: descriptive): the callee 0x00C12940 is
//   ?StartEvent@CSoundEventManager@@QAEPAUCSoundEvent@@JPBUCSoundPlayParameters@@JPAUCDriver@@PBUCPKVoiceParams@@PAJ@Z (was
//   ?Lookup@CTCHeroExperience_00c12cd0@@..., a propagated engine label); this row is its driver overload in the same
//   CSoundEventManager method range 0x00C10C90..0x00C13470 (vtable 0x012B2748): it forwards the driver's sample (+4), the
//   address of the driver's CSoundPlayParameters (+8, 0x14 bytes), the flags, the driver, the voice and the id out-pointer
//   (was ?FindReward@CTCHeroExperience_00c12cd0@@QAEPAUCRewardEntry_00c12cd0@@PAUCRewardKey_00c12cd0@@PAX11@Z).
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CSoundPlayParameters {                         // 0x14 bytes (0x00C12940 copies them)
    unsigned char m_data[0x14];
};

struct CDriver {
    long                 m_kind;                     // +0
    long                 m_sample;                   // +4 the sample the driver plays
    CSoundPlayParameters m_params;                   // +8 its play parameters
};

struct CPKVoiceParams;
struct CSoundEvent;

struct CSoundEventManager {
    CSoundEvent* StartEvent(long sample, const CSoundPlayParameters* source, long flags, CDriver* driver,
                            const CPKVoiceParams* voice, long* eventId);                      // 0x00C12940
    CSoundEvent* StartEvent(CDriver* driver, long flags, const CPKVoiceParams* voice, long* eventId);
};

CSoundEvent* CSoundEventManager::StartEvent(CDriver* driver, long flags, const CPKVoiceParams* voice, long* eventId)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    return StartEvent(driver->m_sample, &driver->m_params, flags, driver, voice, eventId);
}
