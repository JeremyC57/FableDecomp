// behaviour test for 0x00824D30: the version-1 conversion hands back a fresh record (version 1) held by exactly one count
#include <cstdio>
#include <cstdlib>
struct Info { unsigned long refs; void* object; };
struct Rec { unsigned long ByteLength; unsigned char Version; Rec() : Version(1) { } };
struct CP { Rec* object; Info* control;
    CP(Rec* p) : object(p), control(p ? new Info : 0) { if (control) { control->refs = 1; control->object = p; } }
    CP(const CP& x) : object(x.object), control(x.control) { if (control) ++control->refs; }
    ~CP() { if (control && --control->refs == 0) { delete (Rec*)control->object; delete control; } } };
static CP Convert(const void*) { CP result(new Rec); return result; }
int main()
{
    CP p = Convert(0);
    if (!p.object || p.object->Version != 1 || !p.control || p.control->refs != 1) { std::printf("00824d30_TEST FAIL\n"); return 1; }
    std::printf("00824d30_TEST PASS\n");
    return 0;
}