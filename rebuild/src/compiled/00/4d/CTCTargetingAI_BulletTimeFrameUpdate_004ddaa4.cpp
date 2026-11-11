// CTCTargetingAI::BulletTimeFrameUpdate() -- the targeting component keeps choosing its best enemy during bullet time: forwards to FrameUpdate
// Leaf: ?BulletTimeFrameUpdate@CTCTargetingAI@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDAA4, 5 bytes (`mov eax,[ecx]; jmp [eax+0x24]`), thiscall, vtable 0x012411FC slot 10.
// Identity: CTCBase slot 10 = BulletTimeFrameUpdate (engine/CTCBase.h), FableWin.pdb ?BulletTimeFrameUpdate@CTCTargetingAI@@UAEXXZ
//   (0x01B97954, the same 0x19-byte debug forwarder); the tail call goes through slot 9 = FrameUpdate (0x007F6830 family).
// Extent: the catalog row ran on into slot 8 (0x004DDAA9 HasFrameUpdate) and slot 33 (0x004DDAAC); see SPLITS.tsv.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"

void CTCTargetingAI::BulletTimeFrameUpdate()
{
    FrameUpdate();   // virtual: slot 9
}