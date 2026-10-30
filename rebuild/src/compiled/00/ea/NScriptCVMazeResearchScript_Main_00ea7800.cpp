// NScript::CV_MazeResearchScript::Main() -- the Maze-research quest main: bind the EmptyGrave and HistoryBookcase entity scripts, post-add the scripted entities, then set the quest card objective (TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01, HeroGuildComplexInside)
// Leaf: ?Main@CV_MazeResearchScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7800, 374 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012EE444 [dtor 0x00EA86C0, RegisterMain 0x00EA7760 (landed
//   NScript::CV_MazeResearchScript::RegisterMain, which spawns slot 2 as "Main"), Main 0x00EA7800, Init 0x00EA7750,
//   OnPersist 0x00EA80A0]; the only reference to the row is that slot. Ego_r.pdb ?Main@CV_MazeResearchScript@NScript@@UAEXXZ
//   (donor 0x00F117B0, after RegisterMain 0x00F11680 as here). The two binding factories are the landed CEmptyGrave::Alloc
//   0x00EA7990 and CHistoryBookcase::Alloc 0x00EA7A60 (Ego_r.pdb Alloc statics of the class). Catalog label: sub_00ea7800.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930; CEntityScriptBinding<T> vtable
//   0x012EE4AC; CGameScriptInterface slots 64 PostAddScriptedEntities, 296 SetQuestCardObjective, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "EmptyGrave" 0x012EE4B4, "HistoryBookcase" 0x012EE49C, "" 0x0122D70E, "HeroGuildComplexInside" 0x01238FCC,
//   "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01" 0x012EE474.
// Layout: CScriptBase retail 0x40 bytes (polymorphic), Game +0x40; CEntityScriptBinding 0x1c bytes (Flags 0).
// Model: the landed NScript::CQS_MeetSisterScript::Main 0x00E28DE0 (this chunk) / CV_IntroductionToTrophiesScript::Main 0x00E9C4B0.
// Shape: every new-expression's name string is a conditional temporary with its own ebx flag bit; the SetQuestCardObjective
//   strings are built right to left, the GetActiveQuestName result last.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
#include "engine/CCountedPointer.h"
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {   // 0x1c bytes
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
namespace NScript {
class CV_MazeResearchScript : public CScriptBase {
public:
    class CEmptyGrave {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MazeResearchScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA7990
    };
    class CHistoryBookcase {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MazeResearchScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA7A60
    };
    CGameScriptInterface* Game;                         // +0x40
    virtual void Main();
};

void CV_MazeResearchScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MazeResearchScript>("EmptyGrave", this, &CEmptyGrave::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MazeResearchScript>("HistoryBookcase", this, &CHistoryBookcase::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", "HeroGuildComplexInside", "");
}
}   // namespace NScript