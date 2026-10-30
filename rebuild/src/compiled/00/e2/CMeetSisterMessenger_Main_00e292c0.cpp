// CMeetSisterMessenger::Main() -- Arena exit, Theresa's messenger: wait until ArenaExterior is loaded, take the messenger
//   (Me) and then the hero under scripted control (priority 4), cast them as "Messenger" and "Hero", play
//   CS_ARENA_LEAVE_THERESA_01 inside a paused movie sequence and remove the messenger from the world.
// Leaf: ?Main@CMeetSisterMessenger@
// Retail: Fable.exe @ 0x00E292C0, 791 bytes, plain `ret`.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the cast map's head node; inline STLport ctor/dtor at the first wait loop,
//   out-of-line map dtor 0x00CDBFB0 at the later exits), map erase 0x00CD344A, map operator[] 0x00CD3D2E,
//   CCharString 0x0099EBF0 / 0x0099EAE0, CBaseObject ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle (vtable
//   0x0127094C) IsValid 0x00CD23B9 / Release 0x00CD2770 / operator= 0x008ABD10 / dtor 0x007E74D0, movie PImp dtor
//   0x006E7AB0, RunCutsceneMacro 0x00CBFB7D, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30.
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 12 IsRegionLoaded, 70 GetHero, 108 RemoveThing,
//   370 StartMovieSequence, 379 PauseAllNonScriptedEntities.
// Literals: "ArenaExterior" 0x012C6148, "Hero" 0x012C4570, "Messenger" 0x012E22A0, "" 0x0122D70E,
//   "CS_ARENA_LEAVE_THERESA_01" 0x012E2284.
// Layout: CEntityScriptBase as the in-chunk CMK_GHC_HERO3 / CM_RestartMarker models (Game +0x04, Me +0x08, `lea ebx,[esi+8]`);
//   the cast map is the retail 12-byte STLport tree {head, size, compare} with 0x24-byte nodes, as the landed
//   NScript::CCS_PlayCutsceneScript::Main 0x00F01480.
// Shape: a plain `inline` map dtor -- VC expands it at the first early exit and calls the out-of-line copy at the rest
//   (__forceinline expands it everywhere; an out-of-line-only dtor loses the first expansion).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
};
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CHandlePImp {
    void*           Data;
    CCPPointerInfo* Info;
    CHandlePImp& operator=(const CHandlePImp& other);   // 0x008AB1E0
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    __forceinline CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
    void DestroyOutOfLine();                            // the implicit destructor's out-of-line copy 0x006E7B80
};
class CRGBColour { public:
    unsigned char B, G, R, A;
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (STLport, 12 bytes)
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
    inline ~CCutsceneCastMap()
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CMeetSisterMessenger : CEntityScriptBase {
    void Main();
};

void CMeetSisterMessenger::Main()
{
    CCutsceneCastMap cast;
    while (!Game->IsRegionLoaded("ArenaExterior")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase sister;
    CScriptGameResourceObjectScriptedThingBase hero;
    if (sister.IsValid())
        sister.Release();
    while (!Game->StartScriptingEntity(Me, sister, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    cast["Hero"] = hero;
    cast["Messenger"] = sister;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_ARENA_LEAVE_THERESA_01", cast, 0, 0, false, true);
    Game->RemoveThing(Me, false, true);
    game->PauseAllNonScriptedEntities(false);
    movie.DestroyInline();
}