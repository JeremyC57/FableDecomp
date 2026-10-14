// CCreatureAction_BriarRoseConcentrateOutof::CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase*, CThingCreatureBase*) -- base CCreatureAction_CastWillDrainLifeSpell(owner, target, 0, global def index of "BRIAR_ROSE_DRAIN_LIFE_DEF"), own vtable (0x01280BB4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2CB0, length 79 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (implicit conversion of the literal @ 0x01280CF8),
// CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall singleton accessor), CDefinitionManager::GetDefGlobalIndexFromName
// (const CCharString&) @ 0x009AD410, CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell
// (CThingCreatureBase*, CThingCreatureBase*, long, long) @ 0x008BBBA0, CCharString::~CCharString @ 0x0099EAE0
// Shape: the def index is computed inside the base initialiser: the conversion temporary (re-addressed with `lea`
// for the lookup and the destructor: an implicit CCharString(const char*, long = -1), GOTCHAS wave 11) lives in the
// one-dword frame, the index rides in eax into the base call, the temporary is destroyed at the end of the
// initialiser and only then the vtable is stored. `ret 8`: two arguments.
struct CThingCreatureBase;

struct CCharString {                        // retail CCharString is one pointer (fable_string.h); declared locally for the
    char* m_pData;                          //   defaulted length of the conversion constructor
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefinitionManager_Calls {         // engine/CDefinitionManager.h exists but is not used: only this call is modelled
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};

struct CGameDefinitionManager : CDefinitionManager_Calls {
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

struct CCreatureAction_CastWillDrainLifeSpell {                 // retail sizeof 0x11c
    virtual void v00();
    unsigned char _body[0x11c - 4];
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThingCreatureBase* target, long level, long defIndex);   // 0x008BBBA0
};

struct CCreatureAction_BriarRoseConcentrateOutof : CCreatureAction_CastWillDrainLifeSpell {   // vtable 0x01280BB4; adds no data
    virtual void v00();
    CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThingCreatureBase* target);
};

CCreatureAction_BriarRoseConcentrateOutof::CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThingCreatureBase* target)
    : CCreatureAction_CastWillDrainLifeSpell(owner, target, 0, CGameDefinitionManager::Get().GetDefGlobalIndexFromName("BRIAR_ROSE_DRAIN_LIFE_DEF"))
{
}
