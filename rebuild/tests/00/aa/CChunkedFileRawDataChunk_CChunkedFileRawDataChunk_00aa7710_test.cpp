// Behaviour test for CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk (0x00AA7710): delete[] the raw bytes, then the base.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[64];
struct Chunk { virtual ~Chunk() { strcat(g_log, "base"); } };
struct Raw : Chunk { unsigned char* Data; ~Raw() { delete[] Data; strcat(g_log, "data,"); } };
int main()
{
    Chunk* c = new Raw; ((Raw*)c)->Data = new unsigned char[16];
    delete c;
    bool ok = strcmp(g_log, "data,base") == 0;
    printf(ok ? "00aa7710_TEST PASS\n" : "00aa7710_TEST FAIL\n");
    return ok ? 0 : 1;
}