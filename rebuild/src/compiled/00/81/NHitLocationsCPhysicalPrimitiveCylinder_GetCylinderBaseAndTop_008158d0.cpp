// NHitLocations::CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const -- the hit cylinder's end points: with no base dummy object, base = physics position + up * the def's base offset and top = base + up * the def's cylinder height; otherwise base and the up axis come from the named dummy object on the graphic appearance and top = base + up * GetCylinderHeight
// Leaf: ?GetCylinderBaseAndTop@CPhysicalPrimitiveCylinder@NHitLocations@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008158D0, 460 bytes + 4 int3, thiscall `ret 0Ch`.
// Identity: Ego_r.pdb ?GetCylinderBaseAndTop@CPhysicalPrimitiveCylinder@NHitLocations@@QBEXABVCThing@@AAVC3DVector@@1@Z
//   (debug build 0x0080E140, between GetCylinderHeight and GetBoundingSphere). Callers: the landed GetBoundingSphere
//   0x00815AA0, GetFirstIntersectionWithFiniteLine 0x00815BA0, GetFirstIntersectionWithTriangle 0x00815C40,
//   IsPosInside 0x00815CE0 and 0x00816428 -- all declare this address as GetCylinderBaseAndTop. The catalog's
//   "GetDummyObjectOriginTransformed" is descriptive.
// Callees: CDefString::operator==(const char*) 0x005B3440, the physics component's virtual slot 72 (+0x120, GetRHSet,
//   as the landed CTCLook 0x00753930), CDefStringTable::GetString(long) 0x009D49B0 on g_DefStringTable 0x013CA828,
//   CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector&, CRightHandedSet&, float)
//   0x004C08C0, ~CCharString 0x0099EAE0, the def's virtual slot 1 (+4, destroy at refcount 0).
// Strings/constants: "" 0x0122D70E, the 1e-4f epsilon 0x0129BA3C.
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder {vptr, CDefString BaseName +4, Radius +8, Height +0x0C}; retail CThing
//   PhysicsTC +0x60 (Position +0x0C), PTCGraphicAppearance +0x64, def pointer +0x70 (refcount +4, base offset +0x58,
//   cylinder height +0x5C; descriptive names as the landed GetBoundingSphere).
// Shape: the physics pointer is read before the def pointer is copied (add-ref) so it is in esi from the prologue;
//   the up axis is a named copy through a named `const CRightHandedSet&` (x and y stay on the x87 stack, z is copied
//   through ecx; a direct `GetRHSet().Up` copy swaps the first multiply's operands); the dummy-object name lives in its
//   own block (constructed before the GetDummyObject arguments, homed on the dead `thing` slot, destroyed right after);
//   C3DVector has a member-wise copy constructor; both branches end in the same `top = ...` store, which VC7.1 tail-merges.
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet {                                // 24 bytes (Ego_r.pdb)
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
    CRightHandedSet() {}
};
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString {                                     // Ego_r.pdb: long TablePos
    long TablePos;
    bool operator==(const char* text) const;            // 0x005B3440
};
class CDefStringTable {
public:
    CCharString GetString(long tablePos) const;         // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

class CThingDef {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x58 - 0x08];
    float         CylinderBaseOffset;                   // +0x58 (descriptive)
    float         CylinderHeight;                       // +0x5c (descriptive)
};
struct CDefPointer {
    CThingDef* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    CThingDef* operator->() const { return Object; }
};
class CTCPhysicsBase {
public:
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;    // slot 72 (+0x120)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                             // +0x0c
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& rhs,
                        float scale) const;              // 0x004C08C0
};
}
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char   _pad68[0x70 - 0x68];
    CDefPointer     PDef;                               // +0x70
};

namespace NHitLocations {

class CPhysicalPrimitiveBase {
public:
    virtual void v00();
};

class CPhysicalPrimitiveCylinder : public CPhysicalPrimitiveBase {
public:
    CDefString BaseName;                                // +0x04
    float      Radius;                                  // +0x08
    float      Height;                                  // +0x0c
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const;
    float GetCylinderHeight(const CThing& thing) const
    {
        CDefPointer def = thing.PDef;
        float height = Height;
        if (height < g_epsilon_0129ba3c)
            height = def->CylinderHeight;
        return height;
    }
};

void CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const
{
    const CTCPhysicsBase* physics = thing.PhysicsTC;
    CDefPointer def = thing.PDef;
    if (BaseName == "") {
        const CRightHandedSet& rhs = physics->GetRHSet();
        C3DVector up = rhs.Up;
        base = up * def->CylinderBaseOffset + physics->Position;
        top = up * def->CylinderHeight + base;
    }
    else {
        CRightHandedSet rhs;
        {
            CCharString name = g_DefStringTable.GetString(BaseName.TablePos);
            thing.PTCGraphicAppearance->GetDummyObject(name, 0, 0, base, rhs, 1.0f);
        }
        C3DVector up = rhs.Up;
        top = up * GetCylinderHeight(thing) + base;
    }
}

}