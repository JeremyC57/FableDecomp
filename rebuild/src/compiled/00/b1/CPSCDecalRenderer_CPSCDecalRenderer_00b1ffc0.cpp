// CPSCDecalRenderer::~CPSCDecalRenderer() -- particle decal renderer component teardown: release the mesh-decal and landscape-decal primitive handles (each tells its primitive when it was the last handle), then the CBaseClassNonCopyable base
// Leaf: ??1CPSCDecalRenderer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1FFC0, 175 bytes (`jmp` to the base destructor).
// Callees: the primitive's virtual slots 3 (+0xc, OnLastHandleReleased(handle)) and 1 (+4, destroy at the last reference),
//   ~CBaseClass 0x0099A2E0 (each handle's base), ~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Layout = Ego_r.pdb CPSCDecalRenderer (100 bytes): LandscapeDecalsPrimHandle +0x4c, LandscapeDecalsChildPrimIndex +0x54,
//   MeshDecalsPrimHandle +0x58, MeshDecalsChildPrimIndex +0x60; CEnginePrimitiveHandle (vtable 0x0122F598) = CBaseClass + a
//   counted Data pointer at +4 (as the landed CEnginePrimitiveHandleList::resize 0x00BBDCB0 / SetChildPrimitive 0x00BBC310).
// Shape: the renderer stores no vtable of its own (`__declspec(novtable)` chain); members die in reverse order, each handle's
//   inline destructor is Clear() (the uniqueness test, then `Data = 0` = release + two zero stores) followed by the counted
//   pointer's own destructor (a second, null-tested release the compiler cannot fold), then the handle's base destructor.
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
    __forceinline ~CIVCountedPointer() { ReleaseRef(); }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    ~CBaseClass();                                 // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};
class CBaseClassNonCopyable {
public:
    virtual void v000();
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
namespace NParticleEngine {
class __declspec(novtable) CComponent : public CBaseClassNonCopyable {
public:
    unsigned char _pad04[8];
};
}
class __declspec(novtable) CPSCDecalRenderer : public NParticleEngine::CComponent {
public:
    float                  DecalLifeSecs;          // +0x0c
    long                   DecalBankIndex;         // +0x10
    unsigned long          StartColour;            // +0x14
    unsigned long          MidColour;              // +0x18
    unsigned long          EndColour;              // +0x1c
    unsigned char          _bits20[0x14];          // bit-field words +0x20..+0x33
    unsigned char          _flags34[4];            // AlphaFadeEnable.. +0x34
    float                  MaxPoolSize;            // +0x38
    float                  MaxPoolAlpha;           // +0x3c
    float                  MaxPoolLife;            // +0x40
    float                  PoolFrameIncreaseRate;  // +0x44
    float                  StencilCubeWidth;       // +0x48
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle;   // +0x4c
    long                   LandscapeDecalsChildPrimIndex;   // +0x54
    CEnginePrimitiveHandle MeshDecalsPrimHandle;   // +0x58
    long                   MeshDecalsChildPrimIndex;        // +0x60
    ~CPSCDecalRenderer();
};

CPSCDecalRenderer::~CPSCDecalRenderer()
{
}