// CMaterialDef::operator=(const CMaterialDef&) -- memberwise copy: CParentDefClassBase part, hit-sound range, rumble pair, two flags, the five particle-effect id vectors and the blended sword-hit effect map; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C112, length 141 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886,
//          map<long,CBlendedParticleEffectSet>::operator= @ 0x0045C19F (landed as CRetailTree0045c212::operator=).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CMaterialDef's
// vtable 0x012336DC is the 5-byte `jmp 0x0045C112` right before this body; PDB members +0x40..+0xa4 sit
// at retail +0x3c..+0x8c (retail base 0x39 bytes, containers 12 bytes instead of 16).
// Shape: memberwise assignment in declaration order; the final member's source address is formed with
// `add edi,8Ch` because nothing reads edi afterwards.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CBlendedEffectMap {              // retail map<long,CBlendedParticleEffectSet> (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CBlendedEffectMap& operator=(const CBlendedEffectMap& other);   // 0x0045C19F
};

class CMaterialDef : public CParentDefClassBase { public:     // vtable 0x012336DC
    long  FirstHitSound;                        // +0x3c
    long  LastHitSound;                         // +0x40
    float OnHitRumbleLevel;                     // +0x44
    float OnHitRumbleFalloff;                   // +0x48
    bool  DeflectAllHits;                       // +0x4c
    bool  IsDiggable;                           // +0x4d
    CLongVector SlideFootParticleEffect;        // +0x50
    CLongVector SwordHitParticleEffect;         // +0x5c
    CLongVector KnockdownHitFloorParticleEffect;// +0x68
    CLongVector WindBlowDustEffect;             // +0x74
    CLongVector BerserkedSwordHitParticleEffect;// +0x80
    CBlendedEffectMap BlendedSwordHitEffects;   // +0x8c
    CMaterialDef& operator=(const CMaterialDef& other);
};

CMaterialDef& CMaterialDef::operator=(const CMaterialDef& other)
{
    CParentDefClassBase::operator=(other);
    FirstHitSound = other.FirstHitSound;
    LastHitSound = other.LastHitSound;
    OnHitRumbleLevel = other.OnHitRumbleLevel;
    OnHitRumbleFalloff = other.OnHitRumbleFalloff;
    DeflectAllHits = other.DeflectAllHits;
    IsDiggable = other.IsDiggable;
    SlideFootParticleEffect = other.SlideFootParticleEffect;
    SwordHitParticleEffect = other.SwordHitParticleEffect;
    KnockdownHitFloorParticleEffect = other.KnockdownHitFloorParticleEffect;
    WindBlowDustEffect = other.WindBlowDustEffect;
    BerserkedSwordHitParticleEffect = other.BerserkedSwordHitParticleEffect;
    BlendedSwordHitEffects = other.BlendedSwordHitEffects;
    return *this;
}