// sort_heap<pair*> -- pop_heap until one element remains: swap the last into the front slot's value, copy the first to the back, __adjust_heap @ 0x00A6B4F0 from hole 0 (8-byte pairs with a user copy constructor, comparator by value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B990, oracle length 96 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,ecx ; mov eax,edx ; sub eax,edi ; and eax,-8 ; cmp eax,8 ; jle R ; mov ebp,[pred] ; mov ebx,8 ; lea esi,[edx-8] ; sub ebx,edi ; L: mov eax,[edi] ; mov ecx,[esi] ; mov edx,[esi+4] ; mov [esi],eax ; mov eax,[edi+4] ; push ebp ; mov [esi+4],eax ; sub esp,8 ; mov eax,esp ; mov [eax],ecx ; lea ecx,[ebx+esi-8] ; sar ecx,3 ; mov [eax+4],edx ; push ecx ; xor edx,edx ; mov ecx,edi ; call _Adjust_heap ; sub esi,8 ; lea edx,[ebx+esi] ; and edx,-8 ; cmp edx,8 ; jg L ; R: pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// STLport `sort_heap`: while more than one element remains, `__pop_heap` moves the last element's
// value aside (copied into the reserved argument slot: the pair has a user copy constructor),
// copies the front to the back and re-sifts from hole 0 through the out-of-line __fastcall
// `__adjust_heap` @ 0x00A6B4F0 (first in ecx, hole in edx, bottom count, the value, the
// comparator by value), then `--last`. The byte-count form (`and -8 ; cmp 8`) is VC's lowering
// of `1 < last - first`. The manifest label is propagated; the pair's members are modelled as
// dwords. One of 8 bodies of this shape.
// Fixed (rename1, 2026-10-24): comments only -- retail STL is STLport 4.x (`sort_heap` / `__pop_heap` /
//   `__adjust_heap`), not Dinkumware (`_Sort_heap` / `_Adjust_heap`); bytes unchanged.
struct SPair {
    long m_first;
    unsigned long m_second;
    SPair(const SPair& other) : m_first(other.m_first), m_second(other.m_second) { }
};

struct SLess { char m_tag; };              // comparator object passed by value

void __fastcall AdjustHeap_00a6b4f0(SPair* first, long hole, long bottom, SPair value, SLess pred);   // 0x00A6B4F0

inline void PopHeap(SPair* first, SPair* last, SPair* dest, SPair value, SLess pred)
{
    *dest = *first;
    AdjustHeap_00a6b4f0(first, 0, last - first, value, pred);
}

void __fastcall SortHeap_00a6b990(SPair* first, SPair* last, SLess pred)
{
    for (; 1 < last - first; --last)
        PopHeap(first, last - 1, last - 1, *(last - 1), pred);
}