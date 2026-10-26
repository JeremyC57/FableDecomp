// CTCPhysicsStandard::OnCreate() -- start with OldRHSet = RHSet and a zero offset, build the physics mesh info when the parent's graphic data names a (static or animated) mesh that has a physics mesh, run the base OnCreate, take the interaction flags, gravity / air-resistance and pushable flags from the physics def (defaults without one), work out whether the thing starts on the ground, and when it interacts with things settle it on the surface below
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724010, 556 bytes, plain `ret`. CTCPhysicsStandard vtable 0x01265F84 slot 4.
// Callees: CTCBase::GetWorld 0x00686A80, the graphic bank's GetMesh 0x00A24280 / GetAnimatedMesh 0x00A242B0,
//   C3DMeshInfo::HasPhysicsMesh 0x00A23C40 (PhysicsMeshIndex +0x3c != -1), CTCPhysicsBase::CreatePPhysicsMeshInfo 0x006B05A0,
//   CTCPhysicsBase::OnCreate 0x006B0780 (non-virtual), GetMass 0x006AFE00, GetAirResistance 0x006AFDE0,
//   CTCPhysicsBase::GetWorldMap 0x006AF620, CWorldMap::GetGroundSizeZAt 0x0063A300,
//   CTCPhysicsStandard::GetPossibleInteractedWithList 0x008E57D0, free 0x00BFEA14; virtual slots 36 (+0x90),
//   43 SetInteractionFlags (+0xac), 52 (+0xd0, interacts with things), 97 UpdateOnSurface (+0x184),
//   98 GetPossibleCollidedWithArea (+0x188). g_FloatEpsilon 0x0129BA3C; on-ground tolerance 0x0125A15C.
// Layout: engine/CTCPhysicsStandard.h offsets (local model with real C3DVector/CRightHandedSet members, as the landed
//   CTCPhysicsControlled::Move 0x00725C40 / CTCPhysicsRigidBody::OnCreate 0x0075B2D0); flag bytes +0x84/+0x85 per the PDB
//   bitfield order. CThing +0x64 graphic appearance (graphic data +0x44: index +0, type byte +0xd; has-graphic byte +0x51).
#include <math.h>
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Clear() { X = Y = Z = 0.0f; }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class C2DBoxI { public: long Left; long Top; long Right; long Bottom; };
class CThing;
struct CThingPtrVector {                            // retail STLport vector<CThing*> (12 bytes)
    CThing** first;
    CThing** last;
    CThing** endOfStorage;
    CThingPtrVector() : first(0), last(0), endOfStorage(0) { }
    ~CThingPtrVector() { if (first) free(first); }
};
extern const float g_FloatEpsilon;                  // 0x0129BA3C
extern const float g_OnGroundTolerance;             // 0x0125A15C

class C3DMeshInfo { public:
    bool HasPhysicsMesh() const;                    // 0x00A23C40
};
struct CGraphicBank {
    C3DMeshInfo* GetMesh(unsigned long index);              // 0x00A24280
    C3DMeshInfo* GetAnimatedMesh(unsigned long index);      // 0x00A242B0
};
class CWorld { public:
    unsigned char _pad[0x3c];
    CGraphicBank* PGraphicBank;                     // +0x3c
};
class CWorldMap { public:
    float GetGroundSizeZAt(const C3DVector& pos) const;     // 0x0063A300
};
struct CGraphicDataRef {
    unsigned long Index;                            // +0x00
    unsigned char _pad04[9];
    unsigned char Type;                             // +0x0d
    bool IsValid() const { return Type != 0; }
    __forceinline bool HasPhysicsMesh(CGraphicBank* bank) const
    {
        if (Type == 5) {
            if (Index)
                return bank->GetMesh(Index)->HasPhysicsMesh();
        } else if (Type == 4) {
            if (Index)
                return bank->GetAnimatedMesh(Index)->HasPhysicsMesh();
        }
        return false;
    }
};
struct CTCGraphicAppearance {
    unsigned char   _pad00[0x44];
    CGraphicDataRef GraphicData;                    // +0x44
};
class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;       // +0x64
};
class CPhysicsDef { public:
    unsigned char _pad00[0x38];
    float         Mass;                             // +0x38
    long          InteractionFlags;                 // +0x3c
    bool          PushableByHero;                   // +0x40
};

#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
struct CTCPhysicsStandard {
    V4(0)                                           // 0-3
    virtual void OnCreate();                        // slot 4
    virtual void v5(); virtual void v6(); virtual void v7();
    V4(2) V4(3) V4(4) V4(5) V4(6) V4(7)             // 8-31
    V4(8)                                           // 32-35
    virtual void SetUnknown36(bool value);          // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetInteractionFlags(long flags);   // slot 43 (+0xac)
    V4(11) virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual bool InteractsWithThingsCheck();        // slot 52 (+0xd0)
    virtual void v53(); virtual void v54(); virtual void v55();
    V4(14) V4(15) V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22) V4(23)   // 56-95
    virtual void v96();
    virtual void UpdateOnSurface(CThingPtrVector& things, C3DVector& position);   // slot 97 (+0x184)
    virtual C2DBoxI GetPossibleCollidedWithArea();  // slot 98 (+0x188)

    CThing*         ParentThing;                    // +0x04
    long            _pad08;
    C3DVector       Position;                       // +0x0c
    unsigned char   _pad18[0x38 - 0x18];
    const CPhysicsDef* PhysicsDef;                  // +0x38
    unsigned char   _pad3c[0x44 - 0x3c];
    C3DVector       OffsetVector3D;                 // +0x44
    CRightHandedSet RHSet;                          // +0x50
    CRightHandedSet OldRHSet;                       // +0x68
    long            RHSetLastFrameSet;              // +0x80
    unsigned char   AlwaysFacingMovementDirection : 1;   // +0x84
    unsigned char   AngledWithGround : 1;
    unsigned char   FeelsGravity : 1;
    unsigned char   FeelsAirResistance : 1;
    unsigned char   OnGround : 1;
    unsigned char   OnMesh : 1;
    unsigned char   InteractsWithThings : 1;
    unsigned char   CollidesWithThings : 1;
    unsigned char   CollidesWithThingsOneWay : 1;   // +0x85
    unsigned char   CollidesWithLandscape : 1;
    unsigned char   PhysicsEnabled : 1;
    unsigned char   Immoveable : 1;
    unsigned char   NavigationLayers : 1;
    unsigned char   PushableByHero : 1;

    const CWorld& GetWorld() const;                 // 0x00686A80 (CTCBase)
    CWorldMap* GetWorldMap();                       // 0x006AF620 (CTCPhysicsBase)
    void CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic);   // 0x006B05A0
    void BaseOnCreate();                            // 0x006B0780 CTCPhysicsBase::OnCreate
    float GetMass();                                // 0x006AFE00
    float GetAirResistance();                       // 0x006AFDE0
    void GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector& things, float speed);   // 0x008E57D0
};

void CTCPhysicsStandard::OnCreate()
{
    OldRHSet = RHSet;
    OffsetVector3D.Clear();
    CTCGraphicAppearance* graphic = ParentThing->PGraphicAppearance;
    if (graphic && graphic->GraphicData.IsValid()) {
        const CGraphicDataRef& data = graphic->GraphicData;
        const CWorld& world = GetWorld();
        CGraphicBank* bank = world.PGraphicBank;
        unsigned char type = data.Type;
        if (type == 5) {
            if (data.Index && bank->GetMesh(data.Index)->HasPhysicsMesh())
                CreatePPhysicsMeshInfo(data);
        } else if (type == 4) {
            if (data.Index && bank->GetAnimatedMesh(data.Index)->HasPhysicsMesh())
                CreatePPhysicsMeshInfo(data);
        }
    }
    BaseOnCreate();
    if (PhysicsDef) {
        SetInteractionFlags(PhysicsDef->InteractionFlags);
        FeelsGravity = GetMass() > g_FloatEpsilon;
        FeelsAirResistance = GetAirResistance() > g_FloatEpsilon;
        PushableByHero = PhysicsDef->PushableByHero;
    } else {
        SetInteractionFlags(0);
        FeelsGravity = false;
        FeelsAirResistance = false;
        PushableByHero = true;
    }
    C3DVector position(Position);
    OnGround = fabs(position.Z - GetWorldMap()->GetGroundSizeZAt(position)) < g_OnGroundTolerance;
    if (InteractsWithThingsCheck()) {
        CThingPtrVector things;
        GetPossibleInteractedWithList(GetPossibleCollidedWithArea(), things, 1.0f);
        C3DVector here(Position);
        UpdateOnSurface(things, here);
        SetUnknown36(false);
    }
}