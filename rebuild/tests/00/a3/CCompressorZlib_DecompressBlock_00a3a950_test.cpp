#include <cstdio>
#include <stddef.h>
#pragma pack(push, 1)
struct SCompressedBlockInfo { unsigned long UncompressedSize; unsigned long CompressedSize; bool Flag;
    SCompressedBlockInfo(unsigned long u, unsigned long c, bool f) : UncompressedSize(u), CompressedSize(c), Flag(f) { } };
struct SCompressedBlock { unsigned long Tag; SCompressedBlockInfo Info; };
#pragma pack(pop)
static int g_calls = 0; static const SCompressedBlock* g_block = 0; static unsigned long g_blockSize = 0, g_destSize = 0; static void* g_dest = 0; static SCompressedBlockInfo* g_info = 0; static unsigned long g_infoU = 0, g_infoC = 0; static bool g_infoF = false;
struct CCompressorZlib_Retail {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { }
    virtual void Decompress(const SCompressedBlock* block, unsigned long blockSize, void* dest, unsigned long destSize, SCompressedBlockInfo* info)
    { ++g_calls; g_block = block; g_blockSize = blockSize; g_dest = dest; g_destSize = destSize; g_info = info; g_infoU = info->UncompressedSize; g_infoC = info->CompressedSize; g_infoF = info->Flag; }
    unsigned char _body[0x54];
    void DecompressBlock(const SCompressedBlock* block, unsigned long, void*, void* dest)
    {
        unsigned long compressedSize = block->Info.CompressedSize; unsigned long uncompressedSize = block->Info.UncompressedSize; bool flag = block->Info.Flag;
        SCompressedBlockInfo info(uncompressedSize, compressedSize, flag);
        Decompress(block, compressedSize, dest, uncompressedSize, &info);
    }
};
int main()
{
    unsigned char raw[13]; SCompressedBlock* block = (SCompressedBlock*)raw; block->Tag = 9; block->Info.UncompressedSize = 1000; block->Info.CompressedSize = 77; block->Info.Flag = true;
    CCompressorZlib_Retail z; unsigned char dest[4];
    z.DecompressBlock(block, 5, 0, dest);
    if (g_calls != 1 || g_block != block || g_blockSize != 77 || g_dest != dest || g_destSize != 1000 || g_info == 0 || g_infoU != 1000 || g_infoC != 77 || !g_infoF) { std::printf("00a3a950_TEST FAIL\n"); return 1; }
    std::printf("00a3a950_TEST PASS\n"); return 0;
}