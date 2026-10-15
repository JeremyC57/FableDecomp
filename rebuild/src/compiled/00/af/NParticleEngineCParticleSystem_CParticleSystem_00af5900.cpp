// NParticleEngine::CParticleSystem::~CParticleSystem() -- delete all particles and decals, destroy the members, then the base destructor
// Retail 0x00AF5900, 96 bytes. Called only from the deleting destructor 0x00AF7CC0
// (NParticleEngine::CParticleSystem vtable 0x0129F4E8 slot 0); the manifest label
// "CTCInventoryItem::OnDie" is propagated.
// Body: the own vtable store, DeleteAllParticles 0x00AF56E0 and DestroyDecals 0x00AF5770 on `this`;
// then the members die in reverse order (Ego_r.pdb names, retail offsets -- the editor build's
// containers are wider): Name (CCharString, +0x6C, ~CCharString 0x0099EAE0), UpdateParameters (+0x24,
// out-of-line destructor 0x006E2500), Decals (+0x20) and Particles (+0x1C) (one-word retail lists,
// out-of-line destructors 0x00AF6770 / 0x00AF6710), Components (CVectorMap, +0x0C) inline: the
// `__fastcall` _Destroy_range 0x00AF6F90 (first in ecx, last in edx, the empty allocator temporary by
// const reference in the high byte of the `push ecx` frame slot) and `free` 0x00BFEA14 of the block;
// finally the base destructor 0x0099A2E0 (a call: the frame slot is popped after it).
// Leaf: ??1CParticleSystem

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

namespace NParticleEngine {

struct SScalarTag {
    SScalarTag() { }
};

struct CComponentEntry;                            // pair<EPSCType, CComponentInstance>
void __fastcall _Destroy_range(CComponentEntry* first, CComponentEntry* last, const SScalarTag& tag);  // 0x00AF6F90

struct SComponentMap {                             // CVectorMap<EPSCType, CComponentInstance, ...>
    CComponentEntry* _M_start;
    CComponentEntry* _M_finish;
    CComponentEntry* _M_end_of_storage;
    unsigned long m_compare;
    ~SComponentMap()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start)
            free(_M_start);
    }
};

struct SParticleList {                             // std::list<CParticle*> (retail: head only)
    void* head;
    ~SParticleList();                              // 0x00AF6710
};

struct SDecalList {                                // std::list<CDecal*> (retail: head only)
    void* head;
    ~SDecalList();                                 // 0x00AF6770
};

struct SUpdateParameters {                         // CParticleUpdateParameters<unsigned long, unsigned long>
    unsigned char _body[0x24];
    ~SUpdateParameters();                          // 0x006E2500
};

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};

struct CParticleEmitter;
struct CIEngine;

struct CParticleSystem : CBase {                   // vtable 0x0129F4E8
    CParticleEmitter* PParentEmitter;              // +0x04
    CIEngine* PEngine;                             // +0x08
    SComponentMap Components;                      // +0x0C
    SParticleList Particles;                       // +0x1C
    SDecalList Decals;                             // +0x20
    SUpdateParameters UpdateParameters;            // +0x24
    long NoActiveParticles;                        // +0x48
    long NoActiveDecals;                           // +0x4C
    unsigned long m_flags;                         // +0x50 Initialised/Enabled/Dead/... bit-fields
    float GlobalAlpha;                             // +0x54
    float FadeAlpha;                               // +0x58
    float NormalisedTime;                          // +0x5C
    float Scale[3];                                // +0x60
    CCharString Name;                              // +0x6C
    virtual void v00();
    void DeleteAllParticles();                     // 0x00AF56E0
    void DestroyDecals();                          // 0x00AF5770
    ~CParticleSystem();
};

CParticleSystem::~CParticleSystem()
{
    DeleteAllParticles();
    DestroyDecals();
}

}   // namespace NParticleEngine