// CEmptyGrave::Main() -- Maze research empty grave: once the history book is read, mark the grave on the minimap; put the GoodSword in limbo; then each time the hero uses the grave -- if the book was read, the sword not yet taken, the game past position 1700 and Jack's boss battle result is 2 -- script the hero, spawn ParentClass.UnLimboSword, play the saint or redeem Sword-of-Aeons cut-scene by the hero's alignment at the end, take the marker off, flag SwordTaken and deactivate the quest
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8130, 1191 bytes (+ int3 padding), plain `ret`. Literals "HUD_ORB_QUEST_VIGNETTE" 0x01258A64,
//   "GoodSword" 0x012EE628, "ParentClass." 0x012C9934, "UnLimboSword" 0x012EE618, "CS_GET_SWORD_OF_AEONS_SAINT" 0x012EE5FC,
//   "CS_GET_SWORD_OF_AEONS_REDEEM" 0x012EE5DC, "HERO" 0x01255174, "" 0x0122D70E, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 / operator+ 0x0099F570 (__fastcall, hidden
//   result in ecx), operator new 0x00BFEA1A (0x3c), CSpawnedFuncBase ctor 0x00CDD450 (spawned-func vtable 0x012EE458, callback
//   CV_MazeResearchScript::UnLimboSword 0x00EA85E0), CQuestScript AddSpawnedFunction 0x00CB7E50, CBaseClass dtor 0x0099A2E0, operator
//   delete 0x00BFE9BC, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10, cast map ctor 0x00CDBF70 /
//   dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   GameCalls slots 7, 8, 70 GetHero, 72 GetThingWithScriptName, 281 DeactivateQuestLater, 350/352 MiniMap add/remove marker,
//   370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 453 EntitySetInLimbo, 655
//   GetActiveQuestName; CScriptThing slot 20 MsgIsUsedBy.
// Layout: Ego_r.pdb CEmptyGrave (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_MazeResearchScript),
//   PMasterData +0x18. CV_MazeResearchScript (retail = PDB - 0x14): SwordTaken +0x48, BookRead +0x49, Sword +0x4c (CScriptThing),
//   cs_flags +0x58. CQ_SunnyvaleMasterData: PostSavePosition +4, JackBossBattleResult +0x88, JackBossBattleHeroGoodAtEnd +0x8c.
// Shape: the sword assignment is the inline CScriptThing memberwise operator= (counted PImp), the GetThingWithScriptName temporary
//   is destroyed inline; the spawn is the SPAWN macro shape `AddSpawnedFunction(new CSpawnedFunc<T>(CCharString("ParentClass.") +
//   "UnLimboSword", 0, ParentClass, &T::UnLimboSword), "")` whose three name temporaries get conditional-destruction flags.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD16(ma) VPAD2(mb) VPAD1(mc)                      // slots 1..19
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() { }                   // inline here; out-of-line copy 0x004AA840
};
class CScriptGameResourceObjectScriptedThingBase { public:     // vtable 0x0127094C, 0x10 bytes
    virtual void h000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
class CScriptGameResourceObjectMovieBase { public:             // vtable 0x01260EF4, 0x10 bytes
    virtual void m000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    virtual ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneFlagMap { void* _body[3]; };            // map<CCharString, bool>
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CV_MazeResearchScript {
    unsigned char    _pad[0x48];
    bool             SwordTaken;                        // +0x48 (PDB +0x5c)
    bool             BookRead;                          // +0x49 (PDB +0x5d)
    unsigned char    _pad4a[2];
    CScriptThing     Sword;                             // +0x4c (PDB +0x60)
    CCutsceneFlagMap cs_flags;                          // +0x58 (PDB +0x6c)
    void UnLimboSword();                                // 0x00EA85E0
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& region = CCharString("")) { AddSpawnedFunction(func, region); }
};
struct CQ_SunnyvaleMasterData {
    long          _pad0;
    long          PostSavePosition;                     // +0x04 (EGameflowPosition)
    unsigned char _pad8[0x80];
    long          JackBossBattleResult;                 // +0x88
    bool          JackBossBattleHeroGoodAtEnd;          // +0x8c
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CEmptyGrave : CEntityScriptBase {
    CV_MazeResearchScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};

void CEmptyGrave::Main()
{
    if (ParentClass->BookRead) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_VIGNETTE");
    }
    ParentClass->Sword = Game->GetThingWithScriptName("GoodSword");
    Game->EntitySetInLimbo(ParentClass->Sword, true, true);
    if (IsActiveThreadTerminating())
        return;
    do {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->BookRead && !ParentClass->SwordTaken && PMasterData->PostSavePosition > 1700 && PMasterData->JackBossBattleResult == 2) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CScriptGameResourceObjectScriptedThingBase hero;
                    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
                    CCutsceneCastMap cast;
                    cast["HERO"] = hero;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CGameScriptInterface* game = Game;
                    game->PauseAllNonScriptedEntities(true);
                    Game->FixMovieSequenceCamera(true);
                    ParentClass->SpawnFunction(new CSpawnedFunc<CV_MazeResearchScript>(CCharString("ParentClass.") + CCharString("UnLimboSword"), 0, ParentClass, &CV_MazeResearchScript::UnLimboSword));
                    if (PMasterData->JackBossBattleHeroGoodAtEnd) {
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                        RunCutsceneMacro("CS_GET_SWORD_OF_AEONS_SAINT", cast, &ParentClass->cs_flags, 0, false, true);
                    } else {
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                        RunCutsceneMacro("CS_GET_SWORD_OF_AEONS_REDEEM", cast, &ParentClass->cs_flags, 0, false, true);
                    }
                    Game->FixMovieSequenceCamera(false);
                    Game->MiniMapRemoveMarker(Me);
                    game->PauseAllNonScriptedEntities(false);
                }
                ParentClass->SwordTaken = true;
                Game->MiniMapRemoveMarker(Me);
                Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}