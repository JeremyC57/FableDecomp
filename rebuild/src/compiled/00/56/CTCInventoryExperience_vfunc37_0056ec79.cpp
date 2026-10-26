// CTCInventoryExperience::vfunc_37() -- zero the stat indicator position/maximum, then re-seed them
// Retail 0x0056EC79, 19 bytes (vtable 0x0124F108 slot 37). statIndicatorPos (+0x1BC) and
// statIndicatorMax (+0x1C0) are cleared with `and dword ptr [ecx+N],0` (the /Os zero store), then
// ResetStatIndicatorValues 0x0056E5AB is tail-jumped with `this` still in ecx.
// Retail offsets: the PDB header has the indicator pair at +0x200/+0x204; retail is 0x44 bytes narrower
// ahead of them (12-byte STLport containers), so a local retail-offset model is used.
// Layout (supersedes any local-model note above): engine/CTCInventoryExperience.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCInventoryExperience CTCInventoryExperienceDonor
#include "engine/CTCInventoryExperience.h"
#undef CTCInventoryExperience
#pragma optimize("s", on)

class CTCInventoryExperience : public CTCInventoryExperienceDonor {
 public:
    void ResetStatIndicatorValues();    // 0x0056E5AB
    void vfunc_37();
};

void CTCInventoryExperience::vfunc_37()
{
    statIndicatorPos = 0;
    statIndicatorMax = 0;
    ResetStatIndicatorValues();
}