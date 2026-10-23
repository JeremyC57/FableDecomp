// _global::__linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp) -- SGI insertion-sort step: if `value` sorts before the first element, shift [first, last) up one slot and put it first, otherwise sink it from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081BDA0, length 135 bytes (`ret 1Ch`: the 24-byte value and the comparator on
// the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CDateAndTime::CDateAndTime" is propagated over byte-identical skeletons and wrong.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, Distance*) @ 0x00819670 (manifest
// "_Move_backward<pair<unsigned __int64, CPersistInfo>*>" is propagated), __unguarded_linear_insert(last, value, comp)
// @ 0x00819940 (`ret 1Ch`, 24-byte records keyed by their first long, signed).
// Element: STLport pair<long, SRecordInfo> (24 bytes, the 20-byte second a POD); the comparator is an empty functor
// comparing the signed keys (`cmp edi,eax ; jge`).
// Shape: the key is read once into edi and serves the compare, the assignment and the by-value copy; the copy_backward
// front end is inlined (its empty iterator tag a const-ref temporary homed in the dead comparator slot, a null Distance*);
// the record's implicit assignment copies the words from the argument slots; on the other branch STLport's explicit pair
// copy constructor rebuilds the record in the argument area (the key, then the POD second as a struct copy through a
// re-based pointer) after the comparator is pushed.
struct SRecordInfo {                              // 20 bytes, POD
    long m_words[5];
};

struct SKeyedRecord {                             // STLport std::pair<long, SRecordInfo>, 24 bytes
    long        first;                            // +0x00 key
    SRecordInfo second;                           // +0x04
    SKeyedRecord(const SKeyedRecord& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

struct SKeyLess {                                 // CKeyPairCompareLess<long, SRecordInfo>
    bool operator()(const SKeyedRecord& a, const SKeyedRecord& b) const { return a.first < b.first; }
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SKeyedRecord* __fastcall __copy_backward_00819670(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result,
                                         const SRandomAccessIteratorTag& category, long* distance);   // 0x00819670
void __fastcall __unguarded_linear_insert_00819940(SKeyedRecord* last, SKeyedRecord value, SKeyLess comp);   // 0x00819940

static inline SKeyedRecord* copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result)
{
    return __copy_backward_00819670(first, last, result, SRandomAccessIteratorTag(), (long*)0);
}

void __fastcall __linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert_00819940(last, value, comp);
}