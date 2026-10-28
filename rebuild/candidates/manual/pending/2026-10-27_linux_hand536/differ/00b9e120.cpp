// CEngineInternalPrimitive2DStaticMesh::Initialise() -- place a 2D static mesh: its world position is the clip box centre at the layer's view distance, taken from screen to view to world space through the engine camera, and its scale fits the mesh's bounding sphere to the smaller half-extent of the clip box at that distance
// Grade: DIFFER(365v365) -- only the WorldPos temporary copy's registers: retail copies through `mov edx,ecx` with the words in eax, ours `mov eax,ecx` with the words in edx | tried (2026-10-28 retry, 4 grades): FableWin TransformScreenXYLinearZToWorld = TransformScreenXYLinearZToView + TransformViewToWorld(C3DVector&) -> TransformViewToWorld(const C3DVector&, C3DVector&) on a copy (fixed the fst/Y residue), TransformDistScreenToViewHorizontal(float, float&) with a named product (fixed the scale reload and order), WorldPos reference local, named temporary, user operator= (worse), user copy ctor | next: lane gap (register choice) or a real C3DVector WorldPos member once the header carries it
// Leaf: ?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E120, length 365 bytes, plain `ret`.
// Name: FableWin.pdb `?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ` (public; catalog spelling agrees).
// Callees: CEnginePrimitiveRenderer2D::Layer2DToDistance(EEngine2DLayer) 0x00B4A6B0 (FableWin
//   `?Layer2DToDistance@CEnginePrimitiveRenderer2D@@QAEMW4EEngine2DLayer@@@Z`; landed under GetLayerDepth, NAME_FIXES.tsv).
// Inlines (FableWin.pdb CEngineCamera names, all const): TransformScreenXYLinearZToWorld(C3DVector&) = TransformScreenXYLinearZToView
//   (X = (X - WindowCenterX) * OneOverHalfWindowWidth * Z, Y = (WindowCenterY - Y) * OneOverHalfWindowHeight * Z) then
//   TransformViewToWorld(C3DVector&), which forwards a copy to TransformViewToWorld(const C3DVector&, C3DVector&) (that is what
//   keeps the screen Y on the x87 stack, `fst [ecx+4]`); TransformDistScreenToViewHorizontal(float, float&) scales Scale through
//   the reference (so Scale is re-read from memory after `fstp`, as retail).
// Globals: the 2D primitive renderer 0x01436E7C (spelling g_pPrimitiveRenderer2D_01436e7c is descriptive, see NOTES),
//   g_pEngineCamera 0x01436EA0 (engine/CEngineCamera.h retail offsets: OneOverHalfWindowWidth +0xC0, OneOverHalfWindowHeight
//   +0xC4, WindowCenterX +0xC8, WindowCenterY +0xCC, HomogenousViewScaleX +0xD4, InverseViewTransform +0xE4).
// Layout: engine/CEngineInternalPrimitive2DStaticMesh.h (Ego_r.pdb names): Initialised +0xAC, ClipBox +0xB0, Layer +0xC0,
//   BoundingSphere +0xC4 (Radius +0xD0), WorldPos +0x108; CEngineInternalPrimitiveMeshBase::Scale +0x58 (Ego_r.pdb).
// Constant: 0.5f @ 0x0122F59C.
// Shape: the half extents are one C2DVector (homed in the frame), the size their minimum; WorldPos is assigned from a C3DVector
//   temporary; the scale product is a named local inside TransformDistScreenToViewHorizontal (one `c0 * dist * z / d4`
//   expression is reassociated by VC7.1 into `c0 / d4 * ...`).
#define CEngineInternalPrimitive2DStaticMesh CEngineInternalPrimitive2DStaticMeshLayout
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"
#undef CEngineInternalPrimitive2DStaticMesh
#define CEngineCamera CEngineCameraLayout
#include "engine/CEngineCamera.h"
#undef CEngineCamera

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C3DBoundingSphere { public:
    C3DVector Center;                                      // +0x00
    float     Radius;                                      // +0x0c
};
enum EEngine2DLayer { };

class CEngineCamera : public CEngineCameraLayout {
public:
    const CMatrix3x4& PeekInverseViewTransform() const { return *reinterpret_cast<const CMatrix3x4*>(InverseViewTransform); }
    __forceinline void TransformScreenXYLinearZToView(C3DVector& v) const
    {
        v.X = (v.X - WindowCenterX) * OneOverHalfWindowWidth * v.Z;
        v.Y = (WindowCenterY - v.Y) * OneOverHalfWindowHeight * v.Z;
    }
    __forceinline void TransformViewToWorld(const C3DVector& in, C3DVector& out) const
    {
        const CMatrix3x4& m = PeekInverseViewTransform();
        out.X = in.X * m.E11 + in.Y * m.E21 + in.Z * m.E31 + m.E41;
        out.Y = in.X * m.E12 + in.Y * m.E22 + in.Z * m.E32 + m.E42;
        out.Z = in.X * m.E13 + in.Y * m.E23 + in.Z * m.E33 + m.E43;
    }
    __forceinline void TransformViewToWorld(C3DVector& v) const
    {
        TransformViewToWorld(C3DVector(v.X, v.Y, v.Z), v);
    }
    __forceinline void TransformDistScreenToViewHorizontal(float z, float& dist) const
    {
        float d = OneOverHalfWindowWidth * dist * z;
        dist = d / HomogenousViewScaleX;
    }
    __forceinline void TransformScreenXYLinearZToWorld(C3DVector& v) const
    {
        TransformScreenXYLinearZToView(v);
        TransformViewToWorld(v);
    }
};
extern CEngineCamera* g_pEngineCamera;                             // 0x01436EA0

class CEnginePrimitiveRenderer2D {
public:
    float Layer2DToDistance(EEngine2DLayer layer);                // 0x00B4A6B0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D_01436e7c;   // 0x01436E7C

class CEngineInternalPrimitive2DStaticMesh : public CEngineInternalPrimitive2DStaticMeshLayout {
public:
    void Initialise();
private:
    float& Scale() { return *reinterpret_cast<float*>(_pad_0x58); }                         // CEngineInternalPrimitiveMeshBase::Scale +0x58
    const C2DBoxF& ClipBox() const { return *reinterpret_cast<const C2DBoxF*>(&ClipBox_TLX); }
    EEngine2DLayer GetLayer() const { return *reinterpret_cast<const EEngine2DLayer*>(Layer); }
    const C3DBoundingSphere& GetBoundingSphere() const { return *reinterpret_cast<const C3DBoundingSphere*>(BoundingSphere); }
    C3DVector& WorldPos() { return *reinterpret_cast<C3DVector*>(&WorldPos_X); }
};

void CEngineInternalPrimitive2DStaticMesh::Initialise()
{
    float distance = g_pPrimitiveRenderer2D_01436e7c->Layer2DToDistance(GetLayer());
    C2DVector halfSize((ClipBox().BRX - ClipBox().TLX) * 0.5f, (ClipBox().BRY - ClipBox().TLY) * 0.5f);
    float size = halfSize.X < halfSize.Y ? halfSize.X : halfSize.Y;
    WorldPos() = C3DVector(halfSize.X + ClipBox().TLX, halfSize.Y + ClipBox().TLY, distance);
    g_pEngineCamera->TransformScreenXYLinearZToWorld(WorldPos());
    Scale() = size;
    g_pEngineCamera->TransformDistScreenToViewHorizontal(distance, Scale());
    Initialised = true;
    Scale() /= GetBoundingSphere().Radius;
}
