// CDataOutputStream::WriteCompressed(const void* data, long size) -- write a block LZO1X-999 compressed when that saves space: a 2-byte header (0 = stored raw, the compressed length, or 0xFFFF followed by a 4-byte length), the compressed bytes and the last 3 bytes raw; blocks of 1..3 bytes are always stored raw
// Leaf: ?WriteCompressed@CDataOutputStream@@
// Grade: RELOCATION_MATCH -- STLport _Vector_base shape: zeroed members, start = allocate, finish = start, end = start +
//   n, then finish = uninitialized_fill_n(_M_start) where the fill memsets and returns first + n
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996610, 464 bytes (incl. int3 padding), thiscall `ret 8`. FableWin.pdb
//   `?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z`. The catalog's "PersistData_CompressAndVerify" is descriptive.
// Callees: std::vector<unsigned char>(size_type) 0x00514E20 (the compressed buffer; the constant-size work memory's
//   constructor is inlined: malloc 0x00BFEA0E + a 0x1C000-dword rep stosd), lzo1x_999_compress 0x00C08130,
//   lzo1x_decompress 0x00C06B90 (LZO 1.08, `__cdecl`), the stream's Write slot 4 (+0x10), free 0x00BFEA14.
// Shape: the worst-case output size is size * 65 / 64 + 128 (signed divide); only size - 3 bytes are compressed and the
//   result is decompressed back over the source as a check; the header and length temporaries live in the dead
//   argument slots; `push 2` for the first header write is shared by both arms.
#include <string.h>
#pragma intrinsic(memset)
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
extern "C" int __cdecl lzo1x_999_compress(const unsigned char* src, unsigned long srcLength, unsigned char* dst,
                                          unsigned long* dstLength, void* workMemory);   // 0x00C08130
extern "C" int __cdecl lzo1x_decompress(const unsigned char* src, unsigned long srcLength, unsigned char* dst,
                                        unsigned long* dstLength, void* workMemory);     // 0x00C06B90
#define LZO1X_999_MEM_COMPRESS (14 * 16384 * 2)

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
// The POD fill the size constructor ends with: memset's result pointer plus the count (an inline call's return value,
// so it is not shared with the end-of-storage sum computed before the fill).
inline unsigned char* uninitialized_fill_n(unsigned char* __first, unsigned int __n, const unsigned char& __val)
{
    memset(__first, __val, __n);
    return __first + __n;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector
{
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    explicit vector(size_type __n)                      // 0x00514E20 (unsigned char)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = __n ? (_Tp*)malloc(__n) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
        _M_finish = uninitialized_fill_n(_M_start, __n, _Tp());
    }
    ~vector() { if (_M_start != 0) free(_M_start); }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
};
} // namespace std

static inline unsigned char* GetData(std::vector<unsigned char>& v) { return v.size() ? &v[0] : (unsigned char*)&v; }

class CDataOutputStream
{
public:
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const;
    virtual void v03();
    virtual void WriteData(const void* data, long size);   // slot 4 (+0x10)
    void WriteCompressed(const void* data, long size);
};

void CDataOutputStream::WriteCompressed(const void* data, long size)
{
    if (size <= 0)
        return;
    if (size > 3) {
        unsigned long compressedSize = size * 65 / 64 + 128;
        std::vector<unsigned char> compressed(compressedSize);
        std::vector<unsigned char> workMemory(LZO1X_999_MEM_COMPRESS);
        lzo1x_999_compress((const unsigned char*)data, size - 3, GetData(compressed), &compressedSize, GetData(workMemory));
        unsigned long checkSize = size;
        lzo1x_decompress(GetData(compressed), compressedSize, (unsigned char*)data, &checkSize, 0);
        if (compressedSize < (unsigned long)(size - 3)) {
            long header;
            if (compressedSize <= 0x7fff) {
                header = compressedSize;
                WriteData(&header, 2);
            } else {
                header = -1;
                WriteData(&header, 2);
                header = compressedSize;
                WriteData(&header, 4);
            }
            WriteData(GetData(compressed), compressedSize);
            WriteData((const unsigned char*)data + size - 3, 3);
        } else {
            long header = 0;
            WriteData(&header, 2);
            WriteData(data, size);
        }
    } else {
        long header = 0;
        WriteData(&header, 2);
        WriteData(data, size);
    }
}