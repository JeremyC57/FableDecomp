#include <cstdio>
// Behaviour model of the vector deleting destructor: flags bit 1 = array (count in the word before the elements, each
// element destroyed, the block starts at the cookie), bit 0 = free the block.
static int destroyed = 0; static void* freed = 0;
struct O { int body; void Destroy() { ++destroyed; } };
static void* VecDel(O* self, unsigned int flags)
{
    if (flags & 2) {
        unsigned int* cookie = (unsigned int*)self - 1;
        for (unsigned int i = *cookie; i > 0; --i) self[i - 1].Destroy();
        if (flags & 1) freed = cookie;
        return cookie;
    }
    self->Destroy();
    if (flags & 1) freed = self;
    return self;
}
int main()
{
    static unsigned int block[4] = { 3, 0, 0, 0 };
    void* r = VecDel((O*)(block + 1), 3);
    if (destroyed != 3 || freed != block || r != block) { std::printf("00c3f0b0_TEST FAIL\n"); return 1; }
    O one; r = VecDel(&one, 0);
    if (destroyed != 4 || freed != block || r != &one) { std::printf("00c3f0b0_TEST FAIL\n"); return 1; }
    std::printf("00c3f0b0_TEST PASS\n");
    return 0;
}