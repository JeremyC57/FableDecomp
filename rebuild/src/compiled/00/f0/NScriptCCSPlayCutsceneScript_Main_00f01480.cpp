// NScript::CCS_PlayCutsceneScript::Main() -- console cut-scene player: each script frame, wait for the console's play-cut-scene request; then take the hero under scripted control (priority 4), cast him as "Hero" and "HERO", run the cut-scene named by the console inside a paused movie sequence, release the hero and clear the request
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F01480, 734 bytes (736 with padding); PDB ?Main@CCS_PlayCutsceneScript@NScript@@UAEXXZ.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, malloc 0x00BFEA0E / free 0x00BFEA14 (the cast map's head node, inline
//   STLport ctor/dtor as the landed CRetailTree 0x00CDBF70 / 0x00CDBFB0 bodies), the map's recursive erase 0x00CD344A and
//   operator[] 0x00CD3D2E, CBaseObject ctor 0x0099A380 / dtor 0x0099A430 / operator= 0x0099A3B0, scripted-thing handle (vtable
//   0x0127094C) Release 0x00CD2770, movie handle (vtable 0x01260EF4) PImp dtor 0x006E7AB0, CCharString(const char*, long)
//   0x0099EBF0 / dtor 0x0099EAE0, operator delete 0x00BFE9BC, RunCutsceneMacro 0x00CBFB7D (__fastcall; PDB
//   ?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@...@PAV?$map@VCCharString@@_N...@PAV?$map@VCCharString@@V1@...@_N4@Z);
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity (result dropped), 70 GetHero, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities. Globals: the console's cut-scene name (CCharString) 0x0143E91C (Ego_r.pdb dynamic
//   initializer 'GlobalConsoleCutsceneName') and its play request (bool) 0x0143E8FC (descriptive name).
//   Literals "Hero" 0x012C4570, "HERO" 0x01255174, "" 0x0122D70E.
// Layout: CQuestScriptBase as the landed quest scripts (retail CScriptBase 0x40 bytes, Game +0x40); CCS_PlayCutsceneScript adds no
//   member used here. The cast map is the retail 12-byte STLport tree {head, size, compare}, node 0x24 bytes.
// Shape: the landed CQS_ScytheInfoScript::PlayCutscene 0x00E2A900 model; the counted pointer's operator= reads the source's
//   Info before its Data (the other order swaps two loads in the second assignment).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
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
class CScriptThing;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CScriptThingImp;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;
    void Release();                                    // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                               // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (retail STLport, 12 bytes)
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
    __forceinline ~CCutsceneCastMap()
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
    void Erase(node* root);                            // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
extern CCharString GlobalConsoleCutsceneName;          // 0x0143E91C
extern bool GlobalConsolePlayCutscene;                 // 0x0143E8FC
class CScriptBase { public:                                   // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                        // +0x40
    unsigned char _pad44[4];
};
namespace NScript {
class CCS_PlayCutsceneScript : public CQuestScriptBase { public:
    void Main();
};

void CCS_PlayCutsceneScript::Main()
{
    while (!IsActiveThreadTerminating()) {
        while (!GlobalConsolePlayCutscene) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCutsceneCastMap cast;
            CScriptGameResourceObjectScriptedThingBase hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
            cast["Hero"] = hero;
            cast["HERO"] = hero;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro(GlobalConsoleCutsceneName, cast, 0, 0, true, true);
                game->PauseAllNonScriptedEntities(false);
            }
            hero.Release();
            GlobalConsolePlayCutscene = false;
        }
        Game->NewScriptFrame();
    }
}
}