#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERDATABASE_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERDATABASE_H
#include "engine/CSymbolMap.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CCharString.h"
class CAFile;
namespace NParticleEngine {
class CParticleEmitter;class CParticleDataBank;class CEmitterDatabaseSerialiseInfo;
// Layout: original Ego_r TPI3eb4b, native ctor00ae8bf0 and Load stores;
// SymbolMap0, UsingSymbolMap24, Emitters28, EmitterBank38, CurrentHandle40,
// ParFilename44, HeaderFilename48. Every original member is retained.
class CParticleEmitterDatabase {
public:
 CParticleEmitter* CreateEmitterTemplate(long&);
 void DeleteAllEmitterTemplates(){Emitters.clear();CurrentHandle=1;}
protected:
 void Load(CAFile&,const CEmitterDatabaseSerialiseInfo&);
 CSymbolMap SymbolMap;
 bool UsingSymbolMap;
 CVectorMap<long,CCountedPointer<CParticleEmitter> > Emitters;
 CCountedPointer<CParticleDataBank> EmitterBank;
 long CurrentHandle;
 CCharString ParFilename,HeaderFilename;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitterDatabase)==0x4c);
#endif
