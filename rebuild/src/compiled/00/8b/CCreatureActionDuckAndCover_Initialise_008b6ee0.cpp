// CCreatureAction_DuckAndCover::Initialise() -- names the "DIVE_FOR_COVER" animation, runs the base Initialise, normalises the dive direction and turns the creature's physics to face it (atan2 as a 0..1 turn fraction, wrapped with fmod)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B6EE0, length 348 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, ~CCharString
// @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, _CIfmod @ 0x00BFEB28
// (the fmod intrinsic), the physics component's virtual slot 66 (+0x108, one float: the facing fraction).
// Globals: the 128-entry reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 1/(2pi) 0x01237F08,
// 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the double 1.0 0x01237F00.
// Shape: C3DVector's inline fast Normalise (the landed 0x007A6E20 model) on the direction member at +0x114; the physics pointer
// is fetched before the angle maths; `fraction = atan2(X, Y) * (1/2pi)` is a float local tested `< 0 || >= 1` (`test ah,5 ; jnp`
// then `test ah,1 ; jne`) before the fmod wrap and the negative fix-up. Retail CCombatActionBase is 0x114 bytes (the PDB header
// is 0x18 larger), so `this` is modelled locally.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CTCPhysicsBase { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* m_pPhysics;             // +0x60
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_DuckAndCover : public CCombatActionBase { public:
    C3DVector Direction;                    // +0x114 the dive direction
    virtual void Initialise();
};

void CCreatureAction_DuckAndCover::Initialise()
{
    SetAnimationName("DIVE_FOR_COVER");
    CCombatActionBase::Initialise();
    Direction.Normalise();
    CTCPhysicsBase* physics = PCreature.GetPItem()->m_pPhysics;
    float fraction = (float)atan2(Direction.X, Direction.Y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    physics->SetFacingAngle(fraction);
}