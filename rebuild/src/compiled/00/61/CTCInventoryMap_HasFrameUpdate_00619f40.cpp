// CTCInventoryMap::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCInventoryMap@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619F40, thiscall, `ret`.
// Identity: slot 8 of CTCInventoryMap's vtable 0x01257A9C (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryMap or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 35 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCInventoryMap@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryMap.h (opt-ins as in the landed row CPersistTraitsCTCInventoryMapCUIMiniMapMarker_TransferIn_006159a0.cpp).
#define FABLE_RETAIL_CTCINVENTORYMAP_VSLOTS
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCInventoryMap.h"
bool CTCInventoryMap::HasFrameUpdate() const
{
    return true;
}
