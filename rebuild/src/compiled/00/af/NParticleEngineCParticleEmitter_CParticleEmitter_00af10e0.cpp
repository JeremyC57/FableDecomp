// NParticleEngine::CParticleEmitter::~CParticleEmitter() -- destroy the emitter's members in reverse order (name, interpolators, primitive group handle, update parameters, particle-system vector), then the CBaseObject base
// Leaf: ??1CParticleEmitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF10E0, length 178 bytes, plain `ret`. Called by the vector deleting destructor
// 0x00AF10C0 = NParticleEngine::CParticleEmitter vtable 0x0129F348 slot 0 (rtti-vtables.tsv).
// Label correction: the catalog's "CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase" is a propagated label;
// the body and its only caller belong to NParticleEngine::CParticleEmitter (graphics/particle layer -- owner note: replacement work).
// Callees: ~CCharString 0x0099EAE0 (Name), CBaseClass::~CBaseClass 0x0099A2E0 (the two interpolators and the handle's base),
// the handle's persistent primitive virtual slots 3 (last-handle notification) and 1 (destroy), SUpdateParameters dtor 0x006E2500,
// the `__fastcall` _Destroy_range<CCountedPointer<CParticleSystem>> 0x00AF0D80 (first in ecx, last in edx, the empty allocator
// tag in the high byte of the `push ecx` slot), free 0x00BFEA14, CBaseObject::~CBaseObject 0x0099A430 (vtable 0x0129A7D8).
// Shape: the emitter's own body is empty, so its vtable is never re-stored; the CEnginePrimitiveHandle (vtable 0x0122F598) inline
// destructor is the landed CEngineSceneGridCell 0x00B93660 model; the STLport vector dtor is inline (as CParticleSystem 0x00AF5900).
// Layout (retail; Ego_r.pdb CParticleEmitter is 0xc larger from +0x18: the editor's vector and update parameters are wider):
// ParticleSystems +0x08, UpdateParameters +0x14, PrimitiveGroupHandle +0x38, OrientationInterpolator +0x60,
// PositionInterpolator +0x94, Transform/InverseTransform/EndTransform +0xb0, Name +0x140.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

template <class T> class CIVCountedPointer { public:      // intrusive counted pointer (count at +4 of the object)
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};

class CEnginePrimitiveHandle : public CBaseClass { public:       // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CBaseObject { public:                               // vtable 0x0129A7D8 (the intelligent-pointer target base)
    virtual void v00();
    void* m_pLink;                                 // +0x04
    ~CBaseObject();                                // 0x0099A430
};

namespace NParticleEngine {

struct SScalarTag {
    SScalarTag() { }
};

struct CParticleSystemPointer;                     // CCountedPointer<NParticleEngine::CParticleSystem>
void __fastcall _Destroy_range(CParticleSystemPointer* first, CParticleSystemPointer* last, const SScalarTag& tag);   // 0x00AF0D80

struct SParticleSystemVector {                     // std::vector<CCountedPointer<CParticleSystem>> (retail 12 bytes)
    CParticleSystemPointer* _M_start;
    CParticleSystemPointer* _M_finish;
    CParticleSystemPointer* _M_end_of_storage;
    ~SParticleSystemVector()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start)
            free(_M_start);
    }
};

struct SUpdateParameters {                         // CParticleUpdateParameters<unsigned long, unsigned long>
    unsigned char _body[0x24];
    ~SUpdateParameters();                          // 0x006E2500
};

struct C3DOrientationInterpolator : CBaseClass {   // 0x34 bytes
    unsigned char _body[0x30];
};
struct C3DPositionInterpolator : CBaseClass {      // 0x1c bytes
    unsigned char _body[0x18];
};

struct __declspec(novtable) CParticleEmitter : CBaseObject {   // vtable 0x0129F348 (novtable here: the destructor never re-stores it)
    SParticleSystemVector  ParticleSystems;        // +0x08
    SUpdateParameters      UpdateParameters;       // +0x14
    CEnginePrimitiveHandle PrimitiveGroupHandle;   // +0x38
    unsigned char          _flags40[0x20];         // +0x40 bit-fields, OldPos, Velocity
    C3DOrientationInterpolator OrientationInterpolator;   // +0x60
    C3DPositionInterpolator    PositionInterpolator;      // +0x94
    float                  Transforms[3][12];      // +0xb0 Transform / InverseTransform / EndTransform (CMatrix3x4)
    CCharString            Name;                   // +0x140
    virtual void v00();
    ~CParticleEmitter();
};

CParticleEmitter::~CParticleEmitter()
{
}

}   // namespace NParticleEngine