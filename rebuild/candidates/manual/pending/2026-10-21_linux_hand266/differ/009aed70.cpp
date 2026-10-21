// Grade: DIFFER(ours v retail) -- 499v492: retail keeps OriginalParentDefIndex in edi (preloaded from the dead stream-argument slot it shares with the count, frame 0x1c, ebx as read temp); we keep all three record values in frame slots (frame 0x20) -- tried block-scoped count, loop-carried/function-scope/union locals
// CParentDefClassBase::InternalLoadBinary(CDataInputStream& in) -- load the CDefClassBase flags, then the 16-bit sub-def count and that many (key, DefIndex, OriginalParentDefIndex) records appended to SubDefInfoMap; the map is left marked clean
// Leaf: ?InternalLoadBinary@CParentDefClassBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AED70, length 496 bytes (490 + int3 padding), virtual, `ret 4`.
// Callees: CDefClassBase::InternalLoadBinary 0x009ACB20 (landed), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   vector<pair<unsigned long, CSubDefInfo>>::reserve 0x009B4D80 and ::_M_insert_overflow 0x009B60D0 (manifest labels
//   "Deque_Reserve" / "Vector_InsertStructArray_12byte" are propagated).
// Layout (retail, def classes byte packed): SubDefInfoMap CVectorMap at +0x29 = STLport vector (12 bytes) + Compare (+0x35)
//   + Dirty (+0x36); elements are pair<unsigned long, CDefClassBase::CSubDefInfo{DefIndex, OriginalParentDefIndex}> (12 bytes).
//   engine/CParentDefClassBase.h is the PDB view (16-byte vectors), so the base is modelled locally.
// Shape: the stream Read inline of the landed CDefClassBase::InternalLoadBinary 0x009ACB20; the count is an unsigned short
//   homed in the dead argument slot; each record is three reads into locals, a pair temporary and the inline STLport
//   push_back (placement copy / _M_insert_overflow with the __true_type tag), with Dirty set per add and cleared at the end.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};

struct CSubDefInfo {
    long DefIndex;
    long OriginalParentDefIndex;
    CSubDefInfo(long defIndex, long originalParentDefIndex) : DefIndex(defIndex), OriginalParentDefIndex(originalParentDefIndex) { }
};
struct CSubDefInfoPair {                                 // std::pair<unsigned long, CSubDefInfo>
    unsigned long first;
    CSubDefInfo   second;
    CSubDefInfoPair(const unsigned long& a, const CSubDefInfo& b) : first(a), second(b) { }
};
struct __true_type { __true_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CSubDefInfoVector {                               // STLport vector<pair<unsigned long, CSubDefInfo>>, 12 bytes
    CSubDefInfoPair* _M_start;
    CSubDefInfoPair* _M_finish;
    CSubDefInfoPair* _M_end_of_storage;
    void reserve(unsigned long n);                                                                    // 0x009B4D80
    void _M_insert_overflow(CSubDefInfoPair* position, const CSubDefInfoPair& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x009B60D0
    void push_back(const CSubDefInfoPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CSubDefInfoPair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CSubDefInfoMap {                                  // CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<..>>
    CSubDefInfoVector Data;                              // +0x00
    unsigned char     Compare;                           // +0x0c
    bool              Dirty;                             // +0x0d
    void reserve(unsigned long n) { Data.reserve(n); }
    void Add(const CSubDefInfoPair& x) { Data.push_back(x); Dirty = true; }
};

struct CDefClassBase {
    virtual void InternalLoadBinary(CDataInputStream& in);    // 0x009ACB20
};
#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
    unsigned char  _pad04[0x29 - 0x04];                  // CDefClassBase body, InstantiationName (+0x25)
    CSubDefInfoMap SubDefInfoMap;                        // +0x29
    virtual void InternalLoadBinary(CDataInputStream& in);
};
#pragma pack(pop)

void CParentDefClassBase::InternalLoadBinary(CDataInputStream& in)
{
    CDefClassBase::InternalLoadBinary(in);
    unsigned long count;
    {
        unsigned short storedCount;
        in.Read(&storedCount, sizeof(storedCount));
        count = storedCount;
    }
    SubDefInfoMap.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long key;
        long defIndex;
        long originalParentDefIndex;
        in.Read(&key, sizeof(key));
        in.Read(&defIndex, sizeof(defIndex));
        in.Read(&originalParentDefIndex, sizeof(originalParentDefIndex));
        SubDefInfoMap.Add(CSubDefInfoPair(key, CSubDefInfo(defIndex, originalParentDefIndex)));
    }
    SubDefInfoMap.Dirty = false;
}
