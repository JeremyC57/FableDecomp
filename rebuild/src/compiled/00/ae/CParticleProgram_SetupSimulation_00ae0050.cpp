// CParticleProgram::SetupSimulation  retail 0x00ae0050
class CMovableResourceMemoryPool;

class CParticleSimulation { public:
    // external method at 0x00adf610: char __fastcall(void* progField, pool*)
    char Init(void* progField, CMovableResourceMemoryPool* pool);
};

class CParticleProgram { public:
    char pad[8]; // this+8 is the field passed to the simulation
    bool SetupSimulation(CParticleSimulation* sim, CMovableResourceMemoryPool* pool);
};

bool CParticleProgram::SetupSimulation(CParticleSimulation* sim, CMovableResourceMemoryPool* pool)
{
    return sim->Init((char*)this + 8, pool);
}