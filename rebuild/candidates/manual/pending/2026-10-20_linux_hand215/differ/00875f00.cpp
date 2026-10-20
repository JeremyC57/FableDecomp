// CTools::GetMeshExtent(const CThing* thing) -- for a thing with a physics mesh: the squared lengths of the mesh's two world half-axis vectors summed (0 without a mesh)
// Leaf: GetMeshExtent
// Grade: DIFFER(159v175) -- retail reads both half-axis results through their returned pointers and loads all six components onto the FPU first (one spilled through the dead thing slot) before squaring with fld st(n); ours squares each component from the frame (by-value locals, a user copy ctor, reference temporaries + float locals and a by-value inline helper all tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875F00, length 173 bytes (`ret 4`, `this` unused; 176 with padding).
// Callees: CTCPhysicsBase slot 93 (+0x174, the has-a-mesh test), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850
// (by value through a hidden pointer), the mesh object's half-axis getters @ 0x00A11C20 (world X: matrix row times
// half the bound extent) and @ 0x00A11C90 (world Y) (C3DVector by hidden pointer; manifest labels
// "CEngineInternalPrimitiveParticleLine::GetWorldPosition" are propagated), CPhysicsMeshObject::~CPhysicsMeshObject
// @ 0x00A127D0. Constant: 0x0122DEDC = 0.0f.
// Layout: as the landed CTools::GetCollisionThing 0x00875E60 (CThing::PhysicsTC +0x60, CPhysicsMeshObject 0x40 bytes).
// Shape: the sum is a named float (homed in the dead thing slot across the mesh destructor); the two vectors are
// by-value getter results read component by component.
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    float LengthSquared() const { return x * x + y * y + z * z; }
};
struct CPhysicsMeshObject {
    virtual void v00();
    void*         Mesh;                            // +0x04 CIVCountedPointer<CPhysicsMesh const>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    ~CPhysicsMeshObject();                         // 0x00A127D0
    C3DVector GetWorldHalfAxisX() const;           // 0x00A11C20
    C3DVector GetWorldHalfAxisY() const;           // 0x00A11C90
};
struct CTCPhysicsBase {
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
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};

struct CTools {
    float GetMeshExtent(const CThing* thing);
};

float CTools::GetMeshExtent(const CThing* thing)
{
    if (thing->PhysicsTC->GetPhysicsMesh()) {
        CPhysicsMeshObject mesh = thing->PhysicsTC->GetPhysicsMeshObject();
        C3DVector axisX = mesh.GetWorldHalfAxisX();
        C3DVector axisY = mesh.GetWorldHalfAxisY();
        float extent = axisX.LengthSquared() + axisY.LengthSquared();
        return extent;
    }
    return 0.0f;
}
