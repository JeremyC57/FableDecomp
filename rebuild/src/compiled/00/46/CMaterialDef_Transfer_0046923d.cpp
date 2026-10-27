// CMaterialDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CMaterialDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046923D, 151 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x012336DC (RTTI .?AVCMaterialDef@@; Ego_r.pdb ?Transfer@CMaterialDef@@UAEXAAVCPersistContext@@@Z);
//   the 12 Transfer calls take, in call order, the members at +0x3c, +0x40, +0x44, +0x48, +0x4c, +0x4d, +0x50, +0x5c, +0x68, +0x74, +0x80, +0x8c with the per-type callees 0x00431102, 0x00431102, 0x00431061, 0x00431061, 0x0043314A, 0x0043314A, 0x00466A47, 0x00466A47, 0x00466A47, 0x00466A47, 0x00466A47, 0x004692D4, which are
//   Ego_r.pdb CMaterialDef's members FirstHitSound, LastHitSound, OnHitRumbleLevel, OnHitRumbleFalloff, DeflectAllHits, IsDiggable, SlideFootParticleEffect, SwordHitParticleEffect, KnockdownHitFloorParticleEffect, WindBlowDustEffect, BerserkedSwordHitParticleEffect, BlendedSwordHitEffects.
// Shape (family M053, representative 0x0046923D): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CBlendedParticleEffectSet;
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x39 - 4];
};
#pragma pack(pop)
class CMaterialDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    long                     FirstHitSound;                 // +0x3c
    long                     LastHitSound;                  // +0x40
    float                    OnHitRumbleLevel;              // +0x44
    float                    OnHitRumbleFalloff;            // +0x48
    bool                     DeflectAllHits;                // +0x4c
    bool                     IsDiggable;                    // +0x4d
    std::vector<long>        SlideFootParticleEffect;       // +0x50
    std::vector<long>        SwordHitParticleEffect;        // +0x5c
    std::vector<long>        KnockdownHitFloorParticleEffect; // +0x68
    std::vector<long>        WindBlowDustEffect;            // +0x74
    std::vector<long>        BerserkedSwordHitParticleEffect; // +0x80
    std::map<long, CBlendedParticleEffectSet> BlendedSwordHitEffects;        // +0x8c
};
void CMaterialDef::Transfer(CPersistContext& context)
{
    context.Transfer(FirstHitSound);
    context.Transfer(LastHitSound);
    context.Transfer(OnHitRumbleLevel);
    context.Transfer(OnHitRumbleFalloff);
    context.Transfer(DeflectAllHits);
    context.Transfer(IsDiggable);
    context.Transfer(SlideFootParticleEffect);
    context.Transfer(SwordHitParticleEffect);
    context.Transfer(KnockdownHitFloorParticleEffect);
    context.Transfer(WindBlowDustEffect);
    context.Transfer(BerserkedSwordHitParticleEffect);
    context.Transfer(BlendedSwordHitEffects);
}