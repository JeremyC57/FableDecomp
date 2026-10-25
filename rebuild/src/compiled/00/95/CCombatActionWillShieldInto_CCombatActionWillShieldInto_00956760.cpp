// CCombatAction_WillShieldInto::CCombatAction_WillShieldInto(CThingCreatureBase& owner, float shieldUpTime, float shieldHealth) -- base CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")), then the shield up-time and health
// Leaf: ??0CCombatAction_WillShieldInto@@QAE@AAVCThingCreatureBase@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956760, 102 bytes, thiscall `ret 0Ch` (three arguments).
// Identity: the constructor stores vtable 0x0129461C last, which rtti-vtables.tsv gives to CCombatAction_WillShieldInto
//   (slots 0/11/73 = 0x956900 / 0x956990 / 0x9567D0); the Ego_r.pdb name set holds
//   ??0CCombatAction_WillShieldInto@@QAE@AAVCThingCreatureBase@@MM@Z (owner, two floats: `ret 0Ch`), while the
//   WillEnflameInto constructor is (CThingCreatureBase&, long) (`ret 8`, the landed 0x009567F0). Ego_r.pdb members
//   ShieldUpTime +0x12C / ShieldHealth +0x130 sit at +0x114 / +0x118 in retail (CCombatActionBase is 0x114 bytes there).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the "SWORD_SWING" temporary at
//   0x0127AC80), CCombatActionBase::GetSoundPair 0x008561A0 (protected static __fastcall, PDB
//   ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z, hidden result in ecx,
//   owner in edx), CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030.
// Label: landed as CCombatAction_WillEnflameInto::CCombatAction_WillEnflameInto(CThingCreatureBase*, long, long); the
//   catalog already says CCombatAction_WillShieldInto::CCombatAction_WillShieldInto.
class CThingCreatureBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};

class CSoundPair {                                      // user constructor: returned through the hidden pointer
public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase {                               // retail sizeof 0x114
public:
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& owner, const CCharString& name);   // 0x008561A0
    unsigned char _body[0x114 - 4];
};

class CCombatAction_WillShieldInto : public CCombatActionBase {   // vtable 0x0129461C
public:
    CCombatAction_WillShieldInto(CThingCreatureBase& owner, float shieldUpTime, float shieldHealth);
    virtual ~CCombatAction_WillShieldInto();
    float ShieldUpTime;                                 // +0x114
    float ShieldHealth;                                 // +0x118
};

CCombatAction_WillShieldInto::CCombatAction_WillShieldInto(CThingCreatureBase& owner, float shieldUpTime, float shieldHealth)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")),
      ShieldUpTime(shieldUpTime),
      ShieldHealth(shieldHealth)
{
}
