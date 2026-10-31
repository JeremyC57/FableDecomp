// NGraphicAppearance::CTCGraphicAppearanceStaticMesh::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) -- draws the appearance's main static mesh: nothing without a mesh bank; the first draw registers a persistent static-mesh primitive (later position-only draws just refresh its placement); unless the debug theme channel hides it, builds a CEnginePrimitiveStaticMesh at the interpolated position and orientation (scaled render size, alpha, tint, fade distance, lighting channel, bullet-time flag), attaches the current mesh effect and hands it to the primitive; always true
// Grade: DIFFER(564v563) 66 difflines (was 78): the mesh takes the channel copy instead of re-reading appearance.LightingChannel (no &LightingChannel CSE in ebx); left: frame 0x9c v 0x90 (position/primitive temps not shared), Scale*RenderSizeX load order, interpolation order (retry wave 2 sec2b)
// Was: DIFFER(559v563) -- frame 0x98 v 0x90 (the position temporary gets its own slot; retail reuses the CPrimitive temporary's), the x87 interpolation order (ours sums Z,Y,X; retail X,Y,Z), fld RenderSizeX v retail fld Scale (operand order inert), &LightingChannel CSE'd into ebx so ebp/ebx prologue pushes and the UniqueID loads move
// Leaf: ?OnAppearanceDraw@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077BA40, 561 bytes (padded to 576), `ret 8`; vtable 0x0126AD50 slot 0 (the
//   CAGraphicAppearanceComponent base at +0x0C, so `this` is the component and ParentThing reads [this-8]).
//   Ego_r.pdb ?OnAppearanceDraw@CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z.
// Callees: CTCGraphicAppearance::GetFadeDistance 0x004BC180, CBaseClass::CBaseClass 0x0099A2D0 (the handle base),
//   CTCGraphicAppearance::AddPersistentPrimitive 0x004C0050, CPrimitive::~CPrimitive 0x004C4160 (landed as
//   "SPrimitiveHandleHolder"), CTCGraphicAppearance::RefreshPrimitivePos 0x004BC340, CThing::IsAffectedByBulletTime
//   0x004C9920, CRightHandedSet::Interpolate 0x00A23190, CTCGraphicAppearance::GetTintColour 0x004BD040 (built in its
//   argument slot), CEnginePrimitiveStaticMesh ctor 0x0077BD70, CTCGraphicAppearance::GetPMeshEffect 0x004BCF50,
//   CTCGraphicAppearance::SetPrimitiveGraphics 0x004BD620; the mesh effect's virtual slot 1 (Destroy).
// Layout: CTCGraphicAppearanceStaticMesh (Ego_r.pdb, 20 bytes) = CTCBase (vptr, ParentThing +4, flags +8) +
//   CAGraphicAppearanceComponent (vptr +0x0C) + PrimHandle +0x10. Retail CTCGraphicAppearance as the landed
//   SetActivePersistenceFlags 0x004BFFD0 model (Scale +0x34, LightingChannel +0x40, MainGraphic +0x44, Alpha +0x5A).
//   CGraphicAppearanceDrawInfo (PDB): Interpolate +0, Pos +4, Orientation +8, ReasonsForDraw +0x1C. CThing UniqueID +0x18.
//   Retail CEnginePrimitiveStaticMesh is 0x48 bytes with MeshEffect at +0x38 (PDB 76 / +60).
// Shape: size and fade distance are locals computed first; the CPrimitive temporary {1, handle, bank}; the static-mesh
//   primitive is one constructor expression; `mesh.MeshEffect = GetPMeshEffect()` inlines the counted assignment; the
//   mesh effect's count lives in a second base (`add eax,4` release shape, as the landed 0x00410820).
class CBaseClass {                                   // vtable 0x01231710
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual void v000();
};

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                   // +0x04
};

class CIVPrimitivePointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVPrimitivePointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass {  // vtable 0x0122F598
public:
    CIVPrimitivePointer Data;                        // +0x04
    CEnginePrimitiveHandle() { }
    virtual void v001();
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

class CRightHandedSet {
public:
    C3DVector Up;                                    // +0x00
    C3DVector Forward;                               // +0x0C
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};

class C3DPositionInterpolator {
public:
    void* __vfptr;                                   // +0x00
    C3DVector StartPosition;                         // +0x04
    C3DVector EndPosition;                           // +0x10
    __forceinline C3DVector GetPosition(float t) const { return StartPosition + (EndPosition - StartPosition) * t; }
};

class C3DOrientationInterpolator {
public:
    void* __vfptr;                                   // +0x00
    CRightHandedSet StartOrientation;                // +0x04
    CRightHandedSet EndOrientation;                  // +0x1C
    __forceinline CRightHandedSet GetOrientation(float t) const { return StartOrientation.Interpolate(EndOrientation, t); }
};

struct CRGBColour {                                  // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};

struct CEnginePrimitiveThemeChannelDesc {            // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
};

class CMeshEffectVirtual {
public:
    virtual void v000();
    virtual void Destroy();                          // slot 1 (+0x04)
};

struct CMeshEffectRefCount {                         // the count base behind the vptr (+0x04)
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

class CEngineMeshEffect : public CMeshEffectVirtual, public CMeshEffectRefCount {
public:
    long MeshEffect;                                 // +0x08 EEngineMeshEffect
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CMeshEffectPointer {                           // CIVCountedPointer<CEngineMeshEffect>
public:
    CEngineMeshEffect* Object;
    CMeshEffectPointer() : Object(0) { }
    __forceinline ~CMeshEffectPointer()
    {
        if (Object)
            Object->Release();
    }
    __forceinline CMeshEffectPointer& operator=(const CMeshEffectPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 };

class CEnginePrimitive {                             // base of every engine primitive
public:
    long Type;                                       // +0x00 EEnginePrimitiveType
    long InterpolationSet;                           // +0x04
};

class CEnginePrimitiveStaticMesh : public CEnginePrimitive {   // retail 0x48 bytes
public:
    unsigned char      _body[0x38 - 0x08];
    CMeshEffectPointer MeshEffect;                   // +0x38
    unsigned char      _tail[0x48 - 0x3c];
    CEnginePrimitiveStaticMesh(unsigned __int64 thingID, unsigned long meshIndex, const C3DVector& position,
        const CRightHandedSet& orientation, float size, bool lit, unsigned char alpha, CRGBColour tint, float fadeStart,
        EEngineInterpolationSet interpolationSet, const CEnginePrimitiveThemeChannelDesc& channel,
        bool bulletTimeAffectsCloth);                // 0x0077BD70
};

class CThing {
public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UniqueID;                       // +0x18
    bool IsAffectedByBulletTime() const;             // 0x004C9920
};

extern long g_DebugThemeChannel_013756f0;           // 0x013756F0 (only this TU and 0x007E15C0 read it)

namespace NGraphicAppearance {

class CPrimitive {                                   // 16 bytes (Ego_r.pdb)
public:
    long                   Type;                     // +0x00 EPrimitiveType
    CEnginePrimitiveHandle PrimitiveHandle;          // +0x04
    long                   BankIndex;                // +0x0C
    CPrimitive(long type, long bankIndex) : Type(type), BankIndex(bankIndex) { }
    ~CPrimitive();                                   // 0x004C4160
};

#pragma pack(push, 2)
struct CEngineGraphic {                              // 14 bytes (PDB), the main graphic
    unsigned long BankIndex;                         // +0x00
    float         AnimStep;                          // +0x04
    float         RenderSizeX;                       // +0x08
    bool          AdditiveAlpha;                     // +0x0C
    unsigned char Type;                              // +0x0D
};
#pragma pack(pop)

class CTCGraphicAppearance {                         // retail offsets
public:
    unsigned char  _pad_0x00[0x34];
    float          Scale;                            // +0x34
    unsigned char  _pad_0x38[0x40 - 0x38];
    CEnginePrimitiveThemeChannelDesc LightingChannel;   // +0x40
    CEngineGraphic MainGraphic;                      // +0x44
    unsigned char  _pad_0x52[0x5a - 0x52];
    unsigned char  Alpha;                            // +0x5A
    float GetFadeDistance() const;                   // 0x004BC180
    CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive);   // 0x004C0050
    void RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position,
        const C3DOrientationInterpolator& orientation);                // 0x004BC340
    const CRGBColour GetTintColour() const;          // 0x004BD040
    CMeshEffectPointer GetPMeshEffect();             // 0x004BCF50
    void SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& graphics, unsigned short flags);   // 0x004BD620
};

class CGraphicAppearanceDrawInfo {                   // 36 bytes (PDB)
public:
    float                       Interpolate;         // +0x00
    C3DPositionInterpolator*    Pos;                 // +0x04
    C3DOrientationInterpolator* Orientation;         // +0x08
    void*                       Anim;                // +0x0C
    void*                       PTextureMorphs;      // +0x10
    void*                       PTextureDecals;      // +0x14
    void*                       HiddenMaterials;     // +0x18
    unsigned long               ReasonsForDraw;      // +0x1C
    bool                        HiddenMaterialsUsed; // +0x20
    bool                        TextureMorphsUsed;   // +0x21
};

class CTCBase {                                      // 12 bytes
public:
    virtual ~CTCBase();
    CThing* ParentThing;                             // +0x04
    bool    ToBeRemoved;                             // +0x08
    bool    IsInGlobalUpdateTCs;                     // +0x09
};

class CAGraphicAppearanceComponent {                 // 4 bytes: the component interface
public:
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) = 0;
    virtual void OnAppearanceClear() = 0;
};


class CTCGraphicAppearanceStaticMesh : public CTCBase, public CAGraphicAppearanceComponent {
public:
    CPrimitive* PrimHandle;                          // +0x10
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
};

bool CTCGraphicAppearanceStaticMesh::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info)
{
    if (!appearance.MainGraphic.BankIndex)
        return true;
    float size = appearance.Scale * appearance.MainGraphic.RenderSizeX;
    float fadeDistance = appearance.GetFadeDistance();
    if (!PrimHandle) {
        PrimHandle = appearance.AddPersistentPrimitive(CPrimitive(1, appearance.MainGraphic.BankIndex));
    }
    else if (info.ReasonsForDraw == 1) {
        appearance.RefreshPrimitivePos(PrimHandle, *info.Pos, *info.Orientation);
        return true;
    }
    CEnginePrimitiveThemeChannelDesc channel = appearance.LightingChannel;
    if (g_DebugThemeChannel_013756f0 >= 0 && channel.ChannelIndex0 != 0)
        return true;
    bool bulletTime = ParentThing->IsAffectedByBulletTime();
    unsigned char alpha = appearance.Alpha;
    CEnginePrimitiveStaticMesh mesh(ParentThing->UniqueID, appearance.MainGraphic.BankIndex,
        info.Pos->GetPosition(info.Interpolate), info.Orientation->GetOrientation(info.Interpolate), size, true,
        alpha, appearance.GetTintColour(), fadeDistance, ENGINE_INTERPOLATION_SET_0, channel, bulletTime);
    mesh.MeshEffect = appearance.GetPMeshEffect();
    appearance.SetPrimitiveGraphics(PrimHandle, mesh, 0);
    return true;
}

}
