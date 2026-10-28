// _global::__linear_insert(SNamedRecord* first, SNamedRecord* last, SNamedRecord value, SNameLess comp) -- SGI insertion-sort step over string-keyed 0x38-byte records: if `value` sorts before the first element, shift [first, last) up one slot and put it first, otherwise sink it from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A70FF0, 140 bytes (`ret 3Ch`: the 0x38-byte value and the comparator on the stack;
//   first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CEnvironmentSpellEffectInterface::SetRadialBlurParams" is propagated and wrong.
// Callees: CCharString::operator< @ 0x009E6530 (manifest "FastCompare"), __copy_backward(first, last, result,
//   random_access_iterator_tag, Distance*) @ 0x00A6E840 (manifest "_Copy_backward<C3DGeneratorObject*>" is propagated; its
//   0x92492493 / sar 5 divide is the 56-byte stride), CCharString::operator= @ 0x0099EFB0, CCharString::CCharString(const
//   CCharString&) @ 0x0099EC30, __unguarded_linear_insert(last, value, comp) @ 0x00A6F070 (manifest
//   "std_fill_StringDummyObjectPair_Radial"), CCharString::~CCharString @ 0x0099EAE0 (the by-value parameter, destroyed by
//   the callee on both paths).
// Element: STLport pair<CCharString, SRecordData> (0x38 bytes, the 52-byte second a POD copied with `rep movsd`); the
//   comparator is an empty functor ordering the names.
// Shape: the landed 0x0081BDA0 template with a class key: the copy_backward front end is inlined (its empty iterator tag a
//   const-ref temporary homed in the dead comparator slot, a null Distance*); the record's implicit assignment is the
//   string operator= plus a block copy; on the other branch the comparator is pushed and STLport's explicit pair copy
//   constructor rebuilds the record in the argument area.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const;     // 0x009E6530
};

struct SRecordData {                              // 52 bytes, POD
    long m_words[13];
};

struct SNamedRecord {                             // STLport std::pair<CCharString, SRecordData>, 0x38 bytes
    CCharString first;                            // +0x00 key
    SRecordData second;                           // +0x04
    SNamedRecord(const SNamedRecord& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

struct SNameLess {
    bool operator()(const SNamedRecord& a, const SNamedRecord& b) const { return a.first < b.first; }
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SNamedRecord* __fastcall __copy_backward(SNamedRecord* first, SNamedRecord* last, SNamedRecord* result,
                                         const SRandomAccessIteratorTag& category, long* distance);   // 0x00A6E840
void __fastcall __unguarded_linear_insert(SNamedRecord* last, SNamedRecord value, SNameLess comp);   // 0x00A6F070

static inline SNamedRecord* copy_backward(SNamedRecord* first, SNamedRecord* last, SNamedRecord* result)
{
    return __copy_backward(first, last, result, SRandomAccessIteratorTag(), (long*)0);
}

void __fastcall __linear_insert(SNamedRecord* first, SNamedRecord* last, SNamedRecord value, SNameLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}