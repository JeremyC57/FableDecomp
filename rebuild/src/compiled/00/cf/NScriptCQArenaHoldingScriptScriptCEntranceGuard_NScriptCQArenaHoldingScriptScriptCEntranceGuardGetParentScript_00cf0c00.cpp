// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF0C00, 4 bytes: mov eax,[ecx+14h]; ret (8B 41 14 C3).
// Identity (NF196, 2026-11-20, q6522): slot 3 of the CEntranceGuard vtable 0x012C6158 that the landed Alloc 0x00CF0B60 stores (slot 1
//   Main 0x00CF68C0, 2 Init 0x00CF0C10, 4 OnPersist 0x00CF6890, all landed under CEntranceGuard); FableWin.pdb
//   ?GetParentScript@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@UBEPAVCScriptBase@@XZ; same body and slot as the landed
//   CFanMarker / COakvaleFan ::GetParentScript 0x00CF4E90 / 0x00CF1890.
// Was: gapscan stand-in sub_00cf0c00 (`struct T { char pad[0x14]; int field; int Get(); }`, no Leaf line); now on the shared
//   engine/CQ_ArenaHoldingScriptScript.h CEntranceGuard opt-in (as the landed Main 0x00CF68C0).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ENTRANCEGUARD
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"

namespace NScript {
CScriptBase* CQ_ArenaHoldingScriptScript::CEntranceGuard::GetParentScript() const
{
    return &ParentClass;
}
}
