// CMaterialDef::CMaterialDef() -- CParentDefClassBase base, this class's vtable, five empty particle-effect vectors (inline), the blended-effects map (out of line); scalars left uninitialised
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455CD1, length 81 bytes.
// Callees: 0x00430370 CParentDefClassBase::CParentDefClassBase, 0x0045336D std::map<long,CBlendedParticleEffectSet>::map
// The manifest label (C3DMeshMaterial) is propagated; the vtable stored is CMaterialDef's (0x012336DC). No engine
// header exists; the PDB layout is shifted down by 4 (retail base 0x3c) and its vectors are 12 bytes. The fifth
// vector starts at +0x80, past the 8-bit displacement range, so its three stores go through a `lea` base.
//
#pragma optimize("s", on)   // the def-class module is size-optimised (disp8 `lea` bases, late argument setup)
#include "rebuild_abi.h"

template<class T> struct CRetailVector {               // VC6-era Dinkumware vector: first/last/end, no allocator state
    T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) {}
};
struct CParentDefClassBase {
    virtual void v0();
    CParentDefClassBase();                             // 0x00430370
    unsigned char _body[0x38];                         // +0x04 .. +0x3b (retail sizeof 0x3c; the PDB header says 0x3d)
};

struct CBlendedEffectsMap {                            // std::map<long, CBlendedParticleEffectSet>: header, count, comparator
    unsigned char _body[0x0c];
    CBlendedEffectsMap();                              // 0x0045336D
};
struct CMaterialDef : CParentDefClassBase {
    long                FirstHitSound;                       // +0x3c (PDB +0x40)
    long                LastHitSound;                        // +0x40
    float               OnHitRumbleLevel;                    // +0x44
    float               OnHitRumbleFalloff;                  // +0x48
    bool                DeflectAllHits;                      // +0x4c
    bool                IsDiggable;                          // +0x4d
    CRetailVector<long> SlideFootParticleEffect;             // +0x50
    CRetailVector<long> SwordHitParticleEffect;              // +0x5c
    CRetailVector<long> KnockdownHitFloorParticleEffect;     // +0x68
    CRetailVector<long> WindBlowDustEffect;                  // +0x74
    CRetailVector<long> BerserkedSwordHitParticleEffect;     // +0x80
    CBlendedEffectsMap  BlendedSwordHitEffects;              // +0x8c   sizeof == 0x98
    CMaterialDef();
};
FABLE_STATIC_ASSERT(sizeof(CMaterialDef) == 0x98);
CMaterialDef::CMaterialDef()
{
}
