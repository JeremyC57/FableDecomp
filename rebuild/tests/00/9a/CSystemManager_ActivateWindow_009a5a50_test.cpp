#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 0016d3c68c9f7f1c222c53135e3c6f82449e9e36e11a01fc7cef4e74b76e9c1c
// 009a5a50 CSystemManager::ActivateWindow
// Flags: /O2 /Oy
// Layout: retail bool9/10 input+88 sound+124, EverActive+220 Restore+234, SoundRestored+364 SavedState+368; PDB confirms first fields.
struct CInputManager {void SetHasInputFocus(bool);void ClearEvents();};
struct SoundInterface {
virtual void Slot00(){}
virtual void Slot01(){}
virtual void Slot02(){}
virtual void Slot03(){}
virtual void Slot04(){}
virtual void Slot05(){}
virtual void Slot06(){}
virtual void Slot07(){}
virtual void Slot08(){}
virtual void Slot09(){}
virtual void Slot10(){}
virtual void Slot11(){}
virtual void Slot12(){}
virtual void Slot13(){}
virtual void Slot14(){}
virtual void Slot15(){}
virtual void Slot16(){}
virtual void Slot17(){}
virtual void Slot18(){}
virtual void Slot19(){}
virtual void Slot20(){}
virtual void Slot21(){}
virtual void Slot22(){}
virtual void Slot23(){}
virtual void Slot24(){}
virtual void Slot25(){}
virtual void Slot26(){}
virtual void Slot27(){}
virtual void Slot28(){}
virtual void Slot29(){}
virtual void Slot30(){}
virtual void Slot31(){}
virtual void Slot32(){}
virtual void Slot33(){}
virtual void Slot34(){}
virtual void Slot35(){}
virtual void Slot36(){}
virtual void Slot37(){}
virtual void Slot38(){}
virtual void Slot39(){}
virtual void Slot40(){}
virtual void Slot41(){}
virtual void Slot42(){}
virtual void Slot43(){}
virtual void Slot44(){}
virtual void Slot45(){}
virtual void SetVolume(int,float,int)=0;virtual float GetVolume(int)=0;virtual void Slot48(){}virtual bool GetState(int*)=0;void SetPlayback_009915e0(bool);
};
struct CSystemManager {char prefix[9];bool ApplicationActive,ApplicationHasInputFocus;char pad11[77];CInputManager*input;char pad92[32];SoundInterface*sound;char pad128[92];bool EverBeenActive;char pad221[13];bool Restore;char pad235[129];bool SoundRestored;char pad365[3];int SavedState;static void __fastcall ActivateWindow(CSystemManager*,bool);};
extern float gInactiveVolume_013ca60c;extern bool gSoundSuspended_013ca610;
void __fastcall CSystemManager::ActivateWindow(CSystemManager*self,bool active){if(active&&!self->ApplicationActive&&self->EverBeenActive==true)self->Restore=true;self->ApplicationActive=active;self->EverBeenActive=true;if(active){self->ApplicationHasInputFocus=true;if(self->input)self->input->SetHasInputFocus(true);if(self->input)self->input->ClearEvents();if(self->sound&&gSoundSuspended_013ca610){self->sound->SetVolume(0x8000,gInactiveVolume_013ca60c,0);self->sound->SetPlayback_009915e0(true);self->SoundRestored=true;gSoundSuspended_013ca610=false;}return;}self->ApplicationHasInputFocus=false;if(self->input)self->input->SetHasInputFocus(false);if(self->sound&&!gSoundSuspended_013ca610){gInactiveVolume_013ca60c=self->sound->GetVolume(0x8000);self->sound->SetVolume(0x8000,0,0);if(!self->sound->GetState(&self->SavedState))self->SavedState=0;self->sound->SetPlayback_009915e0(false);gSoundSuspended_013ca610=true;}if(self->input)self->input->ClearEvents();}

// END exact source snapshot
float gInactiveVolume_013ca60c=0;bool gSoundSuspended_013ca610=false,focus=false,playing=true;int clears=0;
void CInputManager::SetHasInputFocus(bool x){focus=x;}void CInputManager::ClearEvents(){++clears;}void SoundInterface::SetPlayback_009915e0(bool x){playing=x;}
struct Sound:SoundInterface{float volume;void SetVolume(int,float v,int){volume=v;}float GetVolume(int){return volume;}bool GetState(int*){return false;}};
int main(){CSystemManager s;memset(&s,0,sizeof s);CInputManager input;Sound sound;sound.volume=0.5f;s.input=&input;s.sound=&sound;CSystemManager::ActivateWindow(&s,false);assert(sound.volume==0&&gSoundSuspended_013ca610&&!playing&&s.SavedState==0);CSystemManager::ActivateWindow(&s,true);assert(sound.volume==0.5f&&!gSoundSuspended_013ca610&&playing&&focus&&s.Restore&&s.SoundRestored);s.input=0;s.sound=0;CSystemManager::ActivateWindow(&s,false);assert(!s.ApplicationActive);puts("009a5a50_TEST PASS");}