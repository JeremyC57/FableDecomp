#include <cstdio>
#include <stddef.h>
#pragma pack(push, 1)
struct SCompressedBlockInfo { unsigned long UncompressedSize; unsigned long CompressedSize; bool Flag; };
struct SCompressedBlock { unsigned long Tag; SCompressedBlockInfo Info; };
#pragma pack(pop)
static int g_inflates = 0; static const void* g_src = 0; static unsigned long g_srcSize = 0, g_dstSize = 0; static void* g_dst = 0;
struct CCompressorZlib_Retail {
    virtual void v00() { }
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize) { ++g_inflates; g_src = source; g_srcSize = sourceSize; g_dst = dest; g_dstSize = destSize; return 0; }
    static void ReportInfo(SCompressedBlockInfo* info, const SCompressedBlock* block) { info->CompressedSize = block->Info.CompressedSize; info->UncompressedSize = block->Info.UncompressedSize; info->Flag = block->Info.Flag; }
    void Decompress(const SCompressedBlock* block, unsigned long blockSize, void* dest, unsigned long destSize, SCompressedBlockInfo* info)
    {
        blockSize -= sizeof(SCompressedBlock);
        if (blockSize == 0) { ReportInfo(info, block); return; }
        InternalDecompress(block + 1, blockSize, dest, destSize);
        ReportInfo(info, block);
    }
};
int main()
{
    unsigned char raw[13 + 20]; SCompressedBlock* block = (SCompressedBlock*)raw; block->Tag = 1; block->Info.UncompressedSize = 500; block->Info.CompressedSize = 33; block->Info.Flag = true;
    CCompressorZlib_Retail z; SCompressedBlockInfo info = { 0, 0, false }; unsigned char dest[8];
    z.Decompress(block, 33, dest, 500, &info);
    if (g_inflates != 1 || g_src != raw + 13 || g_srcSize != 20 || g_dst != dest || g_dstSize != 500 || info.UncompressedSize != 500 || info.CompressedSize != 33 || !info.Flag || sizeof(SCompressedBlock) != 13) { std::printf("00a3abe0_TEST FAIL\n"); return 1; }
    info.Flag = false; info.CompressedSize = 0;
    z.Decompress(block, 13, dest, 500, &info);                   // header only: no inflate
    if (g_inflates != 1 || info.CompressedSize != 33 || !info.Flag) { std::printf("00a3abe0_TEST FAIL\n"); return 1; }
    std::printf("00a3abe0_TEST PASS\n"); return 0;
}
