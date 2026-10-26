// BuildingHasVolume(const CThingBuilding* building) -- does the building's physics mesh carry a BUILDING_BOUNDRY containment volume? false for no building, no physics component or no mesh
// Leaf: ?BuildingHasVolume@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FB370, 107 bytes (padded to 112), plain `ret` (`__fastcall`: building in ecx).
// PDB name ?BuildingHasVolume@@YI_NPBVCThingBuilding@@@Z (Ego_r.pdb name set). No call or absolute reference in retail.
// Callees: CTCPhysicsBase::GetPhysicsMesh (slot 93, +0x174), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850
//   (CPhysicsMeshObject by hidden pointer), CPhysicsMeshObject::HasContainmentVolume(const CCharString&) @ 0x00A128A0,
//   ~CPhysicsMeshObject @ 0x00A127D0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 (manifest
//   "_Cons_val<..CParticleMorphs::CEntry..>" is propagated). Literal 0x0124432C = "BUILDING_BOUNDRY".
// Layout: the same models as the landed CThingFilter_IsPotentialCustomer ctor 0x00799D80 (retail CThing PhysicsTC +0x60,
//   CPhysicsMeshObject 0x40 bytes: the 0x44-byte frame is the mesh plus the string temporary).
// Shape: nested ifs; the mesh object is a block-scoped local and the string an implicit conversion temporary, so the
//   found flag survives both destructors in bl (`mov bl,al` ... `mov al,bl`); the false path is a separate epilogue.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

class CPhysicsMeshObject {
public:
    virtual void v00();
    void*         Mesh;                            // +0x04 CIVCountedPointer<CPhysicsMesh const>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    virtual ~CPhysicsMeshObject();                         // 0x00A127D0
    bool HasContainmentVolume(const CCharString& name);   // 0x00A128A0
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6) FILL10(7) FILL10(8)
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual void* GetPhysicsMesh() const;          // slot 93 (+0x174)
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};

class CThingBuilding : public CThing {
};

bool __fastcall BuildingHasVolume(const CThingBuilding* building)
{
    if (building) {
        CTCPhysicsBase* physics = building->PhysicsTC;
        if (physics && physics->GetPhysicsMesh()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            return mesh.HasContainmentVolume("BUILDING_BOUNDRY");
        }
    }
    return false;
}