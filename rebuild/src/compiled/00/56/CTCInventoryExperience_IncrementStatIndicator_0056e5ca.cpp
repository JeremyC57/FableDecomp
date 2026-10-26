// CTCInventoryExperience::IncrementStatIndicator() -- moves the stat-spending indicator down one entry, stopping at the last
// Leaf: IncrementStatIndicator@CTCInventoryExperience
// Retail: Fable.exe @ 0x0056E5CA, 20 bytes; called from CTCInventoryExperience::ProcessInventoryInputDown 0x0056ECCB.
// Shape: size TU (`lea eax,[ecx+0x1bc]` and a memory compare); /O2 loads both fields first.
// Layout: retail CTCInventoryExperience = Ego_r.pdb minus 0x44 here (STLport containers before it): statIndicatorPos +0x1bc,
// statIndicatorMax +0x1c0 (PDB +0x200/+0x204); engine/CTCInventoryExperience.h carries the PDB offsets.
// Layout (supersedes any local-model note above): engine/CTCInventoryExperience.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCInventoryExperience CTCInventoryExperienceDonor
#include "engine/CTCInventoryExperience.h"
#undef CTCInventoryExperience
class CTCInventoryExperience : public CTCInventoryExperienceDonor {
public:
    void IncrementStatIndicator();
};

#pragma optimize("s", on)
void CTCInventoryExperience::IncrementStatIndicator()
{
    if (statIndicatorPos + 1 < statIndicatorMax)
        ++statIndicatorPos;
}