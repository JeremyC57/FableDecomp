// CObjectAugmentationsDef::~CObjectAugmentationsDef() -- destroy the trail-attachment string, the two sound maps, the two long vectors and the particle map in reverse order, then the inlined CDefPointeeBase destructor (vtable store + root-destructor tail jump)
// Leaf: ??1CObjectAugmentationsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ECAFD, length 80 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0, the map<EObjectAugmentationType, CDefString> destructor @ 0x004DD32B
// twice (labelled "CActiveFile::OnReadFinished", propagated), free @ 0x00BFEA14 twice (the inline vector<long>
// destructors), the map<EObjectAugmentationType, CObjectAugmentationParticleSet> destructor @ 0x004EB98F, the def root
// destructor @ 0x009FC550 (tail jump; labelled "~CMemoryBlock"). Evidence: the only caller is the scalar deleting
// destructor @ 0x004ECAE1, slot 0 of CObjectAugmentationsDef's vtable 0x01243974 (rtti-vtables.tsv); the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Layout: Ego_r.pdb CObjectAugmentationsDef (0xa0 bytes) with 12-byte retail maps/vectors (each 4 bytes narrower than the
// PDB): AugmentationParticles +0x30, EmptySlotName +0x3c, InitialAugmentations +0x40, SlotUpgradeCosts +0x4c, InitSounds
// +0x58, LoopingSounds +0x64, the six effect ids +0x70..+0x84, WeaponTrailAttachmentPoint +0x88 (retail size 0x8c).
// Shape: a size TU (`pop ecx` after free: `#pragma optimize("s", on)`, the vector destructors `__forceinline`); the
// class is `__declspec(novtable)` so only the inlined CDefPointeeBase vtable store (0x01230BA0) appears, after the
// member destructors and right before the base-destructor tail jump.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550 (labelled "~CMemoryBlock")
};
class CDefPointeeBase : public CBase_009fc550 { public:          // vtable 0x01230BA0
    virtual void v00();
    __forceinline ~CDefPointeeBase() { }
};

struct CAugmentationParticleMap {                  // retail map<EObjectAugmentationType, CObjectAugmentationParticleSet>
    void*         m_pHeader;
    unsigned long m_count;
    unsigned long m_compare;
    ~CAugmentationParticleMap();                   // 0x004EB98F
};
struct CAugmentationSoundMap {                     // retail map<EObjectAugmentationType, CDefString>
    void*         m_pHeader;
    unsigned long m_count;
    unsigned long m_compare;
    ~CAugmentationSoundMap();                      // 0x004DD32B
};
struct CLongVector {                               // retail vector<long>: first / last / end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    __forceinline ~CLongVector() { if (m_pFirst) free(m_pFirst); }
};

struct __declspec(novtable) CObjectAugmentationsDef : CDefPointeeBase {   // vtable 0x01243974
    unsigned char            _defBase[0x24];                   // +0x04 the definition base fields
    long                     NumberOfSlots;                    // +0x28
    long                     MaximumNumberOfSlots;             // +0x2c
    CAugmentationParticleMap AugmentationParticles;            // +0x30
    unsigned long            EmptySlotName;                    // +0x3c
    CLongVector              InitialAugmentations;             // +0x40
    CLongVector              SlotUpgradeCosts;                 // +0x4c
    CAugmentationSoundMap    InitSounds;                       // +0x58
    CAugmentationSoundMap    LoopingSounds;                    // +0x64
    long                     FireProjectileTrailEffect;        // +0x70
    long                     LightningProjectileTrailEffect;   // +0x74
    long                     FireAndLightningProjectileTrailEffect;   // +0x78
    long                     FireProjectileOnHitEffect;        // +0x7c
    long                     LightningProjectileOnHitEffect;   // +0x80
    long                     FireAndLightningProjectileOnHitEffect;   // +0x84
    CCharString              WeaponTrailAttachmentPoint;       // +0x88
    virtual void v00();
    ~CObjectAugmentationsDef();
};

CObjectAugmentationsDef::~CObjectAugmentationsDef()
{
}