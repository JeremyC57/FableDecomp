// CMaterialDef::GetRandomKnockdownHitFloorParticleEffect() -- a random id from the material's KnockdownHitFloorParticleEffect list (0 when the list is empty)
// Leaf: ?GetRandomKnockdownHitFloorParticleEffect@CMaterialDef@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ee660, 85 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb ?GetRandomKnockdownHitFloorParticleEffect@CMaterialDef@@QAEJXZ over Ego_r.pdb CMaterialDef::KnockdownHitFloorParticleEffect (vector<long>, PDB +116, retail +0x68: the five effect vectors are consecutive 12-byte STLport vectors, the landed GetRandomBerserkedSwordHitParticleEffect 0x008EE720 reads the fifth at +0x80); no direct caller.
// Callees: CWorld::GetLocalSeed() const 0x0049D800 (Ego_r.pdb ?GetLocalSeed@CWorld@@QBEPAKXZ: &Component->LocalSeed) on the main
//   game component's PWorld (0x013B86A0 +0x24), GFROR13(unsigned long) 0x00497890 (`__fastcall`).
// Body: the inlined GFRandom(n, seed) (Ego_r.pdb ?GFRandom@@YIKKPAK@Z): `*seed = GFROR13(*seed * 0x24A1 + 0x24DF); return n ? *seed % n : 0;`,
//   the seed pointer fetched before the size is computed (the landed 0x008EE720 shape).
// Header debt: engine/CMaterialDef.h carries the Ego_r (Dinkumware) offsets; modelled locally with retail ones.
long __fastcall GFROR13(unsigned long value);            // 0x00497890
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
class CWorld {
public:
    unsigned long* GetLocalSeed() const;                   // 0x0049D800
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T* operator->() const { return ptr; }
};
class CMainGameComponent {
public:
    unsigned char      _pad00[0x24];
    scoped_ptr<CWorld> PWorld;                             // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {     // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}
class CMaterialDef {
public:
    unsigned char     _pad00[0x68];
    std::vector<long> KnockdownHitFloorParticleEffect;                               // +0x68
    long GetRandomKnockdownHitFloorParticleEffect();
};
long CMaterialDef::GetRandomKnockdownHitFloorParticleEffect()
{
    if (!KnockdownHitFloorParticleEffect.empty()) {
        unsigned long* seed = g_pMainGameComponent->PWorld->GetLocalSeed();
        return KnockdownHitFloorParticleEffect[GFRandom(KnockdownHitFloorParticleEffect.size(), seed)];
    }
    return 0;
}