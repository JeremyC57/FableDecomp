// CCombatAction_ControlledStrafeJump::Initialise() -- levels the creature onto its original facing, measures the signed turn to the required facing, picks the MOVEMENT_STRAFE_JUMP_[BLOCK_]BACK/LEFT/RIGHT/FORWARD animation, optionally turns the creature by the remaining offset, runs the base Initialise and adds the roll creature mode
// Leaf: ?Initialise@CCombatAction_ControlledStrafeJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085BD70, 830 bytes, plain `ret`; virtual Initialise of CCombatAction_ControlledStrafeJump.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), the physics component's virtual slots 32 (+0x80,
//   ResetRHSet(const CRightHandedSet&), CTCPhysicsStandard 0x00723980), 66 (+0x108, SetFacingAngle), 70 (+0x118, the facing
//   returned by value) and 72 (+0x120, the current right-handed set), _CIacos 0x00BFEBA2, CCharString(const char*, long)
//   0x0099EBF0 / operator+=(const char*) 0x0099F100 / ~CCharString 0x0099EAE0, CVectorMap::LowerBound const 0x004365B0 and
//   non-const 0x0040F020 (inline component lookups 0x30 CTCCombat and 0x31 CTCCreatureModeManager), the combat component's
//   blocking getter 0x007331E0 (a folded `mov al,[ecx+63h]` body: CTCCombat::Blocking is PDB +0x6f, -12 for three 12-byte
//   retail vectors), CCombatActionBase::SetAnimationName 0x008574D0 / GetCombatManager 0x00856B10 / Initialise 0x00857520,
//   CThing::GetPlayer 0x004C7A10, CTCCreatureModeManager::AddMode 0x0073AE10, _CIfmod 0x00BFEB28.
// Globals: -1.0f 0x0122DEE0, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 1/(2pi) 0x01237F08, 360.0f 0x01238020, -135.0f 0x01276948,
//   -45.0f 0x0127693C, 135.0f 0x01276938, 45.0f 0x0125D2B8, the double 1.0 0x01237F00; strings 0x01276954/0x0127694C/
//   0x01276940/0x01238484/0x01238494/0x01276930.
// Layout: retail CCombatActionBase is 0x114 bytes (RequiredFacing +0x114, OriginalFacing +0x120, AddedRollMode +0x12c);
//   engine/CCombatAction_ControlledStrafeJump.h (Ego_r.pdb) puts them 0x18 later (+0x12c/+0x138/+0x144) and flattens the
//   base, so `this` is modelled locally. CThing: TC bitset +0x20, component map +0x44, PhysicsTC +0x60; CPlayer ZTargeting
//   +0x20a; CCombatManager CombatStrafeTurningEnabled +0x1c.
// Shape: the reset set is a TEMPORARY argument `ResetRHSet(CRightHandedSet(up, C3DVector(OF.X, OF.Y, 0)))` -- a named set
//   local schedules the Up stores between the two Forward fstp's and the vptr load after them; the temporary gives retail's
//   early vptr load, push, then the Up/Forward.Z stores. The facing dot is written as three sequenced statements
//   (`c = fY*rY; c += fZ*rZ; c += fX*rX;`) -- every single-expression order (X+Y+Z, X+(Y+Z), (Y+Z)+X, ...) comes out
//   Z,Y,X or Z,X,Y. The blocking test is an inline CThingCreatureBase::IsBlocking (HasTC + const GetTC lookup).
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl acos(double x);
#pragma intrinsic(atan2, fmod, acos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CRightHandedSet { C3DVector Up, Forward; CRightHandedSet() {} CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {} };

struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2)
    VF(30) VF(31)
    virtual void ResetRHSet(const CRightHandedSet& set);       // slot 32 (+0x80)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF10(4) VF10(5)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);               // slot 66 (+0x108)
    VF(67) VF(68) VF(69)
    virtual C3DVector GetFacing();                             // slot 70 (+0x118)
    VF(71)
    virtual const CRightHandedSet* GetRHSet();                 // slot 72 (+0x120)
#undef VF
#undef VF10
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
    const SEntry* LowerBound(const long* type) const;          // 0x004365B0
};
struct CPlayer {
    unsigned char _pad[0x20a];
    bool ZTargeting;                                           // +0x20A
};
struct CTCCombat {
    bool IsBlocking() const;                                   // 0x007331E0
};
struct CTCCreatureModeManager {
    bool AddMode(unsigned long mode);                          // 0x0073AE10
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
    CPlayer* GetPlayer();                                      // 0x004C7A10
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    bool IsBlocking() const
    {
        if (HasTC(0x30))
            return ((CTCCombat*)GetTC(0x30))->IsBlocking();
        return false;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThingCreatureBase* m_pItem;                               // +0x04
    CThingCreatureBase* GetPItem() const;                      // 0x00A01B50
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
    CCharString& operator+=(const char* text);                 // 0x0099F100
};
struct CCombatManager {
    unsigned char _pad00[0x1c];
    bool CombatStrafeTurningEnabled;                           // +0x1C
};
__forceinline float FacingFraction(float x, float y)
{
    float fraction = (float)atan2(x, y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}
class CCombatActionBase {
public:
    virtual void v00(); virtual void v01();
    virtual void Initialise();                                 // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                         // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CCombatManager* GetCombatManager();                        // 0x00856B10
    void SetAnimationName(const CCharString& name);            // 0x008574D0
};
class CCombatAction_ControlledStrafeJump : public CCombatActionBase {
public:
    C3DVector RequiredFacing;                                  // +0x114
    C3DVector OriginalFacing;                                  // +0x120
    bool      AddedRollMode;                                   // +0x12C
    virtual void Initialise();
};

void CCombatAction_ControlledStrafeJump::Initialise()
{
    CTCPhysicsBase& physics = *PCreature.GetPItem()->GetPhysics();
    physics.ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(OriginalFacing.X, OriginalFacing.Y, 0.0f)));
    C3DVector facing = physics.GetFacing();
    float cosAngle = facing.Y * RequiredFacing.Y;
    cosAngle += facing.Z * RequiredFacing.Z;
    cosAngle += facing.X * RequiredFacing.X;
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    long sign = (facing.Y * RequiredFacing.X - facing.X * RequiredFacing.Y < 0.0f) ? -1 : 1;
    float angle = (float)acos(cosAngle) * 0.15915493667125702f;
    angle *= sign;
    float degrees = angle * 360.0f;
    float turnOffset = 0.0f;
    CCharString name("MOVEMENT_STRAFE_JUMP_");
    if (PCreature.GetPItem()->IsBlocking())
        name += "BLOCK_";
    if (degrees < -135.0f) {
        turnOffset = -0.5f;
        name += "BACK";
    } else if (degrees < -45.0f) {
        turnOffset = -0.25f;
        name += "LEFT";
    } else if (degrees > 135.0f) {
        turnOffset = 0.5f;
        name += "BACK";
    } else if (degrees > 45.0f) {
        turnOffset = 0.25f;
        name += "RIGHT";
    } else {
        name += "FORWARD";
    }
    SetAnimationName(name);
    if (GetCombatManager()->CombatStrafeTurningEnabled && !PCreature.GetPItem()->GetPlayer()->ZTargeting) {
        float delta = turnOffset - angle;
        const C3DVector& forward = PCreature.GetPItem()->GetPhysics()->GetRHSet()->Forward;
        float heading = FacingFraction(forward.X, forward.Y);
        PCreature.GetPItem()->GetPhysics()->SetFacingAngle(heading - delta);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31))
        modes->AddMode(70);
    AddedRollMode = true;
}