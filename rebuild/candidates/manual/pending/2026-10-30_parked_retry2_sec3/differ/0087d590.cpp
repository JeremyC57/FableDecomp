// Grade: DIFFER(1238v1215) 334 difflines (wave 2 sec3: first reconstruction from the disassembly; was the empty skeleton, 1v1215 / 327 lines)
// Tried: g1 straight draft (1236v1215/348); g2 dot as `X*RX + (Y*RY + Z*RZ)` and `if (!(cross < 0.0f))` (both now exact:
//   retail's term order and `test ah,5; jnp`; 1236v1215/344); g3 value-bool GetInterface `(bits >> n) & 1` (retail's
//   `shr ecx,10h; and cl,1` bool store; this file, 1238v1215/334).
// Residue: the control flow and every call line up, but our frame is 0x1B4 against retail's 0x1BC and ebx/edi are pushed
//   later, so nearly every esp offset differs (the line count is mostly that); the virtual calls load the physics
//   component into eax instead of ecx; the fade-out `this` is loaded before the CRGBColour bytes.
// Next: find the 8 missing frame bytes (retail keeps the HasMeleeAbilityDef bool and the CDefPointer copy in their own
//   slots; try a named `CDefPointer` copy and `bool` local), then the shared engine headers.
// CCreatureAction_InterruptableMidAttackAutoTurn::Initialise() -- start the auto-turn strike: the STRIKE_MEDIUM_FRONT
//   combat animation, the signed turn fraction toward RequiredFacing (acos of the clamped dot / 2pi), the turning
//   animation, then the base Initialise; turn the creature by the animation's rotation, flag the combat component for a
//   melee ability, and apply the animation's combat-misc screen flash, target placement and knockdown response
// Leaf: Initialise
// Name: catalog CCreatureAction_InterruptableMidAttackAutoTurn::Initialise (vtable slot of the class).
// Model: the landed sibling FrameUpdate 0x0087CB10 (CCombatActionBase PCreature +0x08, PMeleeAbilityDef +0xB8,
//   HasMeleeAbilityDef, CTCCombat Flag94 via interface 0x30); RequiredFacing +0x114 (engine header / Ego_r.pdb).
// Callees: CCombatActionBase::SetCombatAnimation 0x008574D0, Initialise 0x00857520, ~CCombatActionBase 0x0062E900;
//   SetupTurningAnimation 0x0087CFE0; CAnimationEntry::GetRotationInAnimation 0x005DBFD0, GetComponent<> 0x00738A00 /
//   0x0087E150 / 0x0087E1D0; C3DVector::Rotate 0x00A13C60; CDisplayEngine::StartScreenFadeOut 0x00434C00 on g_pEngine
//   0x013B86A0 (+0x28); CCombatAction_GenericStrikeResponseKnockdown ctor 0x00867A70; CThingCreatureBase::
//   SetCurrentAction 0x006644F0; CCharString 0x0099EBF0 / 0x0099EC30 / operator!= 0x0099E960 / dtor 0x0099EAE0;
//   _CIacos (acos intrinsic).
#include <math.h>
#pragma intrinsic(acos, sin, cos)

class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length);          // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator!=(const char* text) const;             // 0x0099E960
};

class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    void Rotate(const C3DVector& axis, float angle);     // 0x00A13C60
};
class CRightHandedSet {
public:
    C3DVector Up;                                        // +0x00
    C3DVector Forward;                                   // +0x0C
    __forceinline CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CTCPhysicsBase {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void SetPosition(const C3DVector& pos);      // slot 30 (+0x78)
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual C3DVector GetForward() const;                // slot 70 (+0x118)
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 75 (+0x12C)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                  // +0x0C
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};
class CCreatureActionBase;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                             // +0x91 bit 0: dead
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0 (CThingCreatureBase)
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool present = (m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (present)
            *ppOut = (T*)FindTC(interfaceType);
        return present;
    }
};
class CTCCombat { public:
    unsigned char _pad00[0x94];
    bool          Flag94;                                // +0x94
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long IVRefCount;                                     // +4
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Destroy(); }
    bool IsValid() const { return Object != 0; }
};

struct CAnimComponentCombatMisc { unsigned char _pad[8]; bool FlashScreen; };
#pragma pack(push, 1)
struct CAnimComponentTargetPlacement { unsigned char _pad[8]; bool Enabled; float Distance; float Angle; };
#pragma pack(pop)
struct CAnimComponentKnockdown {
    unsigned char _pad[8];
    CCharString AnimName;                                // +0x08
    CCharString GetAnimName() const { return AnimName; }
};
class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const;  // 0x005DBFD0
    void GetComponent(const CAnimComponentCombatMisc** out) const;       // 0x00738A00
    void GetComponent(const CAnimComponentTargetPlacement** out) const;  // 0x0087E150
    void GetComponent(const CAnimComponentKnockdown** out) const;        // 0x0087E1D0
};
class CDisplayEngine {
public:
    void StartScreenFadeOut(float fadeOutTime, float fadeInTime, CRGBColour colour, float holdTime);   // 0x00434C00
};
class CEngine {
public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                      // +0x28
};
extern CEngine* g_pEngine;                               // 0x013B86A0

class CCombatActionBase { public:
    virtual void v00();
    unsigned char                 _pad04[4];
    CIntelligentPointer<CThing>   PCreature;             // +0x08
    unsigned char                 _pad10[0x74 - 0x10];
    CAnimationEntry*              PAnimEntry;            // +0x74 CCountedPointer<CAnimationEntry>::Data
    unsigned char                 _pad78[0xa8 - 0x78];
    CIntelligentPointer<CThing>   PTarget;               // +0xa8
    unsigned char                 _padb0[0xb8 - 0xb0];
    CDefPointer<CDefBase>         PMeleeAbilityDef;      // +0xb8
    unsigned char                 _padbc[0x114 - 0xbc];
    CCombatActionBase(const CCombatActionBase&);
    ~CCombatActionBase();                                // 0x0062E900
    void SetCombatAnimation(const CCharString& name);    // 0x008574D0
    void Initialise();                                   // 0x00857520
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefPointer<CDefBase> GetMeleeAbilityDef() const { return PMeleeAbilityDef; }
    bool HasMeleeAbilityDef() const { return GetMeleeAbilityDef().IsValid(); }
};
class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase { public:
    CCharString                 AnimName;                // +0x114
    CIntelligentPointer<CThing> PThingThatHit;           // +0x118
    C3DVector                   PointOfHit;              // +0x120
    CCombatAction_GenericStrikeResponseKnockdown(CThing* creature, CThing* hitter, const CCharString& anim); // 0x00867A70
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public:
    float RequiredFacing_X, RequiredFacing_Y, RequiredFacing_Z;   // +0x114
    void SetupTurningAnimation(float turn);              // 0x0087CFE0
    void Initialise();
};

void CCreatureAction_InterruptableMidAttackAutoTurn::Initialise()
{
    SetCombatAnimation(CCharString("STRIKE_MEDIUM_FRONT", -1));
    C3DVector forward = GetCreature()->PhysicsTC->GetForward();
    float dot = forward.X * RequiredFacing_X + (forward.Y * RequiredFacing_Y + forward.Z * RequiredFacing_Z);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    long sign = -1;
    if (!(forward.Y * RequiredFacing_X - forward.X * RequiredFacing_Y < 0.0f))
        sign = 1;
    float turn = (float)acos(dot) * 0.15915494f * sign;
    SetupTurningAnimation(turn);
    CCombatActionBase::Initialise();

    CAnimationEntry* anim = PAnimEntry;
    float rotation;
    anim->GetRotationInAnimation(&rotation);
    turn = rotation - turn;
    C3DVector facing = GetCreature()->PhysicsTC->GetForward();
    facing.Rotate(C3DVector(0.0f, 0.0f, 1.0f), -turn);
    GetCreature()->PhysicsTC->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing));

    CTCCombat* combat;
    if (HasMeleeAbilityDef() && GetCreature()->GetInterface(&combat, 0x30))
        combat->Flag94 = true;

    const CAnimComponentCombatMisc* misc;
    anim->GetComponent(&misc);
    if (misc->FlashScreen)
        g_pEngine->PDisplayEngine->StartScreenFadeOut(0.1f, 0.1f, CRGBColour(255, 255, 255, 255), 0.25f);

    const CAnimComponentTargetPlacement* placement;
    anim->GetComponent(&placement);
    if (placement->Enabled) {
        if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
            float distance = placement->Distance;
            float angle = placement->Angle;
            C3DVector pos = GetCreature()->PhysicsTC->Position;
            C3DVector dir = GetCreature()->PhysicsTC->GetForward();
            C3DVector side(dir.Y, -dir.X, 0.0f);
            CTCPhysicsBase* targetPhysics = PTarget.GetPItem()->PhysicsTC;
            double t = angle * 6.283185307179586;
            C3DVector offset = (side * (float)sin(t) + dir * (float)cos(t)) * distance;
            targetPhysics->SetPosition(pos + offset);
            targetPhysics->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), -offset));
        }
    }

    const CAnimComponentKnockdown* knockdown;
    anim->GetComponent(&knockdown);
    if (knockdown->GetAnimName() != "") {
        CThing* target = PTarget.GetPItem();
        target->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(target, GetCreature(), knockdown->GetAnimName()));
    }
}
