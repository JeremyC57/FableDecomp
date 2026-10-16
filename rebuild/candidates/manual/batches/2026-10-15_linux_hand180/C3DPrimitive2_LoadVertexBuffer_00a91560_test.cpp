#include <cstdio>
// Byte count = VertexCount * max(reps,1) * vertexSize; the direct path when formats match or the mesh repeats.
static unsigned int Bytes(unsigned int vertexCount, long reps, unsigned int size) { if (reps <= 1) reps = 1; return vertexCount * (unsigned int)reps * size; }
static bool Direct(unsigned int fileFormat, unsigned int initFlags, long reps) { return !(fileFormat != initFlags && reps <= 0); }
int main() {
    if (Bytes(10, 0, 8) != 80 || Bytes(10, 3, 8) != 240 || !Direct(1, 1, 0) || Direct(1, 2, 0) || !Direct(1, 2, 2)) { std::printf("00a91560_TEST FAIL\n"); return 1; }
    std::printf("00a91560_TEST PASS\n"); return 0; }
