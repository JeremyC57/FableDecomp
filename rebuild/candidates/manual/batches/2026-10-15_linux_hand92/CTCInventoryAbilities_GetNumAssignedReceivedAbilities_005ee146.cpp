// CTCInventoryAbilities::GetNumAssignedReceivedAbilities() const -- how many entries of the received-ability order are set (non-zero)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE146, length 61 bytes.
// Callers: 0x00652D14.
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors). In the shipped class the
//         base is 0x14 bytes narrower and every vector is 12 bytes: SpellToAssign +0x134, ExperienceSpentOnAbilities
//         +0x148, ReceivedAbilityOrder +0x154, Abilities +0x160, AbilityButtons +0x16C, ... ReceivedExpressionOrder
//         +0x1BC (all confirmed by the sibling bodies in this chunk), so a local view names the member.
// Pragma: optimize("s", on) -- size TU (unaligned loop head, `first` re-read after the entry test).
// Shape: `i < size()` with an unsigned index (`sar edx,2 ; je` entry test, `jb` back edge), size() re-evaluated
//        per iteration, the element pointer strength-reduced.

struct CAbilityVector {                           // vector<EHeroAbility>
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};

struct CTCInventoryAbilities_Retail {
    unsigned char  _pad_0x000[0x154];
    CAbilityVector ReceivedAbilityOrder;          // +0x154 (PDB +0x170)

    long GetNumAssignedReceivedAbilities() const;
};

long CTCInventoryAbilities_Retail::GetNumAssignedReceivedAbilities() const
{
    long assigned = 0;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        if (ReceivedAbilityOrder[i] != 0)
            ++assigned;
    }
    return assigned;
}
