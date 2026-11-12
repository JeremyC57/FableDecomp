// CTCVillage::IsWorkTime() const -- whether the time of day lies in one of the village def's work windows
// Leaf: ?IsWorkTime@CTCVillage@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658B20, 75 bytes, thiscall, plain `ret`.
// Identity (agentNF25b, 2026-11-12): Ego_r.pdb + FableWin.pdb ?IsWorkTime@CTCVillage@@QBE_NXZ. The body passes CVillageDef::WorkStartTimes
//   (+0x64) and WorkDuration (+0x70) to IsTimeBetween (Ego_r +112 / +128 less the 4 bytes each 12-byte retail vector saves), and Ego_r's
//   donor order IsSchoolTime .. IsWorkTime, IsCookTime matches retail 0x00658B20, 0x00658B6B (IsCookTime, landed: CookStartTimes +0x74 /
//   CookDuration +0x80). Was sub-named `CTCVillage_IsCookTime` (a __fastcall free function calling a __stdcall `CookTest` stand-in);
//   landed binders spelled it IsCookTime_00658b20, IsCookTime const, ComputeB0 and IsWorkTime.
// Callees: CTCVillage::IsTimeBetween(const vector<long>&, long) const 0x00658620.
// Shape: the landed 0x00658B6B form on the shared headers: `return IsTimeBetween(GetPDef()->WorkStartTimes, GetPDef()->WorkDuration);`
//   -- two by-value CDefPointer temporaries (FableWin's inline GetPDef; the duration's first: arguments right to left), released in
//   reverse; the dereferenced one's release needs no null test. `optimize("s", on)`.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ISWORKTIME   // hdr overlay (agentNF25b)
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#pragma optimize("s", on)

bool CTCVillage::IsWorkTime() const
{
    return IsTimeBetween(GetPDef()->WorkStartTimes, GetPDef()->WorkDuration);
}
