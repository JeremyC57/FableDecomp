// CM_RestartMarker::Main() -- bandit-raid restart marker: forever, wait for the parent's restart flag, take the hero under
//   scripted control, start a paused movie sequence, fade out and in, and play CS_BANDITRAID_GATEOPEN (or _NONE when the
//   parent cannot populate the level); then clear the parent's flags
// Leaf: ?Main@CM_RestartMarker@
// Shape: the vector<CScriptThing> local sits at the top of a block spanning the whole iteration body (frame slot +0x1c),
//   modelled with the landed RescuedBandit 0x00D91740 recipe: an INLINE destructor body (destroy range + free), so VC7.1
//   knows the destructor never writes the three pointers and hoists the zero-init above the loop (retail: once, in the
//   prologue) and folds the destructor away at the wait-loop exits where the vector is still known-zero; the out-of-line
//   copy 0x008AC970 is forced with `#pragma inline_depth(0)` at the late exits and over the block's last statements (the
//   loop-end destructor). CRGBColour carries an inline copy constructor so the by-value FadeScreenOut argument is built in
//   its argument slot (`push ecx ; mov eax,esp ; mov [eax+2],bl ...`) instead of a frame temporary.
// Retail: Fable.exe @ 0x00DC21F0, 752 bytes, plain `ret`.
// Model: the landed-in-chunk CMK_GHC_HERO3::Main 0x00D29DB0 classes; handle ctor inline here, operator= 0x008ABD10,
//   movie implicit dtor out of line 0x006E7B80 at the early exits, PopulateLevel 0x00DC0300, vector<CScriptThing>
//   dtor 0x008AC970.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
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
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
class CRGBColour { public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
template<class T> __forceinline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
struct CScriptThingVector {                             // vector<CScriptThing> (STLport, 12 bytes)
    CScriptThing* _M_start; CScriptThing* _M_finish; CScriptThing* _M_end_of_storage;
    __forceinline CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CScriptThingVector()                 // out-of-line copy 0x008AC970 where inline_depth(0) applies
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
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
struct CQ_BanditRaidScript {
    unsigned char _pad[0x4a];
    bool          Flag4a;                               // +0x4a
    unsigned char _pad4b[0x58 - 0x4b];
    bool          RestartRequested;                     // +0x58
    unsigned char _pad59[0x60 - 0x59];
    bool          Flag60;                               // +0x60
    bool PopulateLevel();                               // 0x00DC0300
};
struct CM_RestartMarker : CEntityScriptBase {
    CQ_BanditRaidScript* ParentClass;                   // +0x14
    void Main();
};

void CM_RestartMarker::Main()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        {
        CScriptThingVector things;
        while (!ParentClass->RestartRequested) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->Flag4a = false;
        {
            CScriptGameResourceObjectScriptedThingBase hero;
            if (hero.IsValid())
                hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
            }
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            CCutsceneCastMap cast;
            cast["HERO"] = hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FadeScreenOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 0xff));
            Game->Pause(1.0f);
            Game->FadeScreenIn();
            if (ParentClass->PopulateLevel()) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_BANDITRAID_GATEOPEN", cast, 0, 0, false, false);
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                RunCutsceneMacro("CS_BANDITRAID_GATEOPEN_NONE", cast, 0, 0, false, false);
            }
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
#pragma inline_depth(0)
        ParentClass->RestartRequested = false;
        ParentClass->Flag60 = false;
        }
#pragma inline_depth()
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}