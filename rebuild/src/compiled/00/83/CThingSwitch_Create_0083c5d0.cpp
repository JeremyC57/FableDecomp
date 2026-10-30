// CThingSwitch::Create(long, const C3DVector&, float, CCharString) -- allocate a switch thing, construct it from the def index / position / radius / script name (passed on by value), place its physics component at the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingSwitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083C5D0, 184 bytes, static __fastcall (ecx = def index, edx = position), `ret 8`.
// Identity: ?Create@CThingSwitch@@SIPAV1@JABVC3DVector@@MVCCharString@@@Z (Ego_r.pdb). Allocates 0xE8 bytes and runs
//   the out-of-line constructor 0x00529B00 (manifest label "CMapFilenames" is propagated), then CThingSwitch::
//   ConstructFromParams 0x0083BD80 (landed), which takes the CCharString by value too. The manifest label is "Create".
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380, CThingSwitch::CThingSwitch() 0x00529B00, CCharString copy
//   constructor 0x0099EC30 (the by-value argument built in its stack slot), CThingSwitch::ConstructFromParams(long, const
//   C3DVector&, float, CCharString) 0x0083BD80, CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const
//   CRightHandedSet&) 0x006A06E0, the physics component's slot 3 (+0x0C), the params' destructor 0x0099A300,
//   CThing::DoActivationSequence() 0x004C9CA0, CCharString::~CCharString() 0x0099EAE0 (the by-value parameter, on every
//   exit).
// Layout: retail CThing PhysicsTC at +0x60 (PDB +0x70); CRightHandedSet = Up (0,0,1) + Forward (0,1,0); the params
//   object is vptr + Position + CRightHandedSet (0x28 bytes). Local model; the physics component pointer is the engine
//   CTCPhysicsBase dispatched through a method-only adapter (the landed CThingDeadCreature::Create 0x00835C80 model).
// Shape: the CThingVillage::Create 0x0083B7F0 body (select `new`, zero result declared after it) with a by-value
//   CCharString: copied into the callee's argument slot (`push ecx ; mov ecx,esp`), destroyed once at the shared exit
//   (so the success path stores `result = p` in ebp instead of returning esi).
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

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
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

class CThingSwitch : public CThing {
public:
    CThingSwitch();                                      // 0x00529B00
    static CThingSwitch* __fastcall Create(long defIndex, const C3DVector& pos, float radius, CCharString scriptName);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, float radius, CCharString scriptName);   // 0x0083BD80
    unsigned char _body98[0xe8 - 0x98];
};

CThingSwitch* __fastcall CThingSwitch::Create(long defIndex, const C3DVector& pos, float radius, CCharString scriptName)
{
    CThingSwitch* p = new (false) CThingSwitch;
    CThingSwitch* result = 0;
    if (p && p->ConstructFromParams(defIndex, pos, radius, scriptName)) {
        ::CTCPhysicsBase* physics = p->PhysicsTC;
        reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
        if (p->DoActivationSequence())
            result = p;
    }
    return result;
}