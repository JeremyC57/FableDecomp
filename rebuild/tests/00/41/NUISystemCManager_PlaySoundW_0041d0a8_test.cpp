// Behaviour test for 0x0041d0a8 (NF231, q12628) on the shared headers: NUISystem::CManager::PlaySoundW, the row's body on
// engine/NUISystem_CManager.h, engine/CASoundManager.h (BANKLOOKUP), engine/CASoundEmitter.h, engine/CSoundInit.h and
// engine/CNewFrontendGameComponent.h (GETPSOUNDBANK, the private static read through Get()). The manager, the component and the symbol
// are raw storage (constructors out of line; the row's call is non-virtual here); the sound manager and its 2D emitter are test
// subclasses of the shared classes; GetPSoundBank(long) and CSoundInit() are stubbed. One case per condition. `#define private public`
// (test only, before the component header alone) reaches the private static; the bank word is PSampleBank's Data word (+0x50).
// Replaces the HEAD host model (local CNewFrontendGameComponent / sound system and the ?g_ stand-in).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#include "engine/NUISystem_CManager.h"
#include "engine/CSoundInit.h"
#include "engine/C3DVector.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundManager.h"
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETPSOUNDBANK
#define private public
#include "engine/CNewFrontendGameComponent.h"
#undef private

namespace NUISystem {
void CManager::PlaySoundW(CCharString& symbol, long bankId)
{
    CASoundManager* soundSystem = PSoundManager;
    if (soundSystem)
    {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        if (CNewFrontendGameComponent::Get())
        {
            CASoundBank* bank = CNewFrontendGameComponent::Get()->GetPSoundBank();
            if (bank)
                soundSystem->Get2DEmitter().PlaySoundEvent(*bank, symbol, true, init, false);
        }
        else
        {
            CASoundBank* bank = soundSystem->GetPSoundBank(bankId);
            if (bank)
                PSoundManager->Get2DEmitter().PlaySoundEvent(*bank, symbol, true, init, false);
        }
    }
}
}

static double g_banks_store[2][2];
static CASoundBank* const g_banks[2] = { (CASoundBank*)g_banks_store[0], (CASoundBank*)g_banks_store[1] };
static double g_frontend_bank_store[2];
static int g_calls = 0; static CASoundBank* g_bank = 0; static const CCharString* g_name = 0;
static bool g_loop = false, g_attached = true; static unsigned long g_gain = 0;
CSoundInit::CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1) {}
CASoundBank* CASoundManager::GetPSoundBank(long id) const { return id >= 0 && id < 2 ? g_banks[id] : 0; }
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CASoundEmitter::~CASoundEmitter() {}
CASoundManager::~CASoundManager() {}
void CASoundManager::Update() {}
bool CASoundManager::CanAllocateEmitter() { return false; }
void CASoundManager::SetExclusiveMode(bool) {}
bool CASoundManager::GetExclusiveMode() const { return false; }
void CASoundManager::SetHeadRelativePosition(const C3DVector&) {}
void CASoundManager::SetHeadroomBits(unsigned long) {}
struct Emitter : CASoundEmitter {
    void Activate() { }
    void Deactivate() { }
    unsigned long PlaySample(CASoundBank&, long, bool, const CSoundInit&, bool) { return 0; }
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, const CSoundInit& init, bool attached)
    { ++g_calls; g_bank = &bank; g_name = &name; g_loop = loop; g_gain = init.GainFaderBits; g_attached = attached; return 1; }
    unsigned long PlayAtmos(const CCharString&) { return 0; }
    void SetSpecificSamplePitch(unsigned long, float) { }
    void SetSpecificSampleVolume(unsigned long, float) { }
    void StopSample(CASoundBank&, unsigned long) { }
    void StopSpecificSample(unsigned long) { }
    void StopAllSamples() { }
    void StopAllLoops() { }
    void StopAllCriteriaContainingString(const CCharString&) { }
    void ReleaseLoop(unsigned long) { }
    void Update() { }
    void SetPos(const C3DVector&) { }
    void MovePos(const C3DVector&) { }
    const C3DVector GetPos() const { return *static_cast<const C3DVector*>(0); }
    bool Is3D() const { return false; }
    bool IsPlaying() const { return false; }
    bool IsActive() const { return false; }
    bool IsSpecificSampleLoadedAndPlaying(unsigned long) const { return false; }
    bool IsSpecificSamplePlaying(unsigned long) const { return false; }
    bool IsSamplePlaying(const CASoundBank&, long) const { return false; }
    void GetActiveSampleInfo(std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >&, const CASoundBank&, long) const { }
};
static Emitter* g_emitter = 0;
struct Sound : CASoundManager {
    CASoundBank* CreateSoundBank(const CWideString&) { return 0; }
    void SetEnableAtmos(bool) { }
    CASoundEmitter* Create3DEmitter(const C3DVector&) { return 0; }
    void DestroyPEmitter(CASoundEmitter*) { }
    void SetOutputConfigurationToHeadphones(bool) { }
    bool IsOutputConfigurationSetToHeadphones() { return false; }
    CASoundEmitter& Get2DEmitter() { return *g_emitter; }
    unsigned long GetNoFreeEmitters() const { return 0; }
    CASoundReceiver& GetReceiver() { return *static_cast<CASoundReceiver*>(0); }
    void StopSound(unsigned long) { }
    bool IsSoundPlaying(unsigned long) const { return false; }
    void StopAllSounds() { }
    void PlayMusic(const CWideString&, unsigned long, unsigned long, float, float, long) { }
    void StopMusic(unsigned long) { }
    void SetOutputPCMBufferCount(unsigned long) { }
    void SetOutputPCMBufferSampleSize(unsigned long) { }
    void SetOutputPCMPreloadBufferCount(unsigned long) { }
    void SetDiskByteGranularity(unsigned long) { }
    void SetDiskFrontBufferCount(unsigned long) { }
    void SetDiskBackBufferCount(unsigned long) { }
    void SetAtmosMapBlendTime(float) { }
    void SetDistanceModelRolloff(float) { }
    void SetAtmosSFXDampingDistance(float) { }
    void SetAtmosSFXDampingWeight(float) { }
    void SetReverbParameters(const CReverbParameters&) { }
    void SetReverbLevel(float, float) { }
    void SetReverbTailWaitTimeMS(float) { }
    void SetReverbFadeUpTimeMS(float) { }
    void SetReverbFadeDownTimeMS(float) { }
    void SetHRTFHemisphereMirroring(bool) { }
    void SetMonitorAllowedPostStopTailDurationMS(unsigned long) { }
    void InitAtmos(CAtmosMap*, float, float, long) { }
    bool NewAtmos(const CCharString&, long) { return false; }
    bool AddAtmos2Map(const CCharString&, long, float, float, float) { return false; }
    bool NewChannel(const CCharString&, const CCharString&, long, long, float) { return false; }
    bool SetChannelGain(const CCharString&, const CCharString&, float, float) { return false; }
    void DeleteAllAtmoses() { }
    void StopAllAtmos() { }
    void SendLibCommand(const CCharString&) { }
    void SetPlatform2DGain(float) { }
    void SetPitchFader(unsigned long, float) { }
    void SetGainFader(unsigned long, float, float) { }
    float GetGainFader(unsigned long) { return 0.0f; }
    float GetInstantGainFader(unsigned long) { return 0.0f; }
    bool GetMusicOffset(float& offset) { offset = 0.0f; return false; }
    void FreezeAtmosMaps(bool) { }
    bool ExtractMetaData(const CWideString&, long) { return false; }
};
CASoundManager* PSoundManager = 0;
CNewFrontendGameComponent* CNewFrontendGameComponent::m_pNewFrontendGameComponent = 0;
static double g_mgr_store[(sizeof(NUISystem::CManager) + 7) / 8];
static NUISystem::CManager* const g_mgr = (NUISystem::CManager*)g_mgr_store;
static double g_comp_store[(sizeof(CNewFrontendGameComponent) + 7) / 8];
static double g_sym_store[4];
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    Emitter emitter; g_emitter = &emitter;
    Sound sound;
    CCharString& symbol = *(CCharString*)g_sym_store;
    unsigned long* bankWord = (unsigned long*)((unsigned char*)g_comp_store + 0x50);   // PSampleBank's Data word
    // no sound system: nothing
    g_mgr->NUISystem::CManager::PlaySoundW(symbol, 0);
    Check(g_calls == 0, "no sound system: no call");
    PSoundManager = &sound;
    // no front end: the registry bank bankId, loop, gain fader 0x20, not attached
    g_mgr->NUISystem::CManager::PlaySoundW(symbol, 1);
    Check(g_calls == 1 && g_bank == g_banks[1] && g_name == &symbol && g_loop && !g_attached && g_gain == 0x20, "registry bank 1");
    // no front end, unknown bank: nothing
    g_mgr->NUISystem::CManager::PlaySoundW(symbol, 9);
    Check(g_calls == 1, "unknown registry bank: no call");
    // front end running without a sample bank: nothing (bankId ignored)
    CNewFrontendGameComponent::m_pNewFrontendGameComponent = (CNewFrontendGameComponent*)g_comp_store;
    *bankWord = 0;
    g_mgr->NUISystem::CManager::PlaySoundW(symbol, 0);
    Check(g_calls == 1, "front end without a bank: no call");
    // front end running with its sample bank: that bank (bankId ignored)
    *bankWord = (unsigned long)(CASoundBank*)g_frontend_bank_store;
    g_mgr->NUISystem::CManager::PlaySoundW(symbol, 9);
    Check(g_calls == 2 && g_bank == (CASoundBank*)g_frontend_bank_store && g_loop && !g_attached && g_gain == 0x20, "front-end bank");
    printf(g_fails ? "0041d0a8_TEST FAIL\n" : "0041d0a8_TEST PASS\n");
    return g_fails ? 1 : 0;
}
