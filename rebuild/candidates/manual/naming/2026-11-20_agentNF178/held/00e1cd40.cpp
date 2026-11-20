// CQ_WizardBattleScript::CutsceneScreamers() -- takes the hero under scripted control, casts him as "Hero" and plays CS_HOOK_COAST_SCREAMERS inside a paused movie sequence; the first time through it also kicks off the Q_WizardBattle quest start screen
// Leaf: CutsceneScreamers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1CD40, 653 bytes, plain `ret`.
// Callees: CBaseObject ctor / operator= / dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CRT malloc 0x00BFEA0E / free 0x00BFEA14, operator delete 0x00BFE9BC, the cast map's operator[] 0x00CD3D2E, _M_erase
//   0x00CD344A and destructor 0x00CDBFB0 (out of line on the terminating exit), the handle destructor 0x007E74D0 (same), the
//   movie PImp destructor 0x006E7AB0, the __fastcall cut-scene runner 0x00CBFB7D (as landed in 0x00DC21F0),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the quest's interface (+0x40): StartScriptingEntity (8),
//   GetHero (70), KickOffQuestStartScreen (309), StartMovieSequence (370), PauseAllNonScriptedEntities (379).
// Layout (retail = Ego_r PDB -0x14): QuestStartScreenShown +0x4e. Cast map = local STLport map (12 bytes, 0x24-byte nodes).
// Shape: the PlayCutscene recipe (landed 0x00E2A900) as straight-line code. Both resource handles keep IMPLICIT destructors
//   (retail resets no vptr of its own: 0x007E74D0 stores only 0x0126008C). VC7.1 then inlines the hero handle's at the normal
//   exit and calls it on the terminating return, exactly as retail. The map's destructor is plain inline (a heuristic decision
//   that matches retail's split). The movie handle sits in a holder with a __forceinline destructor, so it is inlined on both
//   exits. Relocation targets were checked symbol by symbol (probe/rpair.py): the ctor stores 0x0127094C / 0x01260EF4.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

#include "engine/CCountedPointer.h"


class CScriptThing;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual FABLE_FORCEINLINE ~CScriptGameResourceObjectBase() {}
};

struct CScriptThingImp;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;              // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
    // implicit destructor (no vptr store of its own); out of line 0x007E74D0
};

struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x01260EF4
    CMovieImpPointer PImp;                              // +0x08
    CScriptGameResourceObjectMovieBase() {}
    // implicit destructor
};

struct CHeldMovie {                                     // holder: its forceinline dtor expands the movie's implicit dtor at every exit
    CScriptGameResourceObjectMovieBase Obj;
    FABLE_FORCEINLINE CHeldMovie() {}
    FABLE_FORCEINLINE ~CHeldMovie() {}
};


// Cast map: the shared STLport 4.6 map (stlport/fable_map.h, retail branch), Ego_r.pdb spelling map<CCharString, CScriptGameResourceObjectScriptedThingBase>:
//   ctor expanded here, operator[] 0x00CD3D2E declared only, the implicit ~map 0x00CDBFB0 called out of line (as 0x00E137B0).
#define FABLE_RETAIL_STLP_MAP
// _M_erase (0x00CD344A) is called out of line; its COMDAT is that row's (declaration only here, NF178b).
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"
void __fastcall RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast, std::map<CCharString, bool>* flags,
                                 std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_WizardBattleScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x6];                  // VillagersTurnOnHero .. MissionOver
    bool                  QuestStartScreenShown;        // +0x4e (retail = PDB - 0x14)
    void CutsceneScreamers();
};

void CQ_WizardBattleScript::CutsceneScreamers()
{
    CScriptGameResourceObjectScriptedThingBase hero;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["Hero"] = hero;
    CHeldMovie movie;
    {
        CCharString sequence("");
        GSI->StartMovieSequence(sequence, movie.Obj);
    }
    CGameScriptInterface* game = GSI;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_HOOK_COAST_SCREAMERS", cast, 0, 0, false, true);
    if (QuestStartScreenShown == false) {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        GSI->KickOffQuestStartScreen("Q_WizardBattle", true, false);
        QuestStartScreenShown = true;
    }
    game->PauseAllNonScriptedEntities(false);
}
}