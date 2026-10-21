// behaviour test for 00684420 CTCCreatureOpinionOfHero ctor: default counters, all-on masks of 18/79/92 entries, five zero
// opinions, self-linked list sentinels, forced attitude 18, cleared flags, the def-driven mask calls and the -1 deed frames
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SNode { SNode* next; SNode* prev; };
static SNode* NewSentinel(unsigned int size) { SNode* n = (SNode*)std::malloc(size); n->next = n; n->prev = n; return n; }
struct SMask { unsigned char* start; unsigned char* finish; unsigned char* end; };
static void MakeMask(SMask& m, unsigned int count, bool value)
{
    m.start = (unsigned char*)std::malloc(count); m.end = m.start + count;
    std::memset(m.start, value, count); m.finish = m.start + count;
}
struct SDef { long personality, reaction, deed; };
struct SOpinion {
    long lastReaction; float hits; float tolerance; long decay;
    SMask attitude, reactionMask, deedMask; float* opinions; SNode* pending; long lastFrame[92]; SNode* known[92]; SNode* transient;
    long forced; bool flags[4]; void* threat; long lastThreat;
    long personalityCall, personalityApply, reactionCall, deedCall;
    void SetPersonalityOverride(long i, bool a) { personalityCall = i; personalityApply = a ? 1 : 0; }
    void SetOpinionReactionMask(long i) { reactionCall = i; }
    void SetOpinionDeedMask(long i) { deedCall = i; }
    void Construct(const SDef* def)
    {
        lastReaction = 0; hits = 0.0f; tolerance = -1.0f; decay = 0x7fffffff;
        MakeMask(attitude, 18, true); MakeMask(reactionMask, 79, true); MakeMask(deedMask, 92, true);
        opinions = (float*)std::malloc(5 * sizeof(float)); std::memset(opinions, 0, 5 * sizeof(float));
        pending = NewSentinel(0xc);
        for (int i = 0; i < 92; ++i) known[i] = NewSentinel(0xc);
        transient = NewSentinel(0x20);
        forced = 18; flags[0] = flags[1] = flags[2] = flags[3] = false; threat = 0; lastThreat = 0;
        SetPersonalityOverride(def->personality, true);
        SetOpinionReactionMask(def->reaction);
        SetOpinionDeedMask(def->deed);
        for (long i = 0; i < 92; ++i) lastFrame[i] = -1;
    }
};
int main()
{
    static SOpinion o;
    SDef def = { 3, 4, 5 };
    o.Construct(&def);
    if (o.tolerance != -1.0f || o.decay != 0x7fffffff || o.forced != 18 || o.threat != 0) { std::printf("00684420_TEST FAIL\n"); return 1; }
    if (o.attitude.finish - o.attitude.start != 18 || o.reactionMask.end - o.reactionMask.start != 79 || o.deedMask.start[91] != 1) { std::printf("00684420_TEST FAIL\n"); return 1; }
    if (o.opinions[4] != 0.0f || o.pending->next != o.pending || o.known[91]->prev != o.known[91] || o.transient->next != o.transient) { std::printf("00684420_TEST FAIL\n"); return 1; }
    if (o.personalityCall != 3 || o.personalityApply != 1 || o.reactionCall != 4 || o.deedCall != 5 || o.lastFrame[0] != -1 || o.lastFrame[91] != -1) { std::printf("00684420_TEST FAIL\n"); return 1; }
    std::printf("00684420_TEST PASS\n");
    return 0;
}
