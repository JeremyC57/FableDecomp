// CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(CArray<CEngineScene*>& sceneList) -- CEnginePrimitiveManager(sceneList), own vtable, the alpha-mask info reset (no bank, index 0, zero position factors, both mask flags clear)
// Grade: DIFFER(77v77) -- copy registers only: retail forwards all four zero dwords of the frame temporary through edx (`mov edx,eax` then four `mov [ecx+n],edx`, `lea ecx,[esi+14h]` after `mov edx,eax`); ours alternates edx/eax and schedules the lea first
// Wave-25 chunk 623 retry (tried, inert): PDB constructor spelling (CArray<CEngineScene*>&) and real base names; `#pragma pack(1)` C4DVector; --flags /Oa and /Ow
// Leaf: ??0CEnginePrimitive2DViewportManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE1E20, 77 bytes, `ret 4`.
// Identity: vtable 0x012A7ED8 = CEnginePrimitive2DViewportManager (rtti-vtables.tsv); Ego_r.pdb
//   ??0CEnginePrimitive2DViewportManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z. Label correction: the catalog label
//   "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is propagated.
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>&) 0x00BACDB0 (PDB spelling; the landed
//   source spells it (long), see NAME_FIXES.tsv).
// Layout = Ego_r.pdb: CEnginePrimitiveManager 0xc (SceneList +4, Enable +8), AlphaMaskInfo (C2DAlphaMaskInfo, 0x2c) +0xc:
//   AlphaMaskBank +0xc, AlphaMaskBankIndex +0x10, AlphaMaskPositionFactors (C4DVector) +0x14, AlphaMaskClipBox (C2DBoxF,
//   empty ctor) +0x24, MaskObjectsOutsideAlphaMask +0x34, FilterMask +0x35. engine/CEnginePrimitive2DViewportManager.h
//   flattens the base and the alpha-mask info into pads, so the classes are restated locally.
// Shape: the alpha-mask info's inline ctor ASSIGNS a zero C4DVector temporary to the position factors (implicit
//   operator= = 16-byte block copy): the temporary keeps its four zero stores in the 16-byte frame.
class CGraphicDataBank;
class CEngineScene;
template <class T> class CArray;

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

class CEnginePrimitiveManager : public CBaseClassNonCopyable {   // vtable 0x012A53E8, 0xc bytes
public:
    CArray<CEngineScene*>* SceneList;                       // +0x04
    bool Enable;                                            // +0x08
    CEnginePrimitiveManager(CArray<CEngineScene*>& sceneList);   // 0x00BACDB0
};

#pragma pack(push, 1)
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
#pragma pack(pop)

class C2DBoxF {
public:
    float X0, Y0, X1, Y1;
    C2DBoxF() { }
};

class C2DAlphaMaskInfo {                                    // 0x2c bytes
public:
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

class CEnginePrimitive2DViewportManager : public CEnginePrimitiveManager {   // vtable 0x012A7ED8
public:
    C2DAlphaMaskInfo AlphaMaskInfo;                         // +0x0c
    CEnginePrimitive2DViewportManager(CArray<CEngineScene*>& sceneList);
    virtual ~CEnginePrimitive2DViewportManager();
};

CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(CArray<CEngineScene*>& sceneList)
    : CEnginePrimitiveManager(sceneList)
{
}
