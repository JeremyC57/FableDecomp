// CTCInventoryAbilities::GetNumAssignedReceivedExpressions() const -- how many entries of the received-expression order are positive (assigned expressions)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE24B, length 61 bytes.
// Callers: 0x00652D8D.
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors). In the shipped class the
//         base is 0x14 bytes narrower and every vector is 12 bytes: SpellToAssign +0x134, ExperienceSpentOnAbilities
//         +0x148, ReceivedAbilityOrder +0x154, Abilities +0x160, AbilityButtons +0x16C, ... ReceivedExpressionOrder
//         +0x1BC (all confirmed by the sibling bodies in this chunk), so a local view names the member.
// Pragma: optimize("s", on) -- size TU (unaligned loop head, `first` re-read after the entry test).
// Shape: a signed `> 0` element test (`jle`); `i < size()` with an unsigned index (`sar edx,2 ; je` entry test,
//        `jb` back edge), size() re-evaluated
//        per iteration, the element pointer strength-reduced.

struct CExpressionVector {                        // vector<long>
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};

struct CTCInventoryAbilities_Retail {
    unsigned char  _pad_0x000[0x1BC];
    CExpressionVector ReceivedExpressionOrder;    // +0x1BC (PDB +0x1F8)

    long GetNumAssignedReceivedExpressions() const;
};

long CTCInventoryAbilities_Retail::GetNumAssignedReceivedExpressions() const
{
    long assigned = 0;
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i) {
        if (ReceivedExpressionOrder[i] > 0)
            ++assigned;
    }
    return assigned;
}