// _global::Copy_SListRecord30_00889650(SListRecord30*, SListRecord30*, SListRecord30*, SRandomAccessIteratorTag, long*) -- SGI __copy over 48-byte records holding a vector and a double, returns the advanced result
// Leaf: Copy_SListRecord30_00889650
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889650, length 111 bytes.
// Callees: the member operator= @ 0x00888EE0 (+0x08, 12 bytes; labelled Vector_Reallocate).
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 48
// is computed once and counted down in ebx. The element's implicit operator= runs member by member: dword +0,
// byte +4, the out-of-line assignment of the 12-byte member at +8, dwords +0x14/+0x18, byte +0x1C, the double at
// +0x20 (an x87 `fld/fstp qword` copy) and the dword at +0x28; the double makes the record 8-aligned (0x2C -> 0x30).
// The manifest label (`_Move<pair<unsigned __int64, CPersistInfo>*>`) is propagated.
struct SMember0C_8 {                             // 12 bytes, operator= @ 0x00888EE0
    long m_words[3];
    SMember0C_8& operator=(const SMember0C_8& other);
};

struct SListRecord30 {                           // 48 bytes
    long          m_key;                         // +0x00
    bool          m_flag;                        // +0x04
    SMember0C_8   m_list;                        // +0x08
    long          m_value14;                     // +0x14
    long          m_value18;                     // +0x18
    bool          m_enabled;                     // +0x1C
    double        m_time;                        // +0x20
    long          m_value28;                     // +0x28
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SListRecord30* __fastcall Copy_SListRecord30_00889650(SListRecord30* first, SListRecord30* last, SListRecord30* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}