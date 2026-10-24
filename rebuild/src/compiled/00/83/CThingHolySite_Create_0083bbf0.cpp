// CThingHolySite::Create(long, const C3DVector&, long, bool) -- allocate a holy-site thing through its class allocator, construct it from the def index / position / player / player-start flag, place its physics component at the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingHolySite
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083BBF0, 147 bytes, static __fastcall (ecx = def index, edx = position), `ret 8`.
// Identity: ?Create@CThingHolySite@@SIPAV1@JABVC3DVector@@J_N@Z (Ego_r.pdb). The object comes from CThingHolySite::Alloc()
//   0x0052AC90 (`new (false) CThingHolySite`, 0xD8 bytes, vtable 0x012452DC; ?Alloc@CThingHolySite@@SIPAVCClass@@XZ),
//   then CThingHolySite::ConstructFromParams 0x0083BAB0 (landed). The manifest label is the bare "Create".
// Callees: CThingHolySite::Alloc() 0x0052AC90, CThingHolySite::ConstructFromParams(long, const C3DVector&, long, bool)
//   0x0083BAB0, CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, the
//   physics component's slot 3 (+0x0C, ConstructFromParams(const CTCParamsPhysicsBase&)), the params' destructor
//   0x0099A300, CThing::DoActivationSequence() 0x004C9CA0.
// Layout: retail CThing PhysicsTC at +0x60 (PDB +0x70); CRightHandedSet = Up (0,0,1) + Forward (0,1,0); the params
//   object is vptr + Position + CRightHandedSet (0x28 bytes). Local model (no CThingHolySite header); the physics
//   component pointer is the engine CTCPhysicsBase dispatched through a method-only adapter (the landed
//   CThingDeadCreature::Create 0x00835C80 model).
// Shape: the CThingVillage::Create 0x0083B7F0 body with the allocation through the class's Alloc() (a plain call, no
//   select): `p = Alloc(); result = 0; if (p && Construct) { physics...; if (Activate) result = p; } return result;`.
#include "engine/CTCPhysicsBase.h"

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0; Up.Y = 0; Up.Z = 1; Forward.X = 0; Forward.Y = 1; Forward.Z = 0; }
};
struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();
    __declspec(noinline) virtual ~CBaseClassNonCopyable() {}
};
struct CTCParamsPhysicsBase : CBaseClassNonCopyable {
    C3DVector       Position;
    CRightHandedSet Rhs;
    __declspec(noinline) CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& a) : Position(p), Rhs(a) {}   // 0x006A06E0
    ~CTCParamsPhysicsBase();                                                                                           // 0x0099A300
};
struct CTCPhysicsMethods {                               // method-only adapter over the engine CTCPhysicsBase
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2();
    virtual void ConstructFromParams(const CTCParamsPhysicsBase& params);   // slot 3 (+0x0C)
};

class CThing {
public:
    virtual void v000();
    unsigned char   _pad04[0x5c];
    ::CTCPhysicsBase* PhysicsTC;                         // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x34];
    bool DoActivationSequence();                         // 0x004C9CA0
};

class CClass;

class CThingHolySite : public CThing {
public:
    static CClass* __fastcall Alloc();                   // 0x0052AC90
    static CThingHolySite* __fastcall Create(long defIndex, const C3DVector& pos, long player, bool playerStart);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, bool playerStart);   // 0x0083BAB0
    unsigned char _body98[0xd8 - 0x98];
};

CThingHolySite* __fastcall CThingHolySite::Create(long defIndex, const C3DVector& pos, long player, bool playerStart)
{
    CThingHolySite* p = (CThingHolySite*)Alloc();
    CThingHolySite* result = 0;
    if (p && p->ConstructFromParams(defIndex, pos, player, playerStart)) {
        ::CTCPhysicsBase* physics = p->PhysicsTC;
        reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
        if (p->DoActivationSequence())
            result = p;
    }
    return result;
}