// SwapKeyedRecord(SKeyedRecord& left, SKeyedRecord& right) -- std::swap of two 16-byte records through a frame temporary
// Grade: DIFFER(ours v retail) -- retail spills the temporary's first three words to the 16-byte frame through eax and keeps the fourth in eax, copying left=right through esi/edi only; the lane keeps the temporary in ebx/ebp/esi/edi with one spill (73v81)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008068B0, length 81 bytes (`ret`, ecx = left, edx = right: a /Gr __fastcall
// template instance). The manifest label (CNearestBuildingWithProperty::CNearestBuildingWithProperty) is propagated and
// wrong: the body swaps two 16-byte records (neighbours: _Move_backward / _Push_heap helpers of 16-byte pair vectors).
// Tried (each graded against 0x00768A70 and 0x008068B0): POD of four words; four words with a memberwise copy constructor
// (+ operator=); pair<long, 12-byte POD> with a memberwise pair copy constructor (this file: it byte-matches the 73-byte
// swaps 0x0086A690 / 0x0086AF10); 12-byte member with its own copy constructor; float members; array members
// (`long[3]`, a struct holding `long v[3]`, first or last); a memcpy-built member; every form again under
// #pragma optimize("s", on). None keeps the temporary in memory the way retail does.
// Leaf: SwapKeyedRecord
struct SValue12 {
    unsigned long m_a;
    unsigned long m_b;
    unsigned long m_c;
};

struct SKeyedRecord {
    long     first;
    SValue12 second;

    SKeyedRecord(const SKeyedRecord& other) : first(other.first), second(other.second) {}
};

void __fastcall SwapKeyedRecord(SKeyedRecord& left, SKeyedRecord& right)
{
    SKeyedRecord temp = left;
    left = right;
    right = temp;
}
