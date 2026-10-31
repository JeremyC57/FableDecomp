// CCreatureAction_IceTrollIcicleAttack::PerformAction() -- fire the ice troll's icicle trail: start from the creature's
//   position and facing (or the WEAPON_FOCUS_01 / _02 dummy of the attack's handedness, 0 or 1), aim at the living target
//   (handedness 1 leads it by one second of its per-frame velocity), flatten the aim, clamp it to within 60 degrees of
//   the facing (else take the facing turned 1/6 rad), and create the CSnowTrollDef::IcicleAttackIndex explosive trail
//   there, telling its driver to ignore the troll
// Grade: RELOCATION_MATCH (1090v1090) (wave 2 sec4b: first reconstruction from the disassembly; was the empty skeleton)
// Graded: g1 draft (member-wise operators, out of line) 1050v1090/194; g2 ctor-form operators + explicit copy ctor 126;
//   g3 EPSILON*EPSILON from the global (not a folded literal) 123; g4 the target position copied into a local 111; g5 the
//   snow-troll CDefPointer in its own block (shares the 0x10 slot) 73; g6 shared engine/CDefPointer.h + the same-TU
//   CDefClassBase::GetSubDef<T> template body (the def stays in ebx across Create and its release loses the null test)
//   54; g7 the dummy-name CCharString as a block-scoped local (constructed before the GetPItem) 24; g8 cross operand order
//   22; g9 the dot's grouping X + (Y + Z) 12; g10 its operand order RELOCATION_MATCH.
// Leaf: ?PerformAction@CCreatureAction_IceTrollIcicleAttack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C6DB0, 1090 bytes, virtual PerformAction, plain `ret`, frame 0x74.
// Layout: retail CCombatActionBase 0x114 (PCreature +0x08, PAnimEntry CCountedPointer<CAnimationEntry> +0x74, PTarget
//   +0xA8); CCreatureAction_IceTrollIcicleAttack adds nothing (Ego_r 0x12C less the 0x18 base shrink); CThing
//   UsingComponents +0x20, PTCDriver +0x5C, PhysicsTC +0x60, PTCGraphicAppearance +0x64, PBaseDef +0x70, dead bit +0x91;
//   CTCPhysicsBase Position +0x0C / OldPosition +0x18; CSnowTrollDef::IcicleAttackIndex +0x28 (Ego_r).
// Callees (PDB spellings): CAnimationEntry::GetComponent<CAnimComponentCombatHandedness> 0x008C7B30, CThing::GetPos
//   0x004C73D0, CTCPhysicsBase::GetFacingVector slot 70, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0,
//   C3DVector::AccurateNormalise 0x00A14440 / Rotate(const C3DVector&, float) 0x00A13C60, CDefClassBase::GetSubDef
//   <CSnowTrollDef> 0x008C7A70, CTCDExplosiveTrail::Create 0x008100D0 (Ego_r ?Create@CTCDExplosiveTrail@@SIPAVCThing@@JABVC3DVector@@0PBV2@@Z),
//   CTCDExplosiveTrail::AddThingToIgnore(CThing&) 0x00810970 (Ego_r 0x0080B430; landed as the descriptive
//   CUnprovenDriverOwner_00810970::AppendTrackedThing -> NAME_FIXES.tsv), __ftol2.
// Globals: g_framesPerSecond 0x01375550 (long), g_epsilon_0129ba3c 0x0129BA3C (1e-4f; no PDB name).
class CBaseObject { public: virtual void v00(); };
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetSquaredMagnitude2D() const { return X * X + Y * Y; }
    C3DVector& AccurateNormalise();                       // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);      // 0x00A13C60
};
inline float operator*(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
class CRightHandedSet { public: C3DVector Up, Forward; };
extern const float g_epsilon_0129ba3c;                    // 0x0129BA3C (1e-4f; no PDB name)
extern long g_framesPerSecond;                            // 0x01375550

class CTCPhysicsBase { public:
    virtual void v00();
    unsigned char _pad04[0x8];
    C3DVector Position;                                   // +0x0C
    C3DVector OldPosition;                                // +0x18
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;            // slot 70 (+0x118)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& rhs,
                        float scale) const;               // 0x004C08C0
};
}
class CThingBaseDef : public CDefClassBase { };
class CSnowTrollDef : public CSubDefClassBase { public:            // RTTI .?AVCSnowTrollDef@@ : CSubDefClassBase
    static CCharString __fastcall StatGetTypeName() { return CCharString("CSnowTrollDef", -1); }
    long IcicleAttackIndex;                               // +0x28
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CSnowTrollDef> 0x008C7A70
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class CTCDExplosiveTrail;
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20 bitset<274>
    unsigned char   _pad44[0x18];
    void*           PTCDriver;                           // +0x5C
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char   _pad68[0x8];
    CDefPointer<const CThingBaseDef> PBaseDef;           // +0x70
    unsigned char   _pad74[0x1d];
    unsigned char   Flags91;                             // +0x91
    const C3DVector& GetPos() const;                     // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    NGraphicAppearance::CTCGraphicAppearance* GetGraphicAppearanceTC() const { return PTCGraphicAppearance; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> bool GetTCDriver(T** out)          // ??$GetTCDriver@V..@@@CThing@@QAE_NPAPAV..@@@Z
    {
        if (UsingComponents[8] & 0x8000) {               // the driver bit (TC 0x10F)
            *out = (T*)PTCDriver;
            return true;
        }
        return false;
    }
};
class CTCDExplosiveTrail { public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, const C3DVector& direction,
                                     const CThing* target);   // 0x008100D0
    void AddThingToIgnore(CThing& thing);                 // 0x00810970
};
class CThingCreatureBase : public CThing { };
class CAnimComponentCombatHandedness { public:
    unsigned char _pad00[0x8];
    long Data;                                            // +0x08
};
class CAnimationEntry { public:
    template <class T> void GetComponent(T** out);
};
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline T* operator->() const { return (T*)GetPItem(); }
    __forceinline operator T*() const { return (T*)GetPItem(); }
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x64];
    CCountedPointer<CAnimationEntry> PAnimEntry;         // +0x74
    unsigned char _pad7c[0x2c];
};
class CCombatActionBase : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PTarget;                 // +0xA8
    unsigned char _padb0[0x64];
    virtual ~CCombatActionBase();
};
class CCreatureAction_IceTrollIcicleAttack : public CCombatActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_IceTrollIcicleAttack::PerformAction()
{
    long attackType = 2;
    if (!PAnimEntry.IsNull()) {
        CAnimComponentCombatHandedness* handedness;
        PAnimEntry->GetComponent(&handedness);
        attackType = handedness->Data;
    }
    C3DVector pos = PCreature->GetPos();
    C3DVector forward = PCreature->GetPhysicsTC()->GetFacingVector();
    C3DVector direction = forward;
    if (attackType == 0) {
        C3DVector dummyPos;
        CRightHandedSet rhs;
        bool found;
        {
            CCharString name("WEAPON_FOCUS_01", -1);
            found = PCreature->GetGraphicAppearanceTC()->GetDummyObject(name, 0, 0, dummyPos, rhs, 1.0f);
        }
        if (found)
            pos = dummyPos;
        if (IS_THING_ALIVE(PTarget))
            { C3DVector targetPos = PTarget->GetPos(); direction = targetPos - pos; }
    } else if (attackType == 1) {
        C3DVector dummyPos;
        CRightHandedSet rhs;
        bool found;
        {
            CCharString name("WEAPON_FOCUS_02", -1);
            found = PCreature->GetGraphicAppearanceTC()->GetDummyObject(name, 0, 0, dummyPos, rhs, 1.0f);
        }
        if (found)
            pos = dummyPos;
        if (IS_THING_ALIVE(PTarget)) {
            C3DVector velocity = PTarget->GetPhysicsTC()->Position - PTarget->GetPhysicsTC()->OldPosition;
            direction = velocity * (float)(long)(float)g_framesPerSecond + PTarget->GetPos() - pos;
        }
    }
    direction.Z = 0.0f;
    if (direction.GetSquaredMagnitude2D() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        direction.AccurateNormalise();
        if (forward.X * direction.X + (direction.Y * forward.Y + direction.Z * forward.Z) < cos(1.0471976f)) {
            direction = forward;
            float cross = forward.X * direction.Y - direction.X * forward.Y;
            direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), (cross > 0.0f ? -1.0f : 1.0f) * 0.16666667f);
        }
    } else {
        direction = forward;
    }
    {
    CDefPointer<const CSnowTrollDef> snowTroll(NULL_DEF);
    PCreature->GetPBaseDef()->GetSubDef(&snowTroll);
    CThing* trail = CTCDExplosiveTrail::Create(snowTroll->IcicleAttackIndex, pos, direction, PTarget);
    CTCDExplosiveTrail* driver;
    if (trail && !trail->IsDead() && trail->GetTCDriver(&driver))
        driver->AddThingToIgnore(*PCreature);
    }
}