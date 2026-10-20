#include <cstdio>
#include <cstring>
static bool g_exists = false;
struct File { int access; long length; void Open(int a) { access = a; } long GetLength() { return length; } };
struct CBankFile {
    unsigned long OpenFlags, ChangeCount, Alignment; File* BankFile; int name;
    bool RetailMode, ReadOnly, WrittenToFile, Solid, FileValid, UpdatingFlag; int infoReads, postOpens; File file;
    void ReadFileInformation(bool) { ++infoReads; }
    void PostOpen() { ++postOpens; }
    void OpenFileReadWrite(int filename, unsigned long flags, unsigned long alignment)
    {
        ReadOnly = false; OpenFlags = flags | 0x1f8; Alignment = alignment;
        ChangeCount = 0; FileValid = false; WrittenToFile = false; UpdatingFlag = false; RetailMode = false;
        name = filename; BankFile = &file;
        if (!(flags & 1) && g_exists) { BankFile->Open(1); Solid = false; }
        else { BankFile->Open(4); WrittenToFile = true; Solid = true; }
        if (BankFile->GetLength() > 0) ReadFileInformation(true);
        PostOpen();
    }
};
int main()
{
    CBankFile b; std::memset(&b, 0, sizeof(b)); b.file.length = 0; b.ReadOnly = true; b.ChangeCount = 5;
    b.OpenFileReadWrite(7, 2, 0x800);
    if (b.OpenFlags != 0x1fa || b.Alignment != 0x800 || b.ReadOnly || b.ChangeCount || b.file.access != 4 || !b.Solid || !b.WrittenToFile || b.infoReads || b.postOpens != 1) { std::printf("009d1160_TEST FAIL\n"); return 1; }
    g_exists = true; b.file.length = 100;
    b.OpenFileReadWrite(7, 0, 0x800);
    if (b.file.access != 1 || b.Solid || b.infoReads != 1 || b.postOpens != 2) { std::printf("009d1160_TEST FAIL\n"); return 1; }
    b.OpenFileReadWrite(7, 1, 0x800);
    if (b.file.access != 4) { std::printf("009d1160_TEST FAIL\n"); return 1; }
    std::printf("009d1160_TEST PASS\n");
    return 0;
}