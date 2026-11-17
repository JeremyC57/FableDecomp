// Behaviour test for CAudioBankTableReader's constructor (0x00C3BB40): it keeps the segment file it is given at +0.
// CSegmentFile stays opaque (libpk/CSegmentFile.h owns its layout); the test only checks the pointer is stored.
#include <stdio.h>
class CSegmentFile;
struct CAudioBankTableReader {
    CSegmentFile* reader;
    CAudioBankTableReader(CSegmentFile* file) : reader(file) { }
};
int main(void)
{
    static char storage[0xB0];
    CSegmentFile* file = reinterpret_cast<CSegmentFile*>(storage);
    CAudioBankTableReader tables(file);
    if (tables.reader != file || (void*)&tables != (void*)&tables.reader) { printf("FAIL_00c3bb40\n"); return 1; }
    printf("PASS_00c3bb40\n");
    return 0;
}
