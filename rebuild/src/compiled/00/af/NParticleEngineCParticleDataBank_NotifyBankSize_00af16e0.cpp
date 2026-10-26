// NParticleEngine::CParticleDataBank::NotifyBankSize(unsigned long count, void* typeCounts) -- resizes the ParticleEmitters array to `count` default (null) entries, then stamps every entry with its owning bank and its index
// Leaf: NotifyBankSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF16E0, length 91 bytes (`ret 8`; the second
//         argument, the bank's CArray<CBankFileDataTypeCount>*, is not read).
// Callees: CArray<NParticleEngine::CParticleEmitterInfo>::Resize(count, const value&) @ 0x00AF2170.
// Layout: PDB NParticleEngine::CParticleEmitterInfo is 12 bytes -- Data (CIVCountedPointer) +0,
// Bank +4, Index +8 -- and ParticleEmitters is a CArray of them (PDB +0x254; retail +0x1e0, the
// retail base classes are 0x74 bytes narrower). No generated header exists for the bank.
// Shape: the fill value is a 12-byte frame temporary zeroed with three immediate stores scheduled
// after the argument pushes; the loop counts `i` in ecx against the unsigned count, keeps the byte
// offset in edx, reaches the element once per pass through a reference local (two operator[]
// uses re-read the data pointer twice) -- the data pointer is still re-read every pass (the
// element stores may alias it) --
// stores Index then Bank.

namespace NParticleEngine {

class CParticleEmitterInfo { public:
    void*         Data;                                                   // +0 CIVCountedPointer<CEmitterData>
    void*         Bank;                                                   // +4 CParticleDataBank*
    unsigned long Index;                                                  // +8
    CParticleEmitterInfo() : Data(0), Bank(0), Index(0) {}
};

} // namespace NParticleEngine

struct CParticleEmitterInfoArray {                                        // retail CArray<CParticleEmitterInfo> (12 bytes)
    NParticleEngine::CParticleEmitterInfo* m_pData;
    long                                   m_size;
    long                                   m_capacity;
    void Resize(unsigned long count, const NParticleEngine::CParticleEmitterInfo& value);   // 0x00AF2170
    NParticleEngine::CParticleEmitterInfo& operator[](unsigned long i) { return m_pData[i]; }
};

struct CParticleDataBank {
    virtual void v00();
    unsigned char             _base[0x1dc];                               // +0x04 bank base classes
    CParticleEmitterInfoArray ParticleEmitters;                           // +0x1e0
    void NotifyBankSize(unsigned long count, void* typeCounts);
};

void CParticleDataBank::NotifyBankSize(unsigned long count, void* typeCounts)
{
    ParticleEmitters.Resize(count, NParticleEngine::CParticleEmitterInfo());
    for (unsigned long i = 0; i < count; ++i) {
        NParticleEngine::CParticleEmitterInfo& info = ParticleEmitters[i];
        info.Index = i;
        info.Bank = this;
    }
}