// std::pair<EObjectAugmentationType, CObjectAugmentationParticleSet>::pair() -- default pair: key 0, zero period, empty point arrays
// Leaf: ??0?$pair@W4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAE@XZ
// Retail: Fable.exe @ 0x004EC9B5, 30 bytes. Catalog label CSoundInit::CSoundInit is wrong (ICF/propagation): the only caller,
// the map deserialiser 0x004EC8C2 (GFSerialiseMapBinaryIn over map<EObjectAugmentationType, CObjectAugmentationParticleSet>),
// builds the pair local at [ebp+10h], reads the key, then CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn
// on its +4, copy-constructs it (0x004EC46D) into the map and destroys it (0x004E25F1).
// Layout: retail CObjectAugmentationParticleSet is 0x2c bytes (Ego_r.pdb 52: its two CArray<CDefString> are 12 bytes each in
// retail, 16 in the PDB). The default ctor zeroes PeriodSecs and leaves the four effect indices alone.
// Shape: size TU (fldz/fstp interleaved with the integer zero stores from one xor ecx,ecx), as 0x00469435.
#pragma optimize("s", on)
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE };
class CDefString;
template <class T> class CArray {                        // retail 12 bytes: start, finish, end of storage
public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    CArray() : Start(0), Finish(0), EndOfStorage(0) {}
};
class CObjectAugmentationParticleSet {
public:
    float              PeriodSecs;                       // +0x00
    long               ParticleEffect;                   // +0x04
    long               ParticleEffectToBlendTo;          // +0x08
    long               AttackParticleEffect;             // +0x0c
    long               AttackParticleEffectToBlendTo;    // +0x10
    CArray<CDefString> StartPoint;                       // +0x14
    CArray<CDefString> EndPoint;                         // +0x20
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) {}
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
};
template struct pair<EObjectAugmentationType, CObjectAugmentationParticleSet>;
}