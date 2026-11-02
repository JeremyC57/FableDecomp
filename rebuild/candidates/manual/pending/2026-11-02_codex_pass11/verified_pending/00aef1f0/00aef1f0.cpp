// NParticleEngine::CParticleEmitter::DatabaseSerialise(CPersistContext &,NParticleEngine::CEmitterDatabaseSerialiseInfo const &) -- transfer the particle emitter database state.
// Leaf: ?DatabaseSerialise@CParticleEmitter@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
#define FABLE_RETAIL_PARTICLE_SERIALISE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/CParticleEmitter.h"
#include "engine/CParticleSystem.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
namespace NParticleEngine {
void CParticleEmitter::DatabaseSerialise(CPersistContext& context,const CEmitterDatabaseSerialiseInfo& info) {
 if(context.IsSaving())return;
 context.Transfer("EmitterName",Name);
 {bool temp=Emitter2D;context.Transfer("Emitter2D",temp,false);Emitter2D=temp;}
 {bool temp=PreWaterEffect;context.Transfer("PreWaterEffect",temp,false);PreWaterEffect=temp;}
 {bool temp=WaterEffect;context.Transfer("WaterEffect",temp,false);WaterEffect=temp;}
 {bool temp=ZBufferWriteable;context.Transfer("ZBufferWriteable",temp,false);ZBufferWriteable=temp;}
 {bool temp=DieIfOffsceen;context.Transfer("DieIfOffsceen",temp,true);DieIfOffsceen=temp;}
 {bool temp=OffscreenUpdate;context.Transfer("OffscreenUpdate",temp,false);OffscreenUpdate=temp;}
 {bool temp=ClipEffectToWeatherMask;context.Transfer("ClipEffectToWeatherMask",temp,false);ClipEffectToWeatherMask=temp;}
 {bool temp=EnableDithering;context.Transfer("EnableDithering",temp,true);EnableDithering=temp;}
 {bool temp=CalcBoundingSphereOnceOnly;context.Transfer("CalcBoundingSphereOnceOnly",temp,false);CalcBoundingSphereOnceOnly=temp;}
 if(context.IsSaving()) {
 {bool temp=ContinuousEmitter;context.Transfer("ContinuousEmitter",temp,false);ContinuousEmitter=temp;}
 {bool temp=IsScreenDisplacement;context.Transfer("IsScreenDisplacement",temp,false);IsScreenDisplacement=temp;}
 {bool temp=ReadZBuffer;context.Transfer("ReadZBuffer",temp,true);ReadZBuffer=temp;}
  context.Transfer("MaxSpawnDistance",MaxSpawnDistance,0.0f);
  context.Transfer("MaxDrawDistance",MaxDrawDistance,0.0f);
  context.Transfer("FadeStart",FadeOutStart,0.0f);
  context.Transfer("FadeInEnd",FadeInEnd,0.0f);
  context.Transfer("FadeInStart",FadeInStart,0.0f);
  long pri=Priority;context.Transfer("Priority",pri,0L);
  for(std::vector<CCountedPointer<CParticleSystem> >::iterator it_system=ParticleSystems.begin();it_system!=ParticleSystems.end();++it_system) {
   context.WriteString("BEGIN_SYSTEM\r\n");
   (*it_system)->DatabaseSerialise(context,info);
   context.WriteString("END_SYSTEM\r\n");
  }
 } else {
  CStringParser& parser=*context.GetPParser();
 {bool temp=ContinuousEmitter;context.Transfer("ContinuousEmitter",temp,false);ContinuousEmitter=temp;}
 {bool temp=IsScreenDisplacement;context.Transfer("IsScreenDisplacement",temp,false);IsScreenDisplacement=temp;}
 {bool temp=ReadZBuffer;context.Transfer("ReadZBuffer",temp,true);ReadZBuffer=temp;}
  context.Transfer("MaxSpawnDistance",MaxSpawnDistance,GetMaxSpawnDistance());
  context.Transfer("MaxDrawDistance",MaxDrawDistance,GetMaxDrawDistance());
  context.Transfer("FadeStart",FadeOutStart,GetFadeOutStartDistance());
  context.Transfer("FadeInEnd",FadeInEnd,GetFadeInEndDistance());
  context.Transfer("FadeInStart",FadeInStart,GetFadeInStartDistance());
  long pri;context.Transfer("Priority",pri,0L);Priority=pri;
  while(parser.SkipPastString("BEGIN_SYSTEM")) {
   CCountedPointer<CParticleSystem> psystem=new CParticleSystem;
   psystem->SetParentEmitter(*this);
   CCharString system_script=parser.ReadAsStringUntilString("END_SYSTEM");
   CStringParser system_parser(system_script);
   CPersistContext context2(system_parser,0,true,0,0,&system_script);
   psystem->DatabaseSerialise(context2,info);
   ParticleSystems.push_back(psystem);
  }
  MaxDrawDistance=(std::min)(GetMaxDrawDistance(),MaxDrawDistance);
  MaxSpawnDistance=(std::min)(FadeOutStart,MaxSpawnDistance);
  FadeOutStart=(std::min)(MaxDrawDistance,FadeOutStart);
  FadeInEnd=(std::min)(FadeOutStart,FadeInEnd);
  FadeInStart=(std::min)(FadeInEnd,FadeInStart);
 }
}
}
