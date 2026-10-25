// behaviour test for 0x00aa72a0 C3DMeshFileXTriangleStripChunk's scalar deleting destructor: delete frees the vector member's buffer (only when it
// has one), then runs the remaining destructor (the string member or the base), then frees the object itself; destroying
// without the delete bit (an explicit destructor call) does the same and leaves the object's storage alone
#include <cstdio>
#include <cstdlib>
#include <new>
static int gOrder[8];
static int gCount = 0;
static int gBlocksFreed = 0;
static void Log(int id) { if (gCount < 8) gOrder[gCount] = id; ++gCount; }
static void CountingFree(void* p) { Log(1); std::free(p); }
class Buffer {                                    // the STLport vector: free(_M_start) when it is non-null
public:
    Buffer() : Start(0), Finish(0), End(0) { }
    ~Buffer() { if (Start != 0) CountingFree(Start); }
    void Grow(unsigned int n) { Start = (unsigned char*)std::malloc(n); Finish = Start; End = Start + n; }
    unsigned char* Start;
    unsigned char* Finish;
    unsigned char* End;
};
class Other { public: ~Other() { Log(2); } void* Data; };   // CCharString / the base destructor
class Holder {
public:
    Other First;
    Buffer Second;
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++gBlocksFreed; std::free(p); }
    static void* operator new(size_t, void* where) { return where; }
    static void operator delete(void*, void*) { }
};
int main()
{
    int fails = 0;
    Holder* p = new Holder;
    p->Second.Grow(16);
    delete p;
    if (gCount != 2 || gOrder[0] != 1 || gOrder[1] != 2 || gBlocksFreed != 1) fails |= 1;
    gCount = 0; gBlocksFreed = 0;
    p = new Holder;                               // empty vector: no free call
    delete p;
    if (gCount != 1 || gOrder[0] != 2 || gBlocksFreed != 1) fails |= 2;
    gCount = 0; gBlocksFreed = 0;
    void* raw = std::malloc(sizeof(Holder));
    Holder* h = new (raw) Holder;
    h->Second.Grow(8);
    h->~Holder();
    if (gCount != 2 || gOrder[0] != 1 || gOrder[1] != 2 || gBlocksFreed != 0) fails |= 4;
    std::free(raw);
    if (fails) { std::printf("00aa72a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00aa72a0_TEST PASS\n");
    return 0;
}