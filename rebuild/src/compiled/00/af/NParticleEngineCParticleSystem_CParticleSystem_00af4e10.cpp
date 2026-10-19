// NParticleEngine::CParticleSystem::CParticleSystem() -- empty component map, empty particle and decal lists, empty update parameters, unnamed; enabled and scaling at unit alpha / scale
// Leaf: ??0CParticleSystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4E10, 194 bytes. The catalog label "CHeroMorphDef" is propagated: the body stores
//   vtable 0x0129F4E8 (rtti-vtables.tsv: NParticleEngine::CParticleSystem; its destructor 0x00AF5900 is landed).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (for the system and for its CParticleUpdateParameters member, vtable
//   0x0125FFDC), malloc @ 0x00BFEA0E (two 12-byte list sentinels), CCharString::CCharString() @ 0x0099E4B0.
// Layout (Ego_r.pdb names, retail offsets as in the landed destructor): PParentEmitter +0x04, PEngine +0x08, Components
//   (CVectorMap: three pointers, empty comparator, Dirty byte +0x19) +0x0C, Particles +0x1C and Decals +0x20 (one-word
//   retail lists), UpdateParameters (+0x24, 0x24 bytes: vptr, two CVectorMaps) , NoActiveParticles +0x48, NoActiveDecals
//   +0x4C, the flag byte +0x50, GlobalAlpha +0x54, FadeAlpha +0x58, NormalisedTime +0x5C, Scale +0x60, Name +0x6C.
// Shape: the maps are STLport _Vector_base (end of storage an _STLP_alloc_proxy member, which places Name's `lea ecx` after
//   the last map pointer) plus comparator and Dirty; members with inline constructors run in declaration order (one shared zero register in ebx); the scalars that
//   follow Name's constructor are body assignments; `Scale = C3DVector(1, 1, 1)` builds a frame temporary and copies it
//   through `lea eax,[esi+60h]` (C3DVector has a user copy ctor and no operator=); the eight flag bits fold into one
//   byte store (0x82 = Enabled | ScaleParticles).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CBaseClass {
    virtual ~CBaseClass();
    CBaseClass();                                  // 0x0099A2D0
};

struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace NParticleEngine {

struct SEmptyCompare { };

template <class T> struct SRetailAllocProxy {     // STLport _STLP_alloc_proxy<T*, T, allocator<T>>: end of storage
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) { }
};

template <class T> struct CRetailVectorBase {     // STLport _Vector_base<T>
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVectorBase() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

template <class T> struct CRetailVectorMap : CRetailVectorBase<T> {   // CVectorMap<K, V, Compare>
    SEmptyCompare m_compare;
    bool Dirty;
    CRetailVectorMap() : Dirty(false) { }
};

template <class T> struct SListNode { SListNode* m_pNext; SListNode* m_pPrev; T m_value; };
template <class T> struct CRetailList {            // std::list<T> (retail: sentinel pointer only)
    SListNode<T>* m_pHead;
    CRetailList()
    {
        m_pHead = 0;
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct SComponentEntry { long m_type; void* m_instance; };
struct SVectorParam { unsigned long m_key; float m_value[3]; };
struct SParamDesc { unsigned long m_key; unsigned long m_desc[3]; };
struct CParticle;
struct CDecal;
struct CParticleEmitter;
struct CIEngine;

struct CParticleUpdateParameters : CBaseClass {    // vtable 0x0125FFDC
    CRetailVectorMap<SVectorParam> VectorParams;   // +0x04
    CRetailVectorMap<SParamDesc> UpdateParams;     // +0x14
};

struct CParticleSystem : CBaseClass {              // vtable 0x0129F4E8
    CParticleEmitter* PParentEmitter;              // +0x04
    CIEngine* PEngine;                             // +0x08
    CRetailVectorMap<SComponentEntry> Components;  // +0x0C
    CRetailList<CParticle*> Particles;             // +0x1C
    CRetailList<CDecal*> Decals;                   // +0x20
    CParticleUpdateParameters UpdateParameters;    // +0x24
    long NoActiveParticles;                        // +0x48
    long NoActiveDecals;                           // +0x4C
    bool Initialised : 1;                          // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                             // +0x54
    float FadeAlpha;                               // +0x58
    float NormalisedTime;                          // +0x5C
    C3DVector Scale;                               // +0x60
    CCharString Name;                              // +0x6C
    CParticleSystem();
};

CParticleSystem::CParticleSystem()
    : PEngine(0)
{
    GlobalAlpha = 1.0f;
    FadeAlpha = 1.0f;
    PParentEmitter = 0;
    NoActiveParticles = 0;
    NoActiveDecals = 0;
    NormalisedTime = 0.0f;
    Initialised = false;
    Enabled = true;
    Dead = false;
    NormalisedTimeEnabled = false;
    UseAllAttractors = false;
    Visible = false;
    StayWithEmitter = false;
    ScaleParticles = true;
    Scale = C3DVector(1.0f, 1.0f, 1.0f);
}

}   // namespace NParticleEngine