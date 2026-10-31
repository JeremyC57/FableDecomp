// CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) -- find the compressed block holding a definition, inflate it into the static 32 KB decompression cache unless that block is already cached, and point the stream at the definition's bytes (bounded by its offset table entry and the next one or the block end)
// Grade: DIFFER(231v231) -- names now PDB-exact (protected const, stream by reference, CDecompressionCache static, Initialise / GetRequiredWorkspaceSize); residue: ebx/ebp swap (retail block offset in ebx, the next entry's first index in ebp) and the compressed-data base added in eax (ours ecx). Tried this pass: compressed size inline, long block offset, named next-first local, if/else end bound; earlier: count before compressedSize, blockOffset before blockFirst.
// Leaf: ?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD5F0, length 231 bytes (ret 8).
// Name: FableWin.pdb / Ego_r.pdb `?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z`
//   (protected const, the stream by reference).
// Callees: CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<..> >::UpperBound(const unsigned long&) const
//   0x009B1FF0 (CompressedOffsets), GFGetTime 0x009E1BC0 (twice, result dropped: a timing probe),
//   CCompressorZlib::GetRequiredWorkspaceSize 0x00A3A7D0 (`xor eax,eax ; ret`), CACompressor::CACompressor(long)
//   0x00A3AEA0, CCompressorZlib::Initialise 0x00A3AA10, InternalDecompress 0x00A3AB00, ~CCompressorZlib 0x00A3ACC0
//   (vtable 0x01245974), CMemoryDataInputStream::OpenMemorySource(const void*, unsigned long) 0x00994570.
// Globals: CDefinitionManager::DecompressionCache 0x0138E190 (FableWin `dynamic initializer for
//   'CDefinitionManager::DecompressionCache'`; CDecompressionCache {Owner, CachedOffset, BufferSize, Buffer[0x8000]} whose
//   buffer head is the ushort offset table).
// Layout: CompressedDefData +0x88 (CArray<unsigned char>, begin pointer), CompressedOffsets +0x94 (sorted {first def index,
//   compressed offset} pairs) as engine/CDefinitionManager.h (retail widths; opaque bytes there, so a local model).
// Shape: the decompressor is a block-scoped CCompressorZlib local (inline constructor: base ctor, own vtable, Initialise;
//   out-of-line destructor); the two GFGetTime calls bracket the inflate; the end bound is a ternary over the next entry.
double __fastcall GFGetTime();                  // 0x009E1BC0

class CACompressor { public:                    // 0x1c bytes in retail
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);           // 0x00A3AEA0
};
class CCompressorZlib : public CACompressor {   // vtable 0x01245974, retail 0x58 bytes
public:
    CCompressorZlib() : CACompressor(GetRequiredWorkspaceSize()) { Initialise(); }
    virtual ~CCompressorZlib();                 // 0x00A3ACC0
    static unsigned long __fastcall GetRequiredWorkspaceSize();   // 0x00A3A7D0
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
private:
    void Initialise();                          // 0x00A3AA10
    unsigned char _zlib[0x58 - 0x1c];
};

class CMemoryDataInputStream { public:
    void OpenMemorySource(const void* source, unsigned long size);   // 0x00994570
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    const_iterator UpperBound(const K& key) const;            // 0x009B1FF0
};

class CDefinitionManager {
public:
    class CDecompressionCache {
    public:
        const CDefinitionManager* Owner;        // +0x0
        long                      CachedOffset; // +0x4
        unsigned long             BufferSize;   // +0x8
        unsigned char             Buffer[0x8000];   // +0xc
    };
protected:
    void DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) const;
private:
    static CDecompressionCache DecompressionCache;   // 0x0138E190
    unsigned char  _pad000[0x88];
    CArray<unsigned char> CompressedDefData;    // +0x88
    CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> > CompressedOffsets;   // +0x94
};

void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) const
{
    CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> >::const_iterator next =
        CompressedOffsets.UpperBound(defIndex);
    unsigned long blockFirst = next[-1].first;
    unsigned long blockOffset = next[-1].second;
    unsigned long compressedSize = next->second - blockOffset;
    unsigned long count = next->first - blockFirst;
    if (DecompressionCache.Owner != this || DecompressionCache.CachedOffset != (long)blockOffset) {
        GFGetTime();
        CCompressorZlib compressor;
        DecompressionCache.BufferSize = compressor.InternalDecompress(CompressedDefData._M_start + blockOffset, compressedSize,
                                                                      DecompressionCache.Buffer, 0x8000);
        DecompressionCache.Owner = this;
        DecompressionCache.CachedOffset = blockOffset;
        GFGetTime();
    }
    const unsigned short* table = (const unsigned short*)DecompressionCache.Buffer;
    unsigned long local = defIndex - blockFirst;
    unsigned long start = table[local];
    unsigned long end = (local + 1 < count) ? table[local + 1] : DecompressionCache.BufferSize;
    stream.OpenMemorySource(DecompressionCache.Buffer + start, end - start);
}
