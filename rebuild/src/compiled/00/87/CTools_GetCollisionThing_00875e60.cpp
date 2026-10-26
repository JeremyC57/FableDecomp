// CTools::GetCollisionThing(const vector<CThing*>& things, const C3DVector& from, const C3DVector& to, C3DVector& hit_pos) -- walks the things and, for each one whose physics component has a collision mesh, intersects the segment from->to with its mesh object; the first hit's intersection position is written out and that thing returned (0 when nothing is hit)
// Leaf: GetCollisionThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875E60, length 155 bytes (padded to 160), `ret 10h` (`this` unused).
// Callees: CTCPhysicsBase slot 93 (+0x174, the has-a-mesh test), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850
// (by value through a hidden pointer), CPhysicsMeshObject::GetPointIntersectionFromTo @ 0x00A12450,
// CPhysicsMeshObject::~CPhysicsMeshObject @ 0x00A127D0.
// Layout: CThing::PhysicsTC at retail +0x60 (Ego_r.pdb 0x70, CThing sits 0x10 below the PDB there);
// CPhysicsMeshObject 0x40 bytes (vptr, Mesh, PositionMatrix, Scale, OO_Scale), CMeshIntersectionData 0x1c bytes
// (IntersectionPosition, Normal, DistanceSquared). The STLport vector walk reloads end() each step.
class C3DVector { public: float x, y, z; };
class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
class CPhysicsMeshObject { public:
    virtual void v00();
    void*         Mesh;                            // +0x04 CIVCountedPointer<CPhysicsMesh const>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    ~CPhysicsMeshObject();                         // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* out) const;   // 0x00A12450
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMesh() const;          // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};
struct CThingPtrVector {                           // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThing* const* begin() const { return _M_start; }
    CThing* const* end() const { return _M_finish; }
};
struct CTools {
    CThing* GetCollisionThing(const CThingPtrVector& things, const C3DVector& from, const C3DVector& to, C3DVector& hit_pos);
};
CThing* CTools::GetCollisionThing(const CThingPtrVector& things, const C3DVector& from, const C3DVector& to, C3DVector& hit_pos)
{
    for (CThing* const* it = things.begin(); it != things.end(); ++it)
    {
        CTCPhysicsBase* physics = (*it)->PhysicsTC;
        if (physics->GetPhysicsMesh())
        {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            CMeshIntersectionData data;
            if (mesh.GetPointIntersectionFromTo(&from, &to, &data))
            {
                hit_pos = data.IntersectionPosition;
                return *it;
            }
        }
    }
    return 0;
}