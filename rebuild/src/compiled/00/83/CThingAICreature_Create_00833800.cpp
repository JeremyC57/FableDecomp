// CThingAICreature::Create(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init) -- static factory: allocate and construct an AI creature, remember its initial position, construct it from the parameters, place its physics with the default facing and run the activation sequence; null on any failure
// Leaf: ?Create@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833800, 197 bytes (+11 int3), static __fastcall `ret 8` (defIndex in ecx, the
//   position in edx, the creation type and the init block on the stack), 0x40-byte frame.
// Identity: Ego_r.pdb name set ?Create@CThingAICreature@@SIPAV1@JABVC3DVector@@JABVCThingAICreatureInit@@@Z; the
//   constructor and ConstructFromParams it calls are landed CThingAICreature rows. The catalog label "Create" is
//   unqualified.
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (static __fastcall; 0x1D8 bytes, flag false),
//   CThingAICreature::CThingAICreature() 0x00831F80, CThingAICreature::ConstructFromParams(long, const C3DVector&, long,
//   const CThingAICreatureInit&) 0x00830360, CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0,
//   the physics TC's virtual slot 3 ConstructFromParams (+0x0c), ~CBaseClassNonCopyable 0x0099A300 (the params
//   temporary), CThing::DoActivationSequence 0x004C9CA0.
// Layout: retail CThing PhysicsTC +0x60; CThingCreatureBase InitialPos +0xe8 (Ego_r.pdb +0x104 less the retail 0x1c,
//   as PDef +0xe0 in the landed ConstructFromParams); sizeof(CThingAICreature) 0x1d8.
// Shape: as the sibling CThingBuilding::Create 0x0082D920 -- `T* c = new (false) T; T* result = 0; if (c) { c->InitialPos
//   = position; if (c->ConstructFromParams(...)) { c->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position,
//   CRightHandedSet())); if (c->DoActivationSequence()) result = c; } } return result;` -- `new (false)` through the
//   protected static operator new (null-tested before the constructor, no placement delete so no EH frame); the zero
//   result in ebp; the params constructor a same-TU noinline stand-in (the header-inline body VC7.1 did not inline).
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
class CThingAICreatureInit;
class CThing : public CClass {
public:
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380 (null when the manager refuses)
    unsigned char   _pad04[0x5c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    bool DoActivationSequence();                        // 0x004C9CA0
};
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad64[0xe8 - 0x64];
    C3DVector     InitialPos;                           // +0xe8
};
class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char _padf4[0x1d8 - 0xf4];
    CThingAICreature();                                 // 0x00831F80
    bool ConstructFromParams(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init);   // 0x00830360
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init);
};

CThingAICreature* __fastcall CThingAICreature::Create(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init)
{
    CThingAICreature* creature = new (false) CThingAICreature;
    CThingAICreature* result = 0;
    if (creature) {
        creature->InitialPos = position;
        if (creature->ConstructFromParams(defIndex, position, creationType, init)) {
            creature->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
            if (creature->DoActivationSequence())
                result = creature;
        }
    }
    return result;
}