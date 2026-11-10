#include <stdio.h>
#include <string.h>
#include <new>
#ifndef FABLE_ENGINE_LHA_CEVENTPLAYOPTIONS_H
#define FABLE_ENGINE_LHA_CEVENTPLAYOPTIONS_H
// Retail opt-in (Codex pass 17): original Ego_r LHA class records, exact member types and offsets.
namespace LHA {
class CVector {public:float X,Y,Z;CVector():X(0),Y(0),Z(0){}};
class CResourceOptions {public:
 float Gain,Pitch,PitchVar,MinDist,MaxDist;
 unsigned int Priority;int LoopCount;unsigned short GainSendFaders,PitchSendFaders;unsigned int Group;bool Interruptable;float Probability;
 CResourceOptions():Gain(1),Pitch(1),PitchVar(0),MinDist(1),MaxDist(20),Priority(1),LoopCount(0),GainSendFaders(0),PitchSendFaders(0),Group(0),Interruptable(true),Probability(1){}
};
class CEventPlayOptions {public:
 unsigned int EmitterId,UserData;float Gain,Pitch;bool Is3D;CVector Pos;float Pan;bool HeadRelative;
 unsigned short WhichParams,WhichCallbacks,WhichGainSendFaders,WhichPitchSendFaders;
 CResourceOptions Params;
 CEventPlayOptions();
};
}
#endif

LHA::CEventPlayOptions::CEventPlayOptions():EmitterId((unsigned int)-1),UserData(0),Gain(1),Pitch(1),Is3D(false),Pan(0),HeadRelative(false),WhichParams(0),WhichCallbacks(0),WhichGainSendFaders(0),WhichPitchSendFaders(0){}

int main(){
 for(int k=0;k<16;++k){
  union Storage{double align;unsigned char bytes[sizeof(LHA::CEventPlayOptions)];}storage;
  memset(storage.bytes,k*17,sizeof(storage.bytes));
  LHA::CEventPlayOptions* o=new(storage.bytes)LHA::CEventPlayOptions;
  if(o->EmitterId!=(unsigned int)-1||o->UserData||o->Gain!=1||o->Pitch!=1||o->Is3D||o->Pos.X||o->Pos.Y||o->Pos.Z||o->Pan||o->HeadRelative||o->WhichParams||o->WhichCallbacks||o->WhichGainSendFaders||o->WhichPitchSendFaders)return 1;
  const LHA::CResourceOptions& p=o->Params;
  if(p.Gain!=1||p.Pitch!=1||p.PitchVar||p.MinDist!=1||p.MaxDist!=20||p.Priority!=1||p.LoopCount||p.GainSendFaders||p.PitchSendFaders||p.Group||!p.Interruptable||p.Probability!=1)return 2;
  o->~CEventPlayOptions();
 }
 puts("00c0a790_TEST PASS");return 0;
}