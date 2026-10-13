// SGI sort<SElement12*, Compare>(12-byte elements) -- introsort loop @ 0x006BB040 to depth 2*lg(n), then the final insertion sort @ 0x006BAF40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB140, oracle length 82 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 12) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push ebp ; mov ebp,[comp] ; push ebp ; lea eax,[ecx+ecx] ; push eax ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; push ebx ; mov edx,esi ; mov ecx,edi ; call __final_insertion_sort ; pop ebx ; E: pop edi ; pop esi ; ret 4
// SGI-STL `sort(first, last, comp)`: for a non-empty range run `__introsort_loop(first, last,
// (T*)0, __lg(last - first) * 2, comp)` (`__lg` is the shift-count loop) and then
// `__final_insertion_sort` out of line @ 0x006BAF40.
// The comparator is a 4-byte functor passed by value. /Gr TU (__fastcall: first in ecx, last
// in edx, comp on the stack). Labels (`make_heap`, `HeapSort`, `Introsort_TripletPartition`) are
// propagated; the body is SGI's `sort`. One of 15 bodies of this shape.
struct SElement12 {
    unsigned char _body[0xc];
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

void __fastcall IntrosortLoop_006bb040(SElement12* first, SElement12* last, SElement12* valueType, long depthLimit, SCompare comp);   // 0x006BB040


void __fastcall FinalInsertionSort_006baf40(SElement12* first, SElement12* last, SCompare comp);   // 0x006BAF40

void __fastcall Sort_SElement12_006bb140(SElement12* first, SElement12* last, SCompare comp)
{
    if (first != last) {
        IntrosortLoop_006bb040(first, last, (SElement12*)0, Lg(last - first) * 2, comp);
        FinalInsertionSort_006baf40(first, last, comp);
    }
}