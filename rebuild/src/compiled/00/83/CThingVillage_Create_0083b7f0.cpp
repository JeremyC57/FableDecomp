// CThingVillage::Create(long, const C3DVector&, long) -- allocate a village thing, construct it from the def index / position / player, place its physics component at the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083B7F0, 164 bytes, static __fastcall (ecx = def index, edx = position), `ret 4`.
// Identity: ?Create@CThingVillage@@SIPAV1@JABVC3DVector@@J@Z (Ego_r.pdb). Allocates 0xE0 bytes and runs the out-of-line
//   constructor 0x005299C0 (stores the CThingVillage vtable 0x012451BC; its manifest label "CThingObject" is propagated),
//   then CThingVillage::ConstructFromParams 0x0083B340. The manifest label is the bare "Create".
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380, CThingVillage::CThingVillage() 0x005299C0,
//   CThingVillage::ConstructFromParams(long, const C3DVector&, long) 0x0083B340, CTCParamsPhysicsBase::
//   CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, the physics component's slot 3 (+0x0C,
//   ConstructFromParams(const CTCParamsPhysicsBase&)), the params' destructor 0x0099A300, CThing::DoActivationSequence()
//   0x004C9CA0.
// Layout: retail CThing PhysicsTC at +0x60 (PDB +0x70); CRightHandedSet = Up (0,0,1) + Forward (0,1,0); the params
//   object is vptr + Position + CRightHandedSet (0x28 bytes). Local model (no CThingVillage header); the physics
//   component pointer is the engine CTCPhysicsBase dispatched through a method-only adapter (the landed
//   CThingDeadCreature::Create 0x00835C80 model).
// Shape: `new (false) CThingVillage` (an out-of-line constructor keeps the `p = mem ? ctor(mem) : 0` select), then
//   a zero result declared AFTER it (declared before p the body grows by 21 bytes), `if (p && Construct) { physics->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
//   if (Activate) result = p; } return result;` -- the zero in ebp also feeds the axis zeros; the physics vtable is
//   spilled into the dead player argument slot across the params constructor.
#include "engine/CTCPhysicsBase.h"

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0; Up.Y = 0; Up.Z = 1; Forward.X = 0; Forward.Y = 1; Forward.Z = 0; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();
    __declspec(noinline) virtual ~CBaseClassNonCopyable() {}
};
class CTCParamsPhysicsBase : public CBaseClassNonCopyable { public:
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
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380
};

class CThingVillage : public CThing {
public:
    CThingVillage();                                     // 0x005299C0
    static CThingVillage* __fastcall Create(long defIndex, const C3DVector& pos, long player);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player);   // 0x0083B340
    unsigned char _body98[0xe0 - 0x98];
};

CThingVillage* __fastcall CThingVillage::Create(long defIndex, const C3DVector& pos, long player)
{
    CThingVillage* p = new (false) CThingVillage;
    CThingVillage* result = 0;
    if (p && p->ConstructFromParams(defIndex, pos, player)) {
        ::CTCPhysicsBase* physics = p->PhysicsTC;
        reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
        if (p->DoActivationSequence())
            result = p;
    }
    return result;
}