// CThingCreatureBase::IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const -- true when a living thing lies within half of viewAngle (in turns) of this creature's XY facing, or of its head-look direction when useLookAngle and it has a CTCLook
// Leaf: ?IsThingInViewAngle@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00664E50, 656 bytes (639 without padding), thiscall `ret 0Ch`, bool in al. Ego_r.pdb
//   name set: ?IsThingInViewAngle@CThingCreatureBase@@QBE_NABVCThing@@M_N@Z; its caller is IsThingInFOV 0x00665190
//   (`call` at 0x00665247). Catalog label "CVertexBufferWin32_CalculateViewingAngle" is propagated and wrong.
// Callees: CThing::GetPos() const 0x004C73D0 (twice: this, then thing), CTCPhysicsBase::GetFacing() vtable slot 70
//   (+0x118, C3DVector through the hidden pointer), CInterfaceMap::LowerBound const 0x004365B0 (CTCLook, interface 0x43),
//   CTCLook::GetLookAngleRelToBodyXY() const 0x00751A80 (catalog "CMap::DrawGetSeaLevel"; fld [+34] ; fadd [+2C]),
//   _CIfmod 0x00BFEB28 through the fmod intrinsic. Constants: 1/(2*pi) float 0x01237F08, 1.0 double 0x01237F00,
//   0.0f / 1.0f / 0.5f at 0x0122DEDC / 0x0122DED8 / 0x0122F59C.
// Shape: angles are turns (atan2 * 1/2pi). The fully inlined NormaliseAngle (`if (a < 0 || a >= 1) { a = fmod(a, 1);
//   if (a < 0) a += 1; }`) runs five times: once per GetAngleXY, once on the look sum, and twice more inside
//   AngleDifference (left operand spilled, right kept in st0, then `fsubr` / `fabs` / wrap past 0.5). The 0x20-byte
//   frame needs the offset as a full C3DVector (thing.GetPos() - GetPos(), right operand first): its dead Z
//   store is dropped but the slot stays, which puts the facing vector at +0x14. The CTCLook lookup happens before the
//   useLookAngle test (no null check); `if (...) return true; return false;` merges with the dead-thing early return at
//   the end. Helpers must be __forceinline (plain inline leaves NormaliseAngle / GetAngleXY out of line).
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl fmod(double, double);
extern "C" double __cdecl atan2(double, double);
#pragma intrinsic(fabs, fmod, atan2)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

__forceinline float NormaliseAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
__forceinline float GetAngleXY(float x, float y) { return NormaliseAngle((float)atan2(x, y) * 0.15915494f); }
__forceinline float AngleDifference(float a, float b)
{
    float difference = (float)fabs(NormaliseAngle(a) - NormaliseAngle(b));
    if (difference > 0.5f)
        difference = 1.0f - difference;
    return difference;
}

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;               // slot 70 (+0x118)
};
class CTCLook {
public:
    float GetLookAngleRelToBodyXY() const;             // 0x00751A80
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                             // +0x91 bit 0: dead
    const C3DVector& GetPos() const;              // 0x004C73D0
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const;
};

bool CThingCreatureBase::IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const
{
    if (thing.IsDead())
        return false;
    C3DVector toThing = thing.GetPos() - GetPos();
    C3DVector facing = PhysicsTC->GetFacing();
    float facingAngle = GetAngleXY(facing.X, facing.Y);
    float lookAngle = facingAngle;
    if (HasTC(0x43)) {
        const CTCLook* look = GetTC<CTCLook>(0x43);
        if (useLookAngle)
            lookAngle = NormaliseAngle(look->GetLookAngleRelToBodyXY() + facingAngle);
    }
    float thingAngle = GetAngleXY(toThing.X, toThing.Y);
    if (AngleDifference(thingAngle, lookAngle) <= viewAngle * 0.5f)
        return true;
    return false;
}