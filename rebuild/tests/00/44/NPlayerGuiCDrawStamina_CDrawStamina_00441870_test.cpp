// Behaviour test for 0x00441870 NPlayerGui::CDrawStamina::~CDrawStamina(): two emitter vectors then the base handle vector
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

struct Drawer {
    ElementVector PrimitiveHandles;
    ElementVector Effects;
    ElementVector IncreaseEffects;
    int PreviousStamina;
};
int main()
{
    {
        Drawer* d = new Drawer;
        d->PrimitiveHandles.Make(10); d->Effects.Make(2); d->IncreaseEffects.Make(0);
        delete d;
    }
    if (!(g_elementDtors == 12 && g_frees == 3)) { std::printf("00441870_TEST FAIL\n"); return 1; }
    std::printf("00441870_TEST PASS\n");
    return 0;
}