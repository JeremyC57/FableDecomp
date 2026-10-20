// CQ_MinionCampScript::CQ_MinionCampScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012D6C60, ChantTimer and MinionSpawnTimer, the two hero-attacker script things, the empty Left/RightSpawnPoints lists and SpawnPointToSpawn
// Leaf: ??0CQ_MinionCampScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9E210, 197 bytes (padded to 208), `ret 8`.
// Identity: the manifest label "CV_FisticuffsClubScript" is propagated and wrong. The factory 0x00DA2F40 (`push 0ACh`,
//   this ctor) is registered at 0x00CD6D27 under "S_QMC"; size 0xac = Ego_r.pdb CQ_MinionCampScript 208 - 0x14 (script
//   base) - 2*8 (the two std::list members are a single 4-byte sentinel pointer in retail, 12 in the PDB).
// Callees: CScriptBase::CScriptBase 0x00CB8110, CGameScriptInterface slot 87 RegisterTimer (+0x15c, global interface
//   0x0143E8F8), CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base, vtable 0x01238C8C), CRT malloc 0x00BFEA0E (the
//   lists' 0x14-byte sentinel nodes: next, prev, CScriptThing).
// Shape: argument words before the derived vtable = inlined `__declspec(novtable)` intermediate base; each VC6-era list
//   ctor is `head = 0; node = malloc(0x14); node->next = node->prev = node; head = node;` (the shared zero register
//   stores the null head before the call; both mallocs' arguments are popped by one `add esp,8`).
// Layout (retail = PDB - 0x14): flags +0x48..+0x4d, NumberOfMinions +0x50, MaxNumberOfMinions +0x54, Min/Max spawn
//   periods +0x58/+0x5c, ChantTimer +0x60, ChantTimerBar +0x64, ChantTimerPaused +0x68, PausedTime +0x6c,
//   MinionSpawnTimer +0x70, CurrentStage +0x74, HeroAttackerOne +0x78, HeroAttackerTwo +0x84, LeftSpawnPoints +0x90,
//   RightSpawnPoints +0x94, ShouldSpawn +0x98, SpawnPointToSpawn +0x9c, BriarState +0xa8.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C (only slot 87 matters here)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                  // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };   // 0x0099A2D0 (vtable 0x01231710)
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass {                 // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;            // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
struct CRetailList_CScriptThing {                  // retail std::list<CScriptThing>: one sentinel-node pointer
    struct node { node* next; node* prev; CScriptThing value; };
    node* head;
    CRetailList_CScriptThing() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
struct CQ_MinionCampScript : CQuestScriptObject {
    bool QuestStartScreenShown, IntroDone, BriarRoseAtAltar, MinionsSummoned, PortalSealed, MissionFailed;   // +0x48
    long NumberOfMinions;                          // +0x50
    long MaxNumberOfMinions;                       // +0x54
    float MinMinionSpawnPeriod;                    // +0x58
    float MaxMinionSpawnPeriod;                    // +0x5c
    CTimer ChantTimer;                             // +0x60
    long ChantTimerBar;                            // +0x64
    bool ChantTimerPaused;                         // +0x68
    long PausedTime;                               // +0x6c
    CTimer MinionSpawnTimer;                       // +0x70
    long CurrentStage;                             // +0x74 (MissionStage)
    CScriptThing HeroAttackerOne;                  // +0x78
    CScriptThing HeroAttackerTwo;                  // +0x84
    CRetailList_CScriptThing LeftSpawnPoints;      // +0x90
    CRetailList_CScriptThing RightSpawnPoints;     // +0x94
    bool ShouldSpawn;                              // +0x98
    CScriptThing SpawnPointToSpawn;                // +0x9c
    long BriarState;                               // +0xa8 (BriarRoseState)
    virtual ~CQ_MinionCampScript();
    CQ_MinionCampScript(void* data, CGameScriptInterface* game);
};
CQ_MinionCampScript::CQ_MinionCampScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
