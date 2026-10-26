// behaviour test for 0x00994570
#include <stdio.h>
#include <string.h>
class CMemoryDataInputStream { public: void* __vftable; long position; long sourceLength; long chunkBase; long chunkPosition; long chunkLength; void* StreamSource; };
struct CMemoryDataInputStream_Methods : CMemoryDataInputStream {
    void OpenMemorySource(const void* source, unsigned long size);
};
void CMemoryDataInputStream_Methods::OpenMemorySource(const void* source, unsigned long size)
{
    position = 0;
    chunkPosition = 0;
    chunkBase = (long)source;
    sourceLength = size;
    chunkLength = size;
    StreamSource = (void*)source;
}

int main()
{
    char buf[16]; CMemoryDataInputStream_Methods s; memset(&s, 0x33, sizeof(s)); void* vt = s.__vftable;
    s.OpenMemorySource(buf, 16);
    int ok = s.position == 0 && s.chunkPosition == 0 && s.chunkBase == (long)buf && s.sourceLength == 16 && s.chunkLength == 16
          && s.StreamSource == buf && s.__vftable == vt;
    printf(ok ? "00994570_TEST PASS\n" : "00994570_TEST FAIL\n");
    return ok ? 0 : 1;
}