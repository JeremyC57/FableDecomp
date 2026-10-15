// Behaviour test for 0x00a5c910: the CSimpleMemoryPool model (set pool, first-fit allocate with split, grow, free all).
#include <cstdio>
#include <cstdlib>
const unsigned int H = 32;                                  // area header (16 in retail; host pointers are wider)
struct Area { unsigned int Size; Area* Next; int Status; };
struct Block { unsigned char* Data; bool Owns; unsigned int Size; Block* Next; };
static int g_deleted;
struct Pool {
    Block* First; unsigned int Capacity, Growth; Area Root;
    void AddFree(Area* a, unsigned int s) { a->Size = s; a->Status = 0x61; a->Next = Root.Next; Root.Next = a; }
    void AddBlock(Block* b) { AddFree((Area*)b->Data, b->Size - H); }
    void DeleteBlocks() { Block* b = First; if (b) { if (b->Owns) { do { Block* n = b->Next; if (b->Owns) std::free(b->Data); delete b; ++g_deleted; b = n; } while (b); } else { delete b; ++g_deleted; } First = 0; } }
    void Reset() { DeleteBlocks(); Root.Next = 0; Capacity = 0x400; }
    void FreeAll() { Root.Next = 0; if (First && !First->Next) { AddBlock(First); return; } DeleteBlocks(); }
    void SetMemoryPool(void* data, unsigned int size) { Reset(); Block* b = new Block; b->Data = (unsigned char*)data; b->Size = size; b->Next = 0; b->Owns = false; First = b; AddBlock(b); Capacity = size - H; }
    void CreateNewMemoryBlock(unsigned int size) { Block* b = new Block; b->Size = size + H; b->Next = 0; b->Data = (unsigned char*)std::malloc(b->Size); b->Owns = true;
        AddBlock(b); if (First) Capacity += size; b->Next = First; First = b; }
    void* Allocate(unsigned int size) {
        size = (size + 15) / 16 * 16; Area* prev = &Root;
        for (Area* a = Root.Next; a; a = a->Next) { if (size <= a->Size) { prev->Next = a->Next; unsigned int rem = a->Size - size; a->Status = 0x67;
                if (rem > H) { AddFree((Area*)((unsigned char*)a + size + H), rem - H); a->Size = size; } return (unsigned char*)a + H; } prev = a; }
        unsigned int g = Capacity; if (First) g = Growth; CreateNewMemoryBlock(g > size ? g : size); return Allocate(size); }
};
static bool Run()
{
    Pool p = {0, 0, 256, {0, 0, 0}};
    static unsigned char buf[512];
    p.SetMemoryPool(buf, sizeof(buf));
    bool ok = p.Capacity == 512 - H && p.Root.Next == (Area*)buf && p.Root.Next->Size == 512 - H && p.Root.Next->Status == 0x61;
    unsigned char* a = (unsigned char*)p.Allocate(10);
    ok = ok && a == buf + H && ((Area*)buf)->Size == 16 && ((Area*)buf)->Status == 0x67 && p.Root.Next == (Area*)(buf + 16 + H);
    unsigned char* b = (unsigned char*)p.Allocate(1000);              // no fit: grows a new owning block
    ok = ok && b && p.First && p.First->Owns && p.First->Next && p.Capacity == 512 - H + 1008;
    g_deleted = 0; p.FreeAll(); ok = ok && p.First == 0 && g_deleted == 2 && p.Root.Next == 0;
    p.CreateNewMemoryBlock(64); p.FreeAll(); ok = ok && p.First && p.Root.Next == (Area*)p.First->Data && p.Root.Next->Size == 64;
    p.Reset();
    return ok;
}
int main()
{
    if (!Run()) { std::printf("00a5c910_TEST FAIL\n"); return 1; }
    std::printf("00a5c910_TEST PASS\n");
    return 0;
}
