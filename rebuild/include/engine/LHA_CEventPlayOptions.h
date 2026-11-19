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
#ifdef FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS
 void SetFlag16(unsigned long index,bool set); // 0x00C0A8A0 (descriptive name; landed as CBitMaskPair_00c0a8c0::SetFlag16, see NAME_FIXES.tsv) (WhichGainSendFaders +0x2a / Params.GainSendFaders +0x4c)
 void SetFlag(unsigned long index,bool set);   // 0x00C0A8C0 (descriptive name; landed as CBitMaskPair_00c0a8c0::SetFlag, see NAME_FIXES.tsv) (WhichPitchSendFaders +0x2c / Params.PitchSendFaders +0x4e)
#endif
#ifdef FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_PARAMSETTERS
 // Opt-in (hand3c2, row 0x00AABF60 CSoundEmitterLH::PlaySample): two of the out-of-line parameter setters (each stores one Params member
 // and its WhichParams bit): 0x00C0A860 `or byte [ecx+26h],20h ; mov [ecx+48h],eax` (Params.LoopCount) and 0x00C0A880 `or byte
 // [ecx+26h],80h ; mov [ecx+54h],al` (Params.Interruptable). No PDB holds an LHA method name: the names are descriptive (landed as
 // CAnimateThing::SetAsFinished, a propagated label). Declarations only.
 void SetLoopCount(int loopCount);              // 0x00C0A860 (descriptive)
 void SetInterruptable(bool interruptable);     // 0x00C0A880 (descriptive)
#endif
};
}
#endif
