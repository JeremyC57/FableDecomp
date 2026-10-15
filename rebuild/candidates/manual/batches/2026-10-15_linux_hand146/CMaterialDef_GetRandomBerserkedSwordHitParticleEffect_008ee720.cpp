// CMaterialDef::GetRandomBerserkedSwordHitParticleEffect() -- pick one entry of the material's berserked sword-hit particle-effect list with the engine's random generator (0 when the list is empty)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE720, length 100 bytes.
// Callees: the random-generator accessor @ 0x0049D800 (returns its object's +8 pointer + 0x16140; the manifest's
//   CVertexBufferWin32::DoSizeof label is a byte-twin propagation) on the engine singleton 0x013B86A0's +0x24 member, GFROR13 @ 0x00497890 (`__fastcall`).
// Layout: the berserked effect vector<long> sits at +0x80 in retail (STLport begin/end/capacity; the PDB's 16-byte debug vectors put it at +0x94).
// Shape: the inlined generator step `seed = GFROR13(seed * 0x24A1 + 0x24DF)` and `n ? r % n : 0`; the generator is a named local fetched
//   BEFORE the size is computed; the non-empty arm is the nested one so `return 0` lands at the end.
long __fastcall GFROR13(unsigned long value);     // 0x00497890
struct CRandomGenerator {
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
    __forceinline unsigned long Random(unsigned long n) { unsigned long r = Next(); return n ? r % n : 0; }
};
struct CEngineRandomOwner {                       // the object at +0x24 of the engine singleton
    CRandomGenerator* GetRandomGenerator();       // 0x0049D800
};
struct CEngineSingleton {                          // 0x013B86A0
    unsigned char _pad[0x24];
    CEngineRandomOwner* m_pOwner;
};
extern CEngineSingleton* g_pEngine;                // 0x013B86A0
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CMaterialDef {
    unsigned char _pad[0x80];                      // vptr/base, sounds, rumble, the first four effect vectors
    vector<long> BerserkedSwordHitParticleEffect;
    long GetRandomBerserkedSwordHitParticleEffect();
};
long CMaterialDef::GetRandomBerserkedSwordHitParticleEffect()
{
    if (!BerserkedSwordHitParticleEffect.empty()) {
        CRandomGenerator* rng = g_pEngine->m_pOwner->GetRandomGenerator();
        return BerserkedSwordHitParticleEffect[rng->Random(BerserkedSwordHitParticleEffect.size())];
    }
    return 0;
}
