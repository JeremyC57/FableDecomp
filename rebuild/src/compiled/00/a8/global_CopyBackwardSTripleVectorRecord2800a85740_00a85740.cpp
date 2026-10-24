// _global::CopyBackward_STripleVectorRecord28_00a85740(STripleVectorRecord28*, STripleVectorRecord28*, STripleVectorRecord28*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 40-byte records of two vectors and a vector map, returns the moved-back result
// Leaf: CopyBackward_STripleVectorRecord28_00a85740
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85740, length 97 bytes.
// Callees: member operator= @ 0x00A80EE0 (+0), 0x00A83D70 (+0xC), 0x00A84940 (+0x18).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// Element: 40 bytes (0x66666667 multiply + `sar 4` = / 40). The implicit operator= calls three out-of-line
// 12-byte member assignments (+0, +0xC, +0x18), skips the byte at +0x24 and moves the byte at +0x25: the third
// member is a CVectorMap (vector base + EMPTY Compare at +0xC + bool Dirty at +0xD, GOTCHAS 00a85360) whose implicit
// operator= assigns the vector, copies nothing for the empty comparator and moves Dirty. The record pads to 0x28.
// The manifest label pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> (12 bytes) is propagated.
struct SVector0C_0 {                             // 12 bytes, operator= @ 0x00A80EE0
    long m_words[3];
    SVector0C_0& operator=(const SVector0C_0& other);
};

struct SVector0C_C {                             // 12 bytes, operator= @ 0x00A83D70
    long m_words[3];
    SVector0C_C& operator=(const SVector0C_C& other);
};

struct SVector0C_18 {                            // 12 bytes, operator= @ 0x00A84940
    long m_words[3];
    SVector0C_18& operator=(const SVector0C_18& other);
};

struct SKeyLess { };                             // empty comparator

struct CVectorMap18 : SVector0C_18 {             // CVectorMap: vector base + Compare + Dirty, 14 bytes
    SKeyLess Compare;                            // +0x0C
    bool     Dirty;                              // +0x0D
};

struct STripleVectorRecord28 {                   // 40 bytes
    SVector0C_0  m_first;                        // +0x00
    SVector0C_C  m_second;                       // +0x0C
    CVectorMap18 m_map;                          // +0x18
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

STripleVectorRecord28* __fastcall CopyBackward_STripleVectorRecord28_00a85740(STripleVectorRecord28* first, STripleVectorRecord28* last, STripleVectorRecord28* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}