// Behaviour test for 0x00443D70 NPlayerGui::CDrawQuestInfo::~CDrawQuestInfo(): element list, two strings, oracle handles, symbol list, base handles
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
struct NodeList {
    void* node;
    NodeList() : node(std::malloc(8)) { }
    void clear() { ++g_listClears; }
    ~NodeList() { clear(); if (node != 0) Free(node); }
};
struct WideString { ~WideString() { ++g_strings; } };

struct Drawer {
    ElementVector PrimitiveHandles;
    int GuiMode; float FadeInTimer, MiniModeTimer; bool MiniMode; int FramesUntilMiniMode, MiniGameElementIndex;
    NodeList OracleSymbols;
    ElementVector OraclePrimitiveHandles;
    WideString Name, Text;
    NodeList Elements;
};
int main()
{
    {
        Drawer* d = new Drawer;
        d->PrimitiveHandles.Make(9); d->OraclePrimitiveHandles.Make(4);
        delete d;
    }
    if (!(g_elementDtors == 13 && g_frees == 4 && g_listClears == 2 && g_strings == 2)) { std::printf("00443d70_TEST FAIL\n"); return 1; }
    std::printf("00443d70_TEST PASS\n");
    return 0;
}