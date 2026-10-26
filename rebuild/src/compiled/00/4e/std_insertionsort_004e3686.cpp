// std::__insertion_sort(SStringEntry16* first, SStringEntry16* last, SStringEntry16Less comp) -- guarded insertion sort of 16-byte {CCharString, 12-byte value} pairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3686, length 53 bytes (unaligned entry: a size-optimised TU).
// Leaf: __insertion_sort
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. Its caller 0x004E415D is
// STLport `__final_insertion_sort` (`and eax,-10h ; cmp eax,100h` = 16 elements of 16 bytes, then this function on the
// first 16 and the unguarded sort on the rest), so this is `__insertion_sort(first, last, comp)`: return on an empty
// range, then `__linear_insert(first, i, _Tp(*i), comp)` @ 0x004E1C8A for i = first+1 .. last (`ret 14h` = 16-byte
// value + comparator). The value type's copy constructor is out of line @ 0x004D788D (it copies a CCharString and then
// a 12-byte member through its own copy constructor 0x004D6C66), so the element is modelled as SStringEntry16.
// Convention: LTCG register form -- first in ecx, last in edx, the 4-byte empty comparator on the stack (`ret 4`).
// Callees: SStringEntry16::SStringEntry16(const SStringEntry16&) @ 0x004D788D, std::__linear_insert @ 0x004E1C8A.
// Pragma: optimize("s") -- the size TU's `push dword ptr [esp+10h]` for the comparator and the loop entered by a jump
// to its bottom test (under /O2 the test is duplicated in front of the loop, 58 v 53 bytes).
#pragma optimize("s", on)
class CCharString { public:
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
struct SValue12 { long m_words[3]; };       // 12-byte value with its own copy constructor (0x004D6C66)
struct SStringEntry16 {                     // pair<CCharString, 12-byte value>
    CCharString first;                      // +0x00
    SValue12    second;                     // +0x04
    SStringEntry16(const SStringEntry16& other);   // 0x004D788D
};
struct SStringEntry16Less { };              // empty comparator
namespace std {
void __fastcall __linear_insert(SStringEntry16* first, SStringEntry16* last, SStringEntry16 val, SStringEntry16Less comp);   // 0x004E1C8A
void __fastcall __insertion_sort(SStringEntry16* first, SStringEntry16* last, SStringEntry16Less comp)
{
    if (first == last)
        return;
    for (SStringEntry16* i = first + 1; i != last; ++i)
        __linear_insert(first, i, SStringEntry16(*i), comp);
}
}