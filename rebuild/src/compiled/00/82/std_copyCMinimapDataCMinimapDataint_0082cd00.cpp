// _global::Copy_SResourceRecord3C_0082cd00(SResourceRecord3C*, SResourceRecord3C*, SResourceRecord3C*, SRandomAccessIteratorTag, long*) -- SGI __copy over 60-byte records with four out-of-line member assignments, returns the advanced result
// Leaf: Copy_SResourceRecord3C_0082cd00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082CD00, length 127 bytes.
// Callees (member operator=, labels are hints): 0x0082C980 (+0x08, 12 bytes), 0x009F2E70 CSurface-labelled (+0x14,
// 16 bytes), 0x009FA1C0 CTexture-labelled (+0x24, 8 bytes), 0x0045BAAA (+0x30, 12 bytes).
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 60
// (0x88888889 multiply + add + `sar 5`) is computed once and counted down in ebx. The element's implicit operator=
// runs member by member in declaration order: two dwords, three out-of-line class assignments, the dword at +0x2C
// (its move scheduled into the last call's argument setup) and a fourth out-of-line assignment at +0x30. Member
// sizes follow from the offsets. The manifest label (`_Move<pair<unsigned __int64, CPersistInfo>*>`) is propagated.
struct SMember0C_8 {                             // 12 bytes, operator= @ 0x0082C980
    long m_words[3];
    SMember0C_8& operator=(const SMember0C_8& other);
};

struct SMember10_14 {                            // 16 bytes, operator= @ 0x009F2E70
    long m_words[4];
    SMember10_14& operator=(const SMember10_14& other);
};

struct SMember08_24 {                            // 8 bytes, operator= @ 0x009FA1C0
    long m_words[2];
    SMember08_24& operator=(const SMember08_24& other);
};

struct SMember0C_30 {                            // 12 bytes, operator= @ 0x0045BAAA
    long m_words[3];
    SMember0C_30& operator=(const SMember0C_30& other);
};

struct SResourceRecord3C {                       // 60 bytes
    unsigned long m_key0;                        // +0x00
    unsigned long m_key4;                        // +0x04
    SMember0C_8   m_list;                        // +0x08
    SMember10_14  m_surface;                     // +0x14
    SMember08_24  m_texture;                     // +0x24
    long          m_value;                       // +0x2C
    SMember0C_30  m_tail;                        // +0x30
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SResourceRecord3C* __fastcall Copy_SResourceRecord3C_0082cd00(SResourceRecord3C* first, SResourceRecord3C* last, SResourceRecord3C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}