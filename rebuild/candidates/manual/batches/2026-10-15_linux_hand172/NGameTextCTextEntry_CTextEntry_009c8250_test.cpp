#include <cstdio>
struct CInfo { long Count; };
struct CPtr { void* Data; CInfo* Info; CPtr(const CPtr& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; } CPtr(void* d, CInfo* i) : Data(d), Info(i) {} };
struct CTextEntry {
    unsigned int size; void* start; void* finish; void* eos; CPtr file; void* owner;
    unsigned int Sizeof() const { return 0x44; }
    CTextEntry(const CPtr& d, void* o) : size(0), start(0), finish(0), eos(0), file(d), owner(o) { size = Sizeof(); }
};
int main()
{
    CInfo info = { 1 }; int x;
    CPtr p(&x, &info);
    CTextEntry e(p, &info);
    if (info.Count != 2 || e.file.Data != &x || e.owner != &info || e.size != 0x44 || e.start || e.finish || e.eos) { std::printf("009c8250_TEST FAIL\n"); return 1; }
    std::printf("009c8250_TEST PASS\n");
    return 0;
}
