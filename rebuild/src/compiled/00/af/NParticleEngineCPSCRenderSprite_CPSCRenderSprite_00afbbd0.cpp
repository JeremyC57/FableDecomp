// NParticleEngine::CPSCRenderSprite::~CPSCRenderSprite() -- clear the sprite and trail primitive handles, then their member destructors and the base chain (CPSCBase's implicit destructor, ~CBaseClassNonCopyable tail call)
// Leaf: ??1CPSCRenderSprite
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFBBD0, 272 bytes, tail `jmp`.
// Label: the catalog's "CPSCRenderMesh::~CPSCRenderMesh" is a propagated byte-twin name: this body stores vtable
//   0x0129F67C = NParticleEngine::CPSCRenderSprite (RTTI) and uses its handles (Ego_r.pdb PrimHandle +0x3c,
//   TrailPrimHandle +0x48); the real ~CPSCRenderMesh is 0x00B017F0.
// Callees: the primitive's virtual slots 3 (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy), ~CBaseClass
//   0x0099A2E0 (each handle's base), ~CBaseClassNonCopyable 0x0099A300 (the component base, tail call).
// Layout: Ego_r.pdb CPSCRenderSprite (0x54, retail offsets equal): CPSCBase (vptr, ParticleSystem +4, flag bits +8),
//   SpriteBankIndex +0x0c .. ChildPrimIndex +0x44, TrailPrimHandle +0x48, ChildTrailPrimIndex +0x50.
// Shape: the body is two inline `Clear()` calls (0x00AF70D0 form); each CEnginePrimitiveHandle member then runs its inline
//   destructor (vtable store, Clear, the counted pointer's own release, base). One shared zero register (ebx).
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CBaseClassNonCopyable {                      // vtable 0x0129A7C4
protected:
    virtual ~CBaseClassNonCopyable();              // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (protected)
};

namespace NParticleEngine {
class CParticleSystem;
class CPSCBase : public CBaseClassNonCopyable { public:
    CParticleSystem* ParticleSystem;               // +0x04
    bool UpdateStarted : 1;                        // +0x08
    bool Initialised : 1;
};
class CPSCRenderSprite : public CPSCBase { public:               // vtable 0x0129F67C
    long                   SpriteBankIndex;        // +0x0c
    long                   TrailBankIndex;         // +0x10
    unsigned long          StartColour;            // +0x14 CRGBColour
    unsigned long          MidColour;              // +0x18
    unsigned long          EndColour;              // +0x1c
    unsigned long          Bits20[6];              // +0x20 packed render settings
    unsigned char          EnableBits;             // +0x38
    CEnginePrimitiveHandle PrimHandle;             // +0x3c
    long                   ChildPrimIndex;         // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;        // +0x48
    long                   ChildTrailPrimIndex;    // +0x50
    virtual ~CPSCRenderSprite();
};

CPSCRenderSprite::~CPSCRenderSprite()
{
    PrimHandle.Clear();
    TrailPrimHandle.Clear();
}
}