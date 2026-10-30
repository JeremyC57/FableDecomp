// CThingMarker::Create(long defIndex, const C3DVector& pos, const CCharString& scriptName) -- allocate and construct a marker thing, construct it from the def index / position / script name (health and max health 1.0), place its physics component at the position with the identity axes, and activate it; null on any failure
// Grade: DIFFER(237v247): 12 diff lines (was 24) -- 'if (!DoActivationSequence()) return 0; result = p;' with the single trailing return result keeps retail's zero-in-edi join, re-test and the xor eax,eax exit; residue: retail returns p through its own epilogue (mov eax,esi) where ours copies p into the result register (mov edi,esi) and shares the result exit
// Leaf: ?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083D460, 247 bytes, static __fastcall (ecx = def index, edx = position, the script
//   name on the stack), `ret 4`.
// Identity (proven): Ego_r.pdb (0x0082EB20) and FableWin.pdb (0x027FF950) hold
//   ?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z, right after ?ConstructFromParams@CThingMarker@@IAE_NJAB
//   VC3DVector@@ABVCCharString@@@Z; the object's final vtable 0x01245524 has the RTTI .?AVCThingMarker@@ (the intermediate
//   0x01244F7C is CThingGameObject, the member's 0x01238C6C CIntelligentPointer<CThing>); the 0xD0-byte allocation is
//   retail sizeof(CThingMarker) (CThingGameObject 0xD0, no own members). The catalog's bare "Create" is the right name
//   without its class.
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (??2CThing@@KIPAXI_N@Z), CThingPhysical::CThingPhysical(
//   unsigned long) 0x005296B0 (type 11), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CThingGameObject::ConstructFromParams(long, const C3DVector&, long, unsigned long, const CCharString&, CThingPhysical*,
//   CThingPhysical*, bool) 0x008388D0 (landed), CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const
//   CRightHandedSet&) 0x006A06E0, the physics component's slot 3 (+0x0C), the params' destructor 0x0099A300,
//   CThing::DoActivationSequence() 0x004C9CA0.
// Layout: retail CThing PhysicsTC +0x60, CThingPhysical MaxHealth +0xB0 / Health +0xB4, CThingGameObject PThingStandingOn
//   +0xC0 (Ego_r.pdb names; the landed CThingSwitch / CThingTrackNode models).
// Shape: the landed CThingSwitch::Create 0x0083C5D0 body with CThingMarker's constructor and ConstructFromParams
//   (`CThingGameObject::ConstructFromParams(defIndex, pos, -1, 0, scriptName, 0, 0, true)`, then Health / MaxHealth = 1)
//   inline.
#define CThingGameObject CThingGameObjectPdb
#define CThingPhysical CThingPhysicalPdb
#include "engine/CThingGameObject.h"
#include "engine/CThingPhysical.h"
#undef CThingGameObject
#undef CThingPhysical
#include "engine/CTCPhysicsBase.h"

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0; Up.Y = 0; Up.Z = 1; Forward.X = 0; Forward.Y = 1; Forward.Z = 0; }
};
class CBaseClass { public: virtual ~CBaseClass(); };
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
    void* m_pStringData;
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CThing {
public:
    virtual void v000();
    unsigned char   _pad04[0x5c];
    ::CTCPhysicsBase* PhysicsTC;                         // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x34];
    bool DoActivationSequence();                         // 0x004C9CA0
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380
};
class CThingPhysical : public CThing {
public:
    CThingPhysical(unsigned long type);                  // 0x005296B0
    virtual void v000();
    unsigned char _pad98[0xb0 - 0x98];
    float MaxHealth;                                     // +0xB0
    float Health;                                        // +0xB4
    unsigned char _padb8[0xc0 - 0xb8];
};
class CThingGameObject : public CThingPhysical {         // vtable 0x01244F7C
public:
    CThingGameObject(unsigned long type) : CThingPhysical(type) { }
    virtual void v000();
    CIntelligentPointer<CThing> PThingStandingOn;        // +0xC0
    unsigned char _padc8[0xd0 - 0xc8];
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};
class CThingMarker : public CThingGameObject {           // vtable 0x01245524, 0xD0 bytes
public:
    CThingMarker() : CThingGameObject(11) { }
    virtual void v000();
    static CThingMarker* __fastcall Create(long defIndex, const C3DVector& pos, const CCharString& scriptName);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, const CCharString& scriptName)
    {
        if (!CThingGameObject::ConstructFromParams(defIndex, pos, -1, 0, scriptName, 0, 0, true))
            return false;
        Health = 1.0f;
        MaxHealth = 1.0f;
        return true;
    }
};

CThingMarker* __fastcall CThingMarker::Create(long defIndex, const C3DVector& pos, const CCharString& scriptName)
{
    CThingMarker* p = new (false) CThingMarker;
    CThingMarker* result = 0;
    if (p && p->ConstructFromParams(defIndex, pos, scriptName)) {
        ::CTCPhysicsBase* physics = p->PhysicsTC;
        reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
        if (!p->DoActivationSequence())
            return 0;
        result = p;
    }
    return result;
}
