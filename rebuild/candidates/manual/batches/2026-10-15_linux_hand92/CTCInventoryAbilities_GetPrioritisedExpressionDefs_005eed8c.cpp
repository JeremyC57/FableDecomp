// CTCInventoryAbilities::GetPrioritisedExpressionDefs(vector<long>*) -- appends the received-expression order (the expression defs in priority order) to the caller's vector
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EED8C, length 67 bytes.
// Callees: std::vector<long>::push_back(const long&) @ 0x00485FF3 (on the output vector).
// Pragma: optimize("s", on) -- size TU (unaligned loop head, `sar ; je` entry test).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         ReceivedExpressionOrder (vector<long>) is at +0x1BC (PDB +0x1F8).
// Shape: an unsigned index over size() re-evaluated per iteration; the element is passed by reference
//        (`lea eax,[eax+edi*4]`) and the output pointer re-read from its argument slot each time.

struct CLongVector {
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
    void push_back(const long& value);            // 0x00485FF3
};

struct CTCInventoryAbilities_Retail {
    unsigned char _pad_0x000[0x1BC];
    CLongVector   ReceivedExpressionOrder;        // +0x1BC (PDB +0x1F8)

    void GetPrioritisedExpressionDefs(CLongVector* defs);
};

void CTCInventoryAbilities_Retail::GetPrioritisedExpressionDefs(CLongVector* defs)
{
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i)
        defs->push_back(ReceivedExpressionOrder[i]);
}
