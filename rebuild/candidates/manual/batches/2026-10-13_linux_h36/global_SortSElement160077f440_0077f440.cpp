// SGI sort<SElement16*, Compare>(16-byte elements) -- introsort loop @ 0x0077F370 to depth 2*lg(n), then the final insertion sort (inlined: threshold 16)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F440, oracle length 115 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 16) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push ebp ; mov ebp,[comp] ; push ebp ; lea eax,[ecx+ecx] ; push eax ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; cmp esi,10h ; mov ecx,edi ; push ebp ; jle S ; lea esi,[edi+0x100] ; mov edx,esi ; call __insertion_sort ; push ebp ; push 0 ; mov edx,ebx ; mov ecx,esi ; call __unguarded_insertion_sort_aux ; ... ret 4 ; S: mov edx,ebx ; call __insertion_sort ; ... ret 4
// SGI-STL `sort(first, last, comp)`: for a non-empty range run `__introsort_loop(first, last,
// (T*)0, __lg(last - first) * 2, comp)` (`__lg` is the shift-count loop) and then
// `__final_insertion_sort`: inlined here -- ranges longer than __stl_threshold (16) get `__insertion_sort(first, first + 16, comp)` followed by `__unguarded_insertion_sort_aux(first + 16, last, (T*)0, comp)`, shorter ones a single `__insertion_sort`.
// The comparator is a 4-byte functor passed by value. /Gr TU (__fastcall: first in ecx, last
// in edx, comp on the stack). Labels (`make_heap`, `HeapSort`, `Introsort_TripletPartition`) are
// propagated; the body is SGI's `sort`. One of 22 bodies of this shape.
struct SElement16 {
    unsigned char _body[0x10];
};

struct SCompare {                        // 4-byte comparison functor passed by value
    void* m_context;
};

template<class Size> inline Size Lg(Size n)
{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}

void __fastcall IntrosortLoop_0077f370(SElement16* first, SElement16* last, SElement16* valueType, long depthLimit, SCompare comp);   // 0x0077F370
void __fastcall InsertionSort_0077ecb0(SElement16* first, SElement16* last, SCompare comp);   // 0x0077ECB0
void __fastcall UnguardedInsertionSortAux_0077ea10(SElement16* first, SElement16* last, SElement16* valueType, SCompare comp);   // 0x0077EA10
__forceinline void FinalInsertionSort(SElement16* first, SElement16* last, SCompare comp)
{
    if (last - first > 16) {
        InsertionSort_0077ecb0(first, first + 16, comp);
        UnguardedInsertionSortAux_0077ea10(first + 16, last, (SElement16*)0, comp);
    }
    else
        InsertionSort_0077ecb0(first, last, comp);
}
void __fastcall Sort_SElement16_0077f440(SElement16* first, SElement16* last, SCompare comp)
{
    if (first != last) {
        IntrosortLoop_0077f370(first, last, (SElement16*)0, Lg(last - first) * 2, comp);
        FinalInsertionSort(first, last, comp);
    }
}
