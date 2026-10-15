#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CStatBarInfo { int id; };
namespace NPlayerGui {
struct CRetailAllocator { CRetailAllocator() { } };
struct CStatBase { virtual void v00() { } CStatBarInfo* Info; CStatBase(CStatBarInfo* info) : Info(info) { note("B"); } };
struct CRetailList_StatUpdate { void* Node; CRetailList_StatUpdate(const CRetailAllocator& al = CRetailAllocator()) : Node(&Node) { note("L"); } };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct CMultiStatBar : CStatBase {
    virtual void v00() { }
    CRetailList_StatUpdate Updates; long CurrentType; CRGBColour CurrentColour; long CurrentGraphic; long CurrentEffect; float CurrentTimeLeft; float WaitingTimeLeft;
    CMultiStatBar(CStatBarInfo* info) : CStatBase(info), Updates(), CurrentType(6), CurrentColour(0, 0, 0, 255), CurrentGraphic(0), CurrentEffect(0),
        CurrentTimeLeft(0.0f), WaitingTimeLeft(0.0f) { }
};
}
int main()
{
    CStatBarInfo info = { 9 };
    NPlayerGui::CMultiStatBar* m = new NPlayerGui::CMultiStatBar(&info);
    if (std::strcmp(g_seq, "BL") || m->Info != &info || m->CurrentType != 6 || m->CurrentColour.R != 0 || m->CurrentColour.G != 0
        || m->CurrentColour.B != 0 || m->CurrentColour.A != 255 || m->CurrentGraphic != 0 || m->CurrentEffect != 0 || m->CurrentTimeLeft != 0.0f
        || m->WaitingTimeLeft != 0.0f) { std::printf("006543af_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("006543af_TEST PASS\n");
    return 0;
}