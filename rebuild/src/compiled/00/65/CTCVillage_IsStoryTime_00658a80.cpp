// CTCVillage::IsStoryTime() const -- whether the time of day lies in one of the village def's story windows
// Leaf: ?IsStoryTime@CTCVillage@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658A80, 75 bytes, thiscall, plain `ret`.
// Identity (agentNF26, 2026-11-12): Ego_r.pdb + FableWin.pdb ?IsStoryTime@CTCVillage@@QBE_NXZ. The body passes CVillageDef::StoryStartTimes
//   (+0x44) and StoryDuration (+0x50) to IsTimeBetween (Ego_r +72 / +88 less the 4 bytes each 12-byte retail vector saves), and Ego_r's
//   donor order IsSchoolTime (0x0069A9A4), IsStoryTime (0x0069A9F1) .. IsWorkTime, IsCookTime matches retail 0x00658A35, 0x00658A80 ..
//   0x00658B20 (landed), 0x00658B6B (landed). Was CTCVillage::IsCookTime (a propagated label; the source graded a __fastcall free
//   function `CTCVillage_IsCookTime` calling a __stdcall `DoCookCheck` stand-in).
// Callees: CTCVillage::IsTimeBetween(const vector<long>&, long) const 0x00658620.
// Shape: the landed 0x00658B20 / 0x00658B6B form on the shared headers: `return IsTimeBetween(GetPDef()->StoryStartTimes, GetPDef()->StoryDuration);`
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
#define FABLE_RETAIL_CTCVILLAGE_SCHOOLSTORYTIME   // hdr overlay (agentNF26)
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#pragma optimize("s", on)

bool CTCVillage::IsStoryTime() const
{
    return IsTimeBetween(GetPDef()->StoryStartTimes, GetPDef()->StoryDuration);
}
