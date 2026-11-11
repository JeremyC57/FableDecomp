#ifndef FABLE_ENGINE_CPARTICLEEMITTERDATABASE_H
#define FABLE_ENGINE_CPARTICLEEMITTERDATABASE_H
#include "engine/CSymbolMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CVectorMap.h"
#include "fable_string.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CParticleEmitter;class CParticleDataBank;
class CParticleEmitterDatabase {
public:long GetEmitterTemplateHandleFromName(const CCharString&)const;
protected:
 CSymbolMap SymbolMap;bool UsingSymbolMap;
 CVectorMap<long,CCountedPointer<CParticleEmitter>,CKeyPairCompareLess<long,CCountedPointer<CParticleEmitter> > > Emitters;
 CCountedPointer<CParticleDataBank> EmitterBank;long CurrentHandle;CCharString ParFilename,HeaderFilename;
};
}
#endif
