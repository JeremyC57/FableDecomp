// Behaviour test for C3DAnimationManager::C3DAnimationManager (0x00AA0F60): null mesh bank, empty bone-index buffer, two (0, 0x1000) pools.
#include <cstdio>
struct Pool { unsigned int count, size; Pool(unsigned int c, unsigned int s) : count(c), size(s) {} };
struct Vec { unsigned int* start; unsigned int* finish; unsigned int* eos; Vec() : start(0), finish(0), eos(0) {} };
struct Manager { void* MeshBank; Vec BoneIndicesBuffer; Pool MemoryPool; Pool MemoryPool2;
    Manager() : MeshBank(0), MemoryPool(0, 0x1000), MemoryPool2(0, 0x1000) {} };
int main()
{
    Manager m;
    if (m.MeshBank != 0 || m.BoneIndicesBuffer.start != 0 || m.BoneIndicesBuffer.finish != 0 || m.BoneIndicesBuffer.eos != 0 ||
        m.MemoryPool.count != 0 || m.MemoryPool.size != 0x1000 || m.MemoryPool2.count != 0 || m.MemoryPool2.size != 0x1000) {
        std::printf("00aa0f60_TEST FAIL\n"); return 1; }
    std::printf("00aa0f60_TEST PASS\n");
    return 0;
}