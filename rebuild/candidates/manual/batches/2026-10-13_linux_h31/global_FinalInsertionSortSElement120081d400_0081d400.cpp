// SGI __final_insertion_sort<SElement12*, Compare>(12-byte elements) -- insertion sort of the first 16, unguarded insertion sort of the rest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081D400, oracle length 80 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; cmp eax,10h ; jle S ; push ebx ; mov ebx,[comp] ; push edi ; lea edi,[ecx+0xc0] ; push ebx ; mov edx,edi ; call __insertion_sort ; push ebx ; push 0 ; mov edx,esi ; mov ecx,edi ; call __unguarded_insertion_sort_aux ; pop edi ; pop ebx ; pop esi ; ret 4 ; S: mov edx,[comp] ; push edx ; mov edx,esi ; call __insertion_sort ; pop esi ; ret 4
// SGI-STL `__final_insertion_sort(first, last, comp)`: ranges longer than __stl_threshold (16)
// get `__insertion_sort(first, first + 16, comp)` @ 0x0081C5F0 followed by
// `__unguarded_insertion_sort_aux(first + 16, last, (T*)0, comp)` @ 0x0081A9E0; shorter
// ones a single `__insertion_sort`. The comparator is a 4-byte functor passed by value. /Gr TU
// (__fastcall: first in ecx, last in edx, comp on the stack). The manifest label (`make_heap`)
// is propagated; the body is SGI's. One of 25 bodies of this shape.
struct SElement12 {
    unsigned char _body[0xc];
};

struct SCompare {                        // 4-byte comparison functor passed by value
    void* m_context;
};

void __fastcall InsertionSort_0081c5f0(SElement12* first, SElement12* last, SCompare comp);   // 0x0081C5F0
void __fastcall UnguardedInsertionSortAux_0081a9e0(SElement12* first, SElement12* last, SElement12* valueType, SCompare comp);   // 0x0081A9E0

void __fastcall FinalInsertionSort_SElement12_0081d400(SElement12* first, SElement12* last, SCompare comp)
{
    if (last - first > 16) {
        InsertionSort_0081c5f0(first, first + 16, comp);
        UnguardedInsertionSortAux_0081a9e0(first + 16, last, (SElement12*)0, comp);
    }
    else
        InsertionSort_0081c5f0(first, last, comp);
}
