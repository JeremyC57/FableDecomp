// _global::__linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp) -- SGI insertion-sort step: if `value` sorts before the first element, shift [first, last) up one slot and put it first, otherwise sink it from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720CB0, length 121 bytes (`ret 1Ch`: the 24-byte value and the comparator on
// the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CDateAndTime::CDateAndTime" is propagated over byte-identical skeletons and wrong.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, Distance*) @ 0x0071EDD0 (manifest
// "_Move_backward<pair<unsigned __int64, CPersistInfo>*>" is propagated), __unguarded_linear_insert(last, value, comp)
// @ 0x0071F5E0 (`ret 1Ch`, 24-byte records keyed by their first long), the record's 16-byte tail copy constructor
// @ 0x005BA48A (manifest "CCompositeBlendDesc::CCompositeBlendDesc" is propagated across byte-identical bodies).
// Element: 24 bytes {long key; long value; a 16-byte tail with a user copy constructor}; the comparator is an empty
// functor comparing the signed keys (`cmp ebx,[esi] ; jge`).
// Shape: the copy_backward front end is inlined: its empty iterator tag is a const-ref temporary homed in the dead
// comparator slot (`lea eax,[esp+2Ch] ; push eax`) with a null Distance*; the record's implicit assignment copies the six
// words from the argument slots; on the other branch the by-value record is rebuilt in the argument area (key and value
// words, then the tail's out-of-line copy constructor) after the comparator is pushed.
// The tail's copy constructor gets a same-TU `auto_inline(off)` stand-in body that writes only *this. VC7.1 then knows
//   value.first survives it and reads the key once into ebx, as retail (an extern declaration re-reads the argument slot).
struct SRecordTail {                              // 16 bytes, user copy constructor (out of line)
    long          m_00;
    long          m_04;
    unsigned char m_bytes[8];
    SRecordTail(const SRecordTail& other);        // 0x005BA48A
};

#pragma auto_inline(off)
SRecordTail::SRecordTail(const SRecordTail& other)   // 0x005BA48A (same-TU stand-in: writes only *this)
    : m_00(other.m_00), m_04(other.m_04)
{
    for (int i = 0; i < 8; ++i)
        m_bytes[i] = other.m_bytes[i];
}
#pragma auto_inline(on)
struct SRecordInfo {                              // 20 bytes
    long        value;                            // +0x00
    SRecordTail tail;                             // +0x04
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

SKeyedRecord* __fastcall __copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result,
                                         const SRandomAccessIteratorTag& category, long* distance);   // 0x0071EDD0
void __fastcall __unguarded_linear_insert(SKeyedRecord* last, SKeyedRecord value, SKeyLess comp);   // 0x0071F5E0

static inline SKeyedRecord* copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result)
{
    return __copy_backward(first, last, result, SRandomAccessIteratorTag(), (long*)0);
}

void __fastcall __linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}