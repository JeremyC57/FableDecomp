// CTCQuestCard::SetTeleporterRegionName(const CCharString& name) -- set the region the quest card's teleporter leads to
// Leaf: ?SetTeleporterRegionName@CTCQuestCard@@QAEXABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007021E0, 8 bytes: `add ecx,24h ; jmp 0x0099EFB0` (TeleporterRegionName = name, tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetTeleporterRegionName@CTCQuestCard@@QAEXABVCCharString@@@Z (Ego_r 0x00723D20, the
//   first function of the class's donor run, as retail 0x007021E0 precedes the quest-card run 0x007021F0..), on TeleporterRegionName
//   +0x24 (engine/CTCQuestCard.h); the sole caller CQuestManager::SetQuestCardObjective 0x004B0E35 looks the TC up by interface 0x6C
//   (CTCQuestCard) first. The catalog's class "CTCTeleporter" was wrong (that class is 0x1C bytes and has no string).
// Callees: ??4CCharString@@QAEABV0@ABV0@@Z 0x0099EFB0.
// Header: engine/CTCQuestCard.h (FABLE_RETAIL_CTCQUESTCARD + FABLE_RETAIL_CTCQUESTCARD_TELEPORTER).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_TELEPORTER
#include "engine/CTCQuestCard.h"
void CTCQuestCard::SetTeleporterRegionName(const CCharString& name)
{
    TeleporterRegionName = name;
}