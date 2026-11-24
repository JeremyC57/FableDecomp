// Behaviour test for 0x0042ebb6 (NF231, q12627 / q12628) on the shared headers: CNewFrontendGameComponent::Shutdown, the row's body under
// the row's defines, on engine/CNewFrontendGameComponent.h (RUN + FILENAMES_FILETIMES), CDefPointer.h, CStartupDefinitionManager.h,
// CFontManager.h, CASoundManager.h and CDisplayManager.h. The component, font manager and display manager are raw storage (their
// constructors are out of line); the def is a test pointee on CDefPointeeBase and the sound manager the test subclass of the shared
// CASoundManager that the 0x009A5A50 test uses. The out-of-line callees are stubbed and logged. One case per condition in the row.
// `#define private public` (test only, the established pattern) reaches the private members; replaces the HEAD host model.
#include <stdio.h>
#include <string.h>
#define private public
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CFrontEndDef.h"
#include "engine/CRGBColour.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontManager.h"
#include "engine/CASoundManager.h"
extern CDisplayManager* GPDisplayManager;

void CNewFrontendGameComponent::Shutdown()
{
    m_pFrontEndDef.Reset(0);
    CStartupDefinitionManager::Set(0);
    m_pNewFrontendGameComponent = 0;
    CleanUpInterfaces();
    PFontManager->SetLoadFontGlyphsAsynchronously(true);
    if (!m_bCreateGame && PSoundManager) {
        PSoundManager->StopAllSounds();
        PSoundManager->StopMusic(0);
        PSoundManager->UnloadAllBanks();
        PSoundManager->CloseAllBanks();
    }
    ReleaseXMVCode();
    FileNames.clear();
    FileTimes.clear();
    if (GPDisplayManager) {
        GPDisplayManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 7);
        GPDisplayManager->SwapScreens();
    }
}

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static double g_comp_store[(sizeof(CNewFrontendGameComponent) + 7) / 8];
static CNewFrontendGameComponent* const g_comp = (CNewFrontendGameComponent*)g_comp_store;
static double g_font_store[16];
static double g_display_store[(sizeof(CDisplayManager) + 7) / 8];
CNewFrontendGameComponent* CNewFrontendGameComponent::m_pNewFrontendGameComponent = 0;
CFontManager* PFontManager = (CFontManager*)g_font_store;
CASoundManager* PSoundManager = 0;
CDisplayManager* GPDisplayManager = 0;
static void* g_erase_first[2];
static void* g_erase_last[2];
static int g_erase_count = 0;

// The out-of-line callees, stubbed and logged.
void __fastcall CStartupDefinitionManager::Set(CStartupDefinitionManager* manager) { Log(manager ? "SetBAD" : "Set0"); }
void CNewFrontendGameComponent::CleanUpInterfaces() { Log(CNewFrontendGameComponent::Get() ? "SceneBAD" : "Scene"); }
void CNewFrontendGameComponent::ReleaseXMVCode() { Log("XMV"); }
template <> std::vector<CWideString>::iterator std::vector<CWideString>::erase(iterator first, iterator last)
{ Log("EraseNames"); if (g_erase_count < 2) { g_erase_first[g_erase_count] = first; g_erase_last[g_erase_count] = last; } ++g_erase_count; return first; }
template <> std::vector<CDateAndTime>::iterator std::vector<CDateAndTime>::erase(iterator first, iterator last)
{ Log("EraseTimes"); if (g_erase_count < 2) { g_erase_first[g_erase_count] = first; g_erase_last[g_erase_count] = last; } ++g_erase_count; return first; }
void CDisplayManager::ClearRenderTarget(const CRGBColour& c, unsigned long flags)
{ Log(c.R == 0 && c.G == 0 && c.B == 0 && c.A == 0xff && flags == 7 ? "Clear" : "ClearBAD"); }
bool CDisplayManager::SwapScreens() { Log("Swap"); return true; }
void CASoundManager::UnloadAllBanks() { Log("Unload"); }
void CASoundManager::CloseAllBanks() { Log("Close"); }
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CASoundManager::~CASoundManager() {}
void CASoundManager::Update() {}
bool CASoundManager::CanAllocateEmitter() { return false; }
void CASoundManager::SetExclusiveMode(bool) {}
bool CASoundManager::GetExclusiveMode() const { return false; }
void CASoundManager::SetHeadRelativePosition(const C3DVector&) {}
void CASoundManager::SetHeadroomBits(unsigned long) {}
// The def's base chain (CDefPointeeBase : CResource : CIVCountedPointeeBase): out-of-line members stubbed.
CResource::CResource() {}
CResource::~CResource() {}
bool CResource::UnloadResource() { return false; }
unsigned long CResource::GetUnloadableMemory() const { return 0; }
bool CResource::IsResourceLoaded() const { return false; }
void CResource::OnResourceLoaded() {}
bool CResource::CanAbortResourceLoad(long) const { return false; }
void CResource::OnResourceLoadAborted() {}
unsigned long CResource::GetDebugMemoryInUse() const { return 0; }
void CResource::ResetDebugMemoryInUse() {}
void CIVCountedPointeeBase::Delete() const { Log("DelBAD"); }
struct TestDef : CDefPointeeBase {
    void Delete() const { Log("DefDelete"); }
};
struct Sound : CASoundManager {
    CASoundBank* CreateSoundBank(const CWideString&) { return 0; }
    void SetEnableAtmos(bool) { }
    CASoundEmitter* Create3DEmitter(const C3DVector&) { return 0; }
    void DestroyPEmitter(CASoundEmitter*) { }
    void SetOutputConfigurationToHeadphones(bool) { }
    bool IsOutputConfigurationSetToHeadphones() { return false; }
    CASoundEmitter& Get2DEmitter() { return *static_cast<CASoundEmitter*>(0); }
    unsigned long GetNoFreeEmitters() const { return 0; }
    CASoundReceiver& GetReceiver() { return *static_cast<CASoundReceiver*>(0); }
    void StopSound(unsigned long) { }
    bool IsSoundPlaying(unsigned long) const { return false; }
    void StopAllSounds() { Log("StopAll"); }
    void PlayMusic(const CWideString&, unsigned long, unsigned long, float, float, long) { }
    void StopMusic(unsigned long fadeOutMS) { Log(fadeOutMS == 0 ? "StopMusic0" : "StopMusicBAD"); }
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

static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s (log %s)\n", what, g_log); ++g_fails; } }
static unsigned long* Word(unsigned long offset) { return (unsigned long*)((unsigned char*)g_comp_store + offset); }
static void Reset()
{
    g_log[0] = 0; g_erase_count = 0;
    *Word(0x6C) = 0x1000; *Word(0x70) = 0x1008; *Word(0x74) = 0x1010;     // FileNames begin / end / capacity
    *Word(0x78) = 0x2000; *Word(0x7C) = 0x201C; *Word(0x80) = 0x2038;     // FileTimes
    PFontManager->LoadFontGlyphsAsynchronously = false;
    CNewFrontendGameComponent::m_pNewFrontendGameComponent = g_comp;
}

int main()
{
    Sound sound;
    TestDef def;
    {   // no game being created, a sound manager and a display: the whole teardown; last def reference -> Delete
        Reset();
        def.IVRefCount = 1; g_comp->m_pFrontEndDef.Object = &def; g_comp->m_bCreateGame = false;
        PSoundManager = &sound; GPDisplayManager = (CDisplayManager*)g_display_store;
        Check((void*)g_comp->FileNames.begin() == (void*)0x1000 && (void*)g_comp->FileTimes.end() == (void*)0x201C, "vector words at +0x6C / +0x78");
        g_comp->Shutdown();
        Check(strcmp(g_log, "DefDelete;Set0;Scene;StopAll;StopMusic0;Unload;Close;XMV;EraseNames;EraseTimes;Clear;Swap;") == 0, "order: full teardown");
        Check(g_comp->m_pFrontEndDef.Object == 0 && def.IVRefCount == 0, "def released and cleared");
        Check(CNewFrontendGameComponent::Get() == 0, "static instance cleared");
        Check(PFontManager->LoadFontGlyphsAsynchronously == true && ((unsigned char*)g_font_store)[0x38] == 1, "font manager +0x38 raised");
        Check(g_erase_first[0] == (void*)0x1000 && g_erase_last[0] == (void*)0x1008, "FileNames erase(begin, end)");
        Check(g_erase_first[1] == (void*)0x2000 && g_erase_last[1] == (void*)0x201C, "FileTimes erase(begin, end)");
    }
    {   // a game being created: no sound teardown; def still referenced -> no Delete; no display: no clear / swap
        Reset();
        def.IVRefCount = 2; g_comp->m_pFrontEndDef.Object = &def; g_comp->m_bCreateGame = true;
        PSoundManager = &sound; GPDisplayManager = 0;
        g_comp->Shutdown();
        Check(strcmp(g_log, "Set0;Scene;XMV;EraseNames;EraseTimes;") == 0, "order: game being created, no display");
        Check(g_comp->m_pFrontEndDef.Object == 0 && def.IVRefCount == 1, "def reference dropped, not deleted");
    }
    {   // no game being created but no sound manager; no def: nothing to release
        Reset();
        g_comp->m_pFrontEndDef.Object = 0; g_comp->m_bCreateGame = false;
        PSoundManager = 0; GPDisplayManager = (CDisplayManager*)g_display_store;
        g_comp->Shutdown();
        Check(strcmp(g_log, "Set0;Scene;XMV;EraseNames;EraseTimes;Clear;Swap;") == 0, "order: no sound manager, no def");
        Check(g_comp->m_pFrontEndDef.Object == 0, "def stays null");
    }
    printf(g_fails ? "0042ebb6_TEST FAIL\n" : "0042ebb6_TEST PASS\n");
    return g_fails ? 1 : 0;
}
