// CParticle::`vector_deleting_destructor'
// Retail 0x00af7b40 — MSVC scalar/vector deleting destructor thunk.
//
// push esi; mov esi,ecx; call 0xaf7b60 (dtor body); test [esp+8],1;
// je skip; mov ecx,esi; call 0xaf8650 (member operator delete);
// skip: mov eax,esi; pop esi; ret 4
// Naming (NF460, queue nf458): the callees are NParticleEngine::CParticle::~CParticle 0x00AF7B60 (??1CParticle@NParticleEngine@@UAE@XZ) and
//   CParticle::operator delete 0x00AF8650 (??3CParticle@NParticleEngine@@SIXPAX@Z), Ego_r + FableWin and both rows' Leaf lines (were the stand-ins
//   CParticle::inner_dtor / member_free on a global CParticle). The class moves into NParticleEngine, so this row's own descriptive free self
//   now takes a NParticleEngine::CParticle* (r456a). Same bytes.

namespace NParticleEngine {
class CParticle { public:
    virtual ~CParticle();                                  // 0x00AF7B60
    static void __fastcall operator delete(void* object);  // 0x00AF8650 (the class pool free, object in ecx)
};
}
using NParticleEngine::CParticle;

void* __fastcall CParticle_scalar_deleting_destructor(
    CParticle* particle, int /*edx*/, unsigned int flags)
{
    particle->CParticle::~CParticle();
    if (flags & 1)
        CParticle::operator delete(particle);
    return particle;
}