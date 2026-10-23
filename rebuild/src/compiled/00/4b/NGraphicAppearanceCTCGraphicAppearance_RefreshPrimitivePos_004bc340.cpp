// NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation) -- hand the engine the primitive's placement at the start of the interpolation (t = 0)
// Copy constructor: the C3DVector model DECLARES its memberwise copy constructor (retail's header class has one); with the
// implicit copy VC7.1 schedules the copies differently (found by the retry-10 copy-constructor sweep).
// Leaf: RefreshPrimitivePos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC340, length 202 bytes (+ int3 padding). __fastcall, `ret 0Ch`.
//         Ego_r.pdb: `?RefreshPrimitivePos@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVC3DPosition
//         Interpolator@@ABVC3DOrientationInterpolator@@@Z`.
// Callees: CRightHandedSet::Interpolate(const CRightHandedSet&, float) const @ 0x00A23190 (by hidden pointer), the engine's
//          virtual SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) (slot 25, +0x64) on
//          g_pEngineGlobals->+0x34 (global @ 0x013B8A18). Float pool 0x0122DEDC = 0.0f.
// Shape: one statement -- a CEnginePrimitivePosition temporary built from the interpolated orientation (evaluated first)
//        and position `Start + (End - Start) * t` through named-result C3DVector operators.
// Layout: C3DPositionInterpolator (Ego_r.pdb): StartPosition +0x04, EndPosition +0x10; C3DOrientationInterpolator:
//         StartOrientation +0x04, EndOrientation +0x1C; CPrimitive::PrimitiveHandle +0x04; CEnginePrimitivePosition
//         {WorldPos, RightHandedSet} (36 bytes).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CRightHandedSet {
public:
    C3DVector Up;                                   // +0x00
    C3DVector Forward;                              // +0x0C
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};

class CEnginePrimitivePosition {                    // 36 bytes
public:
    C3DVector WorldPos;                             // +0x00
    CRightHandedSet RightHandedSet;                 // +0x0C
    CEnginePrimitivePosition(const C3DVector& position, const CRightHandedSet& set) : WorldPos(position), RightHandedSet(set) {}
};

class C3DPositionInterpolator {
public:
    void* __vfptr;                                  // +0x00
    C3DVector StartPosition;                        // +0x04
    C3DVector EndPosition;                          // +0x10
    __forceinline C3DVector GetPosition(float t) const { return StartPosition + (EndPosition - StartPosition) * t; }
};

class C3DOrientationInterpolator {
public:
    void* __vfptr;                                  // +0x00
    CRightHandedSet StartOrientation;               // +0x04
    CRightHandedSet EndOrientation;                 // +0x1C
    __forceinline CRightHandedSet GetOrientation(float t) const { return StartOrientation.Interpolate(EndOrientation, t); }
};

class CEnginePrimitiveHandle;
class CEngine {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24();
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle& primitive, const CEnginePrimitivePosition& position);   // slot 25 (+0x64)
};

struct CEngineGlobals {
    unsigned char _pad_0x00[0x34];
    CEngine*      m_pEngine;                        // +0x34
};
extern CEngineGlobals* g_pEngineGlobals_013b8a18;            // 0x013B8A18

class CEnginePrimitiveHandle {
public:
    long m_header;                                  // +0x00
    void* Data;                                     // +0x04
};

namespace NGraphicAppearance {

class CPrimitive {                                  // 16 bytes (Ego_r.pdb)
public:
    long                   Type;                    // +0x00 EPrimitiveType
    CEnginePrimitiveHandle PrimitiveHandle;         // +0x04
    long                   BankIndex;               // +0x0C
};

class CTCGraphicAppearance {
public:
    void RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation);
};

void CTCGraphicAppearance::RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation)
{
    g_pEngineGlobals_013b8a18->m_pEngine->SetPrimitivePosition(primitive->PrimitiveHandle,
        CEnginePrimitivePosition(position.GetPosition(0.0f), orientation.GetOrientation(0.0f)));
}

}