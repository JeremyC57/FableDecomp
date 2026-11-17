// CTCQuickAccessMenu::IsQuitting() const -- the Quitting flag +0x1C
// Leaf: ?IsQuitting@CTCQuickAccessMenu@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625940, `mov al,[ecx+1Ch] ; ret`.
// Identity (NF59, round 7, queue q10497): Ego_r.pdb 0x0066A160 + FableWin.pdb ?IsQuitting@CTCQuickAccessMenu@@QBE_NXZ; Ego_r order
//   Deactivate / IsActive / IsQuitting = retail 0x00625920 / 0x00625930 / 0x00625940; the landed callers 0x0041E6D3 and 0x00583BD5 bind
//   it through engine/CTCQuickAccessMenu.h (FABLE_RETAIL_CTCQUICKACCESSMENU_ISQUITTING_ACTIVATE), whose Quitting member is +0x1C (ctor).
//   Re-authored on the shared header, as the IsActive twin 0x00625930: the landed row was the address-named stand-in
//   C_00625940::M (catalog _global::GetMember_byte0x1c_00625940, file GetCanItemBeQuickAssigned).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISQUITTING_ACTIVATE
#include "engine/CTCQuickAccessMenu.h"

bool CTCQuickAccessMenu::IsQuitting() const
{
    return Quitting;
}
