// CThingFindHeightZBelowAt::operator()(CThing* thing) -- intersects the From..To segment with the thing's physics mesh and raises Height to the hit Z (+0.007) when that lies above Height and below From.Z; always continues the search
// Leaf: ??RCThingFindHeightZBelowAt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00668AB0, length 144 bytes.
// Callees: CTCPhysicsBase virtual slot 93 (+0x174, the physics mesh parameters), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850
// (0x40-byte CPhysicsMeshObject by hidden pointer), CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector&, const C3DVector&,
// CMeshIntersectionData*) @ 0x00A12450, ~CPhysicsMeshObject @ 0x00A127D0.
// Globals: float 0.007f @ 0x0125A164.
// Layout: PDB CThingFindHeightZBelowAt { C3DVector* From (+0); C3DVector* To (+4); float Height (+8); }; CMeshIntersectionData
// (28 bytes) { C3DVector IntersectionPosition; C3DVector Normal; float DistanceSquared; }; retail CThing PhysicsTC +0x60.
// Shape: the hit Z is a float local kept on the FPU across both compares (`z > Height` = `fcom ; test ah,41h ; jne`,
// `z < From->Z` = `fcom ; test ah,5 ; jp`); the store arm returns true from inside the mesh scope (its own dtor call);
// every other path returns true too.
class C3DVector { public:
    float X, Y, Z;
};
class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;             // +0x00
    C3DVector Normal;                           // +0x0c
    float     DistanceSquared;                  // +0x18
};
class CPhysicsMeshObject { public:
    void*         __vftable;
    void*         m_pMesh;                      // +0x04 counted mesh
    unsigned char _pad08[0x38];
    ~CPhysicsMeshObject();                      // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* out);   // 0x00A12450
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMeshParameters();   // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();  // 0x006B0850
};
class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CThingFindHeightZBelowAt {
    C3DVector* From;                            // +0x00
    C3DVector* To;                              // +0x04
    float      Height;                          // +0x08
    bool operator()(CThing* thing);
};

bool CThingFindHeightZBelowAt::operator()(CThing* thing)
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && physics->GetPhysicsMeshParameters()) {
        CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
        CMeshIntersectionData data;
        if (mesh.GetPointIntersectionFromTo(*From, *To, &data)) {
            float z = data.IntersectionPosition.Z + 0.007f;
            if (z > Height && z < From->Z) {
                Height = z;
                return true;
            }
        }
    }
    return true;
}