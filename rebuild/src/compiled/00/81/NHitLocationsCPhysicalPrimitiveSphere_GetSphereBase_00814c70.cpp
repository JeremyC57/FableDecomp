// Grade: RELOCATION_MATCH (wave 2 sec3 retry: the PDB inline const accessors CThing::GetTCGraphicAppearance() / GetPhysicsTC() give retail's appearance load before the argument pushes and the copy registers)
// NHitLocations::CPhysicalPrimitiveSphere::GetSphereBase(const CThing& thing) const -- the sphere primitive's base: the named dummy object on the thing's graphic appearance when the base name is set and the dummy is found, else the thing's physics position
// Leaf: ?GetSphereBase@CPhysicalPrimitiveSphere@NHitLocations@@QBE?AVC3DVector@@ABVCThing@@@Z
// Retail: Fable.exe @ 0x00814C70, 157 bytes (+ int3 padding), `ret 8`: __fastcall (this -> esi via &BaseName), the
//   hidden C3DVector result pointer then the thing reference on the stack; returns the result pointer.
// Name: Ego_r.pdb / FableWin.pdb `?GetSphereBase@CPhysicalPrimitiveSphere@NHitLocations@@QBE?AVC3DVector@@ABVCThing@@@Z`
//   (name set only). Callers: the sphere primitive's virtuals right after it (GetFirstIntersectionWithFiniteLine
//   0x00814D10, vfunc_8 0x00814D90, IsPosInside 0x00814E10, GetClosestPointToPos2D 0x00814EA0, GetBoundingSphere
//   0x00814F90), each `call 0x814c70` with this = the sphere.
// Callees: CDefString::operator==(const char*) 0x005B3440 (against "" 0x0122D70E); CDefStringTable::GetString(long)
//   0x009D49B0 on g_DefStringTable 0x013CA828 (the inline CDefString -> CCharString conversion; the temporary lives in
//   the thing reference's dead slot); NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString&, long,
//   unsigned long, C3DVector&, CRightHandedSet&, float) const 0x004C08C0; CCharString::~CCharString 0x0099EAE0.
// Layout: CPhysicalPrimitiveSphere BaseName (CDefString) +0x04, Radius +0x08 (Ego_r.pdb); retail CThing PhysicsTC +0x60,
//   PTCGraphicAppearance +0x64 (PDB +0x70/+0x74); CTCPhysicsBase Position +0x0c.
// Label: catalogued `GetDummyObjectOrigin` (_global, descriptive).
// Shape: the physics component is read before the name test (`mov ebp,[edi+60h]`, the `+0Ch` added on the fallback
//   path), so the physics component pointer is a local read at entry; the dummy lookup is its own statement (a `found`
//   flag) so the CCharString temporary dies unconditionally (no `||` conditional-destruction flag); ebx is saved only
//   around the lookup. Tried: `||` in one condition (conditional temp flag, 187v156), an inline CThing::GetDummyObject
//   wrapper (identical bytes), named CCharString + appearance locals (173v156).
class CCharString {
public:
    void* PData;
    ~CCharString();                                          // 0x0099EAE0
};

class CDefStringTable {
public:
    CCharString GetString(long tablePos) const;              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                     // 0x013CA828

class CDefString {
public:
    long TablePos;                                           // +0x0
    bool operator==(const char* text) const;                 // 0x005B3440
    operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
};

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CRightHandedSet {
public:
    C3DVector Up;                                            // +0x00
    C3DVector Forward;                                       // +0x0c
    CRightHandedSet() { }
};

class CTCPhysicsBase {
public:
    unsigned char _pad_0x00[0xc];
    C3DVector     Position;                                  // +0x0c
    const C3DVector& GetPosition() const { return Position; }
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long frame, unsigned long flags, C3DVector& pos,
                        CRightHandedSet& orientation, float scale) const;   // 0x004C08C0
};
}

class CThing {
public:
    unsigned char _pad_0x00[0x60];
    CTCPhysicsBase*                           PhysicsTC;             // +0x60 (PDB +0x70)
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;  // +0x64 (PDB +0x74)
    const C3DVector& GetPos() const { return PhysicsTC->GetPosition(); }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }   // Ego_r ?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ (inline)
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }   // Ego_r ?GetTCGraphicAppearance@CThing@@QBEPBV..@@XZ (inline)
};

namespace NHitLocations {
class CPhysicalPrimitiveSphere {
public:
    void*      __vftable;                                    // +0x00
    CDefString BaseName;                                     // +0x04
    float      Radius;                                       // +0x08
    C3DVector GetSphereBase(const CThing& thing) const;
};

C3DVector CPhysicalPrimitiveSphere::GetSphereBase(const CThing& thing) const
{
    const CTCPhysicsBase* physics = thing.GetPhysicsTC();
    C3DVector pos;
    CRightHandedSet orientation;
    bool found = false;
    if (!(BaseName == ""))
        found = thing.GetTCGraphicAppearance()->GetDummyObject(BaseName, 0, 0, pos, orientation, 1.0f);
    if (!found)
        pos = physics->GetPosition();
    return pos;
}
}