// STLport __insertion_sort<SElement8*, Compare> -- __linear_insert(first, i, T(*i), comp) @ 0x00AE6760 for every element after the first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7110, oracle length 62 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,edx ; mov ebx,ecx ; cmp ebx,edi ; je E ; lea esi,[ebx+0x8] ; cmp esi,edi ; je E ; mov ebp,[comp] ; L: (push comp ; build the 8-byte copy of *i in the argument area) ; mov edx,esi ; mov ecx,ebx ; call __linear_insert ; add esi,0x8 ; cmp esi,edi ; jne L ; E: pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// STLport `__insertion_sort(first, last, comp)`: `if (first != last) for (i = first + 1; i != last;
// ++i) __linear_insert(first, i, T(*i), comp);` -- the value travels BY VALUE (copied into the
// argument area) with the 4-byte comparator functor. /Gr TU (`__fastcall`: first in ecx, last in
// edx, comp on the stack). The manifest label (`_Insertion_sort1`) is propagated. One of 9 bodies.
struct SElement8 {
    long m_00;
    long m_04;
    SElement8(const SElement8& o) : m_00(o.m_00), m_04(o.m_04) { }   // user copy constructor: the by-value argument is built in the argument area
};

struct SCompare {                        // 4-byte comparison functor passed by value
    void* m_context;
};

void __fastcall LinearInsert_00ae6760(SElement8* first, SElement8* last, SElement8 value, SCompare comp);   // 0x00AE6760

void __fastcall InsertionSort_SElement8_00ae7110(SElement8* first, SElement8* last, SCompare comp)
{
    if (first != last)
        for (SElement8* i = first + 1; i != last; ++i)
            LinearInsert_00ae6760(first, i, SElement8(*i), comp);
}