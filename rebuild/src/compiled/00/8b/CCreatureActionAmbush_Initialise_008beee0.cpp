// CCreatureAction_Ambush::Initialise() -- picks the left/right ambush animation from the side the target is on, says the ambush speech, turns the creature to face along the side vector, then runs the base Initialise
// Leaf: ?Initialise@CCreatureAction_Ambush
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BEEE0, 528 bytes, plain `ret`; virtual Initialise of CCreatureAction_Ambush.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PTarget +0xa8), CThing::GetPos 0x004C73D0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation 0x008574D0,
//   CCombatActionBase::GetCombatManager 0x00856B10, CombatDialogueManager::SaySpeech 0x008AFC40, the physics component's
//   virtual slot 66 (+0x108, the facing fraction), _CIfmod 0x00BFEB28 (fmod intrinsic), CCombatActionBase::Initialise 0x00857520.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC,
//   1.0f 0x0122DED8, 1/(2pi) 0x01237F08, the double 1.0 0x01237F00; strings "GENERATE_AMBUSH_LEFT" 0x0127F2EC /
//   "GENERATE_AMBUSH_RIGHT" 0x0127F2D4.
// Layout: retail CCombatActionBase is 0x114 bytes (JumpDest read at +0x114/+0x118), the Ego_r.pdb view is 0x18 larger
//   (engine/CCreatureAction_Ambush.h puts JumpDest at +0x12c and flattens the base to pads), so `this` is modelled locally
//   as in the landed sibling 0x008B6EE0. The combat manager's dialogue manager sits at +0x38, its def pointer at +0x44,
//   CCombatDialogueDef::Ambushing at +0x54.
// Shape: the jump direction is the flattened XY of `JumpDest - position`, built as `C3DVector(C2DVector(a - b))` (Z = 0
//   from the 2D->3D converting ctor) and normalised with the fast inverse square root. The converting-ctor copies are what
//   make VC7.1 scale X/Y in place (`fxch st(2) ; fmul st(2)`) and give the direction its own frame words shared with the
//   negated side temporary; a direct `C3DVector(dx, dy, 0)` or `a - b` + `Z = 0` duplicates the reciprocal instead
//   (`fld st(0) ; fmul st(3)`). The side vector is (dir.Y, -dir.X, 0) and is negated for the right-hand ambush.
//   The speech id is read through a scoped CDefPointer copy (AddRef + inline Release with no null test on release).
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

class C2DVector;
class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    inline C3DVector(const C2DVector& v);
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline void Normalise() { float r = FastInvSqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
};
class C2DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
};
inline C3DVector::C3DVector(const C2DVector& v) : X(v.X), Y(v.Y), Z(0.0f) {}

class CTCPhysicsBase { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    const C3DVector& GetPos() const;   // 0x004C73D0
};
class CThingCreatureBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
    void AddRef() const { ++((CDefBase*)this)->m_refCount; }
    void Release() const { if (--((CDefBase*)this)->m_refCount == 0) ((CDefBase*)this)->Destroy(); }
};
class CCombatDialogueDef : public CDefBase { public:
    unsigned char _pad08[0x54 - 0x08];
    long Ambushing;                         // +0x54
};
template <class T>
class CDefPointer { public:
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CDefPointer() { m_pDef->Release(); }
    T* operator->() const { return m_pDef; }
};
class CombatDialogueManager { public:
    unsigned char SpeechGroupInfoMap[0x0c];                  // +0x00
    CDefPointer<const CCombatDialogueDef> PDialogueDef;      // +0x0c
    bool SaySpeech(CThingCreatureBase& creature, unsigned long speech);   // 0x008AFC40
};
class CCombatManager { public:
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;  // +0x38
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;        // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    CCombatManager* GetCombatManager();     // 0x00856B10
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};
class CCreatureAction_Ambush : public CCombatActionBase { public:
    C3DVector JumpDest;                     // +0x114
    virtual void Initialise();
};

void CCreatureAction_Ambush::Initialise()
{
    C3DVector dir(C2DVector(JumpDest - PCreature.GetPItem()->GetPos()));
    dir.Normalise();
    C3DVector side(dir.Y, -dir.X, 0.0f);
    const C3DVector& target = PTarget.GetPItem()->GetPos();
    if (side.X * target.Y - side.Y * target.X > 0.0f) {
        SetCombatAnimation("GENERATE_AMBUSH_LEFT");
    } else {
        SetCombatAnimation("GENERATE_AMBUSH_RIGHT");
        side = -side;
    }
    long speech;
    {
        CCombatManager* manager = GetCombatManager();
        CDefPointer<const CCombatDialogueDef> def = manager->DialogueManager.PDialogueDef;
        speech = def->Ambushing;
    }
    CCombatManager* manager = GetCombatManager();
    manager->DialogueManager.SaySpeech(*(CThingCreatureBase*)PCreature.GetPItem(), speech);
    CTCPhysicsBase* physics = PCreature.GetPItem()->m_pPhysics;
    float fraction = (float)atan2(side.X, side.Y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    physics->SetFacingAngle(fraction);
    CCombatActionBase::Initialise();
}