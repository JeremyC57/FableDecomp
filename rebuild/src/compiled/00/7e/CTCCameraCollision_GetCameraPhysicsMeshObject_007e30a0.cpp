// CTCCameraCollision::GetCameraPhysicsMeshObject() const -- build the camera-collision physics mesh object: the def's physics mesh placed at the thing's physics position and orientation, scaled by the thing's physics radius / 100
// Leaf: GetCameraPhysicsMeshObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E30A0, length 206 bytes, `ret 4` (hidden result)
// (?GetCameraPhysicsMeshObject@CTCCameraCollision@@QBE?AVCPhysicsMeshObject@@XZ).
// Callees: the thing physics' GetPhysicsMeshInfo (virtual slot 93, +0x174) and orientation getter (slot 72, +0x120; name
// descriptive), CTCBase::GetWorld() const @ 0x00686A80, the main mesh bank's (world +0x3C) GetPhysicsMeshInfo(long) @ 0x00A24250,
// CPhysicsMeshInfo::GetMesh() const @ 0x00A262B0 (CIVCountedPointer by hidden pointer), CPhysicsMeshObject::CPhysicsMeshObject
// (mesh, position, orientation, scale) @ 0x00A12CC0, the mesh's virtual DeleteThis (slot 1), ~CPhysicsMeshObject @ 0x00A127D0.
// Globals: 0.01f 0x01230A5C. The result's vtable 0x0125D084 = CPhysicsMeshObject.
// Layout = Ego_r.pdb CTCCameraCollision: CameraCollisionDef +0x0C (the def's mesh index at +0x28); CPhysicsMeshObject (landed
// 0x00A12CC0 model: Mesh +4, PositionMatrix +8, Scale +0x38, OO_Scale +0x3C).
// Shape: the def and the mesh bank are named locals (the def held in ebp across GetWorld); the object is a named local copied into the result by the implicit copy constructor (no NRVO): vtable, add-ref'd mesh,
// the matrix as a block, the two scales; the mesh temporary of the constructor call is released by the counted pointer's dtor through
// the pointee's DecRef/GetRefCount release (the long `add eax,4 ; dec ; test ; mov` form).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CPhysicsMesh {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) DeleteThis(); }
};
template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; }
    ~CIVCountedPointer()
    {
        if (m_pObject)
            m_pObject->Release();
    }
};
struct CBaseClass {                                // vtable 0x01231710
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
struct CPhysicsMeshObject : CBaseClass {           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A12CC0
    ~CPhysicsMeshObject();                         // 0x00A127D0
};
struct CPhysicsMeshInfo {
    CIVCountedPointer<CPhysicsMesh> GetMesh() const;   // 0x00A262B0
};
struct CMeshBank {
    CPhysicsMeshInfo* GetPhysicsMeshInfo(long index);  // 0x00A24250
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshBank* PMainMeshBank;                      // +0x3c
};
struct CThingPhysicsMeshInfo {
    unsigned char _pad00[4];
    float Radius;                                  // +0x04
};
struct CThingPhysics {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47)
    VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63)
    VF(64) VF(65) VF(66) VF(67) VF(68) VF(69) VF(70) VF(71)
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
    VF(73) VF(74) VF(75) VF(76) VF(77) VF(78) VF(79) VF(80) VF(81) VF(82) VF(83) VF(84) VF(85) VF(86) VF(87) VF(88)
    VF(89) VF(90) VF(91) VF(92)
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();  // slot 93 (+0x174)
    unsigned char _pad04[0x0c - 4];
    C3DVector Position;                            // +0x0c
};
struct CThing {
    unsigned char _pad00[0x60];
    CThingPhysics* PPhysics;                       // +0x60
};
struct CCameraCollisionDef {
    unsigned char _pad00[0x28];
    long PhysicsMesh;                              // +0x28
};
class CTCCameraCollision {
public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    const CCameraCollisionDef* CameraCollisionDef; // +0x0c
    CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)
    CPhysicsMeshObject GetCameraPhysicsMeshObject() const;
};

CPhysicsMeshObject CTCCameraCollision::GetCameraPhysicsMeshObject() const
{
    CThingPhysics* physics = ParentThing->PPhysics;
    const CThingPhysicsMeshInfo* thingMesh = physics->GetPhysicsMeshInfo();
    const CCameraCollisionDef* def = CameraCollisionDef;
    CMeshBank* bank = GetWorld()->PMainMeshBank;
    CPhysicsMeshInfo* info = bank->GetPhysicsMeshInfo(def->PhysicsMesh);
    CPhysicsMeshObject object(info->GetMesh(), physics->Position, physics->GetOrientation(), thingMesh->Radius * 0.01f);
    return object;
}