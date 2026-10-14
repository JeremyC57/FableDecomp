#include <cstdio>
struct CWorld {}; struct CThingAICreature {};
static long g_frame = 1000; static long g_framesPerSecond = 15; static unsigned long g_seed = 1;
long GFGetCurrentFrame() { return g_frame; }
unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; unsigned long* GetPWorldSeed() { return &g_seed; }
    unsigned long RandomFramesWithin(long seconds) { long fps = g_framesPerSecond; unsigned long* seed = GetPWorldSeed(); unsigned long range = fps * seconds; unsigned long next = *seed * 0x24A1U + 0x24DFU; *seed = next; unsigned long rotated = GFROR13(next); *seed = rotated; if (range != 0) return rotated % range; return 0; } };
struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase { bool InitiatedTag; unsigned char _pad15[3]; void* PTCGroupTag; float x, y, z; bool StandModeAddedToCreature; unsigned char _pad29[3]; long FrameLastPlayedIn;
    virtual void InitStateGroup(const CWorld&, CThingAICreature&); };
void CAIStateGroup_MinionKidGroupTag::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{ FrameLastPlayedIn = GFGetCurrentFrame() - RandomFramesWithin(7); if (FrameLastPlayedIn > GFGetCurrentFrame()) FrameLastPlayedIn = GFGetCurrentFrame(); }
int main()
{
    CAIStateGroup_MinionKidGroupTag g; CWorld w; CThingAICreature c;
    unsigned long expectSeed = GFROR13(1u * 0x24A1U + 0x24DFU); long expect = 1000 - (long)(expectSeed % 105u);
    g.InitStateGroup(w, c);
    if (g.FrameLastPlayedIn != expect || g_seed != expectSeed || g.FrameLastPlayedIn > 1000 || g.FrameLastPlayedIn < 1000 - 104) { std::printf("00934090_TEST FAIL\n"); return 1; }
    g_framesPerSecond = 0; g.InitStateGroup(w, c); if (g.FrameLastPlayedIn != 1000) { std::printf("00934090_TEST FAIL\n"); return 1; }
    std::printf("00934090_TEST PASS\n"); return 0;
}
