// Behaviour test for 0x0051A430 vector<CLevelLoader::CObject::CLevelFiles>::_M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CCPPointerInfo { unsigned int RefCount; };
struct CCountedPointer {
    void* Data;
    CCPPointerInfo* Info;
    CCountedPointer(CCPPointerInfo* i) : Data(0), Info(i) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) --Info->RefCount; }
};
struct CLevelFiles {
    long LevelID;
    CCountedPointer PTopologyFile, PNavigationFile, PThingFile;
    CLevelFiles(long id, CCPPointerInfo* t, CCPPointerInfo* n, CCPPointerInfo* f)
        : LevelID(id), PTopologyFile(t), PNavigationFile(n), PThingFile(f) { }
};
struct Vec {
    CLevelFiles* start; CLevelFiles* finish; CLevelFiles* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CLevelFiles* pos, const CLevelFiles& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CLevelFiles* ns = len ? (CLevelFiles*)std::malloc(len * sizeof(CLevelFiles)) : 0;
        CLevelFiles* nf = ns;
        for (CLevelFiles* p = start; p != pos; ++p, ++nf) new (nf) CLevelFiles(*p);
        if (fill_len == 1) { new (nf) CLevelFiles(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CLevelFiles(x);
        if (!atend) for (CLevelFiles* p = pos; p != finish; ++p, ++nf) new (nf) CLevelFiles(*p);
        for (CLevelFiles* p = start; p != finish; ++p) p->~CLevelFiles();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int ok = 1;
    CCPPointerInfo topo = { 0 }, nav = { 0 }, things = { 0 };
    {
        Vec v; v.start = v.finish = v.eos = 0;
        CLevelFiles a(7, &topo, &nav, 0), b(9, 0, &nav, &things);
        v.insert_overflow(v.finish, a, 1, true);
        v.insert_overflow(v.start, b, 3, false);          // [b b b a], cap 1 + 3 = 4
        if (v.size() != 4 || v.eos - v.start != 4) ok = 0;
        if (v.start[0].LevelID != 9 || v.start[3].LevelID != 7) ok = 0;
        // a + 1 copy hold topo; a + b + 4 copies hold nav; b + 3 copies hold things
        if (topo.RefCount != 2 || nav.RefCount != 6 || things.RefCount != 4) ok = 0;
        for (CLevelFiles* p = v.start; p != v.finish; ++p) p->~CLevelFiles();
        std::free(v.start);
        if (topo.RefCount != 1 || nav.RefCount != 2 || things.RefCount != 1) ok = 0;
    }
    if (topo.RefCount != 0 || nav.RefCount != 0 || things.RefCount != 0) ok = 0;
    if (!ok) { std::printf("0051a430_TEST FAIL\n"); return 1; }
    std::printf("0051a430_TEST PASS\n");
    return 0;
}