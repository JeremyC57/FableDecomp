// CCombatAction_GenericResponseToExplosion::Initialise() -- start an explosion knockdown: activate the body reorienter, cancel the hero's weapon aiming, flatten and normalise the blast direction (falling back to the graphic's facing, then +X), pick RESPONSE_GENERIC_HIT_BACK / LEFT / RIGHT_KNOCKDOWN from the angle between the facing and the blast, run the base Initialise, face the creature along the push direction unless mode 0x1d is current, and add a knockdown mode (named from the action def's string table entry, targeting the attacker) before dropping mode 0xd
// Grade: DIFFER(1137v1182) -- 197 difflines (base 1133v1182/203); __forceinline memberwise C3DVector copy ctor. Left: frame 0x3c vs 0x30, ebx/ebp swap for &Owner and the graphic, the mode-def getter result read through eax, the orientation temp layout.
// Leaf: Initialise
// Retail 0x00867200, 1182 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / () 0x00A01B10 / SetPItem 0x00A01B90; CThingCreatureBase::
//   SetBodyReorienterActive 0x00663490; CVectorMap::LowerBound 0x0040F020; CTCHeroRangedAttacks::CancelAimingWeapon
//   0x00789FF0 / ResetProjectileWeaponMode 0x00789B30; CThing::GetPlayer 0x004C7A30; CPlayer::RemoveMode 0x00634190;
//   C3DVector::Normalise 0x00A14440; fmod 0x00BFEB28; AngleDifference 0x00A441E0; CCombatActionBase::SetAnimationName
//   0x008574D0 / Initialise 0x00857520; CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 / AddMode 0x0073ACC0 / RemoveMode
//   0x0073ABA0; the action def's mode-def getter 0x00867F60; CDefStringTable::GetString 0x009D49B0 (table 0x013CA828);
//   CModeBase(CThing*) 0x0073EAC0; CRC Calc 0x004014A0 (`__fastcall`); operator new 0x00BFEA1A; CCharString 0x0099EBF0 /
//   () 0x0099E4B0 / copy 0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0. Mode vtables 0x012770B4 / 0x01267568, target
//   pointer vtable 0x01238C6C, counted-pointer deleter 0x007180B0.
// Layout: from this function's use -- CCombatActionBase {owner +0x08, def +0x74}; explosion response {attacker +0x114,
//   blast direction +0x128, knocked-back flag +0x134}; CThing {interface bits +0x20, map +0x44, graphic +0x60}.
extern "C" double __cdecl atan2(double, double);
extern "C" double __cdecl fmod(double, double);
extern "C" double __cdecl fabs(double);
#pragma intrinsic(atan2, fmod, fabs)
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
struct SStringData { char* Text; long Length; };
struct CCharString {
    SStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
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
struct CModeKnockdown : CModeBase, CModeListener {     // vtable 0x012770B4, 0x30 bytes
    CCharString   Name;                                // +0x20
    unsigned long NameHash;                            // +0x24
    CThingPointer Target;                              // +0x28
    __forceinline CModeKnockdown(const CCharString& name, CThing* target, CThing* owner) : CModeBase(owner)
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
        Target.SetPItem(target);
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
    virtual float SetFacingTurns(float turns);
    virtual void g40();
    virtual void g41();
    virtual void g42();
    virtual void g43();
    virtual void g44();
    virtual void g45();
    virtual const C3DVector* GetFacing(C3DVector* facing);
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
class CCombatAction_GenericResponseToExplosion : public CCombatActionBase {
public:
    CBaseIntelligentPointer Attacker;                  // +0x114
    unsigned char           _pad11c[0x128 - 0x11c];
    C3DVector               BlastDirection;            // +0x128
    bool                    KnockedBack;               // +0x134
    void Initialise();
};

void CCombatAction_GenericResponseToExplosion::Initialise()
{
    Owner.GetPItem()->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ranged;
    if (Owner.GetPItem()->GetInterface(&ranged, 0x81)) {
        ranged->CancelAimingWeapon();
        if (g_ResetProjectileModeOnHit)
            ranged->ResetProjectileWeaponMode();
        Owner.GetPItem()->GetPlayer()->RemoveMode(0x22);
    }
    BlastDirection.Z = 0.0f;
    if (BlastDirection.IsNonZero()) {
        BlastDirection.Normalise();
    } else {
        C3DVector facing;
        BlastDirection = *Owner.GetPItem()->Graphic->GetFacing(&facing);
        BlastDirection.Z = 0.0f;
        if (BlastDirection.IsNonZero())
            BlastDirection.Normalise();
        else
            BlastDirection = C3DVector(1.0f, 0.0f, 0.0f);
    }
    C3DVector away = BlastDirection * -1.0f;
    CThingGraphic* graphic = Owner.GetPItem()->Graphic;
    float turns = (float)atan2(away.X, away.Y) * 0.15915494f;
    if (turns < 0.0f || turns >= 1.0f) {
        turns = (float)fmod(turns, 1.0);
        if (turns < 0.0f)
            turns += 1.0f;
    }
    float difference = AngleDifference(graphic->SetFacingTurns(turns));
    C3DVector pushDirection;
    if ((float)fabs(difference) > 0.25f) {
        SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN"));
        if (Owner.GetPItem()->Size == 1)
            KnockedBack = true;
        pushDirection = BlastDirection;
    } else {
        if (difference < 0.0f)
            SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN"));
        else
            SetAnimationName(CCharString("RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN"));
        pushDirection = away;
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes;
    Owner.GetPItem()->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetType() != 0x1d) {
        Owner.GetPItem()->Graphic->SetOrientation(SOrientation(0.0f, 0.0f, 1.0f, pushDirection));
    }
    CCharString name = CopyString(g_DefStringTable.GetString(Def->GetModeDef().Data->NameId));
    CModeKnockdown* mode = new CModeKnockdown(name, Attacker.GetPItem(), Owner.GetPItem());
    modes->AddMode(CModePointer(mode));
    modes->RemoveMode(0xd);
}
