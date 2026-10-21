// CTCInventoryExperience::IncrementStatIndicator() -- moves the stat-spending indicator down one entry, stopping at the last
// Leaf: IncrementStatIndicator@CTCInventoryExperience
// Retail: Fable.exe @ 0x0056E5CA, 20 bytes; called from CTCInventoryExperience::ProcessInventoryInputDown 0x0056ECCB.
// Shape: size TU (`lea eax,[ecx+0x1bc]` and a memory compare); /O2 loads both fields first.
// Layout: retail CTCInventoryExperience = Ego_r.pdb minus 0x44 here (STLport containers before it): statIndicatorPos +0x1bc,
// statIndicatorMax +0x1c0 (PDB +0x200/+0x204); engine/CTCInventoryExperience.h carries the PDB offsets.
class CTCInventoryExperience {
public:
    unsigned char _pad00[0x1bc];
    long          statIndicatorPos;                      // +0x1bc
    long          statIndicatorMax;                      // +0x1c0
    void IncrementStatIndicator();
};

#pragma optimize("s", on)
void CTCInventoryExperience::IncrementStatIndicator()
{
    if (statIndicatorPos + 1 < statIndicatorMax)
        ++statIndicatorPos;
}