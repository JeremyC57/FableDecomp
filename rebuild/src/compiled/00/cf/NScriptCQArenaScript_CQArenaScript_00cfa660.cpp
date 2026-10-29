// CQ_ArenaScript::CQ_ArenaScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, interface/owner pair, vtable 0x012C7798, twenty empty crowd-loop tags, an empty Rounds vector, two empty CScriptThing members and a registered crowd timer
// Leaf: ??0CQ_ArenaScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA660, length 149 bytes, `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (20 times, inline counted
// loop), CBaseClass::CBaseClass @ 0x0099A2D0 (the CScriptThing root, twice; CScriptThing vtable 0x01238C8C), and
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Layout: Ego_r.pdb CQ_ArenaScript - 0x14, and a further -4 after the Rounds vector (retail STLport vectors are 12
// bytes): CrowdLoopTags (CCharString[4][5]) +0x48, Rounds +0x98, CellsVillage +0xc0, GlobalCrowdTimer +0x100,
// Whisper +0x108.
// Shape: the landed quest-script constructor family (0x00E68F70 novtable script-object base storing the arguments
// before the vtable; 0x00E46BD0 STLport vector and CScriptThing models, whose zero words share ebx; the timer member
// registers itself in its inline constructor).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                    // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

class CBaseClass {
public:
    CBaseClass();                                         // 0x0099A2D0
    virtual ~CBaseClass();                                // 0x0099A2E0
};

class CScriptThing;
class CCPPointerInfo;

struct CCountedPointer_CScriptThing {                // CScriptThing::PImp
    CScriptThing* Data;                              // +0
    CCPPointerInfo* Info;                            // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass { public:                        // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;               // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};

template <class T>
struct allocator {
    allocator() { }
};

template <class T, class A>
struct _STLP_alloc_proxy : A {                       // STLport: the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct CRetailVector : _Vector_base<T> {             // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

class CArenaRoundDef;

namespace NScript {
class CQ_ArenaScript : public CScriptObject {
public:              // vtable 0x012C7798
    CCharString  CrowdLoopTags[4][5];                // +0x48
    CRetailVector<CArenaRoundDef> Rounds;            // +0x98
    long         ArenaState;                         // +0xa4
    long         ArenaRound;                         // +0xa8
    long         ArenaRoundWave;                     // +0xac
    long         GoldMultiplier;                     // +0xb0
    bool         ChamLeaving;                        // +0xb4
    long         GoldTotal;                          // +0xb8
    bool         InHitCutsceneAlready;               // +0xbc
    bool         NeedBertForSpeech;                  // +0xbd
    CScriptThing CellsVillage;                       // +0xc0
    bool         PlayerLeaving;                      // +0xcc
    bool         PlayerWon;                          // +0xcd
    long         TotalCreatures[3];                  // +0xd0
    long         ExtraCreatures;                     // +0xdc
    bool         MissionSucceeded;                   // +0xe0
    bool         MissionFailed;                      // +0xe1
    bool         ArenaSpawnNeeded[16];               // +0xe2
    long         NewCrowdPoints;                     // +0xf4
    long         NewCrowdHappiness;                  // +0xf8
    long         NewCrowdBaseLevel;                  // +0xfc
    CTimer       GlobalCrowdTimer;                   // +0x100
    bool         FinalBattleCS;                      // +0x104
    CScriptThing Whisper;                            // +0x108
    bool         PauseCrowdChecker;                  // +0x114
    bool         WhisperNeededForCutscene;           // +0x115
    long         StopTimeIndex;                      // +0x118
    virtual void v00();
    CQ_ArenaScript(void* owner, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_ArenaScript;

NScript::CQ_ArenaScript::CQ_ArenaScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}