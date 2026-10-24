// _global::UninitFillN_CharStringRecord38_00a6ee90(SCharStringRecord38*, unsigned long, const SCharStringRecord38&, CRetailAllocator) -- _Uninit_fill_n: placement-copy `count` copies of a 0x38-byte CCharString-keyed record from first, return the advanced pointer
// Leaf: UninitFillN_CharStringRecord38_00a6ee90
// Grade: DIFFER(ours v retail) -- 73v68: the lane strength-reduces &first->m_data into ebp and reloads the value pointer from its slot every iteration; retail keeps the value pointer in ebp and forms `lea esi,[ebp+4]` / `lea edi,[ebx+4]` per iteration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6EE90, length 68 bytes, `ret 8`.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: the landed 0x00640260 `_Uninit_fill_n` skeleton (fastcall: first in ecx, count in edx, the value reference and
// the by-value 1-byte allocator on the stack; `test edx,edx ; jbe` for the unsigned count). Every iteration is a
// placement new -- VC null-tests the placement address -- of the record's implicit copy constructor: the CCharString
// copy constructor, then the 52-byte POD tail as a 13-dword `rep movsd`. The copy constructor call and the block move
// need ecx/esi/edi, so the count lives in the slot reserved by the leading `push ecx`.
// Label correction: the catalog's `_Cons_val<allocator<pair<CWideString,CChecksumCacheFile::CEntry>>>` is propagated
// (the element holds a CCharString, 0x0099EC30, not a CWideString). The record is modelled by its size.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct SRecordTail34 {                           // 52 bytes, POD
    long m_words[13];
};
struct SCharStringRecord38 {                     // 0x38 bytes
    CCharString m_name;                          // +0x00
    SRecordTail34 m_data;                        // +0x04
};
struct CRetailAllocator { };

SCharStringRecord38* __fastcall UninitFillN_CharStringRecord38_00a6ee90(SCharStringRecord38* first, unsigned long count, const SCharStringRecord38& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SCharStringRecord38(value);
    return first;
}
