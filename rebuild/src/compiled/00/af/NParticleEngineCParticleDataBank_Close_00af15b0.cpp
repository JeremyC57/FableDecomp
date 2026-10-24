// NParticleEngine::CParticleDataBank::Close() -- drop every particle-emitter info record (`ParticleEmitters.erase(begin(), end())`), then close the CBankFile base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF15B0, 69 bytes (row padded to 80). Virtual slot 5 of
// NParticleEngine::CParticleDataBank's vtable 0x0129F38C; FableWin.pdb names ?Close@CParticleDataBank@NParticleEngine@@UAEXXZ.
// Callees: the LTCG `__fastcall _Move(first, last, dest, tag, bool)` @ 0x00AF1CE0 (manifest label "_Move<CModifierGraphic*>"
// is propagated), the `__fastcall` element-range destroy @ 0x00AF1D90 (same declaration as the landed ~CParticleDataBank
// 0x00AF1600), CBankFile::Close @ 0x009CE360 (same spelling as the landed CLipSyncDataBank::Close 0x00A42DA0).
// Layout: ParticleEmitters (retail 12-byte CArray<CParticleEmitterInfo>) at +0x1E0 = Ego_r.pdb +596 less the 16->12 container
// shrink, after the 0x164-byte CBankFile and the secondary base.
// Shape: the VC6-era Dinkumware `erase(begin(), end())` with no empty-range test (byte twin of CLipSyncDataBank::Close 0x00A42DA0
// at other offsets): the iterators read into locals in the order last, first; both empty tag temporaries share the high byte of
// the one `push ecx` frame word; the kept pointer in edi is stored as the new last before the base Close.
struct __false_type { __false_type() {} };
struct SMoveTag_00af1ce0 { SMoveTag_00af1ce0() {} };
namespace NParticleEngine {
struct CParticleEmitterInfo;
}
NParticleEngine::CParticleEmitterInfo* __fastcall Move_00af1ce0(NParticleEngine::CParticleEmitterInfo* first,
    NParticleEngine::CParticleEmitterInfo* last, NParticleEngine::CParticleEmitterInfo* dest,
    const SMoveTag_00af1ce0& tag, bool overlap);                                        // 0x00AF1CE0
void __fastcall DestroyRange(NParticleEngine::CParticleEmitterInfo* first, NParticleEngine::CParticleEmitterInfo* last,
                             const __false_type& tag);                                  // 0x00AF1D90

struct CBankFile {
    unsigned char _body[0x164];
    void Close_009ce360();                                  // 0x009CE360
};

namespace NParticleEngine {
template<class T> struct CArray {                           // retail 12 bytes: {first, last, end}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CParticleDataBank : CBankFile {
    unsigned char _secondary164[0x1e0 - 0x164];             // secondary base (vtable 0x0129F380) and bank members
    CArray<CParticleEmitterInfo> ParticleEmitters;          // +0x1e0
    void Close();
};

void CParticleDataBank::Close()
{
    CParticleEmitterInfo* last = ParticleEmitters._M_finish;
    CParticleEmitterInfo* first = ParticleEmitters._M_start;
    CParticleEmitterInfo* kept = Move_00af1ce0(last, last, first, SMoveTag_00af1ce0(), false);
    DestroyRange(kept, ParticleEmitters._M_finish, __false_type());
    ParticleEmitters._M_finish = kept;
    CBankFile::Close_009ce360();
}
}