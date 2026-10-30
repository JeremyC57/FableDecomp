// CQ_WaspBossScript::WaspIntro() -- starts the chase, casts the hero and the wasp victim (HERO, VICTIM) in CS_WASPBOSS_INTRO inside a paused movie sequence, then shows the active quest's start screen once
// Leaf: WaspIntro
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

#include "engine/CCountedPointer.h"


struct CScriptThingImp;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;
    // implicit destructor; out of line: 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {                                                   // out of line: 0x008ABD10
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (retail STLport, 12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x24 - 16]; };
    node*         head;
    unsigned long size;
    unsigned char compare;
    __forceinline CCutsceneCastMap()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
    ~CCutsceneCastMap()                                 // out of line: 0x00CDBFB0
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
    void Erase(node* root);                             // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

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

class CQ_WaspBossScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[7];
    bool                  StartChase;                   // +0x4f (PDB +0x63)
    unsigned char         _pad50[4];
    bool                  QuestStartScreened;           // +0x54 (PDB +0x68)
    void WaspIntro();
};

void CQ_WaspBossScript::WaspIntro()
{
    StartChase = true;
    CScriptThing victim = GSI->GetThingWithScriptName("WaspVictim");
    CScriptGameResourceObjectScriptedThingBase hero;
    CScriptGameResourceObjectScriptedThingBase victimHandle;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    GSI->StartScriptingEntity(victim, victimHandle, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    cast["VICTIM"] = victimHandle;
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    CGameScriptInterface* game = GSI;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_WASPBOSS_INTRO", cast, 0, 0, false, true);
    if (!QuestStartScreened) {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        GSI->KickOffQuestStartScreen(GSI->GetActiveQuestName(), true, false);
        QuestStartScreened = true;
    }
    game->PauseAllNonScriptedEntities(false);
}
}