// CTCInGameMenu::GetInterfaceType() const -- returns TCI_IN_GAME_MENU
// Leaf: ?GetInterfaceType@CTCInGameMenu@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048616C, thiscall, `ret`.
// Identity: slot 21 of CTCInGameMenu's vtable 0x01236804 (defined here: the slot holds this body in 1 vtable(s), all of CTCInGameMenu or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot; Ego_r/FableWin name set: ?GetInterfaceType@CTCInGameMenu@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Header: engine/CTCInGameMenu.h (opt-ins as in the landed row CTCInGameMenu_AddRegionAndTimeInfo_0047f58b.cpp).
#define FABLE_RETAIL_CTCINGAMEMENU_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContextReadSection.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include <string.h>
ETCInterfaceType CTCInGameMenu::GetInterfaceType() const
{
    return TCI_IN_GAME_MENU;
}
