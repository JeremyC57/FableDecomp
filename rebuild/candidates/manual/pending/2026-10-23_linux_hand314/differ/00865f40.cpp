// CCombatAction_GenericStrikeResponseBase::Initialise() -- start a strike response: cancel the hero's weapon aiming (interface 0x81) and its aim mode, pick the RESPONSE_GENERIC_HIT_BACK / LEFT / RIGHT animation from the direction to the attacker (or the hit position) against the graphic's facing and the attacker's hand, drop creature mode 0xd and optionally face the hitter, notify interface 0x61, run the base Initialise, store the direction and the half-way frame and clear mode 0x36
// Grade: DIFFER(1106v1161) -- 58 blocks: register choice (this in esi v edi, map/attacker pointer the other), the length-squared sum emitted Z,X,Y v retail Z,Y,X (both written orders give ours), the cos(100 deg) compare lacks retail's fxch pair, and the per-animation CCharString temps tail-merge differently; flow, interface lookups, fast inverse sqrt and the facing-turns wrap line up
// Leaf: Initialise
// Retail 0x00865F40, 1161 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound 0x0040F020; CTCHeroRangedAttacks::
//   CancelAimingWeapon 0x00789FF0 / ResetProjectileWeaponMode 0x00789B30; CThing::GetPlayer 0x004C7A30; CPlayer::RemoveMode
//   0x00634190; CThing::GetPosition 0x004C73D0; CCombatActionBase::SetAnimationName 0x008574D0 (catalog
//   "CTCInGameMenu::SetPDef") / Initialise 0x00857520; CTCCreatureModeManager::RemoveMode 0x0073ABA0 / HasMode 0x0073A8A0;
//   IsFaceHitterOnHit 0x0085F370 (`__fastcall`); fmod 0x00BFEB28 (_CIfmod); CCharString 0x0099EBF0 / ~ 0x0099EAE0.
//   Inverse-sqrt table 0x013CE558; global flag 0x013B86EF. Constants 1/(2 pi) 0x01237F08, 100 degrees 0x01277128.
// Layout: from this function's use (as the landed sibling 0x008C2550) -- CCombatActionBase {owner +0x08, frame count +0x10};
//   strike response {attacker +0x114, hit position +0x11c, half frame +0x138, direction +0x13c}; CThing {interface bits
//   +0x20, interface map +0x44, graphic +0x60, flags +0x6c / +0x91, hand +0x138}.
extern "C" double __cdecl cos(double);
extern "C" double __cdecl atan2(double, double);
extern "C" double __cdecl fmod(double, double);
#pragma intrinsic(cos, atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline float GetLengthSquared() const { return Z * Z + Y * Y + X * X; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern bool g_ResetProjectileModeOnHit;                // 0x013B86EF
struct CTCBase {
    virtual void c00();
    virtual void c01();
    virtual void c02();
    virtual void c03();
    virtual void c04();
    virtual void c05();
    virtual void c06();
    virtual void c07();
    virtual void c08();
    virtual void c09();
    virtual void c0a();
    virtual void c0b();
    virtual void c0c();
    virtual void c0d();
    virtual void c0e();
    virtual void c0f();
    virtual void c10();
    virtual void c11();
    virtual void c12();
    virtual void c13();
    virtual void c14();
    virtual void c15();
    virtual void c16();
    virtual void c17();
    virtual void c18();
    virtual void c19();
    virtual void c1a();
    virtual void c1b();
    virtual void c1c();
    virtual void OnStrikeResponse();
};
struct CTCHeroRangedAttacks : CTCBase {
    void CancelAimingWeapon();                         // 0x00789FF0
    void ResetProjectileWeaponMode();                  // 0x00789B30
};
struct CTCCreatureModeManager : CTCBase {
    void RemoveMode(unsigned long mode);               // 0x0073ABA0
    bool HasMode(unsigned long mode);                  // 0x0073A8A0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
struct CPlayer {
    void RemoveMode(long mode);                        // 0x00634190
};
struct CThingGraphic {
    virtual void g00();
    virtual void g01();
    virtual void g02();
    virtual void g03();
    virtual void g04();
    virtual void g05();
    virtual void g06();
    virtual void g07();
    virtual void g08();
    virtual void g09();
    virtual void g0a();
    virtual void g0b();
    virtual void g0c();
    virtual void g0d();
    virtual void g0e();
    virtual void g0f();
    virtual void g10();
    virtual void g11();
    virtual void g12();
    virtual void g13();
    virtual void g14();
    virtual void g15();
    virtual void g16();
    virtual void g17();
    virtual void g18();
    virtual void g19();
    virtual void g1a();
    virtual void g1b();
    virtual void g1c();
    virtual void g1d();
    virtual void g1e();
    virtual void g1f();
    virtual void g20();
    virtual void g21();
    virtual void g22();
    virtual void g23();
    virtual void g24();
    virtual void g25();
    virtual void g26();
    virtual void g27();
    virtual void g28();
    virtual void g29();
    virtual void g2a();
    virtual void g2b();
    virtual void g2c();
    virtual void g2d();
    virtual void g2e();
    virtual void g2f();
    virtual void g30();
    virtual void g31();
    virtual void g32();
    virtual void g33();
    virtual void g34();
    virtual void g35();
    virtual void g36();
    virtual void g37();
    virtual void g38();
    virtual void g39();
    virtual void g3a();
    virtual void g3b();
    virtual void g3c();
    virtual void g3d();
    virtual void g3e();
    virtual void g3f();
    virtual void g40();
    virtual void g41();
    virtual void SetFacingTurns(float turns);
    virtual void g43();
    virtual void g44();
    virtual void g45();
    virtual void GetFacing(C3DVector* facing);
};
struct CThing {
    virtual void t00();
    virtual void t01();
    virtual void t02();
    virtual void t03();
    virtual void t04();
    virtual void t05();
    virtual void t06();
    virtual void t07();
    virtual void t08();
    virtual void t09();
    virtual void t0a();
    virtual void t0b();
    virtual void t0c();
    virtual void t0d();
    virtual void t0e();
    virtual void t0f();
    virtual void t10();
    virtual void t11();
    virtual void t12();
    virtual void t13();
    virtual void t14();
    virtual void t15();
    virtual void t16();
    virtual void t17();
    virtual void t18();
    virtual void t19();
    virtual void t1a();
    virtual void t1b();
    virtual void t1c();
    virtual void t1d();
    virtual void t1e();
    virtual void t1f();
    virtual void t20();
    virtual void t21();
    virtual void t22();
    virtual void t23();
    virtual void t24();
    virtual void t25();
    virtual void t26();
    virtual void t27();
    virtual void t28();
    virtual void t29();
    virtual void t2a();
    virtual void t2b();
    virtual void t2c();
    virtual void t2d();
    virtual void t2e();
    virtual void t2f();
    virtual void t30();
    virtual void t31();
    virtual void t32();
    virtual void t33();
    virtual void t34();
    virtual void t35();
    virtual void t36();
    virtual void t37();
    virtual void t38();
    virtual void t39();
    virtual void t3a();
    virtual void t3b();
    virtual void t3c();
    virtual void t3d();
    virtual void t3e();
    virtual void t3f();
    virtual void t40();
    virtual void t41();
    virtual void t42();
    virtual void t43();
    virtual void t44();
    virtual void t45();
    virtual void t46();
    virtual void t47();
    virtual void t48();
    virtual void t49();
    virtual void t4a();
    virtual void t4b();
    virtual void t4c();
    virtual void StopAnimating();
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CThingGraphic* Graphic;                            // +0x60
    unsigned char  _pad64[0x6c - 0x64];
    unsigned char  Flags6c;                            // +0x6c
    unsigned char  _pad6d[0x91 - 0x6d];
    unsigned char  Flags91;                            // +0x91
    unsigned char  _pad92[0x138 - 0x92];
    long           Hand;                               // +0x138
    const C3DVector& GetPosition() const;              // 0x004C73D0
    CPlayer* GetPlayer() const;                        // 0x004C7A30
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IsFaceHitterOnHit(CThing* thing);     // 0x0085F370
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    CThing* GetPItem() const;                          // 0x00A01B50
};
class CCombatActionBase {
public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    long                    FrameCount;                // +0x10
    unsigned char           _pad14[0x114 - 0x14];
    void SetAnimationName(const CCharString& name);    // 0x008574D0
    void Initialise();                                 // 0x00857520
};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
public:
    CBaseIntelligentPointer Attacker;                  // +0x114
    C3DVector               HitPosition;               // +0x11c
    unsigned char           _pad128[0x138 - 0x128];
    long                    HalfFrame;                 // +0x138
    C3DVector               Direction;                 // +0x13c
    void Initialise();
};

void CCombatAction_GenericStrikeResponseBase::Initialise()
{
    Owner.GetPItem()->StopAnimating();
    CTCHeroRangedAttacks* ranged;
    if (Owner.GetPItem()->GetInterface(&ranged, 0x81)) {
        ranged->CancelAimingWeapon();
        if (g_ResetProjectileModeOnHit)
            ranged->ResetProjectileWeaponMode();
        Owner.GetPItem()->GetPlayer()->RemoveMode(0x22);
    }
    C3DVector facing;
    Owner.GetPItem()->Graphic->GetFacing(&facing);
    C3DVector delta = HitPosition - Owner.GetPItem()->GetPosition();
    if (Attacker.GetPItem() && !(Attacker.GetPItem()->Flags91 & 1)) {
        CThing* attacker = Attacker.GetPItem();
        delta = attacker->GetPosition() - Owner.GetPItem()->GetPosition();
    }
    delta *= FastInvSqrt(delta.GetLengthSquared());
    C3DVector direction;
    if (DotProduct(facing, delta) < cos(1.7453293f)) {
        direction = -delta;
        SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_BACK"));
    } else {
        direction = delta;
        if (Attacker.GetPItem() && !(Attacker.GetPItem()->Flags91 & 1) && (Attacker.GetPItem()->Flags6c & 8)) {
            long hand = Attacker.GetPItem()->Hand;
            if (hand == 0) {
                SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_LEFT"));
                goto chosen;
            }
            if (hand == 1) {
                SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_RIGHT"));
                goto chosen;
            }
        }
        if (delta.Y * facing.X - facing.Y * delta.X < 0.0f)
            SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_LEFT"));
        else
            SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_RIGHT"));
    }
chosen:
    CTCCreatureModeManager* modes;
    if (Owner.GetPItem()->GetInterface(&modes, 0x31)) {
        modes->RemoveMode(0xd);
        if (IsFaceHitterOnHit(Owner.GetPItem())) {
            CThingGraphic* graphic = Owner.GetPItem()->Graphic;
            float turns = (float)atan2(direction.X, direction.Y) * 0.15915494f;
            if (turns < 0.0f || turns >= 1.0f) {
                turns = (float)fmod(turns, 1.0);
                if (turns < 0.0f)
                    turns += 1.0f;
            }
            graphic->SetFacingTurns(turns);
        }
    }
    CTCBase* hitListener;
    if (Owner.GetPItem()->GetInterface(&hitListener, 0x61))
        hitListener->OnStrikeResponse();
    CCombatActionBase::Initialise();
    Direction = direction;
    HalfFrame = FrameCount / 2;
    if (modes && modes->HasMode(0x36))
        modes->RemoveMode(0x36);
}
