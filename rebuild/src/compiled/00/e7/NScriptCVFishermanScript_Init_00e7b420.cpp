// NScript::CV_FishermanScript::Init() -- reset the script's state: HeroHasSavedFishermanFromWasps, IntroPlayed, WaspsAlive, QuestCardGiven, FishermanTasks = 0/false
// Leaf: ?Init@CV_FishermanScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B420, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 of the script vtable 0x012EA024 (stored by the landed script constructor / Alloc); FableWin.pdb
//   ?Init@CV_FishermanScript@NScript@@UAEXXZ; the members it clears are Ego_r.pdb's CV_FishermanScript fields at their PDB - 0x14 offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; FishermanMustRunAway is left alone.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CV_FishermanScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void Init();
    bool  IntroPlayed;                             // +0x48
    bool  HeroHasSavedFishermanFromWasps;          // +0x49
    long  WaspsAlive;                              // +0x4C
    bool  FishermanMustRunAway;                    // +0x50
    bool  QuestCardGiven;                          // +0x51
    long  FishermanTasks;                          // +0x54
};

void CV_FishermanScript::Init()
{
    HeroHasSavedFishermanFromWasps = false;
    IntroPlayed = false;
    WaspsAlive = 0;
    QuestCardGiven = false;
    FishermanTasks = 0;
}
}