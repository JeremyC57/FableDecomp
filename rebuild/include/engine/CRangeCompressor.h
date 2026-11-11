#ifndef FABLE_ENGINE_CRANGECOMPRESSOR_H
#define FABLE_ENGINE_CRANGECOMPRESSOR_H
// New header (chunk Q16, row 0x00F3ABF0 CRangeCompressor::CalcCompressionScript). CRangeCompressor has no data members in Ego_r.pdb
// (no struct layout; the landed members never read `this`); its nested CScriptEntry is Ego_r.pdb's 24-byte struct (Flags +0, BitCount +4,
// RangeBias +8, RangeShift +0xC, ShuffleMask +0x10, OrMask +0x14). FableWin.pdb has ??0CScriptEntry@CRangeCompressor@@QAE@XZ; retail
// stores only Flags = 0 where an entry is declared (0x00F3ACF8), so the constructor initialises Flags alone. Member functions: the
// Ego_r.pdb / FableWin.pdb name set, with their access (QAE public, AAE private) and the retail addresses where known.
#include "rebuild_abi.h"
#include "engine/CArray.h"
class CRangeCompressor {
public:
    struct CScriptEntry {
        CScriptEntry() : Flags(0) { }
        unsigned long Flags;
        long BitCount;
        unsigned long RangeBias;
        long RangeShift;
        unsigned long ShuffleMask;
        unsigned long OrMask;
    };
    long Compress(const void* source, long width, long height, CArray<unsigned char>& out);   // 0x00F3AFC0
    long Compress(const void* source, long width, long height, unsigned char* dest, long size);  // 0x00F3AE50
    void Decompress(const unsigned char* source, long width, long height, void* dest);
    static double __fastcall GetTotalDecompressTime();
    static long __fastcall GetTotalDecompressData();
private:
    long GetBlockSizeFromFlags(unsigned long flags);
    unsigned long ReadVarSizedUInt(const unsigned char* source, long bytes);
    void WriteVarSizedUInt(unsigned long value, unsigned char* dest, long bytes);
    long CalcShuffleOperations(unsigned long mask, unsigned long* a, unsigned long* b);
    long CalcHeaderSizeNeeded(const CScriptEntry& entry);
    long CalcBitsNeeded(const CArray<unsigned long>& values);
    long CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values);
    long Compress(const unsigned char* source, long width, long height, unsigned char* dest, const CArray<CScriptEntry>& script);
    void TestRangeBias(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes);   // 0x00F3A5D0
    void TestRedundantBitStrip(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes);   // 0x00F3A710
    long CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry);   // 0x00F3A890
    long CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes);   // 0x00F3ABF0
};
FABLE_STATIC_ASSERT(sizeof(CRangeCompressor::CScriptEntry) == 24);
#endif
