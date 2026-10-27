// CSoundLoop_00c377f0::GetSoundSystem() -- the looping sound's sound system: owner (+0) -> +0 -> +4 -> +0x38
// Leaf: ?GetSoundSystem@CSoundLoop_00c377f0@@QAEPAUCSoundSystem_00c377f0@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C377E0, 11 bytes, thiscall, plain `ret`.
// Identity: called by the landed CSoundLoop rows DropFinishedVoice 0x00C377F0, Update 0x00C37840 and the destructor
//   0x00C37930, which all declare it as the record's sound-system accessor; reloc_bindings holds three spellings
//   (one per caller) and this source takes the 0x00C377F0 one. The catalog label "CPlayer::GetCurrentMode" is propagated
//   (no CPlayer in libpk). Descriptive (libpk, no PDB/RTTI).
// Shape: three dependent loads; the chain is three plain structs.
struct CSoundSystem_00c377f0;

struct CAudioCore_00c377e0 {
    unsigned char _pad[0x38];
    CSoundSystem_00c377f0* m_pSoundSystem;              // +0x38
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
    CSoundSystem_00c377f0* GetSoundSystem();
};

CSoundSystem_00c377f0* CSoundLoop_00c377f0::GetSoundSystem()
{
    return m_pOwner->m_pHost->m_pCore->m_pSoundSystem;
}