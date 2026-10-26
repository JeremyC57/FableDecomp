// CTCCreatureFlyInCircles::InitFlightCircle(const C3DVector& centre, float radius, float height) -- store the circle centre and radius, start in fly mode 1 at speed 3, then place the creature on the circle (flagging its interface-0x3a component with whether that point is on the map) facing up the world Y axis
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DD430, 261 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x3a = word 1
//   bit 26 of the presence bitset), CTCBase::GetWorldMap @ 0x00686D30 (manifest "CViewBase::GetDisplayEngine" is
//   propagated), CWorldMap::IsPosInMap(const C3DVector&) const @ 0x004FC600, the creature's physics TC (+0x60) virtual
//   slots 31 (+0x7c, ResetPosition(const C3DVector&)) and 75 (+0x12c, SetRHSet(const CRightHandedSet&)).
// Layout = Ego_r.pdb CTCCreatureFlyInCircles (no containers before these, as the landed ChangeMode 0x007DDCC0):
//   Creature +0x0c, Centre +0x10, Speed +0x1c, CurrentRadius +0x28, Mode +0x34; CThing presence bitset +0x20, interface
//   map +0x44, PhysicsTC +0x60; the 0x3a component's flag bit 0 of +0x50 (landed CModeDragonFlying::OnDeactivate).
// Shape: the radius is loaded onto the FPU first (VC7.1 evaluates the right operand of `centre.X + radius` first);
//   the centre is copied memberwise through a reference to the member; the start point is a named C3DVector; the
//   interface lookup is HasTC + the ternary GetTC entry pick with the key in the dead `radius` argument slot; the
//   orientation is a NAMED CRightHandedSet whose ctor builds each member from the constant vector's fields (six immediate
//   stores; member copies of the vectors store temporaries first), passed through a named physics-TC local (the TC
//   load and the `lea` of the set land above the stores, the vtable load below them; a temporary argument sinks the
//   TC load under the stores, a temporary with the named TC hoists the vtable load and the push above them).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward)
        : Up(up.X, up.Y, up.Z), Forward(forward.X, forward.Y, forward.Z) { }
};
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CTCPhysicsBase { public:                         // the thing's physics TC; only slots 31 and 75 are modelled
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rhs);       // slot 75 (+0x12c)
};
struct CTCInterface3A {                         // interface type 0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_onMap : 1;                  // +0x50 bit 0
};
class CThingCreatureBase { public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos) const;   // 0x004FC600
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorldMap& GetWorldMap();   // 0x00686D30
};

struct CTCCreatureFlyInCircles : public CTCBase {
    unsigned char _pad04[0x0c - 0x04];
    CThingCreatureBase* Creature;               // +0x0c
    C3DVector Centre;                           // +0x10
    float Speed;                                // +0x1c
    float DeltaSpeed;                           // +0x20
    long  SpeedChangeTicks;                     // +0x24
    float CurrentRadius;                        // +0x28
    float DeltaRadius;                          // +0x2c
    long  RadiusChangeTicks;                    // +0x30
    long  Mode;                                 // +0x34 EFlyMode
    void InitFlightCircle(const C3DVector& centre, float radius, float height);
};

void CTCCreatureFlyInCircles::InitFlightCircle(const C3DVector& centre, float radius, float height)
{
    Centre = centre;
    CurrentRadius = radius;
    Mode = 1;
    Speed = 3.0f;
    C3DVector start(centre.X + radius, centre.Y, centre.Z + height);
    if (Creature->HasTC(0x3a)) {
        CTCInterface3A* tc = Creature->GetTC<CTCInterface3A>(0x3a);
        tc->m_onMap = GetWorldMap().IsPosInMap(start);
    }
    Creature->PhysicsTC->ResetPosition(start);
    CTCPhysicsBase* physics = Creature->PhysicsTC;
    CRightHandedSet facing(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
    physics->SetRHSet(facing);
}