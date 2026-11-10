#ifndef FABLE_ENGINE_CASOUNDMANAGER_H
#define FABLE_ENGINE_CASOUNDMANAGER_H
// Retail CASoundManager (the sound-manager base reached through PSoundManager 0x013B8394; movie row 0x006286F0).
// RTTI (vtable 0x0129A574) CASoundManager : CBaseClassNonCopyable : CBaseClass; the live object is a CSoundManagerLH
// (vtable 0x0129D2C4, RTTI CSoundManagerLH : CASoundManager). 54 virtual slots; slot 0 the scalar deleting destructor
// 0x009937B0; slots 3, 6, 12, 13, 52, 53 have base bodies, the other 47 are _purecall (0x00BFEA08) here.
//
// SLOT NAMES (agentSND, 2026-11-10; the full table with every source is w22/agentSND SLOTS.tsv / NOTES.md). The sound module
// is absent from Ego_r.pdb / FableWin.pdb as classes, so the names come from:
//  (a) the Fable Anniversary Xbox 360 PDB (anniv360.pdb publics, lib_sound_manager_base.cpp / lib_sound_manager_un.cpp:
//      CASoundManager and its Unreal-port override CSoundManagerUN, the same interface with 14 slots added) -- names and
//      parameter types; the Anniversary PC exe's RTTI vtables (CASoundManager 0x02E5895C / CSoundManagerUN 0x02E58CA4,
//      68 slots) align with TLC's 54 by the base bodies (Update, CanAllocateEmitter, Set/GetExclusiveMode,
//      SetHeadRelativePosition, SetHeadroomBits) and by every forwarder body;
//  (b) TLC itself: Ego_r.pdb / FableWin.pdb NGlobalConsole::ConsoleAudio_<Name> console commands (SendLibCommand,
//      SetHeadroomBits, SetHeadRelativePosition, SetOutputPCMBufferCount / SampleSize / PreloadBufferCount,
//      SetDiskByteGranularity / FrontBufferCount / BackBufferCount, SetAtmosMapBlendTime, SetDistanceModelRolloff,
//      SetAtmosSFXDampingDistance / Weight, SetMonitorAllowedPostStopTailDurationMS, SetHRTFHemisphereMirroring) in the
//      same order; the RTTI nested type .?AVCAtmosMap@CASoundManager@@ (CAtmosProcess::CMyAtmosMap's base, InitAtmos's
//      argument); the PDB-named callers (CGameScriptInterface::StopSound -> 14, ::IsSoundPlaying -> 15, ::EnableSounds
//      -> 16, GPlayMusic / CMusicManager::PlayMusic(long, const CFadeTimes&, long) -> 17 with CRegisteredMusicEntry's
//      Gain / Offset, CMusicManager::UpdateCurrentMusicOffset -> 49, CUserProfileManager::ApplyValues -> 7 with
//      UsingHeadPhones, CBulletTimeManager::RecalculateSoundPitch -> 45, CAtmosProcess::ApplyReverbSettings -> 29 with
//      CReverbParameters / 30, CAtmosProcess ctor -> 36 with CSoundDef::AtmosListenerDampingMin / Range).
//  Descriptive (no name found anywhere): slot 9 Get2DEmitter (the manager's own 2-D CSoundEmitterLH +0x44; the
//  Anniversary getter body is folded and unnamed; spelled after Create3DEmitter / GetReceiver and the script
//  Play2DSound that uses it) and slot 51 ExtractMetaData (hands a file name to the audio system's meta-data reader,
//  whose only message is "There was a problem serialising the extracted meta data."; the Anniversary slot is an empty
//  stub with no public). Slot 42 StopAllAtmos is the Anniversary name of the matching empty-stub slot (TLC body: give the
//  audio system a null atmos map; ~CAtmosProcess calls it after DeleteAllAtmoses).
// The old placeholder spellings (VirtualNN_<addr>, FlushLoadingQueue_00991670, SetGroupVolume = SetGainFader) are gone; a
// virtual's name never reaches a caller's bytes or relocations (the call goes through the vtable).
//
// Non-virtual members (the CASoundManager TU, between slot 6 0x009915C0 and slot 3 0x00991670, on members the base
// constructor 0x00991910 initialises to true): three bool setters that store a flag and, on a change, call a slot. Their
// Anniversary names are EnableMusic / EnableSound / EnableAtmos (opt-in below); the default path keeps the landed spellings
// until the coordinator respells the callers (NAME_FIXES).
// Size: CSoundManagerLH's constructor 0x00A3EF00 calls ??0CASoundManager (0x00991910) and stores its own first member at
// +0x4C. The base members (landed constructor model 0x00991910: init-block settings +0x04..+0x18, Volume +0x1C,
// ExclusiveMode +0x20 (slots 12 / 13), Enabled24 / Enabled25 / Enabled26 +0x24..+0x26, ResourceBank +0x28, Banks +0x30,
// +0x3C, Busy +0x3D; descriptive names, no PDB) are kept opaque.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CASoundEmitter.h"
#ifdef FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS
#include "engine/CVectorMap.h"
#include "fable_string.h"
#endif
#if defined(FABLE_RETAIL_CASOUNDMANAGER_BANKREGISTRATION)
class CWideString;
class CCharString;
#endif
class CASoundBank;
class CASoundReceiver;
class CCharString;
class CWideString;
class C3DVector;
class CReverbParameters;
#ifdef FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
class CLipSyncDataBank;
#endif
#ifdef FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
class C2DCoordI;
#endif
class CASoundManager : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS
    // Retail opt-in (agentNF3, 2026-11-10, rows 0x00991690 / 0x009916F0 / 0x00991720 / 0x00991750 / 0x009917E0): the registered-bank
    // records of the CASoundManager TU. Fable Anniversary 360 PDB publics (lib_sound_manager_base.cpp): the nested class
    // CASoundManager::CRegisteredBank and CVectorMap<long, CRegisteredBank, CKeyPairCompareLess<..> > (LowerBound / Sort / insert
    // instances); TLC bodies: each record is a pair<long, CRegisteredBank> of 0x24 bytes in the CVectorMap at +0x30 (the base
    // constructor 0x00991910), the sound bank at record +0x04, the lip-sync bank at +0x0C and the symbol name at +0x14 (the five
    // linear scans read exactly those words). The Anniversary PDB has no member names: the member names are descriptive. Needs
    // FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS and FABLE_RETAIL_CASOUNDMANAGER_SIZE40; layout and size (0x40) unchanged.
    class CRegisteredBank {                                              // 0x20 bytes
    public:
        CASoundBank*            PSoundBank;                              // +0x00 (record +0x04)
        unsigned char           _pad_0x04[0x4];
        const CLipSyncDataBank* PLipSyncBank;                            // +0x08 (record +0x0C)
        unsigned char           _pad_0x0c[0x4];
        CCharString             SymbolName;                              // +0x10 (record +0x14)
        unsigned char           _pad_0x14[0x20 - 0x14];
    };
#endif
#ifdef FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
    // Retail opt-in (agentSND2, 2026-11-10): the nested abstract atmos map InitAtmos (slot 36) takes. RTTI .?AVCAtmosMap@CASoundManager@@
    // is the base of CAtmosProcess::CMyAtmosMap (vtable 0x0125D47C: two virtuals, no destructor; the landed overrides
    // 0x006B1580 GetBlockValue / 0x006B15C0 GetBlockHeight(const C2DCoordI&) -- descriptive, no PDB names).
    class CAtmosMap {
    public:
        virtual long GetBlockValue(const C2DCoordI& block) = 0;          // slot 0
        virtual float GetBlockHeight(const C2DCoordI& block) = 0;        // slot 1
    };
#else
    class CAtmosMap;                                                     // .?AVCAtmosMap@CASoundManager@@ (InitAtmos's map)
#endif
#ifdef FABLE_RETAIL_CASOUNDMANAGER_BANKREGISTRATION
    // Retail opt-in (Codex 2026-11-10): native009919c0/00991c10 and CASoundManager constructor/dtor RTTI.
    // Audio module has no PDB symbols: these member names describe the observed bank-registration behavior.
    void RegisterSoundBank(unsigned long,const CWideString&,const CWideString&,const CWideString&,const CCharString&);
    void RegisterAtmosSoundBank(unsigned long,const CWideString&,const CWideString&);
#ifndef FABLE_RETAIL_CASOUNDMANAGER_GETPSOUNDBANK_LONG
    class CASoundBank* GetBank(long);
#endif
#endif
#ifdef FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
    // Retail opt-in (agentSND2, 2026-11-10): the non-virtual bank lookups of the CASoundManager TU (0x009915C0..0x00991A..), named
    // from the Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order: CanAllocateEmitter, EnableMusic /
    // Sound / Atmos, Update, GetPSoundBank(const CCharString&), GetPLipSyncBank, GetAnyPLipSyncBank, UnloadAllBanks,
    // GetPSoundBank(long), ~CASoundManager, CloseAllBanks, ctor, RegisterSoundBank, RegisterAtmosSoundBank) with the TLC bodies as
    // the second source: each walks the registered-bank records (pair<long, CRegisteredBank>, 0x24 bytes: sound bank +0x04,
    // lip-sync bank +0x0C, symbol name +0x14) of the CVectorMap at +0x30.
    //   0x009916F0 returns the lip-sync bank of the record whose sound bank is the argument; 0x00991720 the first non-null lip-sync
    //   bank; 0x00991750 clears the sound flag +0x24 (StopAllSounds, slot 16, when it was set) and calls slot 2 of every bank;
    //   0x009917E0 compares the record name with the CCharString argument (strcmp 0x00BFEBA8) and returns its bank; 0x00991840
    //   finds the record by key; 0x009918F0 erases every record.
    // 0x00991690 (the record whose lip-sync bank is the argument -> its sound bank) has no Anniversary counterpart: descriptive,
    // spelled as a third GetPSoundBank overload.
    CASoundBank* GetPSoundBank(const CLipSyncDataBank& lipSyncBank) const;               // 0x00991690 (descriptive)
    const CLipSyncDataBank* GetPLipSyncBank(const CASoundBank& soundBank) const;           // 0x009916F0
    const CLipSyncDataBank* GetAnyPLipSyncBank() const;                                    // 0x00991720
    void UnloadAllBanks();                                                                 // 0x00991750
    CASoundBank* GetPSoundBank(const CCharString& symbolName) const;                       // 0x009917E0
#if !defined(FABLE_RETAIL_CASOUNDMANAGER_BANKREGISTRATION) || defined(FABLE_RETAIL_CASOUNDMANAGER_GETPSOUNDBANK_LONG)
    // FABLE_RETAIL_CASOUNDMANAGER_GETPSOUNDBANK_LONG (agentNF3, 2026-11-10): with BANKREGISTRATION, declare 0x00991840 under its
    // Anniversary 360 PDB name GetPSoundBank(long) const instead of the descriptive GetBank(long) (one address, one name).
    CASoundBank* GetPSoundBank(long bankID) const;                                         // 0x00991840
#endif
    void CloseAllBanks();                                                                  // 0x009918F0
#endif
    virtual ~CASoundManager();                                                            // slot  0 ??_G 0x009937B0
protected:
    virtual CASoundBank* CreateSoundBank(const CWideString& fileName) = 0;                // slot  1 (+0x04) LH 0x00A3E430
    virtual void SetEnableAtmos(bool enable) = 0;                                         // slot  2 (+0x08) LH 0x00A3E3E0
public:
    virtual void Update();                                                                // slot  3 (+0x0C) 0x00991670; LH 0x00A3EB20
    virtual CASoundEmitter* Create3DEmitter(const C3DVector& position) = 0;               // slot  4 (+0x10) LH 0x00A3ECB0
    virtual void DestroyPEmitter(CASoundEmitter* emitter) = 0;                            // slot  5 (+0x14) LH 0x00A3EB60
    virtual bool CanAllocateEmitter();                                                    // slot  6 (+0x18) 0x009915C0; LH 0x00A3EC70
    virtual void SetOutputConfigurationToHeadphones(bool headphones) = 0;                 // slot  7 (+0x1C) LH 0x00A3E350
    virtual bool IsOutputConfigurationSetToHeadphones() = 0;                              // slot  8 (+0x20) LH 0x00A3E380
    virtual CASoundEmitter& Get2DEmitter() = 0;                                           // slot  9 (+0x24) LH 0x00A3E3A0 (descriptive)
    virtual unsigned long GetNoFreeEmitters() const = 0;                                  // slot 10 (+0x28) LH 0x00A3EC90
    virtual CASoundReceiver& GetReceiver() = 0;                                           // slot 11 (+0x2C) LH 0x00A3E3B0
    virtual void SetExclusiveMode(bool exclusive);                                        // slot 12 (+0x30) 0x00993690 (both vtables)
    virtual bool GetExclusiveMode() const;                                                // slot 13 (+0x34) 0x009936A0 (both vtables)
    virtual void StopSound(unsigned long sampleID) = 0;                                   // slot 14 (+0x38) LH 0x00A3EBF0
    virtual bool IsSoundPlaying(unsigned long sampleID) const = 0;                        // slot 15 (+0x3C) LH 0x00A3EC30
    virtual void StopAllSounds() = 0;                                                     // slot 16 (+0x40) LH 0x00A3EBC0
    virtual void PlayMusic(const CWideString& fileName, unsigned long fadeOutMS, unsigned long fadeInMS,
                           float gain, float offset, long loopCount) = 0;                 // slot 17 (+0x44) LH 0x00A3E4B0
    virtual void StopMusic(unsigned long fadeOutMS) = 0;                                  // slot 18 (+0x48) LH 0x00A3E520
    virtual void SetOutputPCMBufferCount(unsigned long count) = 0;                        // slot 19 (+0x4C) LH 0x00A3E560
    virtual void SetOutputPCMBufferSampleSize(unsigned long size) = 0;                    // slot 20 (+0x50) LH 0x00A3E580
    virtual void SetOutputPCMPreloadBufferCount(unsigned long count) = 0;                 // slot 21 (+0x54) LH 0x00A3E5A0
    virtual void SetDiskByteGranularity(unsigned long granularity) = 0;                   // slot 22 (+0x58) LH 0x00A3E5C0
    virtual void SetDiskFrontBufferCount(unsigned long count) = 0;                        // slot 23 (+0x5C) LH 0x00A3E5E0
    virtual void SetDiskBackBufferCount(unsigned long count) = 0;                         // slot 24 (+0x60) LH 0x00A3E600
    virtual void SetAtmosMapBlendTime(float time) = 0;                                    // slot 25 (+0x64) LH 0x00A3E620
    virtual void SetDistanceModelRolloff(float rolloff) = 0;                              // slot 26 (+0x68) LH 0x00A3E640
    virtual void SetAtmosSFXDampingDistance(float distance) = 0;                          // slot 27 (+0x6C) LH 0x00A3E660
    virtual void SetAtmosSFXDampingWeight(float weight) = 0;                              // slot 28 (+0x70) LH 0x00A3E680
    virtual void SetReverbParameters(const CReverbParameters& parameters) = 0;            // slot 29 (+0x74) LH 0x00A3E6A0
    virtual void SetReverbLevel(float level, float time) = 0;                             // slot 30 (+0x78) LH 0x00A3E760
    virtual void SetReverbTailWaitTimeMS(float time) = 0;                                 // slot 31 (+0x7C) LH 0x00A3E780
    virtual void SetReverbFadeUpTimeMS(float time) = 0;                                   // slot 32 (+0x80) LH 0x00A3E7A0
    virtual void SetReverbFadeDownTimeMS(float time) = 0;                                 // slot 33 (+0x84) LH 0x00A3E7C0
    virtual void SetHRTFHemisphereMirroring(bool mirroring) = 0;                          // slot 34 (+0x88) LH 0x00A3E7E0
    virtual void SetMonitorAllowedPostStopTailDurationMS(unsigned long duration) = 0;     // slot 35 (+0x8C) LH 0x00A3E830
    virtual void InitAtmos(CAtmosMap* map, float dampingWeight, float dampingDistance,
                           long groupMask) = 0;                                           // slot 36 (+0x90) LH 0x00A3E850
    virtual bool NewAtmos(const CCharString& name, long groupMask) = 0;                   // slot 37 (+0x94) LH 0x00A3F140
    virtual bool AddAtmos2Map(const CCharString& name, long index, float a, float b,
                              float c) = 0;                                               // slot 38 (+0x98) LH 0x00A3ED90
    virtual bool NewChannel(const CCharString& atmosName, const CCharString& channelName,
                            long sample, long a, float b) = 0;                            // slot 39 (+0x9C) LH 0x00A3F250
    virtual bool SetChannelGain(const CCharString& atmosName, const CCharString& channelName,
                                float gain, float time) = 0;                              // slot 40 (+0xA0) LH 0x00A3EED0
    virtual void DeleteAllAtmoses() = 0;                                                  // slot 41 (+0xA4) LH 0x00A3F340
    virtual void StopAllAtmos() = 0;                                                      // slot 42 (+0xA8) LH 0x00A3E920
    virtual void SendLibCommand(const CCharString& command) = 0;                          // slot 43 (+0xAC) LH 0x00A3EAE0
    virtual void SetPlatform2DGain(float gain) = 0;                                       // slot 44 (+0xB0) LH 0x00A3E540
    virtual void SetPitchFader(unsigned long groupMask, float pitch) = 0;                 // slot 45 (+0xB4) LH 0x00A3E980
    virtual void SetGainFader(unsigned long groupMask, float gain, float fadeTime) = 0;   // slot 46 (+0xB8) LH 0x00A3E9D0
    virtual float GetGainFader(unsigned long groupMask) = 0;                              // slot 47 (+0xBC) LH 0x00A3EA20
    virtual float GetInstantGainFader(unsigned long groupMask) = 0;                       // slot 48 (+0xC0) LH 0x00A3EA80
    virtual bool GetMusicOffset(float& offset) = 0;                                       // slot 49 (+0xC4) LH 0x00A3E480
    virtual void FreezeAtmosMaps(bool freeze) = 0;                                        // slot 50 (+0xC8) LH 0x00A3E3C0
    virtual bool ExtractMetaData(const CWideString& fileName, long flags) = 0;            // slot 51 (+0xCC) LH 0x00A3E800 (descriptive)
    virtual void SetHeadRelativePosition(const C3DVector& position);                      // slot 52 (+0xD0) 0x009936B0; LH 0x00A41800
    virtual void SetHeadroomBits(unsigned long bits);                                     // slot 53 (+0xD4) 0x009936C0; LH 0x00A3E330
#ifdef FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES
    // Retail opt-in (agentSND 2026-11-10): the Anniversary 360 PDB names of the three flag setters (CASoundManager::
    // EnableMusic / EnableSound / EnableAtmos(bool), lib_sound_manager_base.cpp), matched by body: 0x009915E0 (+0x25,
    // StopMusic(0) on disable), 0x00991600 (+0x24, StopAllSounds on disable), 0x00991620 (+0x26, SetEnableAtmos on change).
    void EnableMusic(bool enable);                                                        // 0x009915E0
    void EnableSound(bool enable);                                                        // 0x00991600
    void EnableAtmos(bool enable);                                                        // 0x00991620
#else
    void SetEnabled(bool);                                               // 0x00991600 +0x24; slot 16 on disable (landed binding: CSoundManagerLH::SetEnabled)
    void SetEnabled25_009915e0(bool);                                    // 0x009915E0 +0x25; slot 18 (0) on disable (descriptive)
    void SetEnabled26_00991620(bool);                                    // 0x00991620 +0x26; slot 2 (value) on change (descriptive)
#endif
#ifdef FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
    // Retail opt-in (agentSND2, 2026-11-10): the three enable flags the base constructor 0x00991910 sets to true (`mov [esi+24h..26h],al`)
    // and EnableSound 0x00991600 / EnableMusic 0x009915E0 / EnableAtmos 0x00991620 store; CMusicManager::UpdateCurrentMusicOffset
    // 0x006BD8A0 reads +0x25 before GetMusicOffset. No PDB members: descriptive names after the setters. Layout unchanged.
protected:
    unsigned char _pad_0x04[0x24 - 0x04];
public:
    bool SoundEnabled;                                                   // +0x24
    bool MusicEnabled;                                                   // +0x25
    bool AtmosEnabled;                                                   // +0x26
private:
#ifdef FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#ifdef FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS
    unsigned char _pad_0x27[0x30 - 0x27];
public:
    CVectorMap<long, CRegisteredBank> RegisteredBanks;                   // +0x30 (retail 16-byte CVectorMap)
};
#else
    unsigned char _pad_0x27[0x40 - 0x27];
};
#endif
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x40);
#else
    unsigned char _pad_0x27[0x4c - 0x27];
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x4c);
#endif
#else
private:
#ifdef FABLE_RETAIL_CASOUNDMANAGER_SIZE40
    // Retail opt-in (agentSND2, 2026-11-10): the base is 0x40 bytes. The base constructor 0x00991910 stores its last member at
    // +0x3D (`mov byte ptr [esi+3Dh],bl`) and never touches +0x40..+0x4B; the CSoundManagerLH constructor 0x00A3EF00 assigns
    // +0x40 (the libpk sound system, from 0x00C096E0), +0x44 (the 2-D CSoundEmitterLH, 0x00AAC530) and +0x48 (the
    // CSoundReceiverLH, 0x00AAD020) in its body, and the CSoundManagerLH getter Get2DEmitter 0x00A3E3A0 (`lea eax,..`/+0x44)
    // and every CSoundEmitterLH row read +0x40 through the manager pointer: they are CSoundManagerLH's own first members
    // (landed CSoundManagerLH dtor 0x00A3EDE0 models the base at 0x40 the same way).
    unsigned char _pad_0x04[0x40 - 0x04];                                // +0x04 members (opaque, see above)
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x40);
#else
    unsigned char _pad_0x04[0x4c - 0x04];                                // +0x04 members (opaque, see above)
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x4c);
#endif
#endif

extern CASoundManager* PSoundManager;                                    // 0x013B8394 ?PSoundManager@@3PAVCASoundManager@@A (ESTABLISHED_SPELLINGS)

#endif // FABLE_ENGINE_CASOUNDMANAGER_H
