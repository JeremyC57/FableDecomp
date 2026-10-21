// std::pair<long, CBlendedParticleEffectSet>::pair() -- default pair: key 0 and an empty blended particle effect set
// Leaf: ??0?$pair@JVCBlendedParticleEffectSet@@@std@@QAE@XZ
// Retail: Fable.exe @ 0x00469435, 23 bytes. Catalog label CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo is wrong: the only
// caller, the map deserialiser 0x00469381 (GFSerialiseMapBinaryIn), builds a 0x14-byte pair local, then calls
// CBlendedParticleEffectSet::TransferBinaryIn (0x0046944C) on its +4. Ego_r.pdb has ??0?$pair@JVCBlendedParticleEffectSet@@@std@@QAE@XZ
// and CBlendedParticleEffectSet = {long Particle1Index; float Particle1Value; long Particle2Index; float Particle2Value}.
// Shape: size TU (fldz/fstp interleaved with the integer zero stores from one xor ecx,ecx).
#pragma optimize("s", on)
class CBlendedParticleEffectSet {
public:
    long  Particle1Index;                                // +0x00
    float Particle1Value;                                // +0x04
    long  Particle2Index;                                // +0x08
    float Particle2Value;                                // +0x0c
    CBlendedParticleEffectSet() : Particle1Index(0), Particle1Value(0.0f), Particle2Index(0), Particle2Value(0.0f) {}
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
};
template struct pair<long, CBlendedParticleEffectSet>;
}