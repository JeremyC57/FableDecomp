// CQ_OrchardFarmRaidScript::DoMultiplierCutscene() -- takes the hero into a paused movie sequence and plays the Whisper intro cut-scene that fits where he is (Greatwood Lake, the GWL marker or the lake path) and which side he took, then shows the combat-multiplier tutorial once
// Leaf: DoMultiplierCutscene
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD1AF0, 959 bytes, plain `ret`.
// Callees: as CutsceneScreamers 0x00E1CD40 (handle / cast map / movie / RunCutsceneMacro 0x00CBFB7D) plus
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), ~CScriptThing 0x004AA840, the movie handle's implicit destructor
//   0x006E7B80 (out of line on seven exits); interface slots 7, 8, 12 IsRegionLoaded, 41 MsgIsTutorialClickedPast, 70,
//   72 GetThingWithScriptName, 118 DisplayTutorial, 370, 371 FixMovieSequenceCamera, 379.
// Layout (retail = Ego_r PDB -0x18): HeroTeam +0x78, ShownCombatMultiplierTutorial +0x89.
// Shape: one RunCutsceneMacro call per branch (VC7.1 tail-merges them into one string ctor + call); every exit unpauses
//   explicitly. The handle and map destructors are out of line everywhere, and the movie's implicit one is inline except
//   on the seven returns wrapped in `#pragma inline_depth(0)`. Relocation targets checked (probe/rpair.py).
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    virtual ~CScriptThing();                            // 0x004AA840
};

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
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    void _M_erase(_Rb_tree_node_base* root);            // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETutorialCategory { TUTORIAL_CATEGORY_COMBAT_MULTIPLIER = 9 };
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

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

class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    unsigned char         _pad44[0x34];
    long                  HeroTeam;                     // +0x78 (retail = PDB - 0x18)
    unsigned char         _pad7c[0xd];
    bool                  ShownCombatMultiplierTutorial;   // +0x89
    void DoMultiplierCutscene();
};

void CQ_OrchardFarmRaidScript::DoMultiplierCutscene()
{
    CScriptGameResourceObjectScriptedThingBase hero;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    {
        CCharString sequence("");
        GSI->StartMovieSequence(sequence, movie);
    }
    CGameScriptInterface* game = GSI;
    game->PauseAllNonScriptedEntities(true);
    GSI->FixMovieSequenceCamera(true);
    if (GSI->IsRegionLoaded("GreatwoodLake") && HeroTeam == 0) {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        RunCutsceneMacro("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", cast, 0, 0, false, true);
    } else {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        if (IsDistanceBetweenThingsUnder(GSI->GetThingWithScriptName("MK_OFI_GWL"), GSI->GetHero(), 20.0f)) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return;
            }
            if (HeroTeam == 1) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", cast, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_ORCHARD_GOOD_WHISPERINTRO_GWL", cast, 0, 0, false, true);
            }
        } else {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return;
            }
            if (HeroTeam == 1) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_ORCHARD_EVIL_WHISPERINTRO_LOP", cast, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_ORCHARD_GOOD_WHISPERINTRO_LOP", cast, 0, 0, false, true);
            }
        }
    }
    GSI->FixMovieSequenceCamera(false);
    if (ShownCombatMultiplierTutorial == false) {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        if (GSI->DisplayTutorial(TUTORIAL_CATEGORY_COMBAT_MULTIPLIER)) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            while (!GSI->MsgIsTutorialClickedPast()) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        }
        ShownCombatMultiplierTutorial = true;
    }
    game->PauseAllNonScriptedEntities(false);
}
}