// NScript::CQ_ArenaHoldingScriptScript::Init() -- reset the script's state: PlayerAttackedFans, FansCanLeaveNow, OakvaleInsult, ArenaOver, InHitCutsceneAlready = 0/false
// Leaf: ?Init@CQ_ArenaHoldingScriptScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF06D0, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 of the script vtable 0x012C60A0 (stored by the landed script constructor / Alloc); FableWin.pdb
//   ?Init@CQ_ArenaHoldingScriptScript@NScript@@UAEXXZ; the members it clears are Ego_r.pdb's CQ_ArenaHoldingScriptScript fields at their PDB - 0x14 offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; every state member is cleared.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_ArenaHoldingScriptScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void Init();
    bool  PlayerAttackedFans;                      // +0x48
    bool  FansCanLeaveNow;                         // +0x49
    bool  OakvaleInsult;                           // +0x4A
    bool  ArenaOver;                               // +0x4B
    bool  InHitCutsceneAlready;                    // +0x4C
};

void CQ_ArenaHoldingScriptScript::Init()
{
    PlayerAttackedFans = false;
    FansCanLeaveNow = false;
    OakvaleInsult = false;
    ArenaOver = false;
    InHitCutsceneAlready = false;
}
}