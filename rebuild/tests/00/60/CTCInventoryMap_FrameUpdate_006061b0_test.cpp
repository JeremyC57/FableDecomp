// Test for 0x006061B0 CTCInventoryMap::FrameUpdate: self-contained behaviour model -- the region updaters run (and the event frame
// is stamped) only once LastEventFrame + RegionEventLife has been reached (unsigned compare); a pending quest drop fails and
// deactivates the quest of the first live active card whose def index matches, if that card has a quest-card component, and the
// request is cleared whether or not a card was found.
#include <cstdio>
#include <cstring>

static long g_frame = 0;
struct Card { bool exists, dead, hasQuestCard; unsigned short defIndex; const char* scriptName; };
struct Log { int failed, deactivated, updates; const char* lastFailed; unsigned long lastDeactivateFrames; };
static Log g_log;
struct Map {
    long questToBeDropped, lastEventFrame; unsigned long regionEventLife;
    void FrameUpdate(const Card* cards, int count)
    {
        if ((unsigned long)g_frame >= lastEventFrame + regionEventLife) {
            ++g_log.updates;
            lastEventFrame = g_frame;
        }
        if (questToBeDropped > 0) {
            for (int i = 0; i < count; ++i) {
                const Card& c = cards[i];
                if (c.exists && !c.dead && (long)c.defIndex == questToBeDropped) {
                    if (c.hasQuestCard) {
                        ++g_log.failed; g_log.lastFailed = c.scriptName;
                        ++g_log.deactivated; g_log.lastDeactivateFrames = 0;
                    }
                    break;
                }
            }
            questToBeDropped = 0;
        }
    }
};

static int fail(int n) { std::printf("006061b0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Card cards[4] = {
        { false, false, true, 7, "Q_NONE" }, { true, true, true, 7, "Q_DEAD" },
        { true, false, true, 7, "Q_LIVE" }, { true, false, true, 7, "Q_SECOND" } };
    Map m = { 7, 100, 50 };
    g_frame = 149;
    m.FrameUpdate(cards, 4);
    if (g_log.updates != 0 || m.lastEventFrame != 100) return fail(1);
    if (m.questToBeDropped != 0 || g_log.failed != 1 || std::strcmp(g_log.lastFailed, "Q_LIVE") || g_log.deactivated != 1) return fail(2);
    g_frame = 150;
    m.FrameUpdate(cards, 4);
    if (g_log.updates != 1 || m.lastEventFrame != 150 || g_log.failed != 1) return fail(3);
    m.questToBeDropped = 9;   // no card matches: request still cleared
    m.FrameUpdate(cards, 4);
    if (m.questToBeDropped != 0 || g_log.failed != 1) return fail(4);
    cards[2].hasQuestCard = false; m.questToBeDropped = 7;   // matching card without the component: nothing failed, search stops
    m.FrameUpdate(cards, 4);
    if (m.questToBeDropped != 0 || g_log.failed != 1) return fail(5);
    std::printf("006061b0_TEST PASS\n");
    return 0;
}