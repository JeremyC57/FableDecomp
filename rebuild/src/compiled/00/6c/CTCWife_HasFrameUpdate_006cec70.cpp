// CTCWife::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWife@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CEC70, thiscall, `ret`.
// Identity: slot 8 of CTCWife's vtable 0x0125ED5C (defined here: the slot holds this body in 1 vtable(s), all of CTCWife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWife@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCWife.h (opt-ins as in the landed row CTCWife_OnSerialise_006d2bd0.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "fable_string.h"
#include "engine/CTCWife.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
bool CTCWife::HasFrameUpdate() const
{
    return true;
}
