// CV_TravellingHeroesScript::PlayCutscene(CCharString name) -- take the hero under scripted control, cast him with the rival, girl and boy handles, run the named cut-scene inside a paused movie sequence with the fixed camera, then clear the cut-scene arguments
// Leaf: ?PlayCutscene@CV_TravellingHeroesScript@NScript@@QAEXVCCharString@@@Z
// Label: (catalog says CV_ExposeMayorScript, propagated:
//   ExposeMayor's PlayCutscene takes (CCharString, bool, bool); this one is `ret 4`, its cast keys are HERO/RIVAL/GIRL/BOY and
//   it reads handles +0x48/+0x58/+0x68 and the argument map +0x80 = Ego_r CV_TravellingHeroesScript seh_Rival / seh_Girl /
//   seh_Boy / csargs at PDB 92/108/124/148.)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE2DB0, 0x384 bytes, `ret 4` (the by-value name is destroyed here).
// Callees: CBaseObject ctor / operator= / dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   malloc 0x00BFEA0E / free 0x00BFEA14, operator delete 0x00BFE9BC, the cast map's operator[] 0x00CD3D2E / _M_erase 0x00CD344A,
//   the argument map's clear 0x009AACE0, the movie PImp dtor 0x006E7AB0, the __fastcall cut-scene runner 0x00CBFB7D; Game
//   slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities.
// Shape: the landed CQ_WizardBattleScript::CutsceneScreamers 0x00E1CD40 with a __forceinline handle operator= (the implicit one
//   is emitted out of line once four assignments share the function).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, seh_Rival +0x48, seh_Girl +0x58, seh_Boy +0x68, csargs +0x80.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
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
struct CCutsceneArgMap {                                // map<CCharString, CCharString> (12 bytes)
    void* _body[3];
    void clear();                                       // 0x009AACE0
};
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
};
namespace NScript {
class CV_TravellingHeroesScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48 (PDB 92)
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58 (PDB 108)
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68 (PDB 124)
    bool ThunderKidsTalking;                            // +0x78
    bool ThunderSceneStarted;                           // +0x79
    bool ThunderSceneDone;                              // +0x7a
    bool MazeCutsceneDone;                              // +0x7b
    bool WhisperCutscenePlayed;                         // +0x7c
    bool SnowTrollSceneDone;                            // +0x7d
    CCutsceneArgMap csargs;                             // +0x80 (PDB 148)
    void PlayCutscene(CCharString name);
};
} // namespace NScript
using NScript::CV_TravellingHeroesScript;

void NScript::CV_TravellingHeroesScript::PlayCutscene(CCharString name)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    cast["RIVAL"] = seh_Rival;
    cast["GIRL"] = seh_Girl;
    cast["BOY"] = seh_Boy;
    CHeldMovie movie;
    {
        CCharString sequence("");
        Game->StartMovieSequence(sequence, movie.Obj);
    }
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
    csargs.clear();
    game->PauseAllNonScriptedEntities(false);
}