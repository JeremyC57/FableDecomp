// NScript::CV_MurderTwistScript::Init() -- no items found yet, state and event both 1
// Leaf: ?Init@CV_MurderTwistScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8720, 19 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012EE634, which its constructor 0x00EA86F0 (manifest label CEngineSubPrimitiveBase::CEngineSubPrimitiveBase, propagated) and Alloc 0x00EAE890 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_MurderTwistScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_MurderTwistScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: slot 2 (Main 0x00EA8830) binds the MurderTwistLeader / MurderTwistBrother* entities.
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    virtual void Init();
};

void CV_MurderTwistScript::Init()
{
    MurderTwistItemsFound = 0;
    MurderTwistState = 1;
    MurderTwistEvent = 1;
}
}