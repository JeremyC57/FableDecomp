#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 917a7ad3cba9e8233494347dcf9246aed685d4444ead3498dee19473aa8b3404
// 009a5a50 CSystemManager::ActivateWindow
// Flags: /O2 /Oy
// Layout: retail bool9/10 input+88 sound+124, EverActive+220 Restore+234, SoundRestored+364 SavedState+368; PDB confirms first fields.
class CInputManager { public:void SetHasInputFocus(bool);void ClearEvents();};
// NF127 (2026-11-20, q4308): the local sound-interface stand-in (46 filler slots, volume / state getters and the address-named music setter)
//   replaced by the shared engine/CASoundManager.h: slot 46 SetGainFader (+0xB8), slot 47 GetGainFader (+0xBC), slot 49 GetMusicOffset(float&)
//   (+0xC4) and EnableMusic 0x009915E0 (Fable Anniversary 360 PDB name, opt-in FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES, the spelling the
//   landed self row and 0x006286F0 bind); SavedState is the music offset (float).
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES
#include "engine/CASoundManager.h"
class CSystemManager { public:char prefix[9];bool ApplicationActive,ApplicationHasInputFocus;char pad11[77];CInputManager*input;char pad92[32];CASoundManager*sound;char pad128[92];bool EverBeenActive;char pad221[13];bool Restore;char pad235[129];bool SoundRestored;char pad365[3];float SavedState;static void __fastcall ActivateWindow(CSystemManager*,bool);};
extern float gInactiveVolume_013ca60c;extern bool gSoundSuspended_013ca610;
void __fastcall CSystemManager::ActivateWindow(CSystemManager*self,bool active){if(active&&!self->ApplicationActive&&self->EverBeenActive==true)self->Restore=true;self->ApplicationActive=active;self->EverBeenActive=true;if(active){self->ApplicationHasInputFocus=true;if(self->input)self->input->SetHasInputFocus(true);if(self->input)self->input->ClearEvents();if(self->sound&&gSoundSuspended_013ca610){self->sound->SetGainFader(0x8000,gInactiveVolume_013ca60c,0);self->sound->EnableMusic(true);self->SoundRestored=true;gSoundSuspended_013ca610=false;}return;}self->ApplicationHasInputFocus=false;if(self->input)self->input->SetHasInputFocus(false);if(self->sound&&!gSoundSuspended_013ca610){gInactiveVolume_013ca60c=self->sound->GetGainFader(0x8000);self->sound->SetGainFader(0x8000,0,0);if(!self->sound->GetMusicOffset(self->SavedState))self->SavedState=0;self->sound->EnableMusic(false);gSoundSuspended_013ca610=true;}if(self->input)self->input->ClearEvents();}
// END exact source snapshot
float gInactiveVolume_013ca60c=0;bool gSoundSuspended_013ca610=false,focus=false,playing=true;int clears=0;
void CInputManager::SetHasInputFocus(bool x){focus=x;}void CInputManager::ClearEvents(){++clears;}
// The shared CASoundManager's out-of-line members, stubbed for the test (NF127 rewrite on engine/CASoundManager.h; the behaviour checks are unchanged).
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CASoundManager::~CASoundManager(){}
void CASoundManager::Update(){}
bool CASoundManager::CanAllocateEmitter(){return false;}
void CASoundManager::SetExclusiveMode(bool){}
bool CASoundManager::GetExclusiveMode() const{return false;}
void CASoundManager::SetHeadRelativePosition(const C3DVector&){}
void CASoundManager::SetHeadroomBits(unsigned long){}
void CASoundManager::EnableMusic(bool x){playing=x;}
struct Sound:CASoundManager{
    float volume;
    CASoundBank* CreateSoundBank(const CWideString& fileName) { return 0; }
    void SetEnableAtmos(bool enable) { }
    CASoundEmitter* Create3DEmitter(const C3DVector& position) { return 0; }
    void DestroyPEmitter(CASoundEmitter* emitter) { }
    void SetOutputConfigurationToHeadphones(bool headphones) { }
    bool IsOutputConfigurationSetToHeadphones() { return false; }
    CASoundEmitter& Get2DEmitter() { return *static_cast<CASoundEmitter*>(0); }
    unsigned long GetNoFreeEmitters() const { return 0; }
    CASoundReceiver& GetReceiver() { return *static_cast<CASoundReceiver*>(0); }
    void StopSound(unsigned long sampleID) { }
    bool IsSoundPlaying(unsigned long sampleID) const { return false; }
    void StopAllSounds() { }
    void PlayMusic(const CWideString& fileName, unsigned long fadeOutMS, unsigned long fadeInMS, float gain, float offset, long loopCount) { }
    void StopMusic(unsigned long fadeOutMS) { }
    void SetOutputPCMBufferCount(unsigned long count) { }
    void SetOutputPCMBufferSampleSize(unsigned long size) { }
    void SetOutputPCMPreloadBufferCount(unsigned long count) { }
    void SetDiskByteGranularity(unsigned long granularity) { }
    void SetDiskFrontBufferCount(unsigned long count) { }
    void SetDiskBackBufferCount(unsigned long count) { }
    void SetAtmosMapBlendTime(float time) { }
    void SetDistanceModelRolloff(float rolloff) { }
    void SetAtmosSFXDampingDistance(float distance) { }
    void SetAtmosSFXDampingWeight(float weight) { }
    void SetReverbParameters(const CReverbParameters& parameters) { }
    void SetReverbLevel(float level, float time) { }
    void SetReverbTailWaitTimeMS(float time) { }
    void SetReverbFadeUpTimeMS(float time) { }
    void SetReverbFadeDownTimeMS(float time) { }
    void SetHRTFHemisphereMirroring(bool mirroring) { }
    void SetMonitorAllowedPostStopTailDurationMS(unsigned long duration) { }
    void InitAtmos(CAtmosMap* map, float dampingWeight, float dampingDistance, long groupMask) { }
    bool NewAtmos(const CCharString& name, long groupMask) { return false; }
    bool AddAtmos2Map(const CCharString& name, long index, float a, float b, float c) { return false; }
    bool NewChannel(const CCharString& atmosName, const CCharString& channelName, long sample, long a, float b) { return false; }
    bool SetChannelGain(const CCharString& atmosName, const CCharString& channelName, float gain, float time) { return false; }
    void DeleteAllAtmoses() { }
    void StopAllAtmos() { }
    void SendLibCommand(const CCharString& command) { }
    void SetPlatform2DGain(float gain) { }
    void SetPitchFader(unsigned long groupMask, float pitch) { }
    void SetGainFader(unsigned long groupMask, float gain, float fadeTime) { volume = gain; }
    float GetGainFader(unsigned long groupMask) { return volume; }
    float GetInstantGainFader(unsigned long groupMask) { return 0.0f; }
    bool GetMusicOffset(float& offset) { offset = 1.0f; return false; }
    void FreezeAtmosMaps(bool freeze) { }
    bool ExtractMetaData(const CWideString& fileName, long flags) { return false; }
};
int main(){CSystemManager s;memset(&s,0,sizeof s);CInputManager input;Sound sound;sound.volume=0.5f;s.input=&input;s.sound=&sound;CSystemManager::ActivateWindow(&s,false);assert(sound.volume==0&&gSoundSuspended_013ca610&&!playing&&s.SavedState==0);CSystemManager::ActivateWindow(&s,true);assert(sound.volume==0.5f&&!gSoundSuspended_013ca610&&playing&&focus&&s.Restore&&s.SoundRestored);s.input=0;s.sound=0;CSystemManager::ActivateWindow(&s,false);assert(!s.ApplicationActive);puts("009a5a50_TEST PASS");}
