// CDataOutputStream::WriteCompressed(const void* data, long size) -- write a block LZO1X-999 compressed when that saves space: a 2-byte header (0 = stored raw, the compressed length, or 0xFFFF followed by a 4-byte length), the compressed bytes and the last 3 bytes raw; blocks of 1..3 bytes are always stored raw
// Leaf: ?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z
// Grade: RELOCATION_MATCH -- STLport _Vector_base shape: zeroed members, start = allocate, finish = start, end = start +
//   n, then finish = uninitialized_fill_n(_M_start) where the fill memsets and returns first + n
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996610, 464 bytes (incl. int3 padding), thiscall `ret 8`. FableWin.pdb
//   `?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z`. The catalog's "PersistData_CompressAndVerify" is descriptive.
// Callees: CArray<unsigned char>(unsigned int) 0x00514E20 (the compressed buffer; the constant-size work memory's
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

// NF93 (q11297): the local STLport vector model replaced by engine/CArray.h: both buffers are CArray<unsigned char> (Ego_d types
//   WriteCompressed's work_mem / compressed_data so; FableWin ??0?$CArray@E@@QAE@I@Z 0x00514E20, called for the compressed buffer, expanded
//   in place for the constant-size work memory); GetAsCArray is the start, or the array's own address when empty.
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"

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
        CArray<unsigned char> compressed(compressedSize);
        CArray<unsigned char> workMemory(LZO1X_999_MEM_COMPRESS);
        lzo1x_999_compress((const unsigned char*)data, size - 3, compressed.GetAsCArray(), &compressedSize, workMemory.GetAsCArray());
        unsigned long checkSize = size;
        lzo1x_decompress(compressed.GetAsCArray(), compressedSize, (unsigned char*)data, &checkSize, 0);
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
            WriteData(compressed.GetAsCArray(), compressedSize);
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