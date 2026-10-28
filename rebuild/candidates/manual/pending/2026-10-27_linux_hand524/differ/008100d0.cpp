// CTCDExplosiveTrail::Create(long defIndex, const C3DVector& position, const C3DVector& direction, const CThing* target) -- create an explosive-trail thing for player 4 on the map under the position; when it carries the explosive-trail driver, aim the driver at the target, place the physics component at the position facing the flattened, normalised direction, and activate it; a thing without the driver is killed; null on any failure
// Grade: DIFFER(280v280) -- register swap only: retail keeps the position in ebx (pushed at entry) and the driver / physics pointer in a shrink-wrapped edi; ours swaps ebx and edi. The C3DVector user copy constructor took it from 300v280 to 280v280. 4 grades (probes a: driver declared first, b: no physics local -> 273v280).
// Leaf: ?Create@CTCDExplosiveTrail@@SIPAVCThing@@JABVC3DVector@@0PBV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008100D0, 280 bytes, static __fastcall (ecx = def index, edx = position, the direction
//   and the target on the stack), `ret 8`.
// Identity (proven): Ego_r.pdb (0x0080AA40) and FableWin.pdb (0x027B55B0) hold
//   ?Create@CTCDExplosiveTrail@@SIPAVCThing@@JABVC3DVector@@0PBV2@@Z (two stack arguments = `ret 8`); retail 0x008100D0
//   follows the landed CTCDExplosiveTrail::OnInitialActivate 0x008100A0, the driver it tests is the CTCDExplosiveTrail
//   bit, the member it writes is PTarget (Ego_r +0xC, CIntelligentPointer<const CThing>, temporary vtable 0x01237F1C =
//   .?AV?$CIntelligentPointer@$$CBVCThing@@@@), and its caller 0x008C71B3 then calls 0x00810970 =
//   CTCDExplosiveTrail::AddThingToIgnore(CThing&) on the same driver. The catalog's bare "Create" lacks its class.
// Callees: CWorldMap::GetMapNumberAt (slot 16, +0x40) on g_pThingManager 0x013B8A1C +0x28, CThing::Create(long, long, long,
//   bool) 0x004CA1D0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem
//   0x00A01B50 / ~CBaseIntelligentPointer 0x00A01C10, C3DVector::AccurateNormalise() 0x00A14440, CTCParamsPhysicsBase::
//   CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, the physics component's slot 3 (+0x0C), the
//   params' destructor 0x0099A300, CThing::DoActivationSequence() 0x004C9CA0, CThing::Kill(bool) 0x004C9B80.
// Layout: retail CThing UsingComponents (std::bitset<274>) +0x20 (the driver bit is bit 15 of word 8), PTCDriver +0x5C,
//   PhysicsTC +0x60 (PDB +0x6C / +0x70); CRightHandedSet {Up, Forward} (Ego_r.pdb, ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z).
// Shape: the landed CTCDExperienceOrb::Create 0x00779E90 (true pushed before the map lookup, a zero result in ebp, the
//   Kill(false) arm) with the landed CThingVillage::Create 0x0083B7F0 physics placement.
#include "engine/CTCPhysicsBase.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& AccurateNormalise();                          // 0x00A14440
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
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
struct CTCPhysicsMethods {                                   // method-only adapter over the engine CTCPhysicsBase
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2();
    virtual void ConstructFromParams(const CTCParamsPhysicsBase& params);   // slot 3 (+0x0C)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
private:
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <const CThing>: vtable 0x01237F1C
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;     // slot 16 (+0x40)
};
class CThingManager {
public:
    unsigned char _pad_0x00[0x28];
    CWorldMap* WorldMap;                                         // +0x28 (PDB +0x34)
};
extern CThingManager* g_pThingManager;                           // 0x013B8A1C

class CTCDExplosiveTrail;

class CThing {
public:
    unsigned char     _pad_0x00[0x20];
    unsigned long     UsingComponents[9];                        // +0x20 bitset<274>
    unsigned char     _pad_0x44[0x5c - 0x44];
    void*             PTCDriver;                                 // +0x5c (PDB +0x6c)
    ::CTCPhysicsBase* PhysicsTC;                                 // +0x60 (PDB +0x70)
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                 // 0x004C9CA0
    void Kill(bool immediately);                                 // 0x004C9B80
    bool GetTCDriver(CTCDExplosiveTrail** out)                   // GetTCDriver<CTCDExplosiveTrail>
    {
        if (UsingComponents[8] & 0x8000) {
            *out = (CTCDExplosiveTrail*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDExplosiveTrail {
public:
    void*                             __vftable;             // +0x00 (CTCBase)
    CThing*                           ParentThing;           // +0x04
    unsigned char                     _pad_0x08[0x4];
    CIntelligentPointer<const CThing> PTarget;               // +0x0C
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, const C3DVector& direction,
                                     const CThing* target);
};

CThing* __fastcall CTCDExplosiveTrail::Create(long defIndex, const C3DVector& position, const C3DVector& direction,
                                              const CThing* target)
{
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(position), true);
    CThing* result = 0;
    if (thing) {
        CTCDExplosiveTrail* trail;
        if (thing->GetTCDriver(&trail)) {
            trail->PTarget = target;
            C3DVector forward(direction.X, direction.Y, 0.0f);
            forward.AccurateNormalise();
            ::CTCPhysicsBase* physics = thing->PhysicsTC;
            reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(
                CTCParamsPhysicsBase(position, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), forward)));
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}
