// CSummoner2::Main() -- Hero Souls arena summoner script: wait (one script frame at a time) until the hero hits this summoner (other than with special ability 14), then flag PlayerHitSummoner2 on the arena quest script
// Leaf: Main
// Label: catalogued as a second CSummoner1::Main; the body stores PlayerHitSummoner2 and is referenced from the CSummoner2
// vtable (0x012D3ED0), so it is CSummoner2::Main (Ego_r.pdb CSummoner2, same 28-byte layout).
// Retail 0x00D7EED0, 336 bytes, `ret`. Vtable reference 0x012D3ED0.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the filter registration 0x00F35B10,
// CScriptThing::~CScriptThing 0x004AA840, IsDone 0x00F35B30, CGameScriptInterface slot 7 NewScriptFrame (+0x1c),
// CCharString::CCharString(const char*, long) 0x0099EBF0 ("SCRIPT_NAME_HERO" 0x0125D1C8), ~CCharString 0x0099EAE0,
// CScriptThing slots 21 MsgIsHitBy (+0x54), 41 MsgIsHitBySpecialAbilityFrom (+0xa4), 42 MsgIsHitByAnySpecialAbilityFrom (+0xa8).
// Layout = Ego_r.pdb CSummoner2 (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18; CQ_HeroSoulsArenaScript::PlayerHitSummoner2 retail +0x4E (PDB 0x62 - 0x14, as the landed arena rows).
// Shape: as the landed CHeroSoulsEnemy::Main 0x00D7F110 (scoped CIsThingAlive filter, NewScriptFrame, IsDone); the wait
// is a `while (!(hit))` loop whose condition builds three conditional CCharString temporaries (ebx construction flags,
// the condition parked in a frame byte); after the loop an IsDone test guards the quest flag store.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                   // slot 21 (+0x54)
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;              // slot 42 (+0xa8)
    unsigned char _body[8];
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};
struct CIsThingAlive {                                  // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CQ_HeroSoulsArenaScript {
    unsigned char _pad00[0x4d];
    bool          PlayerHitSummoner1;                   // +0x4d
    bool          PlayerHitSummoner2;                   // +0x4e
};
struct CQ_SunnyvaleMasterData;

class CSummoner2 {
public:
    void*                    __vftable;                 // +0x00
    CGameScriptInterface*    Game;                      // +0x04
    CScriptThing             Me;                        // +0x08
    CQ_HeroSoulsArenaScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    void RegisterFilter(CIsThingAlive& filter);         // 0x00F35B10
    bool IsDone();                                      // 0x00F35B30
    void Main();
};

void CSummoner2::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!(Me.MsgIsHitBy("SCRIPT_NAME_HERO")
             || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    ParentClass->PlayerHitSummoner2 = true;
}