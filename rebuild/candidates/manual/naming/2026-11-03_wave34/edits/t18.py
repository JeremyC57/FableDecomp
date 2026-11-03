P="rebuild/src/compiled/00/ae/CParticleEmitterDatabase_GenerateHeaderFile_00ae8810.cpp"
EDITS=[
(P,"""class CParticleEmitter { public:
    void GenerateHeader(CPersistContext& context, long options);   // 0x00AEF1F0
};""","""class CEmitterDatabaseSerialiseInfo;
class CParticleEmitter { public:
    void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);   // 0x00AEF1F0 (Ego_r / FableWin ?DatabaseSerialise@CParticleEmitter@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z)
};"""),
(P,"    void GenerateHeaderFile(const CCharString& filename, long options);","    void GenerateHeaderFile(const CCharString& filename, const NParticleEngine::CEmitterDatabaseSerialiseInfo& info);"),
(P,"void CParticleEmitterDatabase::GenerateHeaderFile(const CCharString& filename, long options)","void CParticleEmitterDatabase::GenerateHeaderFile(const CCharString& filename, const NParticleEngine::CEmitterDatabaseSerialiseInfo& info)"),
(P,"it->Emitter->GenerateHeader(context, options);","it->Emitter->DatabaseSerialise(context, info);"),
]
