// CExplosionDef::Transfer(CPersistContext& context) -- persist the CExplosionDef's fields (18 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CExplosionDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB02F, 211 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CExplosionDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012425F4, whose RTTI type descriptor is .?AVCExplosionDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CExplosionDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CExplosionDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J063: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A, Transfer(std::vector<CExplosionRing >&) 0x004EB17A, Transfer<EQuakeStrength>(EQuakeStrength&) 0x004EB102, Transfer<EQuakeLength>(EQuakeLength&) 0x004EB13E.
// Layout: Ego_r.pdb CExplosionDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std
enum EQuakeLength { };                                 // Ego_r.pdb (enumerators not needed)
enum EQuakeStrength { };                               // Ego_r.pdb (enumerators not needed)
class CExplosionRing;
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(std::vector<CExplosionRing >& value);          // 0x004EB17A
    template <class T> void Transfer(T& value);              // enum instances: EQuakeStrength 0x004EB102, EQuakeLength 0x004EB13E
};
class CExplosionDef : public CSubDefClassBase {    // vtable 0x012425F4
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012425F4 slot 18
    float Damage;                                      // +0x28
    float ExplosionRadius;                             // +0x2C
    long FireDamage;                                   // +0x30
    float ExplosionLifeTime;                           // +0x34
    bool DamageFalloffWithRadius;                      // +0x38
    long ExplosionBlendSmall;                          // +0x3C
    long ExplosionBlendLarge;                          // +0x40
    long ExplosionReplacementBlendSmall;               // +0x44
    long ExplosionReplacementBlendLarge;               // +0x48
    float SecondsBeforeEffectReplacement;              // +0x4C
    float ExplosionRadiusSmall;                        // +0x50
    float ExplosionRadiusLarge;                        // +0x54
    std::vector<CExplosionRing > ExplosionRings;       // +0x58
    bool ExplosionShouldKnockdown;                     // +0x64 (Ego_r +0x68)
    bool CauseFireEffectOnHitCreatures;                // +0x65 (Ego_r +0x69)
    bool CausesRumble;                                 // +0x66 (Ego_r +0x6A)
    EQuakeStrength RumbleStrength;                     // +0x68 (Ego_r +0x6C)
    EQuakeLength RumbleLength;                         // +0x6C (Ego_r +0x70)
};

void CExplosionDef::Transfer(CPersistContext& context)
{
    context.Transfer(Damage);
    context.Transfer(ExplosionRadius);
    context.Transfer(FireDamage);
    context.Transfer(ExplosionLifeTime);
    context.Transfer(DamageFalloffWithRadius);
    context.Transfer(ExplosionBlendSmall);
    context.Transfer(ExplosionBlendLarge);
    context.Transfer(ExplosionReplacementBlendSmall);
    context.Transfer(ExplosionReplacementBlendLarge);
    context.Transfer(SecondsBeforeEffectReplacement);
    context.Transfer(ExplosionRadiusSmall);
    context.Transfer(ExplosionRadiusLarge);
    context.Transfer(ExplosionRings);
    context.Transfer(ExplosionShouldKnockdown);
    context.Transfer(CauseFireEffectOnHitCreatures);
    context.Transfer(CausesRumble);
    context.Transfer(RumbleStrength);
    context.Transfer(RumbleLength);
}