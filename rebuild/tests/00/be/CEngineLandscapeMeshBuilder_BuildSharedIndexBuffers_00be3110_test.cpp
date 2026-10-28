// Behaviour test for 0x00BE3110 CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IndexBuffer { int Bytes; unsigned short* Data; };
static int g_created = 0;
struct Builder {
    IndexBuffer* SharedIndexBuffers[17][17];
    // stand-in generation tables: a strip of sizeX*sizeY indices 0..n-1
    void BuildSharedIndexBufferGenerationTables(unsigned short* indices, int sizeX, int sizeY, int& count)
    {
        count = sizeX * sizeY;
        for (int i = 0; i < count; ++i) indices[i] = (unsigned short)i;
    }
    void BuildSharedIndexBuffers()                                  // the row's body
    {
        for (int x = 2; x <= 16; x *= 2)
            for (int y = 2; y <= 16; y *= 2) {
                unsigned short* indices = (unsigned short*)calloc(x * y * 12, sizeof(unsigned short));
                int count = 0;
                BuildSharedIndexBufferGenerationTables(indices, x, y, count);
                IndexBuffer* buffer = (IndexBuffer*)malloc(sizeof(IndexBuffer));
                buffer->Bytes = count * 2;
                buffer->Data = (unsigned short*)malloc(buffer->Bytes);
                ++g_created;
                memcpy(buffer->Data, indices, count * 2);
                SharedIndexBuffers[x][y] = buffer;
                free(indices);
            }
    }
};

int main()
{
    static Builder b;
    memset(&b, 0, sizeof(b));
    b.BuildSharedIndexBuffers();
    int ok = g_created == 16;
    for (int x = 2; x <= 16; x *= 2)
        for (int y = 2; y <= 16; y *= 2) {
            IndexBuffer* ib = b.SharedIndexBuffers[x][y];
            ok &= ib != 0 && ib->Bytes == x * y * 2 && ib->Data[x * y - 1] == (unsigned short)(x * y - 1);
        }
    ok &= b.SharedIndexBuffers[3][2] == 0 && b.SharedIndexBuffers[16][1] == 0;
    printf(ok ? "00be3110_TEST PASS\n" : "00be3110_TEST FAIL\n");
    return ok ? 0 : 1;
}