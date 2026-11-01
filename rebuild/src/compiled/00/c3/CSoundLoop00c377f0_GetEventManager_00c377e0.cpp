// CSoundLoop_00c377f0::GetEventManager() -- the looping sound's sound system: owner (+0) -> +0 -> +4 -> +0x38
// Leaf: ?GetEventManager@CSoundLoop_00c377f0@@QAEPAUCSoundEventManager@@XZ
// Naming (naming wave 21): the record is one class, CSoundLoop_00c377f0 (DropFinishedVoice 0x00C377F0, GetEventManager 0x00C377E0, Update 0x00C37840, dtor 0x00C37930), and the object its accessor reaches (owner -> host -> core+4 -> +0x38) is the landed CSoundEventManager (CSoundSystem+0x38), not a second sound-system class.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C377E0, 11 bytes, thiscall, plain `ret`.
// Identity: called by the landed CSoundLoop rows DropFinishedVoice 0x00C377F0, Update 0x00C37840 and the destructor
//   0x00C37930, which all declare it as the record's sound-system accessor; reloc_bindings holds three spellings
//   (one per caller) and this source takes the 0x00C377F0 one. The catalog label "CPlayer::GetCurrentMode" is propagated
//   (no CPlayer in libpk). Descriptive (libpk, no PDB/RTTI).
// Shape: three dependent loads; the chain is three plain structs.
struct CSoundEventManager;

struct CAudioCore_00c377e0 {
    unsigned char _pad[0x38];
    CSoundEventManager* m_pSoundSystem;              // +0x38
};

struct CAudioHost_00c377e0 {
    unsigned char _pad[4];
    CAudioCore_00c377e0* m_pCore;                       // +0x04
};

struct CLoopOwner_00c377e0 {
    CAudioHost_00c377e0* m_pHost;                       // +0x00
};

struct CSoundLoop_00c377f0 {
    CLoopOwner_00c377e0* m_pOwner;                      // +0x00
    long  m_voice;                                      // +0x04
    CSoundEventManager* GetEventManager();
};

CSoundEventManager* CSoundLoop_00c377f0::GetEventManager()
{
    return m_pOwner->m_pHost->m_pCore->m_pSoundSystem;
}