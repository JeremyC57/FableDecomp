// Test: CMemoryFile::Copy -- copies access type, position, length, open flag and pathname, and appends the other buffer's bytes
#include <cstdio>
#include <cstring>
class CMemoryFile { public:
    int AccessType; unsigned long Position; unsigned long Length; bool Opened; char Pathname[16]; unsigned char buf[16]; int size;
    void Copy(const CMemoryFile& o)
    {
        AccessType = o.AccessType; Position = o.Position; Length = o.Length; Opened = o.Opened;
        std::strcpy(Pathname, o.Pathname);
        for (int i = 0; i < o.size; ++i) buf[size++] = o.buf[i];   // copy(begin, end, back_inserter(DataBuffer))
    }
};
int main()
{
    CMemoryFile src = { 2, 3, 4, true, "a.bin", { 9, 8, 7 }, 3 };
    CMemoryFile dst = { 0, 0, 0, false, "", { 1 }, 1 };
    dst.Copy(src);
    if (dst.AccessType != 2 || dst.Position != 3 || dst.Length != 4 || !dst.Opened || std::strcmp(dst.Pathname, "a.bin")
        || dst.size != 4 || dst.buf[0] != 1 || dst.buf[1] != 9 || dst.buf[3] != 7) { std::printf("009e9760_TEST FAIL\n"); return 1; }
    std::printf("009e9760_TEST PASS\n");
    return 0;
}