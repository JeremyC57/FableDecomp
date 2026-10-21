// Grade: DIFFER(301v314) -- frame 0x8c vs 0x80 (no temp-slot overlap: the uid-high copy shares the scaled-difference vector's dead Y word in retail), UniqueID low preloaded into ebx in retail, x87 add order of the interpolated position
// CThingTrackNode::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- after the physical base, submit the track node's def graphic as a static mesh at the interpolated position and orientation
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837B30, length 314 bytes (+ int3 padding), `ret 4` (vtable slot 36).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0, CThing def getter @ 0x004C79A0 (the graphic record at def +0xe6), the physics component's virtual slot 71 (+0x11c, the interpolated right-handed set by hidden pointer),
// CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh @ 0x0077BD70 (landed; 13 stack words), the engine's virtual slot 20
// (+0x50, add primitive), the mesh effect's slot 1 (destroy) when the temporary's reference drops to zero.
// Layout: CEnginePrimitivePrepareInfo (Ego_r.pdb, as PreparePrimitivesForEngine 0x0051EAB0): Current.WFInterpolate +4,
// Engine +0x34; CThing UniqueID +0x18, PhysicsTC +0x60 (Position +0x0c, LastPosition +0x18); CThingPhysical Scale +0x9c;
// the engine graphic's mesh index +0 and size +8. CEnginePrimitiveStaticMesh as the landed ctor (0x4c bytes, mesh effect
// pointer +0x3c).
// Shape: the primitive is a temporary handed straight to the engine (the ctor's `this` is pushed), its tint a by-value
// CRGBColour built in the argument slot, its theme-channel argument an uninitialised default; the position is
// `(Position - LastPosition) * t + LastPosition`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    CRGBColour(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
enum EEngineInterpolationSet { INTERPOLATION_DEFAULT = 0 };
struct CEngineMeshEffect {
    virtual void v00();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long m_refCount;                                           // +0x04
    void Release() { --m_refCount; if (m_refCount == 0) Destroy(); }
};
struct CEnginePrimitiveStaticMesh {                            // 0x4c bytes
    unsigned char      _pad00[0x3c];
    CEngineMeshEffect* MeshEffect;                             // +0x3c
    unsigned char      _pad40[0x4c - 0x40];
    CEnginePrimitiveStaticMesh(unsigned __int64 uid, unsigned long mesh, const C3DVector& pos, const CRightHandedSet& rhs,
                               float size, bool lit, unsigned char alpha, CRGBColour tint, float fade,
                               EEngineInterpolationSet interp, const CEnginePrimitiveThemeChannelDesc& channel, bool bullet);   // 0x0077BD70
    ~CEnginePrimitiveStaticMesh() { if (MeshEffect) MeshEffect->Release(); }
};
class CIEngine {
public:
    virtual void e000(); virtual void e001(); virtual void e002(); virtual void e003(); virtual void e004(); virtual void e005();
    virtual void e006(); virtual void e007(); virtual void e008(); virtual void e009(); virtual void e010(); virtual void e011();
    virtual void e012(); virtual void e013(); virtual void e014(); virtual void e015(); virtual void e016(); virtual void e017();
    virtual void e018(); virtual void e019();
    virtual void AddPrimitive(const CEnginePrimitiveStaticMesh& primitive);   // slot 20 (+0x50)
};
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
struct CEnginePrimitivePrepareInfo {
    CInterpolationInfoSet Current;                             // +0x00
    unsigned char         _pad08[0x34 - 0x08];
    CIEngine*             Engine;                              // +0x34
};
class CTCPhysicsBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual CRightHandedSet GetInterpolatedRHSet(float interpolate) const;   // slot 71 (+0x11c)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                                    // +0x0c
    C3DVector     LastPosition;                                // +0x18
};
struct CEngineGraphic {
    unsigned long MeshIndex;                                   // +0x00
    unsigned char _pad04[4];
    float         Size;                                        // +0x08
};
#pragma pack(push, 4)
class CThingPhysical {
public:
    virtual void v00();
    unsigned char    _pad04[0x18 - 0x04];
    unsigned __int64 UniqueID;                                 // +0x18
    unsigned char    _pad20[0x60 - 0x20];
    CTCPhysicsBase*  PhysicsTC;                                // +0x60
    unsigned char    _pad64[0x9c - 0x64];
    float            Scale;                                    // +0x9c
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
};
#pragma pack(pop)
#pragma pack(push, 1)
struct CThingTrackNodeDef {
    unsigned char  _pad00[0xe6];
    CEngineGraphic Graphic;                                    // +0xe6
};
#pragma pack(pop)
class CThingTrackNode : public CThingPhysical {
public:
    const CThingTrackNodeDef* GetDef();                        // 0x004C79A0
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingTrackNode::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        const CEngineGraphic* graphic = &GetDef()->Graphic;
        float t = info.Current.WFInterpolate;
        info.Engine->AddPrimitive(CEnginePrimitiveStaticMesh(UniqueID, graphic->MeshIndex,
            (PhysicsTC->Position - PhysicsTC->LastPosition) * t + PhysicsTC->LastPosition,
            PhysicsTC->GetInterpolatedRHSet(info.Current.WFInterpolate), graphic->Size * Scale, true, 0xff,
            CRGBColour(0xff, 0xff, 0xff, 0xff), 0.0f, INTERPOLATION_DEFAULT, CEnginePrimitiveThemeChannelDesc(), true));
        return true;
    }
    return false;
}
