// CBlendedParticleEffectSet::TransferBinaryOut(CDataOutputStream& out) -- write the two particle indices then the two blend values
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00469345, length 60 bytes.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (x2), CDataOutputStream::WriteFloat @ 0x00993EE0 (x2).
// Layout from Ego_r.pdb (16 bytes): Particle1Index +0, Particle1Value +4, Particle2Index +8,
// Particle2Value +0xc. The indices go out first (+0, +8), then the values (+4, +0xc); each float
// is pushed through the x87 (`fld ; push ecx ; fstp [esp]`). The longs are pushed straight from
// memory and the stream reference is loaded first: a size-optimised TU (`#pragma optimize("s",on)`).
#pragma optimize("s", on)
class CDataOutputStream { public:
    void WriteFloat(float value);               // 0x00993EE0
public:
    void WriteSLONG(long);   // 0x00993EB0
};

class CBlendedParticleEffectSet { public:
    long  Particle1Index;                       // +0x00
    float Particle1Value;                       // +0x04
    long  Particle2Index;                       // +0x08
    float Particle2Value;                       // +0x0c
    void TransferBinaryOut(CDataOutputStream& out) const;
};

void CBlendedParticleEffectSet::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteSLONG(Particle1Index);
    out.WriteSLONG(Particle2Index);
    out.WriteFloat(Particle1Value);
    out.WriteFloat(Particle2Value);
}