#include <cstdio>
// behaviour copy of GFDataIsInsideBlock<C4DVector*> @ 0x00BFB750
class C4DVector { public: float x, y, z, w; };

template <class T>
bool GFDataIsInsideBlock(T& pointer, void* block, long size)
{
    if (pointer == 0)
        return false;
    return (void*)pointer >= block && (void*)pointer < (void*)((char*)block + size);
}

int main()
{
    C4DVector block[4];
    C4DVector* p = 0;
    int ok = 1;
    if (GFDataIsInsideBlock(p, block, sizeof(block))) ok = 0;          // null pointer
    p = &block[0];
    if (!GFDataIsInsideBlock(p, block, sizeof(block))) ok = 0;         // first element
    p = &block[3];
    if (!GFDataIsInsideBlock(p, block, sizeof(block))) ok = 0;         // last element
    p = block + 4;
    if (GFDataIsInsideBlock(p, block, sizeof(block))) ok = 0;          // one past the end
    p = &block[1];
    if (GFDataIsInsideBlock(p, &block[2], 2 * sizeof(C4DVector))) ok = 0;  // before the block
    if (!ok) { std::printf("00bfb750_TEST FAIL\n"); return 1; }
    std::printf("00bfb750_TEST PASS\n");
    return 0;
}