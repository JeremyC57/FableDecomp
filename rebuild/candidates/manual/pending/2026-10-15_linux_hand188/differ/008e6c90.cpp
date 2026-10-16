// CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase*, long) -- untargeted form: base CCombatActionBase(owner, 150, zeroed sound), SpellLevel, then the animation named by SPECIAL_ABILITIES_MULTI_STRIKE_DEF's Animations[SpellLevel - 1]
// Grade: DIFFER(217v210) -- retail caches the def object in edi after GetDef and releases it with `dec [edi+4] ; jne` (no re-read); ours re-reads the def pointer slot for the index and the release (and swaps the name/def frame slots) | tried: function-scope CDefPointer + block-scoped name + temporary GetString argument | next: lane gap: CDefPointer kept in a register across calls (GOTCHAS wave 25 chunk 163 / retry 5)
// Leaf: ??0CCreatureAction_CastStrengthMultiStrikeProper@@QAE@PAUCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6C90, length 210 bytes, `ret 8`. Vtable 0x012877A4.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CCharString ctor
// 0x0099EBF0 / dtor 0x0099EAE0 ("SPECIAL_ABILITIES_MULTI_STRIKE_DEF" @ 0x01235A9C), CGameDefinitionManager::Get() 0x0044C6B0 (twice),
// GetDefIndexFromName 0x009AD390, GetDef<CSpecialAbilitiesMultiStrikeDef> 0x007CD760, CDefStringTable::GetString 0x009D49B0 (global
// table @ 0x013CA828), SetAnimationName 0x008574D0; the def's virtual slot 1 on the last release.
// Shape: as CCreatureAction_CastStrengthMultiStrikeProper::Initialise 0x008E6690 (named def-name block, function-scope CDefPointer);
// the GetString result is a temporary argument of SetAnimationName; Animations[SpellLevel - 1] reads `[def+level*4+4Ch]`.
// Layout: SpellLevel at retail +0x114 (PDB +0x12c); Animations (CDefString[4]) at def +0x50 (Ego_r.pdb).
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CDefString { long Index; };

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CSpecialAbilitiesMultiStrikeDef {
    virtual void v00();
    virtual void Release();                     // slot 1 (+4): destroys the def when the last reference goes
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CDefString    Sounds[4];                    // +0x40
    CDefString    Animations[4];                // +0x50
};

struct CDefPointerMultiStrike {                 // CDefPointer<const CSpecialAbilitiesMultiStrikeDef>
    CSpecialAbilitiesMultiStrikeDef* Object;
    CDefPointerMultiStrike() : Object(0) { }
    ~CDefPointerMultiStrike()
    {
        if (Object && --Object->RefCount == 0)
            Object->Release();
    }
    CSpecialAbilitiesMultiStrikeDef* operator->() const { return Object; }
};

struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
    long GetDefIndexFromName(const CCharString& name, long column);       // 0x009AD390
    bool GetDef(long index, CDefPointerMultiStrike* out);                 // 0x007CD760
};

struct CThingCreatureBase;
struct CThing;
struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CastStrengthMultiStrikeProper : CCombatActionBase {   // vtable 0x012877A4
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, CThing* target, long spellLevel);
    CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, long spellLevel);
};

CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper(CThingCreatureBase* owner, long spellLevel)
    : CCombatActionBase(owner, 150, CSoundPair())
    , SpellLevel(spellLevel)
{
    CDefPointerMultiStrike def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_STRIKE_DEF");
        index = CGameDefinitionManager::Get().GetDefIndexFromName(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    SetAnimationName(g_DefStringTable.GetString(def->Animations[SpellLevel - 1]));
}
