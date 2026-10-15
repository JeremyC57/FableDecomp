#include <cstdio>
#include <cstring>
static unsigned char g_buf[256];
struct Path { int id; };
struct File { int pathId; unsigned long block; long readLen; void* readTo; void* proc;
    Path GetPathname() { Path p; p.id = pathId; return p; } unsigned long GetBlockSize() { return block; }
    void Read(void* b, long n, void* pr) { readTo = b; readLen = n; proc = pr; std::memset(b, 7, n); } };
struct Buf { unsigned char* start; unsigned long size; void resize(unsigned long n) { start = g_buf; size = n; } };
struct MemFile { long position, length; Buf data; bool opened; Path path;
    void OpenFromFile(File* f, long length_, void* proc)
    {
        path = f->GetPathname(); position = 0; length = length_; opened = true;
        unsigned long bs = f->GetBlockSize();
        data.resize((length_ + bs - 1) / bs * bs);
        f->Read(data.start, length_, proc);
    } };
int main()
{
    File f; f.pathId = 42; f.block = 64; MemFile m; m.position = 9; m.opened = false; m.data.start = 0; m.data.size = 0;
    int proc = 0; m.OpenFromFile(&f, 100, &proc);
    if (m.path.id != 42 || m.position != 0 || m.length != 100 || !m.opened || m.data.size != 128 || f.readLen != 100 || f.readTo != g_buf || f.proc != &proc || g_buf[99] != 7)
    { std::printf("009e9420_TEST FAIL\n"); return 1; }
    std::printf("009e9420_TEST PASS\n");
    return 0;
}
