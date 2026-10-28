// CEngineInternalPrimitive2DStaticMesh::Initialise() -- place a 2D static mesh: its world position is the clip box centre at the layer's view distance, taken from screen to view to world space through the engine camera, and its scale fits the mesh's bounding sphere to the smaller half-extent of the clip box at that distance
// Grade: DIFFER(373v365) -- three x87/register residues: the WorldPos temporary is copied through eax/edx swapped; retail keeps the screen-transformed Y on the stack (`fst [ecx+4]`) and reloads only X and Z for the view-to-world transform (ours stores Y and reloads all three); the scale is one product `c0 * Scale * distance / d4` read from memory after `fstp Scale` (a single expression is reassociated to `c0 / d4 * ...` by the lane, split statements keep Scale in a register) | tried (4 grades): float halves vs a C2DVector (C2DVector homes them in the frame as retail), ternary vs Min template, Scale stored early/late, camera local, operator* / C3DVector copy / float locals for the view transform, one-expression vs split scale | next: a two-argument TransformViewToWorld(const C3DVector&, C3DVector&) (FableWin has both overloads) or an engine Min helper; the scale probably needs Scale written through an inline accessor
// Leaf: ?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E120, length 365 bytes, plain `ret`.
// Name: FableWin.pdb `?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ` (public; catalog spelling agrees).
// Callees: CEnginePrimitiveRenderer2D::Layer2DToDistance(EEngine2DLayer) 0x00B4A6B0 (FableWin
//   `?Layer2DToDistance@CEnginePrimitiveRenderer2D@@QAEMW4EEngine2DLayer@@@Z`; landed under GetLayerDepth, NAME_FIXES.tsv).
// Inlines (FableWin.pdb CEngineCamera names): TransformScreenXYLinearZToView(C3DVector&) -- X = (X - WindowCenterX) *
//   OneOverHalfWindowWidth * Z, Y = (WindowCenterY - Y) * OneOverHalfWindowHeight * Z; TransformViewToWorld(C3DVector&) --
//   the vector read into registers, then each component written from InverseViewTransform (+0xE4).
// Globals: the 2D primitive renderer 0x01436E7C (spelling g_pPrimitiveRenderer2D_01436e7c is descriptive, see NOTES),
//   g_pEngineCamera 0x01436EA0 (engine/CEngineCamera.h retail offsets: OneOverHalfWindowWidth +0xC0, OneOverHalfWindowHeight
//   +0xC4, WindowCenterX +0xC8, WindowCenterY +0xCC, HomogenousViewScaleX +0xD4, InverseViewTransform +0xE4).
// Layout: engine/CEngineInternalPrimitive2DStaticMesh.h (Ego_r.pdb names): Initialised +0xAC, ClipBox +0xB0, Layer +0xC0,
//   BoundingSphere +0xC4 (Radius +0xD0), WorldPos +0x108; CEngineInternalPrimitiveMeshBase::Scale +0x58 (Ego_r.pdb).
// Constant: 0.5f @ 0x0122F59C.
// Shape: the half extents are one C2DVector (homed in the frame), the size their minimum; WorldPos is assigned from a C3DVector
//   temporary; the camera is read once for both transforms and again for the scale, which is written as separate statements
//   (one `c0 * Scale * distance / d4` expression is reassociated by VC7.1 into `c0 / d4 * ...`).
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
    __forceinline void TransformViewToWorld(C3DVector& v) const
    {
        const CMatrix3x4& m = PeekInverseViewTransform();
        float x = v.X;
        float y = v.Y;
        float z = v.Z;
        v.X = x * m.E11 + y * m.E21 + z * m.E31 + m.E41;
        v.Y = x * m.E12 + y * m.E22 + z * m.E32 + m.E42;
        v.Z = x * m.E13 + y * m.E23 + z * m.E33 + m.E43;
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
    const CEngineCamera* camera = g_pEngineCamera;
    camera->TransformScreenXYLinearZToView(WorldPos());
    camera->TransformViewToWorld(WorldPos());
    Scale() = size;
    const CEngineCamera* view = g_pEngineCamera;
    Scale() = view->OneOverHalfWindowWidth * Scale() * distance;
    Scale() /= view->HomogenousViewScaleX;
    Initialised = true;
    Scale() /= GetBoundingSphere().Radius;
}
