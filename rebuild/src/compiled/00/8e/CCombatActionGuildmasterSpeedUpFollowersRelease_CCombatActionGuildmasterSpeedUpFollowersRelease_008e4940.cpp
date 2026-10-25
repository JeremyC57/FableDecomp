// CCombatAction_Guildmaster_SpeedUpFollowersRelease::CCombatAction_Guildmaster_SpeedUpFollowersRelease(CThingCreatureBase& owner) -- the combat base for the owner with value 150 and the owner's "SWORD_SWING" sound pair
// Leaf: ??0CCombatAction_Guildmaster_SpeedUpFollowersRelease@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4940, 82 bytes (+ int3 padding), thiscall, `ret 4`. Vtable 0x01286C4C.
// Identity: CCombatAction_Guildmaster_SpeedUpFollowersRelease::CCombatAction_Guildmaster_SpeedUpFollowersRelease(CThingCreatureBase&) -- proven; evidence: it stores vtable 0x01286C4C whose RTTI is .?AVCCombatAction_Guildmaster_SpeedUpFollowersRelease@@ :
//   CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass; FableWin.pdb name set holds
//   ??0CCombatAction_Guildmaster_SpeedUpFollowersRelease@@QAE@AAVCThingCreatureBase@@@Z; the literal 0x0127AC80 is "SWORD_SWING". Family G046 (8 combat action
//   constructors; the propagated label "CCombatAction_Guildmaster_HealFollowersInto" is right only for 0x009784A0).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCombatActionBase::GetSoundPair(const
//   CThingCreatureBase&, const CCharString&) 0x008561A0 (`__fastcall` protected static of FableWin.pdb, CSoundPair returned
//   through a hidden pointer), CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030
//   (FableWin.pdb), CCharString::~CCharString() 0x0099EAE0.
// Shape: the literal converts to a CCharString temporary first and dies after the base call, before the vtable store; no EH
//   frame (built without /GX).
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
};
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase(CThingCreatureBase& creature, long value, const CSoundPair& sound);   // 0x00858030
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);  // 0x008561A0
};
class CCombatAction_Guildmaster_SpeedUpFollowersRelease : public CCombatActionBase {
public:
    CCombatAction_Guildmaster_SpeedUpFollowersRelease(CThingCreatureBase& owner);
};

CCombatAction_Guildmaster_SpeedUpFollowersRelease::CCombatAction_Guildmaster_SpeedUpFollowersRelease(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING"))
{
}