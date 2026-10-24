// Behaviour test for 0x00441C20 NPlayerGui::CDrawPerceivers::~CDrawPerceivers(): the things list then the base handle vector
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_elementDtors = 0, g_frees = 0, g_listClears = 0, g_strings = 0, g_queues = 0;
static void Free(void* p) { if (p) { std::free(p); ++g_frees; } }
struct Element {                                   // an 8-byte-on-x86 polymorphic element (handle / intelligent pointer)
    virtual ~Element() { ++g_elementDtors; }
    void* Data;
};
struct ElementVector {                             // STLport vector teardown: destroy [start, finish), then free start
    Element* start; Element* finish; Element* eos;
    ElementVector() : start(0), finish(0), eos(0) { }
    void Make(int n)
    {
        start = static_cast<Element*>(std::malloc(n * sizeof(Element)));
        for (int i = 0; i < n; ++i) new (start + i) Element();
        finish = eos = start + n;
    }
    ~ElementVector()
    {
        for (Element* p = start; p != finish; ++p) p->~Element();
        if (start != 0) Free(start);
    }
};
struct ThingList {                                 // STLport list: clear() then free the sentinel node
    void* node;
    ThingList() : node(std::malloc(8)) { }
    void clear() { ++g_listClears; }
    ~ThingList() { clear(); if (node != 0) Free(node); }
};

struct Drawer {
    ElementVector PrimitiveHandles;
    int State, Allegiance;
    float FrameInterp, ThrobScale;
    int ThingCount;
    ThingList Things;
};
int main()
{
    {
        Drawer* d = new Drawer;
        d->PrimitiveHandles.Make(3);
        delete d;
    }
    if (!(g_elementDtors == 3 && g_frees == 2 && g_listClears == 1)) { std::printf("00441c20_TEST FAIL\n"); return 1; }
    std::printf("00441c20_TEST PASS\n");
    return 0;
}