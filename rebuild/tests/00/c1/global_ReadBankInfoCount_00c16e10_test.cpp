// Behaviour test for ReadBankInfoCount (0x00C16E10): open "LHFileSegmentBankInfo", skip two 0x104 name fields, return the count (0 when the count read is empty, -1 on any failure).
#include <stdio.h>
#include <string.h>
struct File {
    int openResult; int reads[3]; long countValue; int calls;
    int OpenSegment(const char* name) { return strcmp(name, "LHFileSegmentBankInfo") == 0 ? openResult : -1; }
    int Read(void* buffer, int size) { int r = reads[calls++]; if (size == 4 && r == 4) memcpy(buffer, &countValue, 4); return r; }
};
long ReadBankInfoCount(File* file)
{
    char name[0x104 + 1]; long count; int read;
    bool missing = file->OpenSegment("LHFileSegmentBankInfo") < 0;
    if (missing) return -1;
    if (file->Read(name, 0x104) != 0x104) return -1;
    if (file->Read(name, 0x104) != 0x104) return -1;
    count = 0;
    read = file->Read(&count, 4);
    if (read < 0) return -1;
    if (read == 0) return 0;
    return count;
}
static int fail() { printf("00c16e10_TEST FAIL\n"); return 1; }
int main()
{
    File a = { 0, { 0x104, 0x104, 4 }, 7, 0 }; if (ReadBankInfoCount(&a) != 7) return fail();
    File b = { -1, { 0x104, 0x104, 4 }, 7, 0 }; if (ReadBankInfoCount(&b) != -1 || b.calls != 0) return fail();
    File c = { 0, { 0x104, 3, 4 }, 7, 0 }; if (ReadBankInfoCount(&c) != -1 || c.calls != 2) return fail();
    File d = { 0, { 0x104, 0x104, 0 }, 7, 0 }; if (ReadBankInfoCount(&d) != 0) return fail();
    File e = { 0, { 0x104, 0x104, -2 }, 7, 0 }; if (ReadBankInfoCount(&e) != -1) return fail();
    printf("00c16e10_TEST PASS\n"); return 0;
}