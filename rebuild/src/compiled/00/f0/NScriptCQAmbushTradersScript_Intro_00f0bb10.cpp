// CQ_AmbushTradersScript::Intro() -- the Gibbet Hill ambush intro: take the four bandit allies and the hero under scripted control, cast them as BAN1..BAN4 and HERO and play CS_GIBBETHILL_AMBUSH_INTRO (with the quest's cut-scene flag map) in a paused movie sequence with the camera fixed, release all five and set BanditIntroDone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0BB10, length 1579 bytes.
// Callees: CBaseObject ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CRT malloc 0x00BFEA0E / free 0x00BFEA14 (cast-map header), operator delete 0x00BFE9BC, cast map
//   operator[] 0x00CD3D2E and _M_erase 0x00CD344A, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770, movie
//   PImp dtor 0x006E7AB0, cut-scene runner 0x00CBFB7D (__fastcall); the game script interface (+0x40) slots 8
//   StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities.
// Strings: "HERO" 0x01255174, "BAN1" 0x012D1F04, "BAN2" 0x012D1EFC, "BAN3" 0x012D1EF4, "BAN4" 0x012D241C,
//   "CS_GIBBETHILL_AMBUSH_INTRO" 0x012F828C, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14, -4 per preceding 12-byte STLport vector): BanditIntroDone +0x4d, AllBandits +0xa8,
//   CSFlagMap +0x1b8.
// Shape: cast/movie model of the landed CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0 (local cast map with malloc'd
//   header and inline destructor, implicit-literal movie name, one named copy of Game for the pause pair); here every
//   handle destructor is inline.
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
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    virtual ~CScriptThing() {}                          // out of line: 0x004AA840
};
template <class T> struct vector {                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};

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
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
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
struct CCutsceneFlagMap {                               // map<CCharString, bool> (retail STLport, 12 bytes)
    void* _body[3];
};
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x4d - 0x48];
    bool                  BanditIntroDone;              // +0x4d
    unsigned char         _pad4e[0xa8 - 0x4e];
    vector<CScriptThing>  AllBandits;                   // +0xa8
    unsigned char         _padb4[0x1b8 - 0xb4];
    CCutsceneFlagMap      CSFlagMap;                    // +0x1b8
    void Intro();
};

void CQ_AmbushTradersScript::Intro()
{
    CScriptGameResourceObjectScriptedThingBase bandit1;
    CScriptGameResourceObjectScriptedThingBase bandit2;
    CScriptGameResourceObjectScriptedThingBase bandit3;
    CScriptGameResourceObjectScriptedThingBase bandit4;
    Game->StartScriptingEntity(AllBandits[0], bandit1, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[1], bandit2, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[2], bandit3, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[3], bandit4, (EScriptAIPriority)4);
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    {
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        cast["BAN1"] = bandit1;
        cast["BAN2"] = bandit2;
        cast["BAN3"] = bandit3;
        cast["BAN4"] = bandit4;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_INTRO", cast, &CSFlagMap, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
    if (bandit1.IsValid())
        bandit1.Release();
    if (bandit2.IsValid())
        bandit2.Release();
    if (bandit3.IsValid())
        bandit3.Release();
    if (bandit4.IsValid())
        bandit4.Release();
    if (hero.IsValid())
        hero.Release();
    BanditIntroDone = true;
}
}