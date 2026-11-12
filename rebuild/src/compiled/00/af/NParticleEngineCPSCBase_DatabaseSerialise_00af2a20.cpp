// NParticleEngine::CPSCBase::DatabaseSerialise(CPersistContext&, NParticleEngine::CEmitterDatabaseSerialiseInfo const &) -- does nothing
// Leaf: ?DatabaseSerialise@CPSCBase@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A20, thiscall, `ret 8`.
// Identity: slot 17 of NParticleEngine::CPSCBase's vtable 0x0129F3F4 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts DatabaseSerialise(CPersistContext &, NParticleEngine::CEmitterDatabaseSerialiseInfo const &) at this slot; Ego_r/FableWin name set: ?DatabaseSerialise@CPSCBase@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z; `ret` matches the signature.
class CPersistContext;
namespace NParticleEngine { class CEmitterDatabaseSerialiseInfo; }
namespace NParticleEngine {
class CPSCBase {
public:
    virtual void DatabaseSerialise(CPersistContext&, NParticleEngine::CEmitterDatabaseSerialiseInfo const &);
};
void CPSCBase::DatabaseSerialise(CPersistContext&, NParticleEngine::CEmitterDatabaseSerialiseInfo const &)
{
}
}
