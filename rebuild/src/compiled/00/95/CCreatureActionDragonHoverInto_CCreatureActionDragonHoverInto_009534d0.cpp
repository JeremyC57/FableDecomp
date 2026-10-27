// CCreatureAction_DragonHoverInto::CCreatureAction_DragonHoverInto(CThingCreatureBase& owner) -- the combat-action base for owner with action code 250 and the owner's "SWORD_SWING" sound pair, then this class's vtable
// Leaf: ??0CCreatureAction_DragonHoverInto@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009534D0, 82 bytes, thiscall, `ret 4`, returns this. Vtable 0x01293BC4.
// Identity (proven): it stores vtable 0x01293BC4 whose RTTI is .?AVCCreatureAction_DragonHoverInto@@; FableWin.pdb ??0CCreatureAction_DragonHoverInto@@QAE@AAVCThingCreatureBase@@@Z
//   (donor 0x02BE0570); only caller CAIStateGroup_JackDragon::IntoHover 0x00951D60.
// Callees: CCharString(const char*, long) 0x0099EBF0, CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const
//   CCharString&) 0x008561A0 (protected static `__fastcall`, Ego_r.pdb ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@...),
//   CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030, ~CCharString 0x0099EAE0.
//   Literal 0x0127AC80 "SWORD_SWING".
// Shape (family K041, representative 0x008C38B0): the landed H030 combat-action constructors (e.g. 0x008CA5A0) with the
//   owner-only base constructor: the literal converts to a CCharString temporary first, the CSoundPair comes back through
//   the hidden pointer, the string dies after the base call, then the vtable store.
class CThing { };
class CThingCreatureBase : public CThing { };
class CCharString { public:                              // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CSoundPair { public:                               // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& owner, const CCharString& name);   // 0x008561A0
};
class CCreatureAction_DragonHoverInto : public CCombatActionBase {
public:
    CCreatureAction_DragonHoverInto(CThingCreatureBase& owner);
    virtual ~CCreatureAction_DragonHoverInto();
};
CCreatureAction_DragonHoverInto::CCreatureAction_DragonHoverInto(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING"))
{
}