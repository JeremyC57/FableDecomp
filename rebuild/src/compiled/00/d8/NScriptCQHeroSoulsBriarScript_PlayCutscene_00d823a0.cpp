// NScript::CQ_HeroSoulsBriarScript::PlayCutscene(CCharString name) -- play a Briar Rose cut-scene: take the hero under scripted control, cast him as HERO and the quest's Briar Rose handle as ROSE, then run the named cut-scene inside a paused movie sequence with the movie camera fixed, and clear the cut-scene arguments
// Leaf: ?PlayCutscene@CQ_HeroSoulsBriarScript@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D823A0, 667 bytes (`ret 4`; the by-value name is destroyed by the callee).
//   Catalog label CWomanToAttract::DoLeavingCutscene is propagated and wrong: the callers are CQ_HeroSoulsBriarScript::Main
//   0x00D80E40 and CBriarRose::Main 0x00D82640, `this` holds the CGameScriptInterface at +0x40 (a quest script, not an
//   entity script), and FableWin.pdb names ?PlayCutscene@CQ_HeroSoulsBriarScript@NScript@@QAEXVCCharString@@@Z.
// Callees: CBaseObject ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString 0x0099EBF0 / 0x0099EAE0, CRT
//   malloc 0x00BFEA0E / free 0x00BFEA14 (cast-map header), operator delete 0x00BFE9BC, cast map operator[] 0x00CD3D2E and
//   _M_erase 0x00CD344A, argument map clear 0x009AACE0, movie PImp dtor 0x006E7AB0, cut-scene runner 0x00CBFB7D (__fastcall);
//   CGameScriptInterface slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera,
//   379 PauseAllNonScriptedEntities. Literals "HERO" 0x01255174, "ROSE" 0x012CC604, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte STLport maps): Game +0x40, PlayedBriarCS +0x48, seh_BriarRose +0x4c (PDB +0x60),
//   csargs +0x5c (PDB +0x70).
// Shape: the landed CQ_WaspBossScript::WaspIntro 0x00E12F20 model (local cast map with malloc'd header, implicit-literal
//   movie name, one named copy of Game for the pause pair); here the cast map's destructor is inline (free of the header).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
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
    __forceinline ~CCutsceneCastMap()                   // inline here (out of line elsewhere: 0x00CDBFB0)
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
struct CCutsceneArgMap {                                // map<CCharString, CCharString> (retail STLport, 12 bytes)
    void* _body[3];
    void clear();                                       // 0x009AACE0
};
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

class CQ_HeroSoulsBriarScript : public CScriptBase { public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool                  PlayedBriarCS;                // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;   // +0x4c
    CCutsceneArgMap       csargs;                       // +0x5c
    void PlayCutscene(CCharString name);
};

void CQ_HeroSoulsBriarScript::PlayCutscene(CCharString name)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    cast["ROSE"] = seh_BriarRose;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
    csargs.clear();
    game->PauseAllNonScriptedEntities(false);
}
}