// CTCQuickAccessMenu::IsActive() const -- the Active flag +0x0C -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CTCQuickAccessMenu.h.
// Leaf: ?IsActive@CTCQuickAccessMenu@@QBE_NXZ
// Evidence: Ego_r.pdb / FableWin.pdb ?IsActive@CTCQuickAccessMenu@@QBE_NXZ; the header ISACTIVE opt-in names 0x00625930 (out of line); Active +0x0C (ctor).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#include "engine/CTCQuickAccessMenu.h"

bool CTCQuickAccessMenu::IsActive() const
{
    return Active;
}
