#include <cstdio>
#include <cstring>
struct FileException { int Type; };
static void Throw(int type) { FileException e; e.Type = type; throw e; }
struct MemoryFile {
    long Position; long Length; unsigned char* Data;
    void Read(void* buffer, long size)
    {
        if (!buffer) Throw(1);
        if (size) {
            if (Position + size > Length) Throw(3);
            for (; size > 0; --size) { *(unsigned char*)buffer = Data[Position++]; buffer = (unsigned char*)buffer + 1; }
        }
    }
};
int main()
{
    unsigned char data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MemoryFile f = { 2, 8, data };
    unsigned char out[4] = { 0, 0, 0, 0 };
    f.Read(out, 3);
    bool ok = out[0] == 3 && out[1] == 4 && out[2] == 5 && out[3] == 0 && f.Position == 5;
    int caught = 0;
    try { f.Read(0, 1); } catch (FileException& e) { caught += e.Type == 1; }
    try { f.Read(out, 4); } catch (FileException& e) { caught += e.Type == 3; }
    f.Read(out, 0);
    ok = ok && caught == 2 && f.Position == 5;
    if (!ok) { std::printf("009e9220_TEST FAIL\n"); return 1; }
    std::printf("009e9220_TEST PASS\n");
    return 0;
}