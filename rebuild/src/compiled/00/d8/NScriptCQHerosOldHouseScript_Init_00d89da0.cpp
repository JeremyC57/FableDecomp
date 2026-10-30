// NScript::CQ_HerosOldHouseScript::Init() -- reset the script's state: Helping, Helped, WifeAttacked, WifeAttackedAndLeft, BootyDugUp = 0/false
// Leaf: ?Init@CQ_HerosOldHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89DA0, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 of the script vtable 0x012D5018 (stored by the landed script constructor / Alloc); FableWin.pdb
//   ?Init@CQ_HerosOldHouseScript@NScript@@UAEXXZ; the members it clears are Ego_r.pdb's CQ_HerosOldHouseScript fields at their PDB - 0x14 offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; MissionAborted is left alone.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_HerosOldHouseScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void Init();
    bool  Helping;                                 // +0x48
    bool  Helped;                                  // +0x49
    bool  MissionAborted;                          // +0x4A
    bool  WifeAttacked;                            // +0x4B
    bool  WifeAttackedAndLeft;                     // +0x4C
    bool  BootyDugUp;                              // +0x4D
};

void CQ_HerosOldHouseScript::Init()
{
    Helping = false;
    Helped = false;
    WifeAttacked = false;
    WifeAttackedAndLeft = false;
    BootyDugUp = false;
}
}