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
