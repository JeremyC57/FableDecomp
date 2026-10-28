// CExpressionSet::TransferBinaryIn(CMemoryDataInputStream&) -- read the entry count, resize the expression vector-map with default entries, read the entries as one raw block, re-sort the map if it was marked dirty, then skip the trailing dword
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081B510, length 406 bytes, `ret 4`.
// Callees: CArray<CEntry>::resize(unsigned long, const CEntry&) @ 0x0081D980, std::sort<CEntry*, CKeyPairCompareLess>
//   @ 0x0081E090 (__fastcall: first, last, comparator by value), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: PDB CExpressionSet = CVectorMap<EExpressionType, CExpressionSet::CEntry, CKeyPairCompareLess<..> > with the
//   retail 12-byte vector at +0, Compare +0xc, Dirty +0xd; CEntry is 0x28 bytes (Type +0, the default entry has -1 at
//   +0x10, offsets read from the retail body). CDataInputStream inline Read as in the landed 0x00B16F60.
// Shape: the Read test is written `size <= remaining` (cmp size,[remaining] ; jg); the data pointer (GetData select) is
//   taken before the byte count; the trailing dword is read back into the count local (one shared argument slot).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else {
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
        }
    }
};

enum EExpressionType { EXPRESSION_NONE = 0 };

struct SExpressionData {
    long Values[9];
};

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const V& a, const V& b) const { return a.Type < b.Type; }
};

class CExpressionSet { public:
    class CEntry { public:
        EExpressionType Type;                           // +0x00
        SExpressionData Data;                           // +0x04
        CEntry() : Type(EXPRESSION_NONE), Data() { Data.Values[3] = -1; }
    };
    template <class T> struct CArray {
        T* First;
        T* Last;
        T* End;
        unsigned long size() const { return Last - First; }
        T* GetData() { return size() > 0 ? First : (T*)this; }
        void resize(unsigned long count, const T& value);   // 0x0081D980
    };
    typedef CKeyPairCompareLess<EExpressionType, CEntry> CCompare;
    CArray<CEntry> Expressions;                         // +0x00
    CCompare       Compare;                             // +0x0c
    bool           Dirty;                               // +0x0d
    void TransferBinaryIn(CMemoryDataInputStream& in);
};

void __fastcall SortEntries(CExpressionSet::CEntry* first, CExpressionSet::CEntry* last, CExpressionSet::CCompare compare);   // 0x0081E090

void CExpressionSet::TransferBinaryIn(CMemoryDataInputStream& in)
{
    long count;
    in.Read(&count, 4);
    Expressions.resize(count, CEntry());
    CEntry* data = Expressions.GetData();
    long bytes = Expressions.size() * sizeof(CEntry);
    if (bytes > 0)
        in.Read(data, bytes);
    if (Dirty) {
        SortEntries(Expressions.First, Expressions.Last, Compare);
        Dirty = false;
    }
    in.Read(&count, 4);
}