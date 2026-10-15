#include <cstdio>
struct Arr { char* start; char* finish; char* eos; long size() const { return finish - start; } const char* GetData() const { return size() ? start : (const char*)this; } };
struct Out { Arr Data; };
struct In { long position, size; const char* data; long flags, sizeCopy; const char* source;
    In(const Out& o) : position(0), size(o.Data.size()), data(o.Data.GetData()), flags(0), sizeCopy(o.Data.size()) { source = o.Data.GetData(); } };
int main()
{
    char buf[8] = "abcdef"; Out o; o.Data.start = buf; o.Data.finish = buf + 6; o.Data.eos = buf + 8;
    In a(o);
    if (a.position != 0 || a.size != 6 || a.sizeCopy != 6 || a.data != buf || a.source != buf || a.flags != 0) { std::printf("00995ee0_TEST FAIL\n"); return 1; }
    Out e; e.Data.start = e.Data.finish = e.Data.eos = 0; In b(e);
    if (b.size != 0 || b.data != (const char*)&e.Data || b.source != (const char*)&e.Data) { std::printf("00995ee0_TEST FAIL\n"); return 1; }
    std::printf("00995ee0_TEST PASS\n");
    return 0;
}
