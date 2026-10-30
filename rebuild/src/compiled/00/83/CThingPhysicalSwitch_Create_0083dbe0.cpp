// CThingPhysicalSwitch::Create(long, const C3DVector&, long, const CCharString&) -- allocate a physical-switch thing through its class allocator, construct it (untriggered, game-object base with the script name), place its physics component at the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingPhysicalSwitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083DBE0, 158 bytes, static __fastcall (ecx = def index, edx = position), `ret 8`.
// Identity: ?Create@CThingPhysicalSwitch@@SIPAV1@JABVC3DVector@@JABVCCharString@@@Z (Ego_r.pdb). The object comes from
//   CThingPhysicalSwitch::Alloc() 0x0052AD50; the protected ConstructFromParams(long, const C3DVector&, long, const
//   CCharString&) is inline here: Triggered (+0xd0) = false, then CThingGameObject::ConstructFromParams 0x008388D0 with
//   flags 1, no creators and the final flag set. The manifest label is the bare "Create".
// Callees: CThingPhysicalSwitch::Alloc() 0x0052AD50, CThingGameObject::ConstructFromParams(long, const C3DVector&, long,
//   unsigned long, const CCharString&, CThingPhysical*, CThingPhysical*, bool) 0x008388D0 (landed),
//   CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, the physics
//   component's slot 3 (+0x0C, ConstructFromParams(const CTCParamsPhysicsBase&)), the params' destructor 0x0099A300,
//   CThing::DoActivationSequence() 0x004C9CA0.
// Layout: retail CThing PhysicsTC at +0x60 (PDB +0x70); CThingPhysicalSwitch::Triggered +0xd0 (PDB +0xe8, as the landed
//   CThingPhysicalSwitch rows); CRightHandedSet = Up (0,0,1) + Forward (0,1,0); the params object is vptr + Position +
//   CRightHandedSet (0x28 bytes). Local model; the physics component pointer is the engine CTCPhysicsBase dispatched
//   through a method-only adapter (the landed CThingDeadCreature::Create 0x00835C80 model).
// Shape: the CThingHolySite::Create 0x0083BBF0 body with the switch's inline ConstructFromParams (the Triggered store is
//   scheduled among the base call's argument pushes; the two creator zeros come from the zero register ebp).
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

class CThingPhysical;
class CCharString { public: void* m_pData; };

class CThing {
public:
    virtual void v000();
    unsigned char   _pad04[0x5c];
    ::CTCPhysicsBase* PhysicsTC;                         // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x34];
    bool DoActivationSequence();                         // 0x004C9CA0
};

class CThingGameObject : public CThing {
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long flags, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
    unsigned char _body98[0xd0 - 0x98];
};

class CClass;

class CThingPhysicalSwitch : public CThingGameObject {
public:
    static CClass* __fastcall Alloc();                   // 0x0052AD50
    static CThingPhysicalSwitch* __fastcall Create(long defIndex, const C3DVector& pos, long player, const CCharString& scriptName);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, const CCharString& scriptName)
    {
        Triggered = false;
        return CThingGameObject::ConstructFromParams(defIndex, pos, player, 1, scriptName, 0, 0, true);
    }
    bool          Triggered;                             // +0xd0 (PDB +0xe8)
    unsigned char _bodyd1[3];
};

CThingPhysicalSwitch* __fastcall CThingPhysicalSwitch::Create(long defIndex, const C3DVector& pos, long player, const CCharString& scriptName)
{
    CThingPhysicalSwitch* p = (CThingPhysicalSwitch*)Alloc();
    CThingPhysicalSwitch* result = 0;
    if (p && p->ConstructFromParams(defIndex, pos, player, scriptName)) {
        ::CTCPhysicsBase* physics = p->PhysicsTC;
        reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
        if (p->DoActivationSequence())
            result = p;
    }
    return result;
}