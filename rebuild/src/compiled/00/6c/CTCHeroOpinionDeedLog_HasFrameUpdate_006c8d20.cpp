// CTCHeroOpinionDeedLog::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroOpinionDeedLog@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C8D20, thiscall, `ret`.
// Identity: slot 8 of CTCHeroOpinionDeedLog's vtable 0x0125EA44 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroOpinionDeedLog or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroOpinionDeedLog@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroOpinionDeedLog.h (opt-ins as in the landed row CTCVillage_RemoveCrimeIfApologisable_0065c1fc.cpp).
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_VSLOTS
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CRIMES
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeed.h"
#include "engine/CVectorMap.h"
bool CTCHeroOpinionDeedLog::HasFrameUpdate() const
{
    return true;
}
