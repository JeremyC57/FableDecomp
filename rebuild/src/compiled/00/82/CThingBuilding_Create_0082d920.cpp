// CThingBuilding::Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName) -- static factory: allocate a building, construct it from the parameters, place its physics at the position with the default facing and run the activation sequence; null on any failure
// Leaf: ?Create@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D920, 153 bytes (+7 int3), static __fastcall `ret 8` (defIndex in ecx, the
//   position in edx, a3 and the script name on the stack), 0x40-byte frame (the CRightHandedSet and the physics params).
// Identity: Ego_r.pdb name set ?Create@CThingBuilding@@SIPAV1@JABVC3DVector@@JABVCCharString@@@Z; the Alloc it calls
//   (0x0052AC10, landed CThingBuilding::Alloc) names the class. The catalog label "Create" is unqualified.
// Callees: CThingBuilding::Alloc() 0x0052AC10 (static __fastcall), CThingGameObject::ConstructFromParams 0x008388D0
//   (landed, eight arguments -- CThingBuilding::ConstructFromParams(long, const C3DVector&, long, const CCharString&)
//   is inline: a4 = 1, both creators 0, final flag true), CTCParamsPhysicsBase(const C3DVector&, const
//   CRightHandedSet&) 0x006A06E0, the physics TC's virtual slot 3 ConstructFromParams (+0x0c), ~CBaseClassNonCopyable
//   0x0099A300 (the params temporary), CThing::DoActivationSequence 0x004C9CA0.
// Layout: retail CThing PhysicsTC +0x60 (as the landed CTCDExperienceOrb::Construct 0x00779D20).
// Shape: `T* b = Alloc(); T* result = 0; if (b && b->ConstructFromParams(...)) { b->GetPhysicsTC()->
//   ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet())); if (b->DoActivationSequence()) result = b; }
//   return result;` (as the landed CThingDeadCreature::Create 0x00835C80) -- the zero result lives in ebp and serves
//   every zero push/store, so the TC's vtable spills to the dead argument slot; the default facing (Up 0,0,1 / Forward
//   0,1,0) is built inline in the frame, the TC read before the params temporary.
// Levers: the params constructor is the header-inline body VC7.1 did not inline, here a same-TU noinline stand-in (an
//   external declaration schedules the facing stores before the argument pushes); `result = 0` is declared after the
//   Alloc call (else the `xor ebp,ebp` moves above it).
class C3DVector {
public:
    float X, Y, Z;
};
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
};
class CCharString {
public:
    void* PStringData;
};
class CBaseClassNonCopyable {
public:
    virtual void v00();
    CBaseClassNonCopyable();                            // 0x0099A2F0
    ~CBaseClassNonCopyable();                           // 0x0099A300
};
class CTCParamsBase : public CBaseClassNonCopyable { };
class CTCParamsPhysicsBase : public CTCParamsBase {     // vtable 0x0125C818
public:
    C3DVector       Position;                           // +0x04
    CRightHandedSet Rhs;                                // +0x10
    __declspec(noinline) CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs)   // 0x006A06E0
        : Position(position), Rhs(rhs) { }
    virtual void v00();
};
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(const CTCParamsBase& params);   // slot 3 (+0x0c)
};

class CClass {
public:
    virtual void v00();
};
class CThingPhysical;
class CThing : public CClass {
public:
    unsigned char   _pad04[0x5c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    bool DoActivationSequence();                        // 0x004C9CA0
};
class CThingGameObject : public CThing {
public:
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};
class CThingBuilding : public CThingGameObject {
public:
    static CClass* __fastcall Alloc();                  // 0x0052AC10
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName)
    {
        return CThingGameObject::ConstructFromParams(defIndex, &position, a3, 1, scriptName, 0, 0, true);
    }
    static CThingBuilding* __fastcall Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName);
};

CThingBuilding* __fastcall CThingBuilding::Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName)
{
    CThingBuilding* building = (CThingBuilding*)Alloc();
    CThingBuilding* result = 0;
    if (building && building->ConstructFromParams(defIndex, position, a3, scriptName)) {
        building->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (building->DoActivationSequence())
            result = building;
    }
    return result;
}