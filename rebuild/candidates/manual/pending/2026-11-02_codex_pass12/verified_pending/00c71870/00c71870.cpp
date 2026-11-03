// CSalOpenALReverb::Update(const CCodaEnvironment*) -- submit changed environment parameters to EAX3.
// Leaf: ?Update@CSalOpenALReverb@@UAEXPBVCCodaEnvironment@@@Z
#include "engine/CSalOpenALReverb.h"
#include "engine/CSalOpenAL.h"
void CSalOpenALReverb::Update(const CCodaEnvironment* environment)
{
 unsigned int changed=environment->GetCommit();
 const CENVIRONMENT& params=environment->GetParams();
 if(!changed)return;
 if(changed&0x1)g_OpenAL.EAX3SetGlobal(5,(void*)&params.iLevel,sizeof(params.iLevel));
 if(changed&0x2)g_OpenAL.EAX3SetGlobal(6,(void*)&params.iLevelHF,sizeof(params.iLevelHF));
 if(changed&0x4)g_OpenAL.EAX3SetGlobal(7,(void*)&params.iLevelLF,sizeof(params.iLevelLF));
 if(changed&0x8)g_OpenAL.EAX3SetGlobal(8,(void*)&params.fDecayTime,sizeof(params.fDecayTime));
 if(changed&0x10)g_OpenAL.EAX3SetGlobal(9,(void*)&params.fDecayHFRatio,sizeof(params.fDecayHFRatio));
 if(changed&0x20)g_OpenAL.EAX3SetGlobal(10,(void*)&params.fDecayLFRatio,sizeof(params.fDecayLFRatio));
 if(changed&0x40)g_OpenAL.EAX3SetGlobal(11,(void*)&params.iReflections,sizeof(params.iReflections));
 if(changed&0x80)g_OpenAL.EAX3SetGlobal(12,(void*)&params.fReflectionsDelay,sizeof(params.fReflectionsDelay));
 if(changed&0x100)g_OpenAL.EAX3SetGlobal(13,(void*)&params.vReflectionsPan,sizeof(params.vReflectionsPan));
 if(changed&0x200)g_OpenAL.EAX3SetGlobal(14,(void*)&params.iReverb,sizeof(params.iReverb));
 if(changed&0x400)g_OpenAL.EAX3SetGlobal(15,(void*)&params.fReverbDelay,sizeof(params.fReverbDelay));
 if(changed&0x800)g_OpenAL.EAX3SetGlobal(16,(void*)&params.vReverbPan,sizeof(params.vReverbPan));
 if(changed&0x1000)g_OpenAL.EAX3SetGlobal(3,(void*)&params.fSize,sizeof(params.fSize));
 if(changed&0x2000)g_OpenAL.EAX3SetGlobal(4,(void*)&params.fDiffusion,sizeof(params.fDiffusion));
 if(changed&0x8000)g_OpenAL.EAX3SetGlobal(17,(void*)&params.fEchoTime,sizeof(params.fEchoTime));
 if(changed&0x10000)g_OpenAL.EAX3SetGlobal(18,(void*)&params.fEchoDepth,sizeof(params.fEchoDepth));
 if(changed&0x20000)g_OpenAL.EAX3SetGlobal(19,(void*)&params.fModulationTime,sizeof(params.fModulationTime));
 if(changed&0x40000)g_OpenAL.EAX3SetGlobal(20,(void*)&params.fModulationDepth,sizeof(params.fModulationDepth));
 if(changed&0x80000)g_OpenAL.EAX3SetGlobal(22,(void*)&params.uiHFReference,sizeof(params.uiHFReference));
 if(changed&0x80000)g_OpenAL.EAX3SetGlobal(23,(void*)&params.uiLFReference,sizeof(params.uiLFReference));
}
