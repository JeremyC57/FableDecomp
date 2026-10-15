// SwapKeyedRecord(SKeyedRecord& left, SKeyedRecord& right) -- std::swap of two 16-byte {key, 12-byte value} records through a frame temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086A690, length 73 bytes (`ret`, ecx = left, edx = right: a /Gr __fastcall
// template instance, like the other SGI/Dinkumware algorithm helpers in this module).
// The manifest label (CNearestBuildingWithProperty::CNearestBuildingWithProperty) is propagated and wrong: the body
// swaps two records. Its neighbours are the insertion-sort / heap helpers of a 16-byte keyed vector (0x0086A640, 0x0086A6E0, the
// _Push_heap over pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> @ 0x0086A770), so the
// record is a {key word, 12-byte POD value} pair. Shape: the temporary is copy-constructed through a user memberwise
// copy constructor (the key read into eax first, the value block copied through the re-based pointer `lea esi,[ecx+4]
// ; mov ebx,esi` into esi/edi/ebx with ebx spilled to the frame), then the implicit assignment copies the whole
// 16-byte record through ebx/ebp and the temporary is stored back. A POD record (no user copy constructor) copies the
// key with the value as one block.
// Leaf: SwapKeyedRecord
struct SValue12 {                    // 12-byte POD value
    unsigned long m_a;
    unsigned long m_b;
    unsigned long m_c;
};

struct SKeyedRecord {
    long     first;                  // +0x00 key
    SValue12 second;                 // +0x04 value

    SKeyedRecord(const SKeyedRecord& other) : first(other.first), second(other.second) {}
};

void __fastcall SwapKeyedRecord(SKeyedRecord& left, SKeyedRecord& right)
{
    SKeyedRecord temp = left;
    left = right;
    right = temp;
}
