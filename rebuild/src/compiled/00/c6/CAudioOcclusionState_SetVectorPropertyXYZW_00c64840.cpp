// CAudioOcclusionState::SetOcclusion(parameters) -- validates and updates EAX-style occlusion parameters.
// Retail00c64840 length245; propagated CEntity/SetVectorProperty_XYZW label is wrong.
// Caller00c6859f validates16-byte payload. Adjacent00c647b0 accepts8-byte obstruction,
// 00c64940 accepts8-byte exclusion. Float pools122dedc=0,122ded8=1,1237f0c=10.
// Native fields dirty+4, owning audio context+14, occlusion record+30; context error+3d4.
// No applicable engine header for this third-party audio state is available.
struct CAudioPropertyContext {unsigned char UnrelatedAudioState[0x3d4];int Error;};
struct COcclusionParameters {int Level;float LowFrequencyRatio,RoomRatio,DirectRatio;};
struct CAudioOcclusionState {
 unsigned char UnrelatedPrefix[4];unsigned int DirtyFlags;unsigned char UnrelatedProperties[12];
 CAudioPropertyContext* Context;unsigned char EarlierProperties[0x18];COcclusionParameters Occlusion;
 int SetOcclusion(const COcclusionParameters& p);
};
int CAudioOcclusionState::SetOcclusion(const COcclusionParameters& p){
 if(p.Level < -10000 || p.Level > 0 || p.LowFrequencyRatio < 0.0f || p.LowFrequencyRatio > 1.0f ||
 p.RoomRatio < 0.0f || p.RoomRatio > 10.0f || p.DirectRatio < 0.0f || p.DirectRatio > 10.0f){
 Context->Error=-2;return -3;
 }
 if(p.Level!=Occlusion.Level || p.LowFrequencyRatio!=Occlusion.LowFrequencyRatio || p.RoomRatio!=Occlusion.RoomRatio || p.DirectRatio!=Occlusion.DirectRatio){
 Occlusion.Level=p.Level;Occlusion.LowFrequencyRatio=p.LowFrequencyRatio;Occlusion.RoomRatio=p.RoomRatio;Occlusion.DirectRatio=p.DirectRatio;DirtyFlags|=0x400000;
 }
 return 0;
}