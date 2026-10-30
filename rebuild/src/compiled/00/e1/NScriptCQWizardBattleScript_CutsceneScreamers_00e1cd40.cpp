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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    long RefCount;                                      // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

class CScriptThing;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual __forceinline ~CScriptGameResourceObjectBase() {}
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
    __forceinline CHeldMovie() {}
    __forceinline ~CHeldMovie() {}
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    __forceinline _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(0x24); }
};

struct CCutsceneCastMap : _Rb_tree_base {               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
    __forceinline CCutsceneCastMap() : _M_node_count(0)
    {
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CCutsceneCastMap()                   // out of line: 0x00CDBFB0
    {
        if (_M_node_count) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
    void _M_erase(_Rb_tree_node_base* root);            // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
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
    CCutsceneCastMap cast;
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