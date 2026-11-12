// CTCHeroStats::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroStats@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005814C6, thiscall, `ret`.
// Identity: slot 23 of CTCHeroStats's vtable 0x0124F70C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroStats or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroStats@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroStats.h (opt-ins as in the landed row CTCHeroStats_GetMaxDrunkenness_0057a89d.cpp).
#define FABLE_RETAIL_CTCHEROSTATS_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
bool CTCHeroStats::IsComponentSerialisable() const
{
    return true;
}
