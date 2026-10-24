// Behaviour test for 0x00444ED0 NPlayerGui::CDrawGuildSeal::~CDrawGuildSeal(): message queue, position storage, base handles
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
struct PodVector {
    float* start; float* finish; float* eos;
    PodVector() : start(static_cast<float*>(std::malloc(16))), finish(start), eos(start + 4) { }
    ~PodVector() { if (start) Free(start); }
};
struct MessageQueue { ~MessageQueue() { ++g_queues; } };

struct Drawer {
    ElementVector PrimitiveHandles;
    bool Enabled, ReadyToLevelUp; unsigned char FlashAlpha; int FlashCycles; float ReminderTime;
    PodVector LevelUpPositions;
    int LevelUpGraphic, CurrentMessageEndFrame, NextMessageDelayEndFrame;
    MessageQueue Messages;
};
int main()
{
    {
        Drawer* d = new Drawer;
        d->PrimitiveHandles.Make(10);
        delete d;
    }
    if (!(g_elementDtors == 10 && g_frees == 2 && g_queues == 1)) { std::printf("00444ed0_TEST FAIL\n"); return 1; }
    std::printf("00444ed0_TEST PASS\n");
    return 0;
}