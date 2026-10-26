// CRangeCompressor::Decompress(const unsigned char* data, long count, long stride, void* dest) -- undo Compress: a leading 0 byte means the data is stored raw (copied), else read each column's script entry header and unpack its count bit-packed values (unshuffle, set-mask, bias) into dest, stride bytes apart, until the 0x80 end marker; the time spent and the bytes produced are added to the decompression statistics
// Leaf: Decompress
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F39ED0, length 783 bytes (ret 0x10); PDB
// ?Decompress@CRangeCompressor@@QAEXPBEJJPAX@Z.
// Callees: the high-resolution clock 0x009E1BC0 (returns a double) at entry and exit, CalcShuffleOperations
// 0x00F39E50 (landed); data: the decompression time 0x0143E970 (double) and byte count 0x0143E978, the byte-width
// masks 0x013AC944.
// Layout: the stream written by the sibling Compress 0x00F3A2E0.
// Shape: each value is written at `column + j * stride` (VC strength-reduces it into the ebx walking pointer; a named
// `out` pointer local moves the bit accumulator into ebx and spills the flags byte); the flags-bit tests in the value
// loop are hoisted by VC into the three byte locals; the bit-count check is a 64-bit range test of blockSize * 8 that
// returns without closing the timer.
#include <string.h>
double __fastcall GFGetTime();                                   // 0x009E1BC0
extern double g_RangeDecompressTime;                       // 0x0143E970
extern unsigned long g_RangeDecompressBytes;               // 0x0143E978
extern const unsigned long g_RangeCompressorByteMasks[];   // 0x013AC944

struct CRangeCompressor {
    long GetBlockSizeFromFlags(unsigned long flags)
    {
        if (flags & 0x40)
            return 4;
        return (flags & 0x20) ? 2 : 1;
    }
    unsigned long ReadValue(const unsigned char* source, long blockSize)
    {
        if (blockSize == 4)
            return *(const unsigned long*)source;
        if (blockSize == 2)
            return *(const unsigned short*)source;
        return *source;
    }
    void WriteValue(unsigned char* dest, unsigned long value, long blockSize)
    {
        if (blockSize == 4)
            *(unsigned long*)dest = value;
        else if (blockSize == 2)
            *(unsigned short*)dest = (unsigned short)value;
        else
            *dest = (unsigned char)value;
    }
    static bool IsValidBitCount(__int64 bits) { return bits >= 0 && bits <= 0x7fffffff; }
    long CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts);   // 0x00F39E50
    void Decompress(const unsigned char* data, long count, long stride, void* dest);
};

void CRangeCompressor::Decompress(const unsigned char* data, long count, long stride, void* dest)
{
    g_RangeDecompressTime -= GFGetTime();
    g_RangeDecompressBytes += count * stride;
    if (*data == 0) {
        memcpy(dest, data + 1, count * stride);
        g_RangeDecompressTime += GFGetTime();
        return;
    }
    ++data;
    unsigned char* column = (unsigned char*)dest;
    unsigned char flags = *data++;
    while (!(flags & 0x80)) {
        long blockSize = GetBlockSizeFromFlags(flags);
        if (!IsValidBitCount(blockSize * 8))
            return;
        long bits = *data++;
        unsigned long bias = 0;
        long gap = 0;
        unsigned long setMask = 0;
        long runs = 0;
        unsigned long runMasks[16];
        unsigned long shifts[16];
        if (flags & 0x11) {
            bias = ReadValue(data, blockSize);
            data += blockSize;
        }
        if (flags & 2)
            gap = *data++;
        if (flags & 8) {
            setMask = ReadValue(data, blockSize);
            data += blockSize;
        }
        if (flags & 4) {
            runs = CalcShuffleOperations(ReadValue(data, blockSize), runMasks, shifts);
            data += blockSize;
        }
        unsigned long mask = (1 << bits) - 1;
        long used = 32;
        if (bits == 0)
            used = 0;
        unsigned long acc = 0;
        for (long j = 0; j < count; ++j) {
            unsigned long value;
            if (bits == 32) {
                value = *(const unsigned long*)data;
                data += 4;
            } else {
                if (used == 32) {
                    acc = *(const unsigned long*)data;
                    data += 4;
                    used = 0;
                }
                value = (acc >> used) & mask;
                used += bits;
                if (used > 32) {
                    acc = *(const unsigned long*)data;
                    value |= (acc << (bits - used + 32)) & mask;
                    data += 4;
                    used -= 32;
                }
            }
            if (flags & 1)
                value = (value + bias) & g_RangeCompressorByteMasks[blockSize];
            value <<= gap;
            if (flags & 4) {
                unsigned long unpacked = 0;
                for (long r = 0; r < runs; ++r)
                    unpacked |= (value << shifts[r]) & runMasks[r];
                value = unpacked;
            }
            value |= setMask;
            if (flags & 0x10)
                value = (value + bias) & g_RangeCompressorByteMasks[blockSize];
            WriteValue(column + j * stride, value, blockSize);
        }
        column += blockSize;
        flags = *data++;
    }
    g_RangeDecompressTime += GFGetTime();
}