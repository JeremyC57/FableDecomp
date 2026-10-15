// CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(long type) -- CEnginePrimitiveManager(type), own vtable, the alpha-mask info reset (no bank, index 0, zero position factors, both mask flags clear)
// Grade: DIFFER(77v77) -- only the copy registers differ: retail forwards all four zero dwords of the frame temporary through edx (`mov edx,eax` then four `mov [ecx+n],edx`), the lane alternates edx/eax and schedules `lea ecx,[esi+14h]` before `mov edx,eax` (copy-register allocation lane gap). Tried: member copy-constructed from the temporary (temporary elided, 51 bytes), user memberwise operator= (temporary elided), implicit assignment in the body (this, 77v77).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE1E20, length 77 bytes (+ int3 padding).
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager(long) @ 0x00BACDB0 (landed). Vtable 0x012A7ED8 =
// CEnginePrimitive2DViewportManager (rtti-vtables.tsv): the chunk label
// "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is propagated.
// Layout = Ego_r.pdb: CEnginePrimitiveManager 0xc, AlphaMaskInfo (C2DAlphaMaskInfo, 0x2c bytes) +0xc:
// AlphaMaskBank +0xc, AlphaMaskBankIndex +0x10, AlphaMaskPositionFactors (C4DVector) +0x14,
// AlphaMaskClipBox (C2DBoxF, left alone by its empty constructor) +0x24, MaskObjectsOutsideAlphaMask
// +0x34, FilterMask +0x35.
// Shape: the alpha-mask info is a member object with an inline constructor (all its stores follow the
// vtable store); its position factors are ASSIGNED a zero C4DVector temporary in the constructor body
// (implicit assignment = a 16-byte block copy): the temporary keeps its own four zero stores in the
// 16-byte frame while the copy is forwarded from the zero register through the member's address in ecx.
struct CGraphicDataBank;

struct CEnginePrimitiveManager_Calls {                      // 0xc bytes (vtable 0x012A53E8)
    virtual void v00();
    long m_type;                                            // +0x04
    bool m_enabled;                                         // +0x08
    CEnginePrimitiveManager_Calls(long type);               // 0x00BACDB0
};

struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};

struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF() { }
};

struct C2DAlphaMaskInfo {                                   // 0x2c bytes
    CGraphicDataBank* AlphaMaskBank;                        // +0x00
    unsigned long     AlphaMaskBankIndex;                   // +0x04
    C4DVector         AlphaMaskPositionFactors;             // +0x08
    C2DBoxF           AlphaMaskClipBox;                     // +0x18
    bool              MaskObjectsOutsideAlphaMask;          // +0x28
    bool              FilterMask;                           // +0x29
    C2DAlphaMaskInfo()
        : AlphaMaskBank(0)
        , AlphaMaskBankIndex(0)
        , MaskObjectsOutsideAlphaMask(false)
        , FilterMask(false)
    {
        AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
    }
};

struct CEnginePrimitive2DViewportManager : CEnginePrimitiveManager_Calls {   // vtable 0x012A7ED8
    C2DAlphaMaskInfo AlphaMaskInfo;                         // +0x0c
    virtual void v00();
    CEnginePrimitive2DViewportManager(long type);
};

CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(long type)
    : CEnginePrimitiveManager_Calls(type)
    , AlphaMaskInfo()
{
}
