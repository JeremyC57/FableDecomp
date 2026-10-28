// _global::CopyBackward_SListRecord14_00a37a80(SListRecord14* first, SListRecord14* last, SListRecord14* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 20-byte {4-byte list, long, byte, byte, long, CCharString} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SListRecord14_00a37a80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37A80, length 103 bytes (+ int3 padding), `ret 0Ch`.
// Callees: the leading member's operator= @ 0x00A37940 (catalog "List_CopyOrMoveNodes"), CCharString::operator= @ 0x0099EFB0.
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the out-of-line assignment at +0, the word +4, the bytes +8/+9, the word +0xC, the CCharString +0x10.
// Label correction: the catalog's `_Copy_backward<CSymbolMap::CSymbolInfo*>` is propagated; member names are descriptive.
class CCharString { public:
    void* m_pData;                               // +0x00
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SList_00a37940 {                         // 4 bytes
    void* m_pHead;
    SList_00a37940& operator=(const SList_00a37940& other);   // 0x00A37940
};
struct SListRecord14 {                          // 20 bytes
    SList_00a37940 m_list;                       // +0x00
    long          m_value;                       // +0x04
    unsigned char m_flagA;                       // +0x08
    unsigned char m_flagB;                       // +0x09
    long          m_extra;                       // +0x0c
    CCharString   m_name;                        // +0x10
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SListRecord14* __fastcall CopyBackward_SListRecord14_00a37a80(SListRecord14* first, SListRecord14* last, SListRecord14* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}