// NScript::CV_ParanoidWhispersScript::Init() -- paranoid state 1, digging spot hidden
// Leaf: ?Init@CV_ParanoidWhispersScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBEEC0, 12 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012F0F38, which the landed NScript::CV_ParanoidWhispersScript::CV_ParanoidWhispersScript 0x00EBEE90 (and the landed Alloc 0x00EC11B0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_ParanoidWhispersScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_ParanoidWhispersScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ParanoidWhispersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long ParanoidState;                                 // +0x48
    bool HideDiggingSpot;                               // +0x4c
    virtual void Init();
};

void CV_ParanoidWhispersScript::Init()
{
    ParanoidState = 1;
    HideDiggingSpot = true;
}
}