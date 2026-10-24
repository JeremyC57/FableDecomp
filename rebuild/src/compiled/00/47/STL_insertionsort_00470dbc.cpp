// _STL::__insertion_sort(SULongKeyedRecord* first, SULongKeyedRecord* last, SULongKeyLess comp) -- STLport insertion sort over 12-byte pair<unsigned long, {long, long}> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470DBC, 76 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack); size-optimised frame-pointer TU (`push ebp ; mov ebp,esp ; ... leave`).
// Callee: 0x00470E08 __linear_insert(first, last, value, comp) (manifest "std::sort_insertion_12byte"; only this body
// calls it); it orders on an UNSIGNED key at +0 (`cmp ecx,[ebx] ; jae`) and assigns the 12 bytes with three movsd.
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (propagated); the code is
// STLport's `__insertion_sort`. Same module as the landed 0x00471239 (its __unguarded_insertion_sort_aux sibling).
// Shape: `last` lives in the 4-byte frame word (`[ebp-4]`), the comparator is pushed straight from its slot (`push
// [ebp+8]`), STLport's explicit pair copy constructor builds the by-value `*i` in the argument area with the payload
// read through a second induction pointer (`lea edi,[esi+4]`) -- a nested two-dword `second`, as in 0x0076BB70.
// Frame pointer with esi/edi free: optimize("y", off) + ("s", on), like the module's other landed bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct SPayload8 {                        // second half of the record (POD)
    long m_value0;                        // +0x00
    long m_value1;                        // +0x04
};

struct SULongKeyedRecord {                // std::pair<unsigned long, SPayload8>, 12 bytes
    unsigned long first;                  // +0x00 sort key
    SPayload8 second;                     // +0x04
    SULongKeyedRecord(const SULongKeyedRecord& o) : first(o.first), second(o.second) { }
};

struct SULongKeyLess {                    // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SULongKeyedRecord* first, SULongKeyedRecord* last, SULongKeyedRecord value, SULongKeyLess comp);   // 0x00470E08

void __fastcall __insertion_sort(SULongKeyedRecord* first, SULongKeyedRecord* last, SULongKeyLess comp)
{
    if (first == last)
        return;
    for (SULongKeyedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}