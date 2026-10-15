// CEngineInternalPrimitive2DViewport::CEngineInternalPrimitive2DViewport() -- 2D viewport primitive constructor: base primitive of type 0x21 on render layer 0x80000000, cleared alpha-mask info and child handle array, post-render enabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE21F0, length 105 bytes.
// Callees: CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask) @ 0x00B8FF00 (out of line).
// Vtable 0x012A7F14 (rtti-vtables.tsv: CEngineInternalPrimitive2DViewport). Members from the PDB (engine/CEngineInternalPrimitive2DViewport.h
// keeps AlphaMaskInfo/Primitives opaque, so the class is modelled locally with the PDB member names and the retail 12-byte CArray).
// Shape: the 16-byte frame that is zeroed and never read is the C4DVector TEMPORARY of `AlphaMaskPositionFactors = C4DVector(0,0,0,0)`
// in the C2DAlphaMaskInfo constructor body (VC7.1 keeps the dead temporary stores; the member is written through `lea ecx,[esi+6Ch]`
// with its own zero register edx); the two bools are body assignments AFTER that copy (as initialisers their byte stores move above
// the third vector word); AlphaMaskClipBox, Viewport, the FOVs and Layer are left uninitialised; `or byte ptr [esi+34h],8` is
// PostRenderEnable = 1 (bit 3 of the base flag byte).
struct CGraphicDataBank;
struct CEnginePrimitiveHandle;

struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};

struct C2DAlphaMaskInfo {
    CGraphicDataBank* AlphaMaskBank;               // +0x00
    unsigned long     AlphaMaskBankIndex;          // +0x04
    C4DVector         AlphaMaskPositionFactors;    // +0x08
    C2DBoxF           AlphaMaskClipBox;            // +0x18
    bool              MaskObjectsOutsideAlphaMask; // +0x28
    bool              FilterMask;                  // +0x29
    C2DAlphaMaskInfo()
        : AlphaMaskBank(0), AlphaMaskBankIndex(0)
    {
        AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
        MaskObjectsOutsideAlphaMask = false;
        FilterMask = false;
    }
};

struct CHandleArray {                              // CArray<CEnginePrimitiveHandle>, retail 12 bytes
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CHandleArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    unsigned char _pad04[0x30];
    unsigned char Enable : 1;                      // +0x34
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    unsigned char _pad35[0x13];
    CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask);   // 0x00B8FF00
};

class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase {
public:
    C2DBoxF          Viewport;                     // +0x48
    float            HorizontalFOV;                // +0x58
    float            VerticalFOV;                  // +0x5c
    bool             Use2DFOV;                     // +0x60
    C2DAlphaMaskInfo AlphaMaskInfo;                // +0x64
    long             Layer;                        // +0x90 EEngine2DLayer
    CHandleArray     Primitives;                   // +0x94
    virtual void v001();
    CEngineInternalPrimitive2DViewport();
};

CEngineInternalPrimitive2DViewport::CEngineInternalPrimitive2DViewport()
    : CEngineInternalPrimitiveBase(0x21, 0x80000000)
{
    PostRenderEnable = 1;
}
