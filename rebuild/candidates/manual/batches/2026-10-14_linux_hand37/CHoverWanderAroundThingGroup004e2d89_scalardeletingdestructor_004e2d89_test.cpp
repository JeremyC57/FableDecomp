#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
struct CMember_004df9ef { unsigned char _body[4]; ~CMember_004df9ef() { g_seq[g_n++] = 'm'; } };
struct CAIStateGroup_HoverWanderAroundThing { virtual void v00() { } unsigned char _pad[416]; ~CAIStateGroup_HoverWanderAroundThing() { g_seq[g_n++] = 'b'; } };
struct CHoverWanderAroundThingGroup_004e2d89 { CAIStateGroup_HoverWanderAroundThing m_base; CMember_004df9ef m_member; ~CHoverWanderAroundThingGroup_004e2d89() { } };
static void Destroy_004e2d89(CHoverWanderAroundThingGroup_004e2d89* o) { delete o; }

int main() {
    CHoverWanderAroundThingGroup_004e2d89* o = new CHoverWanderAroundThingGroup_004e2d89; Destroy_004e2d89(o); if (std::strcmp(g_seq, "mb")) { std::printf("004e2d89_TEST FAIL\n"); return 1; }
    std::printf("004e2d89_TEST PASS\n"); return 0;
}
