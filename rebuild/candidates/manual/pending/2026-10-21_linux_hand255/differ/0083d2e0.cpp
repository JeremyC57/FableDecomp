// Grade: DIFFER(356v372) -- flow, base call, graphic, interpolators and mesh-effect release match; retail pre-evaluates the size fld, both PhysicsTC loads and the 64-bit UniqueID (ebx + a spilled high word, a 4th callee-saved register) before the two interpolated-transform calls and sinks the theme-desc zero stores next to the slot-71 call; tried: temporary vs named mesh, size / id / physics locals
// CThingMarker::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo& info) -- after the physical thing's own preparation, a marker draws its current graphic as a white static mesh (sized by the graphic's render size times the thing's scale, at the interpolated position/orientation) when the display engine shows markers, then lets its graphic appearance prepare against the physics position/orientation interpolators
// Leaf: ?DrawPreparePrimitivesForRendering@CThingMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D2E0, length 372 bytes (+12 int3), `ret 4`; CThing vtable slot 36
//   (the catalog's "CThingMarker::vfunc_36"; slot 36 is CThing::DrawPreparePrimitivesForRendering, see the landed
//   CThingDeadCreature row).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0, CThing::GetDisplayEngine @ 0x004C7A70 and its
//   virtual slot 9 (+0x24, unnamed bool), CThingMarker::GetCurrentGraphic @ 0x0083D190 (manifest "CThingVillage::
//   GetCurrentGraphic", propagated; returns a CEngineGraphic by hidden pointer), the physics component's virtual slot 71
//   (+0x11c, interpolated orientation) and 0x0049D150 (interpolated position; manifest "GetFacingVector"),
//   CEnginePrimitiveStaticMesh ctor @ 0x0077BD70, the engine's virtual slot 20 (+0x50, add primitive),
//   C3DPositionInterpolator ctor @ 0x006AF570, the physics slots 72/73 (+0x120/+0x124), C3DOrientationInterpolator ctor
//   @ 0x005508D0, CTCGraphicAppearance::PrepareForRenderIfNeeded @ 0x004BE490, ~CBase @ 0x0099A2E0.
// Layout (retail): CThing UniqueID +0x18, PhysicsTC +0x60, PTCGraphicAppearance +0x64 (Ego_r.pdb +0x70/+0x74, CThing
//   0x10 lower), CThingPhysical::Scale +0x9c (PDB +0xb4); CEnginePrimitivePrepareInfo interpolation fraction +4, Engine
//   +0x34; physics component positions +0x0c / +0x18; CEnginePrimitiveStaticMesh MeshEffect +0x3c (CIVCountedPointer).
#ifndef _MSC_VER
#define __int64 long long
#endif
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    CRGBColour(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) {}
};
enum EEngineInterpolationSet { INTERPOLATION_DEFAULT = 0 };
struct CEngineMeshEffect {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +4
};
struct CEnginePrimitiveStaticMesh {
    unsigned char      _pad00[0x3c];
    CEngineMeshEffect* MeshEffect;                      // +0x3c CIVCountedPointer<CEngineMeshEffect>
    unsigned char      _pad40[0x4c - 0x40];
    CEnginePrimitiveStaticMesh(unsigned __int64 id, unsigned long mesh, const C3DVector& pos, const CRightHandedSet& rhs,
                               float size, bool lit, unsigned char alpha, CRGBColour tint, float fade,
                               EEngineInterpolationSet interp, const CEnginePrimitiveThemeChannelDesc& channel,
                               bool bulletTime);        // 0x0077BD70
    ~CEnginePrimitiveStaticMesh()
    {
        if (MeshEffect && --MeshEffect->m_refCount == 0)
            MeshEffect->Destroy();
    }
};
struct CEngineGraphic {
    unsigned long BankIndex;                            // +0
    float         AnimStep;                             // +4
    float         RenderSizeX;                          // +8
    bool          AdditiveAlpha;                        // +0xc
    unsigned char Type;                                 // +0xd
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CEnginePrimitiveStaticMesh& primitive);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    long          _interp0;                             // +0x00
    float         InterpolationFraction;                // +0x04
    unsigned char _pad08[0x34 - 8];
    CIEngine*     Engine;                               // +0x34
};
struct CDisplayEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool v09();                                 // slot 9 (+0x24)
};

struct CBase {
    virtual void v00();
    ~CBase();                                           // 0x0099A2E0
};
struct C3DPositionInterpolator : CBase {
    C3DVector Start, End;
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);                     // 0x006AF570
};
struct C3DOrientationInterpolator : CBase {
    CRightHandedSet Start, End;
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);       // 0x005508D0
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual CRightHandedSet GetInterpolatedRHSet(float fraction) const;     // slot 71 (+0x11c)
    virtual const CRightHandedSet& GetLastRHSet() const;                    // slot 72 (+0x120)
    virtual const CRightHandedSet& GetRHSet() const;                        // slot 73 (+0x124)
    C3DVector GetInterpolatedPosition(float fraction) const;                // 0x0049D150
    unsigned char _pad04[0x0c - 4];
    C3DVector     LastPosition;                         // +0x0c
    C3DVector     Position;                             // +0x18
};
struct CTCGraphicAppearance {
    void PrepareForRenderIfNeeded(CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                                  const C3DOrientationInterpolator& orientation);   // 0x004BE490
};

struct CThing {
    unsigned char          _pad00[0x18];
    unsigned __int64       UniqueID;                    // +0x18
    unsigned char          _pad20[0x60 - 0x20];
    CTCPhysicsBase*        PhysicsTC;                   // +0x60
    CTCGraphicAppearance*  PTCGraphicAppearance;        // +0x64
    CDisplayEngine* GetDisplayEngine() const;           // 0x004C7A70
};
struct CThingPhysical : CThing {
    unsigned char _pad68[0x9c - 0x68];
    float         Scale;                                // +0x9c
    bool DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo& info);     // 0x006A64D0
};
struct CThingMarker : CThingPhysical {
    CEngineGraphic GetCurrentGraphic() const;           // 0x0083D190
    bool DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo& info);
};

bool CThingMarker::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        if (GetDisplayEngine()->v09()) {
            CEngineGraphic graphic = GetCurrentGraphic();
            info.Engine->AddPrimitive(CEnginePrimitiveStaticMesh(UniqueID, graphic.BankIndex,
                PhysicsTC->GetInterpolatedPosition(info.InterpolationFraction),
                PhysicsTC->GetInterpolatedRHSet(info.InterpolationFraction),
                graphic.RenderSizeX * Scale, true, 0xff, CRGBColour(0xff, 0xff, 0xff, 0xff), 0.0f, INTERPOLATION_DEFAULT,
                CEnginePrimitiveThemeChannelDesc(), true));
        }
        CTCGraphicAppearance* appearance = PTCGraphicAppearance;
        if (appearance) {
            C3DPositionInterpolator position(PhysicsTC->Position, PhysicsTC->LastPosition);
            CTCPhysicsBase* physics = PhysicsTC;
            C3DOrientationInterpolator orientation(physics->GetRHSet(), physics->GetLastRHSet());
            appearance->PrepareForRenderIfNeeded(info, position, orientation);
        }
        return true;
    }
    return false;
}
