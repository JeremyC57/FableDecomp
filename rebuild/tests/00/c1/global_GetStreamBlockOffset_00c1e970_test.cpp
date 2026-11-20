#include <cstdio>
#include "libpk/CStreamBlockReader.h"

// The header declares the reader's three slots without bodies; the test gives the base its own (unused) bodies so its
// vtable links.
void CStreamBlockReader::Close() { }
bool CStreamBlockReader::Read(unsigned long, unsigned long, unsigned long, void*, CPooledResource*, long) { return false; }
unsigned long CStreamBlockReader::GetBlockSize() { return 1; }

// A reader whose slot 2 (call [eax+8]) GetBlockSize returns the test's block size.
class CTestBlockReader : public CStreamBlockReader
{ public:
    unsigned long block;
    void Close() { }
    bool Read(unsigned long, unsigned long, unsigned long, void*, CPooledResource*, long) { return false; }
    unsigned long GetBlockSize() { return block; }
};

int main()
{
    CTestBlockReader s;
    s.block = 7;

    unsigned long r1 = GetStreamBlockOffset(&s, 10u); // 10%7=3
    unsigned long r2 = GetStreamBlockOffset(&s, 14u); // 14%7=0
    unsigned long r3 = GetStreamBlockOffset(&s, 8u);  // 8%7=1

    s.block = 3;
    unsigned long r4 = GetStreamBlockOffset(&s, 9u);  // 0

    if (r1 == 3 && r2 == 0 && r3 == 1 && r4 == 0) {
        printf("BOAST_MODULO_OK\n");
        return 0;
    }
    printf("FAIL r1=%lu r2=%lu r3=%lu r4=%lu\n", r1, r2, r3, r4);
    return 1;
}
