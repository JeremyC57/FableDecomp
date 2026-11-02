// test: CEngineLandscapeMeshBuilder::~CEngineLandscapeMeshBuilder -- delete the shared index buffers at power-of-two indices [2..16][2..16]
#include <stdio.h>
static int deleted[17][17];
struct IndexBuffer { int i, j; IndexBuffer(int a, int b) : i(a), j(b) { } virtual ~IndexBuffer() { ++deleted[i][j]; } };
struct Builder {
    IndexBuffer* SharedIndexBuffers[17][17];
    ~Builder() { for (long i = 2; i <= 16; i *= 2) for (long j = 2; j <= 16; j *= 2) delete SharedIndexBuffers[i][j]; }
};
int main() {
    Builder* b = new Builder;
    for (int i = 0; i < 17; ++i) for (int j = 0; j < 17; ++j) b->SharedIndexBuffers[i][j] = 0;
    for (int i = 2; i <= 16; i *= 2) for (int j = 2; j <= 16; j *= 2) if ((i + j) % 3) b->SharedIndexBuffers[i][j] = new IndexBuffer(i, j);
    b->SharedIndexBuffers[3][5] = new IndexBuffer(3, 5);   // a non-power slot the dtor must not touch
    IndexBuffer* keep = b->SharedIndexBuffers[3][5];
    delete b;
    int total = 0;
    for (int i = 0; i < 17; ++i) for (int j = 0; j < 17; ++j) total += deleted[i][j];
    int expect = 0;
    for (int i = 2; i <= 16; i *= 2) for (int j = 2; j <= 16; j *= 2) if ((i + j) % 3) { ++expect; if (deleted[i][j] != 1) { printf("00be2b20_TEST FAIL\n"); return 1; } }
    if (total != expect || deleted[3][5] != 0) { printf("00be2b20_TEST FAIL total\n"); return 1; }
    delete keep;
    printf("00be2b20_TEST PASS\n");
    return 0;
}