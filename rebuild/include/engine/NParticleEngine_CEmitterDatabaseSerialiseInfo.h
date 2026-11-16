#ifndef FABLE_ENGINE_NPARTICLEENGINE_CEMITTERDATABASESERIALISEINFO_H
#define FABLE_ENGINE_NPARTICLEENGINE_CEMITTERDATABASESERIALISEINFO_H
class CSymbolMap;
namespace NParticleEngine {
// Both original PDBs: two const CSymbolMap references; original two-reference ctor.
class CEmitterDatabaseSerialiseInfo {public:
 CEmitterDatabaseSerialiseInfo(const CSymbolMap&g,const CSymbolMap&m):GraphicSymbols(g),MeshSymbols(m){}
 const CSymbolMap& GraphicSymbols;
 const CSymbolMap& MeshSymbols;
};
}
#endif
