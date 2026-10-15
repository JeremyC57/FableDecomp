#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_deleted = 0;
struct CMemoryBlock {
    virtual ~CMemoryBlock() { ++g_deleted; }
    unsigned long  IVRefCount;
    unsigned char* BlockData;
    bool           OwnsData;
    unsigned long  BlockSize;
    CMemoryBlock*  NextBlock;
    CMemoryBlock(bool owns, unsigned long size, CMemoryBlock* next) : IVRefCount(1), BlockData(0), OwnsData(owns), BlockSize(size), NextBlock(next) { }
};
struct CArea { unsigned long Size; CArea* NextFreeArea; long Status; };
struct CSimpleMemoryPool {                      // the header layout, Root as its CArea type (member access only)
    void*         __vftable;
    CMemoryBlock* FirstMemoryBlock;
    unsigned long Capacity;
    unsigned long GrowthGranularity;
    CArea         Root;
};
struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    void ClearMemoryBlocks()
    {
        CMemoryBlock* block = FirstMemoryBlock;
        if (block) {
            if (block->OwnsData) {
                do { CMemoryBlock* next = block->NextBlock; delete block; block = next; } while (block);
            } else {
                delete block;
            }
            FirstMemoryBlock = 0;
        }
    }
};
int main()
{
    CSimpleMemoryPool_Methods p; p.FirstMemoryBlock = new CMemoryBlock(true, 16, new CMemoryBlock(false, 32, new CMemoryBlock(true, 48, 0)));
    p.ClearMemoryBlocks();
    if (g_deleted != 3 || p.FirstMemoryBlock != 0) { std::printf("00a5c600_TEST FAIL\n"); return 1; }
    CMemoryBlock* second = new CMemoryBlock(true, 8, 0);
    p.FirstMemoryBlock = new CMemoryBlock(false, 16, second);
    p.ClearMemoryBlocks();
    if (g_deleted != 4 || p.FirstMemoryBlock != 0 || second->BlockSize != 8) { std::printf("00a5c600_TEST FAIL\n"); return 1; }
    delete second;
    p.ClearMemoryBlocks();
    if (g_deleted != 5) { std::printf("00a5c600_TEST FAIL\n"); return 1; }
    std::printf("00a5c600_TEST PASS\n");
    return 0;
}