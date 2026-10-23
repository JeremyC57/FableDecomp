// CRockTrollTrigger::Main() -- the trader-escort earth-troll trigger: register an alive-filter over Me, wait until the hero is within the script def's TE_EarthTrollTriggerDistance, then (once) play the Darkwood trader-troll cut-scene with the hero cast inside a paused movie sequence unless it was already shown, and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0AD40, length 544 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840,
// RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF and
// RunCutsceneMacro 0x00CBFB7D (both __fastcall), scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator=
// 0x008ABD10, CCharString 0x0099EBF0 / 0x0099EAE0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
// movie handle ctor 0x006E7B60 / dtor 0x006E7B80; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity,
// 70 GetHero, 108 RemoveThing, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities. Literals "HERO" 0x01255174,
// "" 0x0122D70E, "CS_DARKWOOD_TRADER_TROLL" 0x012DFD20.
// Layout: Ego_r.pdb CRockTrollTrigger (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
// (CQ_TraderEscortScript), PMasterData +0x18, RockTrollSpawned +0x1c. CQ_TraderEscortScript ShownEarthTroll PDB +0x85 =
// retail +0x71 (quest-script base 0x14 smaller); CScriptDef TE_EarthTrollTriggerDistance PDB +0xECC = retail +0xE1C.
// Shape: the landed CPrisonRaceWarden::Main 0x00DDD030 (handle / map / movie storage views) and CHTE_Prisoner::Main
// 0x00D69F50 (scoped alive-filter).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CScriptThing {                                  // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CScriptGameResourceObjectScriptedThingBase {    // vtable 0x0127094C, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x007E72A0 (out-of-line ctor)
    void Destroy();                                    // 0x007E74D0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CScriptGameResourceObjectMovieBase {            // vtable 0x01260EF4, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x006E7B60
    void Destroy();                                    // 0x006E7B80
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD32(ra) VPAD4(rb) VPAD1(rc)                                                   // slots 71..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD256(sa) VPAD4(sb) VPAD1(sc)                                                  // slots 109..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(ta)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
};
struct CScriptDef {
    unsigned char _pad[0xe1c];
    long TE_EarthTrollTriggerDistance;                 // +0xE1C (PDB +0xECC)
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CQ_TraderEscortScript {
    unsigned char _pad[0x71];
    bool ShownEarthTroll;                              // +0x71 (PDB +0x85)
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CRockTrollTrigger : CEntityScriptBase {
    CQ_TraderEscortScript* ParentClass;                // +0x14
    void*                  PMasterData;                // +0x18
    bool                   RockTrollSpawned;           // +0x1c
    void Main();
};

void CRockTrollTrigger::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    long distance = g_pScriptDef->TE_EarthTrollTriggerDistance;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)distance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!RockTrollSpawned) {
        if (IsActiveThreadTerminating())
            return;
        RockTrollSpawned = true;
        if (!ParentClass->ShownEarthTroll) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectScriptedThingBase hero;
            hero.Construct();
            Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
            CCutsceneCastMap cast;
            cast.Construct();
            cast["HERO"] = hero;
            CScriptGameResourceObjectMovieBase movie;
            movie.Construct();
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_DARKWOOD_TRADER_TROLL", cast, 0, 0, false, true);
            ParentClass->ShownEarthTroll = true;
            game->PauseAllNonScriptedEntities(false);
            movie.Destroy();
            cast.Destroy();
            hero.Destroy();
        }
    }
    Game->RemoveThing(Me, false, true);
}
