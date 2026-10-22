// Grade: DIFFER(861v826) -- everything up to the first AddPrimitiveManager matches; retail keeps the converted counted-pointer temp's count block in ebp across the call and releases with dec/jne on it, ours re-reads the temp's count from the frame after the call (and re-tests after the dec).
// CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D() -- build the 2D renderer: engine-component base, empty manager list and counted pointers, five 2D primitive managers created over a shared function-static pool and registered, the horizontal FOV from the engine options (degrees -> radians, pi/4 when not positive), the default light direction (normalised (1,1,1)) and colours, and the 83-entry depth-band table
// Leaf: ??0CEnginePrimitiveRenderer2D
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4AC10, length 832 bytes (body 0x33a + int3 padding), plain `ret` (no EH frame).
// Callees: CEngineComponent::CEngineComponent 0x00B59710, operator new 0x00BFEA1A / delete 0x00BFE9BC, atexit 0x004012BC (static
//   pool dtor 0x0122AD60), manager ctors 0x00BA1AC0 / 0x00BD1F30 / 0x00BAD040 / 0x00BBBF40 (+ inline derived, vtable 0x012A22D0) /
//   0x00BE1E20, CCountedPointer<T>::Reset 0x00B4B360 / 0x00B4B410 / 0x00B4B4C0 / 0x00B4B620 / 0x00B4B570, AddPrimitiveManager
//   0x00B4ABB0, C3DVector::Normalise 0x00A14440.
// Layout: local model (engine/CEnginePrimitiveRenderer2D.h is PDB padding only).
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        SCountBlock* count = m_pCount;
        if (count) {
            if (--count->m_references == 0) {
                count->m_pfnDestroy(count->m_pObject);
                delete count;
            }
        }
    }
    void Reset(T* object);
};
struct CPrimitiveManagerPool {
    long A, B, C;
    CPrimitiveManagerPool() : A(0), B(0), C(0) {}
    ~CPrimitiveManagerPool();
};
struct CEnginePrimitiveManager2DBase { virtual ~CEnginePrimitiveManager2DBase(); };
struct CEnginePrimitiveManager2DAnimatedMeshes : CEnginePrimitiveManager2DBase { unsigned char _b[0x318 - 4]; CEnginePrimitiveManager2DAnimatedMeshes(CPrimitiveManagerPool* pool); };   // 0x00BA1AC0
struct CEnginePrimitiveManager2DText : CEnginePrimitiveManager2DBase { unsigned char _b[0x690 - 4]; CEnginePrimitiveManager2DText(CPrimitiveManagerPool* pool); };   // 0x00BD1F30
struct CEnginePrimitiveManager2DSprites : CEnginePrimitiveManager2DBase { unsigned char _b[0x38 - 4]; CEnginePrimitiveManager2DSprites(CPrimitiveManagerPool* pool); };   // 0x00BAD040
struct CEnginePrimitiveManager2DLinesBase : CEnginePrimitiveManager2DBase { unsigned char _b[0x28 - 4]; CEnginePrimitiveManager2DLinesBase(CPrimitiveManagerPool* pool); };   // 0x00BBBF40
struct CEnginePrimitiveManager2DLines : CEnginePrimitiveManager2DLinesBase {   // vtable 0x012A22D0
    CEnginePrimitiveManager2DLines(CPrimitiveManagerPool* pool) : CEnginePrimitiveManager2DLinesBase(pool) {}
    virtual void Slot1();
};
struct CEnginePrimitiveManager2DMeshes : CEnginePrimitiveManager2DBase { unsigned char _b[0x38 - 4]; CEnginePrimitiveManager2DMeshes(CPrimitiveManagerPool* pool); };   // 0x00BE1E20

struct C3DVector {
    float X, Y, Z;
    void Normalise();                              // 0x00A14440
};
struct CRGBFloatColour { float R, G, B, A; };
struct CEngineOptions { unsigned char _pad[0x5c]; float FieldOfViewDegrees; };   // +0x5c
extern CEngineOptions* g_pEngineOptions;          // 0x01436E20
extern float g_bandHalfWidth_0139a3d0;            // 0x0139A3D0
extern float g_bandWidth_0139a3c0;                // 0x0139A3C0
extern float g_bandHalfWidthLarge_0139a3c4;       // 0x0139A3C4
extern float g_bandWidthLarge_0139a3bc;           // 0x0139A3BC

struct CEngineComponent {
    virtual ~CEngineComponent();
    void** m_ppOwnerLink;                          // +0x04
    void*  m_pOwner;                               // +0x08
    CEngineComponent();                            // 0x00B59710
};
struct CEngineInternalPrimitiveBase;
struct CManagerList { void* First; void* Last; void* End; CManagerList() : First(0), Last(0), End(0) {} };

struct CEnginePrimitiveRenderer2D : CEngineComponent {
    CManagerList                                       Managers;          // +0x0c
    CCountedPointer<CEnginePrimitiveManager2DAnimatedMeshes> AnimatedMeshes;   // +0x18
    CCountedPointer<CEnginePrimitiveManager2DText>     Text;              // +0x20
    CCountedPointer<CEnginePrimitiveManager2DSprites>  Sprites;           // +0x28
    CCountedPointer<CEnginePrimitiveManager2DMeshes>   Meshes;            // +0x30
    CCountedPointer<CEnginePrimitiveManager2DLines>    Lines;             // +0x38
    CEngineInternalPrimitiveBase*                      Primitives;        // +0x40
    CEngineInternalPrimitiveBase*                      OverlayPrimitives; // +0x44
    float                                              HorizontalFOV;     // +0x48
    CRGBFloatColour                                    ColourA;           // +0x4c
    CRGBFloatColour                                    ColourB;           // +0x5c
    CRGBFloatColour                                    ColourC;           // +0x6c
    C3DVector                                          LightDirection;    // +0x7c
    float                                              BandCentre[0x53];  // +0x88
    float                                              BandIndex[0x53];   // +0x1d4
    bool                                               BandUsed[0x53];    // +0x320
    CEnginePrimitiveRenderer2D();
    virtual ~CEnginePrimitiveRenderer2D();
    void AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager2DBase>& manager);   // 0x00B4ABB0
};

CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D()
    : Primitives(0), OverlayPrimitives(0)
{
    static CPrimitiveManagerPool s_pool;
    AnimatedMeshes.Reset(new CEnginePrimitiveManager2DAnimatedMeshes(&s_pool));
    Text.Reset(new CEnginePrimitiveManager2DText(&s_pool));
    Sprites.Reset(new CEnginePrimitiveManager2DSprites(&s_pool));
    Lines.Reset(new CEnginePrimitiveManager2DLines(&s_pool));
    Meshes.Reset(new CEnginePrimitiveManager2DMeshes(&s_pool));
    {
        const CCountedPointer<CEnginePrimitiveManager2DBase> manager(AnimatedMeshes);
        AddPrimitiveManager(manager);
    }
    {
        const CCountedPointer<CEnginePrimitiveManager2DBase> manager(Text);
        AddPrimitiveManager(manager);
    }
    {
        const CCountedPointer<CEnginePrimitiveManager2DBase> manager(Sprites);
        AddPrimitiveManager(manager);
    }
    {
        const CCountedPointer<CEnginePrimitiveManager2DBase> manager(Lines);
        AddPrimitiveManager(manager);
    }
    {
        const CCountedPointer<CEnginePrimitiveManager2DBase> manager(Meshes);
        AddPrimitiveManager(manager);
    }
    HorizontalFOV = g_pEngineOptions->FieldOfViewDegrees * (1.0f / 360.0f) * 6.2831855f;
    if (HorizontalFOV <= 0.0f)
        HorizontalFOV = 0.78539819f;
    LightDirection.X = 1.0f;
    LightDirection.Y = 1.0f;
    LightDirection.Z = 1.0f;
    LightDirection.Normalise();
    ColourA.R = 0.5f;
    ColourA.G = 0.5f;
    ColourA.B = 0.5f;
    ColourA.A = 1.0f;
    ColourB.R = 0.5f;
    ColourB.G = 0.5f;
    ColourB.B = 0.5f;
    ColourB.A = 1.0f;
    ColourC.R = 0.0f;
    ColourC.G = 0.0f;
    ColourC.B = 0.0f;
    ColourC.A = 1.0f;
    float depth = 0.1f;
    for (long i = 0; i < 0x53; ++i) {
        if (i == 0x48) {
            BandCentre[i] = depth + g_bandHalfWidthLarge_0139a3c4;
            depth += g_bandWidthLarge_0139a3bc;
        } else {
            BandCentre[i] = depth + g_bandHalfWidth_0139a3d0;
            depth += g_bandWidth_0139a3c0;
        }
        BandUsed[i] = true;
        BandIndex[i] = (float)i;
    }
}
