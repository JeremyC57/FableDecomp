// CEngineSceneGridCell::CEngineSceneGridCell(CEngineSceneGridCell*) -- an empty grid cell under the given parent: no primitives, two empty group-primitive handles, counts zero (the total and render mask marked valid), and a random preload-frame offset
// Leaf: ??0CEngineSceneGridCell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B935C0, length 153 bytes (+ int3 padding).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (per handle), GFROR13 @ 0x00497890 (__fastcall). Globals: the random seed
// @ 0x0143B950, the preload-frame range @ 0x0139AC68. Vtable 0x0122F598 = CEnginePrimitiveHandle (rtti-vtables.tsv).
// Layout: the PDB CEngineSceneGridCell (0x48 bytes; Parent, FirstPrimitive, BoundingBox, DrawDistance, the
// CEnginePrimitiveHandle[2] at +0x24, the counts, a five-bit flag byte at +0x44, OccludedFrames, PreloadFrames); no engine
// header exists for this class. CEnginePrimitiveHandle is the landed 8-byte model (CBaseClass + CIVCountedPointer).
// Shape: the handle array is built by an inline counted loop (`dec ebp ; jne`: base call, vtable, null pointer); the five
// bit-field assignments merge into one `and 0E6h ; or 6` read-modify-write; the zero lives in ebx; the inline LCG
// (`seed * 0x24A1 + 0x24DF`, stored, then rotated by GFROR13 and stored again) is reduced modulo the range unless the
// range is 0 (the range is loaded before the call); the byte store is duplicated into both return paths.
struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

struct CEnginePrimitiveHandle;

struct CEnginePersistentPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

template <class T> struct CIVCountedPointer {      // intrusive counted pointer (count at +4 of the object)
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

struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CEngineInternalPrimitiveBase;

struct CEngineSceneGridCell {                      // 0x48 bytes (PDB layout, matches retail)
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    float                         BoundingBox[6];  // +0x08 C3DBoundingBox
    float                         DrawDistance;    // +0x20
    CEnginePrimitiveHandle        GroupPrimitiveHandle[2];   // +0x24
    long                          LocalCount;      // +0x34
    unsigned long                 TotalCount;      // +0x38
    unsigned long                 RenderMask;      // +0x3c
    unsigned long                 OccludedLayers;  // +0x40
    unsigned char                 BoundingBoxValid : 1;     // +0x44 bit 0
    unsigned char                 TotalCountValid : 1;      //        bit 1
    unsigned char                 RenderMaskValid : 1;      //        bit 2
    unsigned char                 WasRendered : 1;          //        bit 3
    unsigned char                 HasOcclusionResults : 1;  //        bit 4
    unsigned char                 OccludedFrames;  // +0x45
    unsigned char                 PreloadFrames;   // +0x46
    CEngineSceneGridCell(CEngineSceneGridCell* parent);
};

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CRandomSeed {
    unsigned long m_seed;
    unsigned long Rand(unsigned long range)
    {
        m_seed = m_seed * 0x24a1 + 0x24df;
        m_seed = GFROR13(m_seed);
        return range ? m_seed % range : 0;
    }
};
extern CRandomSeed   g_randomSeed_0143b950;        // 0x0143B950
extern unsigned long g_preloadFrameRange_0139ac68; // 0x0139AC68

CEngineSceneGridCell::CEngineSceneGridCell(CEngineSceneGridCell* parent)
    : Parent(parent)
    , FirstPrimitive(0)
{
    BoundingBoxValid = 0;
    TotalCountValid = 1;
    RenderMaskValid = 1;
    WasRendered = 0;
    HasOcclusionResults = 0;
    LocalCount = 0;
    TotalCount = 0;
    RenderMask = 0;
    OccludedLayers = 0;
    OccludedFrames = 0;
    PreloadFrames = (unsigned char)g_randomSeed_0143b950.Rand(g_preloadFrameRange_0139ac68);
}