// CEngineInternalPrimitive2DAnimatedMesh::Initialise() -- fit the 2D animated mesh into its clip box: scale = min(half box width, half box height) through the camera's window terms divided by the mesh sphere radius, the view-space position from the box centre (TransformScreenXYLinearZToView at the layer's distance) transformed to world space, then set Initialised
// Grade: DIFFER(363v374) -- x87 order: retail stores Scale = min(halfW, halfH), reloads it from memory for OneOverHalfWindowWidth*Scale*distance/HomogenousViewScaleX and multiplies the early-loaded 1/radius by it afterwards; VC7.1 forwards the stored value, reassociates the product and schedules `mov byte [esi+128h],1` one store earlier (/Op, optimize("g",off), radius-local placements, `Scale /= r`, float& alias, std::min/if-else forms and a SetScaleFromCamera inline were all inert)
// Leaf: ?Initialise@CEngineInternalPrimitive2DAnimatedMesh@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCF4F0, 374 bytes, thiscall, plain `ret`. 1 grade used.
// Identity (proven): Ego_r.pdb ?Initialise@CEngineInternalPrimitive2DAnimatedMesh@@QAEXXZ in donor order right after the landed
//   SetupMesh2D 0x00BCF430; called by Update 0x00BD2BC0 just before `Initialised = false`.
// Callees: CEnginePrimitiveRenderer2D::Layer2DToDistance 0x00B4A6B0 on g_pEnginePrimitiveRenderer2D 0x01436E7C; camera terms from
//   g_pEngineCamera 0x01436EA0 (OneOverHalfWindowWidth/Height +0xc0/+0xc4, WindowCenterX/Y +0xc8/+0xcc, HomogenousViewScaleX
//   +0xd4, InverseViewTransform +0xe4).
enum EEngine2DLayer { };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    __forceinline void TransformPoint(C3DVector& v) const
    {
        float x = v.X, y = v.Y, z = v.Z;
        v.X = x * E11 + y * E21 + z * E31 + E41;
        v.Y = x * E12 + y * E22 + z * E32 + E42;
        v.Z = x * E13 + y * E23 + z * E33 + E43;
    }
};
class CEngineCamera { public:
    unsigned char _pad00[0xc0];
    float OneOverHalfWindowWidth;      // +0xc0
    float OneOverHalfWindowHeight;     // +0xc4
    float WindowCenterX;               // +0xc8
    float WindowCenterY;               // +0xcc
    float AspectRatio;                 // +0xd0
    float HomogenousViewScaleX;        // +0xd4
    float HomogenousViewScaleY;        // +0xd8
    float ZBufferFactor;               // +0xdc
    float ZBufferOffset;               // +0xe0
    CMatrix3x4 InverseViewTransform;   // +0xe4
    __forceinline void TransformScreenXYLinearZToView(C3DVector& v) const
    {
        v.X = (v.X - WindowCenterX) * OneOverHalfWindowWidth * v.Z;
        v.Y = (WindowCenterY - v.Y) * OneOverHalfWindowHeight * v.Z;
    }
    __forceinline void TransformViewToWorld(C3DVector& v) const { InverseViewTransform.TransformPoint(v); }
    __forceinline void TransformScreenXYLinearZToWorld(C3DVector& v) const
    {
        TransformScreenXYLinearZToView(v);
        TransformViewToWorld(v);
    }
};
class CEnginePrimitiveRenderer2D { public:
    float Layer2DToDistance(EEngine2DLayer layer);    // 0x00B4A6B0
};
extern CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D;   // 0x01436E7C
extern CEngineCamera* g_pEngineCamera;                             // 0x01436EA0

class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DPositionInterpolator { public:
    void* __vftable;
    C3DVector StartPosition;           // +0x04
    C3DVector EndPosition;             // +0x10
};
class CEngineInternalPrimitive2DAnimatedMesh { public:
    unsigned char           _pad00[0x58];
    float                   Scale;                 // +0x58
    unsigned char           _pad5c[0x84 - 0x5c];
    C3DPositionInterpolator WorldPos;              // +0x84
    unsigned char           _pada0[0x128 - 0xa0];
    bool                    Initialised;           // +0x128
    EEngine2DLayer          Layer;                 // +0x12c
    C2DBoxF                 ClipBox;               // +0x130
    C3DBoundingSphere       ObjectBoundingSphere;  // +0x140
    void Initialise();
};

void CEngineInternalPrimitive2DAnimatedMesh::Initialise()
{
    float distance = g_pEnginePrimitiveRenderer2D->Layer2DToDistance(Layer);
    float halfWidth = (ClipBox.BRX - ClipBox.TLX) * 0.5f;
    float halfHeight = (ClipBox.BRY - ClipBox.TLY) * 0.5f;
    Scale = halfWidth < halfHeight ? halfWidth : halfHeight;
    Scale = g_pEngineCamera->OneOverHalfWindowWidth * Scale * distance / g_pEngineCamera->HomogenousViewScaleX;
    Scale *= 1.0f / ObjectBoundingSphere.Radius;
    WorldPos.StartPosition = C3DVector(ClipBox.TLX + halfWidth, ClipBox.TLY + halfHeight, distance);
    g_pEngineCamera->TransformScreenXYLinearZToWorld(WorldPos.StartPosition);
    WorldPos.EndPosition = WorldPos.StartPosition;
    Initialised = true;
}
