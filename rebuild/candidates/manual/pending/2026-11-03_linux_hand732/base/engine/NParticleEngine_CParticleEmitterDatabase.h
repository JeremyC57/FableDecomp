#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERDATABASE_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERDATABASE_H
// NParticleEngine::CParticleEmitterDatabase (Ego_r TPI): SymbolMap +0x00, UsingSymbolMap +0x24, Emitters +0x28
// (CVectorMap<long,CCountedPointer<CParticleEmitter> >, 0x10), EmitterBank +0x38, CurrentHandle +0x40, ParFilename +0x44,
// HeaderFilename +0x48; sizeof 0x4C (ctor 0x00AE8BF0, Load 0x00AE8D50 stores). Load is protected (?Load@...@IAEXAAVCAFile@@...);
// CreateEmitterTemplate / DeleteAllEmitterTemplates public (Ego_r / FableWin; DeleteAllEmitterTemplates inline in retail).
#include "engine/CSymbolMap.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
class CAFile;
namespace NParticleEngine {
class CParticleEmitter; class CParticleDataBank; class CEmitterDatabaseSerialiseInfo;
class CParticleEmitterDatabase {
public:
    CParticleEmitter* CreateEmitterTemplate(long&);
    void DeleteAllEmitterTemplates() { Emitters.clear(); CurrentHandle=1; }
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
