// CAIStateGroup_NymphFloatAround::Update() -- reset the nymph's attack flags and target, start flying around (or, back from limbo, play the teleport action first), poll CheckIfCanAttack every frame until ready, then stop flying and, if a target lies more than 1 unit away in the plane, face it; always Terminate()
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C3D0, length 624 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0xF5 = CTCNymph, bit 21 of +0x3c, unchecked), CBaseIntelligentPointer
// ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CCreatureAction_NymphTeleportIntoLimbo
// ctor @ 0x008CA800 / ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, CTCNymph::StartFlyingAround
// @ 0x0080F5E0 / CheckIfCanAttack @ 0x0080F210 / StopFlyingAround @ 0x0080F870, GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame
// @ 0x0049D870, IsStopping @ 0x008FD010, the brain's virtual slot 2 (yield), CThing::GetPos @ 0x004C73D0, the physics
// body's virtual slot 74 (+0x128, set orientation), Terminate @ 0x008FCF40.
// Data: the reciprocal-square-root mantissa table @ 0x013CE558; 1.0f / 3.0f / 0.5f / 0.0f @ 0x0122DED8 / 0x0122DED4 /
// 0x0122F59C / 0x0122DEDC.
// Layout: local models. CTCNymph from Ego_r.pdb with the 12-byte retail spline vector (every member after FlightSpline is 4 lower):
// FlyingAround +0x40, CanAttack +0x58, ReadyToAttack +0x59, HasBeenInLimbo +0x5b, PTarget +0x68. Retail CThing physics body +0x60.
// Shape: the shared `push 0` serves both the StartFlyingAround argument and the action's wait flag; a planar direction with Z = 0
// normalised by the landed 0x007A6E20 inline Normalise, then a CRightHandedSet (Up 0,0,1; Forward) by reference.
extern unsigned long GFastOneOverSqrtTable[128];   // 0x013CE558

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    float GetLengthSquared2D() const { return X * X + Y * Y; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF(70) VF(71) VF(72) VF(73)
    virtual void SetOrientation(const CRightHandedSet& orientation);   // slot 74 (+0x128)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0xc];
    CPhysicsBody* PPhysics;                     // +0x60
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
#pragma pack(push, 1)
class CTCNymph { public:
    unsigned char _pad0[0x40];
    bool FlyingAround;                          // +0x40
    unsigned char _pad41[0x58 - 0x41];
    bool CanAttack;                             // +0x58
    bool ReadyToAttack;                         // +0x59
    bool ReadyToSummon;                         // +0x5a
    bool HasBeenInLimbo;                        // +0x5b
    unsigned char _pad5c[0x68 - 0x5c];
    CIntelligentPointerCreature PTarget;        // +0x68
    void StartFlyingAround(long frames);        // 0x0080F5E0
    void CheckIfCanAttack(long frame);          // 0x0080F210
    void StopFlyingAround(long mode);           // 0x0080F870
};
#pragma pack(pop)
class CCreatureActionBase { public:
    virtual void v00();
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _body[0x110];                 // retail sizeof 0x114
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_NymphTeleportIntoLimbo : public CCombatActionBase { public:
    CCreatureAction_NymphTeleportIntoLimbo(CThingCreatureBase* owner);    // 0x008CA800
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CAIBrain { public:
    virtual void v00();
    virtual void v01();
    virtual void Yield();                       // slot 2 (+8)
    unsigned char _pad[0x14];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
public:
    void Terminate();                           protected: // 0x008FCF40
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
};
struct CAIStateGroup_NymphFloatAround : CAIStateGroupBase {
    virtual void Update();
};

extern "C" int StandInSink(int, int);
void CAIStateGroup_NymphFloatAround::Update()
{
    CTCNymph* nymph;
    GetCreature()->GetInterface(&nymph, 0xf5);
    nymph->CanAttack = false;
    nymph->ReadyToAttack = false;
    nymph->PTarget = CIntelligentPointerCreature(0);
    if (!nymph->FlyingAround) {
        if (nymph->HasBeenInLimbo) {
            if (AIAction_PerformActionAndWait(CCreatureAction_NymphTeleportIntoLimbo(GetCreature()), false) != 2) {
                Terminate();
                return;
            }
        } else
            nymph->StartFlyingAround(0);
    }
    long frame = GetPWorld()->GetFrame();
    while (!nymph->ReadyToAttack) {
        nymph->CheckIfCanAttack(frame);
        if (IsStopping() || !nymph->FlyingAround) {
            Terminate();
            return;
        }
        PBrain->Yield();
    }
    nymph->StopFlyingAround(-1);
    if (((CThing*)nymph->PTarget.GetPItem())) {
        const CThing* creature = GetCreature();
        const CThing* target = ((CThing*)nymph->PTarget.GetPItem());
        const C3DVector& from = creature->GetPos();
        const C3DVector& to = target->GetPos();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        if (direction.GetLengthSquared2D() > 1.0f) {
            direction.Normalise();
            GetCreature()->PPhysics->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
        }
    }
    Terminate();
}