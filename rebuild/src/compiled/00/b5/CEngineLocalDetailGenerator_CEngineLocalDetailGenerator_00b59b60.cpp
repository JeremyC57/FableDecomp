// CEngineLocalDetailGenerator::CEngineLocalDetailGenerator(bool dynamicUpdate) -- base generator, own vtable, empty object-collection array, the dynamic-update flag from the argument, the tuning defaults (z-sprites on, stipple bias 4, batch 32, 2 ms max update, enabled, 32 KB file block) and the eight fade-distance entries {0, 1}
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59B60, length 127 bytes (`ret 4`).
// Callees: the base ctor @ 0x00B59710 (vtable 0x012A2544; vptr, +4 = 0, +8 = true, +9 = false; catalog "CNavigationLayer").
// Label correction: the catalog's "CSpeechAnalyser::CSpeechAnalyser" is propagated -- the vtable stored is 0x012A2588 =
// CEngineLocalDetailGenerator (rtti-vtables.tsv; slots 1/3 are 0x00B59A30 / the landed LoadNewScene 0x00B599C0).
// Layout: the Ego_r.pdb members from ZSpriteGenerationEnabled on sit 0x38 lower in retail (+0x24..+0x38, table +0x3c, matching
// the landed ModifyFadeDistanceForVideoOptions 0x00B597E0); only the first array (+0x0c) is initialised between base and +0x24.
// Shape: the argument member is the only initialiser (stored right after the array zeros); the constants are body assignments
// (VC groups the two `true` bytes through cl), the table an element loop that VC unrolls into eight zero then eight 1.0f stores.
// Leaf: ??0CEngineLocalDetailGenerator
struct CEngineBaseGenerator {                       // 0xc bytes, ctor 0x00B59710 (vtable 0x012A2544)
    virtual void v00();
    long m_04;
    bool m_08;
    bool m_09;
    CEngineBaseGenerator();                         // 0x00B59710
};

struct CRuntimeFadeDistanceModiferTableEntry {
    float DistanceClampMin;
    float DistanceFactor;
};

template <class T> struct CRetailArray {            // 12-byte retail array {first, last, end}
    T* m_first; T* m_last; T* m_end;
    CRetailArray() : m_first(0), m_last(0), m_end(0) {}
};
class CLocalDetailObjectCollectionType;

class CEngineLocalDetailGenerator : public CEngineBaseGenerator { public:   // vtable 0x012A2588
    CRetailArray<CLocalDetailObjectCollectionType*> ObjectCollectionTypes;   // +0x0c
    unsigned char _pad18[0xc];                       // +0x18 (retail layout differs from the PDB here)
    bool          ZSpriteGenerationEnabled;          // +0x24
    long          StippleAlphaMeshBias;              // +0x28
    long          RepeatedMeshBatchSize;             // +0x2c
    float         LocalDetailMaxUpdateTime;          // +0x30
    bool          LocalDetailEnabled;                // +0x34
    bool          DynamicUpdateEnabled;              // +0x35
    long          MinimumFileBlockSize;              // +0x38
    CRuntimeFadeDistanceModiferTableEntry RuntimeFadeDistanceModiferTable[8];   // +0x3c
    virtual void v00();
    CEngineLocalDetailGenerator(bool dynamicUpdate);
};

CEngineLocalDetailGenerator::CEngineLocalDetailGenerator(bool dynamicUpdate)
    : DynamicUpdateEnabled(dynamicUpdate)
{
    ZSpriteGenerationEnabled = true;
    StippleAlphaMeshBias = 4;
    RepeatedMeshBatchSize = 0x20;
    LocalDetailMaxUpdateTime = 0.002f;
    LocalDetailEnabled = true;
    MinimumFileBlockSize = 0x8000;
    for (int i = 0; i < 8; ++i) {
        RuntimeFadeDistanceModiferTable[i].DistanceClampMin = 0.0f;
        RuntimeFadeDistanceModiferTable[i].DistanceFactor = 1.0f;
    }
}