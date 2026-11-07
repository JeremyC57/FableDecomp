// NScript::CGameflowScript::Init() -- set the master data's post-save position to EGP_INTRO, clear CoreQuestWaiting, declare the gossip categories
// Split: the chunk 853 row 0x00CE6CF0 (2237 bytes) holds TWO functions: this Init (16 bytes, its last instruction `jmp +0` falls into the
//   next one) and DeclareGossipCategories at 0x00CE6D00 (2221 bytes, split/00ce6d00.cpp). Graded alone against Fable.exe bytes
//   (bin/q2.py 00ce6cf0 16): MATCH. The catalogue must recut the row into 0x00CE6CF0 (16) + 0x00CE6D00 (2221) before landing.
// Leaf: ?Init@CGameflowScript@NScript@@UAEXXZ
// Identity: Ego_r.pdb ?Init@CGameflowScript@NScript@@UAEXXZ (0x00CCF330) and the CGameflowScript vtable 0x012C3FB0 slot next to
//   RegisterMain 0x00CE75B0 / Main 0x00CE7670; MasterScriptData->PostSavePosition (CQ_SunnyvaleMasterData) = EGP_INTRO, CoreQuestWaiting +0x48.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();
    void DeclareGossipCategories();
    unsigned long CoreQuestWaiting;                         // +0x48
};
}

void NScript::CGameflowScript::Init()
{
    MasterScriptData->PostSavePosition = EGP_INTRO;
    CoreQuestWaiting = 0;
    DeclareGossipCategories();
}