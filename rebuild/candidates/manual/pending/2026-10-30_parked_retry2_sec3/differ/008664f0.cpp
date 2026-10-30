// CCombatAction_GenericStrikeResponseKnockdown::Initialise() -- start a knockdown response: either play the override animation, or work out where the blow came from (the attacker, projected onto its weapon line when it has a weapon interface 0xc8 that is swinging) and pick RESPONSE_GENERIC_HIT_BACK / LEFT / RIGHT_KNOCKDOWN against the creature's facing and knock direction, setting the push direction; then run the base Initialise, face along the push direction unless mode 0x1d is current, add the named knockdown mode and drop modes 0x42, 0xd and 0x36
// Grade: DIFFER(1957v2028) -- 228 difflines (base 1963v2028/231); __forceinline memberwise C3DVector copy ctor. Left: the inline CCharString == "" materialises a bool (sete) where retail branches, the hit-position copy's dead Z word, x87 term order of the weapon-line projection, register/slot choices.
// Leaf: Initialise
// Retail 0x008664F0, 2028 bytes, plain `ret` (virtual).
// Callees: as CCombatAction_GenericResponseToExplosion::Initialise 0x00867200 plus CCharString == / != const char*
//   0x004115A0 / 0x0099E960, CThing::GetPosition 0x004C73D0 and the weapon-interface lower_bound 0x004365B0. Mode vtables
//   0x01267E78 / 0x01267568; cos(constant 0x012771A0).
// Layout: from this function's use -- CCombatActionBase {owner +0x08, def +0x74}; knockdown {override animation +0x114,
//   attacker +0x118, hit position +0x120, knock direction +0x12d (packed), knocked-back flag +0x13a}; CThing {size +0x10,
//   interface bits +0x20, map +0x44, graphic +0x60, flags +0x91}; the graphic's weapon line +0xc / +0x18.
extern "C" double __cdecl atan2(double, double);
extern "C" double __cdecl fmod(double, double);
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl cos(double);
extern "C" int __cdecl strcmp(const char*, const char*);
#pragma intrinsic(atan2, fmod, fabs, cos, strcmp)
void* __cdecl operator new(unsigned int size);
extern float g_Epsilon;                                // 0x0129BA3C
float __stdcall AngleDifference(float angle);            // 0x00A441E0
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
struct C3DVector {
    float X, Y, Z; __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                  // 0x00A14440
    bool IsNonZero() const
    {
        float z = Z;
        float y = Y;
        float x = X;
        return x * x + y * y + z * z > g_Epsilon * g_Epsilon;
    }
};
__forceinline C3DVector operator*(const C3DVector& v, float s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator-(const C3DVector& a) { return C3DVector(-a.X, -a.Y, -a.Z); }
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct SStringData { char* Text; long Length; };
struct CCharString {
    SStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    bool IsEqual(const char* text) const;              // 0x004115A0
    bool operator!=(const char* text) const;           // 0x0099E960
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp("", text) == 0;
        return IsEqual(text);
    }
};
inline CCharString CopyString(const CCharString& s) { return s; }
struct CDefStringTable {
    CCharString GetString(long id);                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828
extern bool g_ResetProjectileModeOnHit;                // 0x013B86EF
struct SOrientation {
    C3DVector Up;
    C3DVector Forward;
    SOrientation(float upX, float upY, float upZ, const C3DVector& forward) : Up(upX, upY, upZ), Forward(forward) { }
};
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* Data;
    CBaseIntelligentPointer();                         // 0x00A01B10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {       // vtable 0x01238C6C
    virtual void v00();
};
struct CModeBase {                                     // 0x1c bytes
    CModeBase(CThing* owner);                          // 0x0073EAC0
    virtual void v00();
    virtual unsigned long GetType();                   // slot 1 (+0x04)
    unsigned char _pad04[0x1c - 0x04];
};
struct CModeListener {                                 // vtable 0x01267568
    virtual void v00();
};
struct CModeStrikeKnockdown : CModeBase, CModeListener {   // vtable 0x01267E78, 0x28 bytes
    CCharString   Name;                                // +0x20
    unsigned long NameHash;                            // +0x24
    __forceinline CModeStrikeKnockdown(const CCharString& name, CThing* owner) : CModeBase(owner)
    {
        Name = name;
        const char* text;
        unsigned long length;
        if (name.PStringData == 0) {
            length = 0;
            text = "";
        } else {
            length = name.PStringData->Length;
            text = name.PStringData->Text;
        }
        NameHash = Calc(0, text, length);
    }
    virtual unsigned long GetType();
};
struct CCPPointerInfo {
    long  Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
void __fastcall DeleteModeBase(void* object);          // 0x007180B0
struct CModePointer {                                  // CCountedPointer<CModeBase>
    CModeBase*      Data;
    CCPPointerInfo* Info;
    __forceinline CModePointer(CModeBase* p) : Data(p)
    {
        if (p) {
            CCPPointerInfo* info = (CCPPointerInfo*)operator new(sizeof(CCPPointerInfo));
            if (info) {
                info->Count = 1;
                info->Destroy = DeleteModeBase;
                info->Object = p;
            }
            Info = info;
        } else
            Info = 0;
    }
};
struct CTCBase {
    virtual void v00();
};
struct CTCHeroRangedAttacks : CTCBase {
    void CancelAimingWeapon();                         // 0x00789FF0
    void ResetProjectileWeaponMode();                  // 0x00789B30
};
struct CTCCreatureModeManager : CTCBase {
    CModeBase* GetCurrentMode();                       // 0x0073A6E0
    bool AddMode(CModePointer mode);                   // 0x0073ACC0
    void RemoveMode(unsigned long mode);               // 0x0073ABA0
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
struct CWeaponInterface : CTCBase {
    unsigned char _pad04[0x1b - 0x04];
    bool          Swinging;                            // +0x1b
};
struct CWeaponMap {
    struct SEntry { long type; CWeaponInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x004365B0
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
    virtual void SetOrientation(const SOrientation& orientation);
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
    virtual void g42();
    virtual void g43();
    virtual void g44();
    virtual void g45();
    virtual const C3DVector* GetFacing(C3DVector* facing);
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     WeaponTip;                           // +0x0c
    C3DVector     WeaponBase;                          // +0x18
};
struct CThing {
    virtual void v00();
    unsigned char  _pad04[0x10 - 0x04];
    unsigned char  Size;                               // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CThingGraphic* Graphic;                            // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  Flags91;                            // +0x91
    const C3DVector& GetPosition() const;              // 0x004C73D0
    __forceinline bool GetWeapon(CWeaponInterface** ppOut)
    {
        if (m_presentBits[0xc8 >> 5] & (1UL << (0xc8 & 31))) {
            long type = 0xc8;
            CWeaponMap* map = (CWeaponMap*)&m_interfaces;
            CWeaponMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (it == map->last || 0xc8 < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    CPlayer* GetPlayer() const;                        // 0x004C7A30
    void SetBodyReorienterActive(bool active);         // 0x00663490
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
struct CModeDef {
    unsigned char _pad00[8];
    long          NameId;                              // +0x08
};
struct CModeDefRef { CModeDef* Data; };
struct CCombatActionDef {
    CModeDefRef GetModeDef() const;                    // 0x00867F60
};
class CCombatActionBase {
public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    unsigned char           _pad10[0x74 - 0x10];
    CCombatActionDef*       Def;                       // +0x74
    unsigned char           _pad78[0x114 - 0x78];
    void SetAnimationName(const CCharString& name);    // 0x008574D0
    void Initialise();                                 // 0x00857520
};
#pragma pack(push, 1)
struct CPackedVector { float X, Y, Z; };
#pragma pack(pop)
class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase {
public:
    CCharString             OverrideAnimation;         // +0x114
    CBaseIntelligentPointer Attacker;                  // +0x118
    C3DVector               HitPosition;               // +0x120
    unsigned char           _pad12c;
    CPackedVector           KnockDirection;            // +0x12d
    bool                    _pad139;
    bool                    KnockedBack;               // +0x13a
    void Initialise();
};

void CCombatAction_GenericStrikeResponseKnockdown::Initialise()
{
    KnockedBack = false;
    if (OverrideAnimation == "")
        Owner.GetPItem()->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ranged;
    if (Owner.GetPItem()->GetInterface(&ranged, 0x81)) {
        ranged->CancelAimingWeapon();
        if (g_ResetProjectileModeOnHit)
            ranged->ResetProjectileWeaponMode();
        Owner.GetPItem()->GetPlayer()->RemoveMode(0x22);
    }
    C3DVector pushDirection;
    Owner.GetPItem()->Graphic->GetFacing(&pushDirection);
    if (OverrideAnimation != "") {
        SetAnimationName(OverrideAnimation);
    } else {
        C3DVector source = HitPosition;
        if (Attacker.GetPItem() && !(Attacker.GetPItem()->Flags91 & 1)) {
            source = Attacker.GetPItem()->GetPosition();
            CWeaponInterface* weapon;
            if (Attacker.GetPItem()->GetWeapon(&weapon) && weapon->Swinging) {
                CThingGraphic* graphic = Attacker.GetPItem()->Graphic;
                C3DVector line = Attacker.GetPItem()->Graphic->WeaponTip - graphic->WeaponBase;
                float lengthSquared = line.X * line.X + line.Z * line.Z + line.Y * line.Y;
                if (lengthSquared > g_Epsilon * g_Epsilon) {
                    CThing* attacker = Attacker.GetPItem();
                    C3DVector offset = attacker->GetPosition() - Owner.GetPItem()->GetPosition();
                    float t = (offset.X * line.X + offset.Z * line.Z + offset.Y * line.Y) / lengthSquared;
                    source = line * t + Attacker.GetPItem()->GetPosition();
                }
            }
        }
        const C3DVector& ownerPosition = Owner.GetPItem()->GetPosition();
        C3DVector fromSource(source.X - ownerPosition.X, source.Y - ownerPosition.Y, 0.0f);
        if (fromSource.IsNonZero())
            fromSource.Normalise();
        C3DVector knock(KnockDirection.X, KnockDirection.Y, 0.0f);
        if (knock.IsNonZero())
            knock.Normalise();
        C3DVector facing;
        Owner.GetPItem()->Graphic->GetFacing(&facing);
        facing.Z = 0.0f;
        if (facing.IsNonZero())
            facing.Normalise();
        pushDirection = facing;
        if (cos(2.0943952f) > DotProduct(facing, fromSource)) {
            SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN"));
            if (Owner.GetPItem()->Size == 1)
                KnockedBack = true;
            if (Attacker.GetPItem() && !(Attacker.GetPItem()->Flags91 & 1)) {
                const C3DVector& position = Owner.GetPItem()->GetPosition();
                C3DVector away(source.X - position.X, source.Y - position.Y, 0.0f);
                if (away.IsNonZero()) {
                    away.Normalise();
                    pushDirection = -away;
                } else
                    pushDirection = C3DVector(0.0f, -1.0f, 0.0f);
            }
        } else {
            if (Attacker.GetPItem() && !(Attacker.GetPItem()->Flags91 & 1)) {
                const C3DVector& position = Owner.GetPItem()->GetPosition();
                C3DVector toward(source.X - position.X, source.Y - position.Y, 0.0f);
                if (toward.IsNonZero()) {
                    toward.Normalise();
                    pushDirection = toward;
                } else
                    pushDirection = C3DVector(0.0f, 1.0f, 0.0f);
            }
            C3DVector currentFacing;
            Owner.GetPItem()->Graphic->GetFacing(&currentFacing);
            SetAnimationName(CCharString(knock.Y * currentFacing.X - currentFacing.Y * knock.X > 0.0f
                                             ? "RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN"
                                             : "RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN"));
        }
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes;
    Owner.GetPItem()->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetType() != 0x1d)
        Owner.GetPItem()->Graphic->SetOrientation(SOrientation(0.0f, 0.0f, 1.0f, pushDirection));
    CCharString name = CopyString(g_DefStringTable.GetString(Def->GetModeDef().Data->NameId));
    CModeStrikeKnockdown* mode = new CModeStrikeKnockdown(name, Owner.GetPItem());
    modes->AddMode(CModePointer(mode));
    modes->RemoveMode(0x42);
    modes->RemoveMode(0xd);
    modes->RemoveMode(0x36);
}
