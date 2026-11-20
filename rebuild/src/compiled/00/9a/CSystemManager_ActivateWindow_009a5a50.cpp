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