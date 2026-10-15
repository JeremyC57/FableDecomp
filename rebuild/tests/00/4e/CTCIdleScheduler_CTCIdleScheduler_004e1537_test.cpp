#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* p) : ParentThing(p) { std::strcat(g_seq, "B"); } virtual ~CTCBase() { } };
struct CIdleStateMap { int size; CIdleStateMap() : size(3) { std::strcat(g_seq, "M"); } };
struct CIdleWeightMap { int size; CIdleWeightMap() : size(4) { std::strcat(g_seq, "W"); } };
struct CTCIdleScheduler : CTCBase {
    CIdleStateMap IdleStates; CIdleWeightMap IdleStatesWeight; long Preferred; long FrameLastChecked;
    CTCIdleScheduler(CThing* parent) : CTCBase(parent) { Reset(); }
    void Reset() { std::strcat(g_seq, "R"); IdleStates.size = 0; IdleStatesWeight.size = 0; Preferred = 0; FrameLastChecked = 0; }
};
int main()
{
    CThing thing; thing.id = 1;
    CTCIdleScheduler s(&thing);
    if (std::strcmp(g_seq, "BMWR") || s.ParentThing != &thing || s.IdleStates.size || s.IdleStatesWeight.size || s.Preferred || s.FrameLastChecked)
    { std::printf("004e1537_TEST FAIL\n"); return 1; }
    std::printf("004e1537_TEST PASS\n");
    return 0;
}