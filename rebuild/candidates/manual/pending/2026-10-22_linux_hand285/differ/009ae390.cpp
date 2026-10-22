// Grade: DIFFER(595v600) -- frame/size now 0x8c; remaining: this held in ebp (retail spills it to [esp+0x20] only), slot order of this v compressedSize, fill-char byte at +0x10 v +0x13, scratch-buffer memset count copy (ebp v esi)
// CEngineInternalPrimitiveBase::CompressPrimitive(CMemoryDataOutputStream& vertices, vector<unsigned short>& indices, long key) -- append one compressed primitive block: rebase the indices, serialise indices then vertex bytes into a memory stream, zlib-compress it into a scratch buffer (stream size + 1 KB), append the result to the compressed data and record {key, offset} in the chunk map; the inputs are cleared
// Leaf: CompressPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AE390, length 600 bytes (`ret 0Ch`). The manifest class is doubtful (data
//   vector +0x88, chunk map +0x94 with its sort flag +0xa1).
// Callees: default compression level 0x00A3A7D0 (returns 0), CCompressor ctor 0x00A3AEA0, CCompressorZlib::Init 0x00A3AA10 /
//   InternalCompress 0x00A3AA40 / dtor 0x00A3ACC0 (vtable 0x01245974), CMemoryDataOutputStream ctor 0x00996AF0 / Write
//   0x00996CC0 / GetSize 0x00994B40 / PeekData 0x00995E70 / Reset 0x00996B60 / dtor 0x00996B30, malloc 0x00BFEA0E,
//   memmove 0x00BFEAE6, vector<char>::_M_fill_insert 0x00485F3A, vector<pair<long,long>>::_M_insert_overflow 0x009B6300, free.
#include <stdlib.h>
#include <string.h>
long __cdecl GetDefaultCompressionLevel();         // 0x00A3A7D0
struct CCompressor {
    virtual ~CCompressor();
    unsigned char _body[0x54];
    CCompressor(long level);                       // 0x00A3AEA0
};
struct CCompressorZlib : CCompressor {             // vtable 0x01245974
    CCompressorZlib(long level) : CCompressor(level) { Init(); }
    virtual ~CCompressorZlib();                    // 0x00A3ACC0
    void Init();                                   // 0x00A3AA10
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AA40
};
struct CMemoryDataOutputStream {
    unsigned char _body[0x18];
    CMemoryDataOutputStream();                     // 0x00996AF0
    ~CMemoryDataOutputStream();                    // 0x00996B30
    void Write(const void* data, long size);       // 0x00996CC0
    long GetSize();                                // 0x00994B40
    void* PeekData();                              // 0x00995E70
    void Reset();                                  // 0x00996B60
};
inline char* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return (char*)result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}
struct CIndexVector {                              // vector<unsigned short>
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    unsigned short* GetData() { return size() > 0 ? _M_start : (unsigned short*)this; }
    void clear() { _M_finish = (unsigned short*)CopyTrivial(_M_finish, _M_finish, _M_start); }
};
struct CByteVector {                               // vector<char>
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    char* GetData() { return size() ? _M_start : (char*)this; }
    void _M_fill_insert(char* pos, unsigned long n, const char& x);   // 0x00485F3A
    void resize(unsigned long n, char x = 0)
    {
        if (n < size())
            _M_finish = CopyTrivial(_M_finish, _M_finish, _M_start + n);
        else
            _M_fill_insert(_M_finish, n - size(), x);
    }
};
struct CScratchBuffer {                            // vector<char>(n, 0)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CScratchBuffer(unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        char* p = 0;
        if (n)
            p = (char*)malloc(n);
        _M_start = p;
        _M_finish = p;
        _M_end_of_storage = p + n;
        _M_finish = Fill(_M_start, n);
    }
    static char* Fill(char* p, unsigned long n) { memset(p, 0, n); return p + n; }
    ~CScratchBuffer() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    char* GetData() { return size() ? _M_start : (char*)this; }
};
struct CChunk { long Key; unsigned long Offset; CChunk(long k, unsigned long o) : Key(k), Offset(o) { } };
struct __true_type { __true_type() { } };
struct CChunkMap {
    CChunk* _M_start;
    CChunk* _M_finish;
    CChunk* _M_end_of_storage;
    unsigned char Compare;                         // +0x0c
    bool          NeedsSort;                       // +0x0d
    void _M_insert_overflow(CChunk* pos, const CChunk& x, const __true_type&, unsigned long n, bool atEnd);   // 0x009B6300
    void push_back(const CChunk& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) { long k = x.Key; unsigned long o = x.Offset; _M_finish->Key = k; _M_finish->Offset = o; }
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    void Insert(const CChunk& x) { push_back(x); NeedsSort = true; }
};
struct CEngineInternalPrimitiveBase {
    unsigned char _pad00[0x88];
    CByteVector   CompressedData;                  // +0x88
    CChunkMap     Chunks;                          // +0x94
    void CompressPrimitive(CMemoryDataOutputStream& vertices, CIndexVector& indices, long key);
};

void CEngineInternalPrimitiveBase::CompressPrimitive(CMemoryDataOutputStream& vertices, CIndexVector& indices, long key)
{
    CCompressorZlib compressor(GetDefaultCompressionLevel());
    CMemoryDataOutputStream stream;
    unsigned short indexBytes = (unsigned short)(indices.size() * 2);
    for (unsigned long i = 0; i < indices.size(); ++i)
        indices._M_start[i] += indexBytes;
    stream.Write(indices.GetData(), indices.size() * 2);
    stream.Write(vertices.PeekData(), vertices.GetSize());
    CScratchBuffer buffer(stream.GetSize() + 0x400);
    unsigned long compressedSize = compressor.InternalCompress(stream.PeekData(), stream.GetSize(), buffer.GetData(), buffer.size());
    unsigned long offset = CompressedData.size();
    CompressedData.resize(offset + compressedSize);
    memcpy(CompressedData._M_start + offset, buffer.GetData(), compressedSize);
    Chunks.Insert(CChunk(key, offset));
    vertices.Reset();
    indices.clear();
}
