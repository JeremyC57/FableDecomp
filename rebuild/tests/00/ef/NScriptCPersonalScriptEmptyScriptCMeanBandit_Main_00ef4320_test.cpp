// Behaviour test for CMeanBandit::Main 0x00EF4320: the same loop driven through a scripted game model.
#include <cstdio>
#include <cstring>
struct Thing { int id; };
struct Game {
    char log[512]; int frames; int stripFrom, stripTo, terminateAt; float range; bool bound, steal; int rangeSet;
    Game() : frames(0), stripFrom(0), stripTo(0), terminateAt(0), range(-1.0f), bound(false), steal(false), rangeSet(0) { log[0] = 0; }
    void add(const char* s) { std::strcat(log, s); }
    void NewScriptFrame() { ++frames; add("F"); }
    bool IsHeroNaked() const { return frames >= stripFrom && frames < stripTo; }
    void EntitySetAsHavingBoundHands(Thing&, bool b) { bound = b; add(b ? "B" : "b"); }
    void SetStealStealableItems(Thing, bool b) { steal = b; add("S"); }
    void SetCombatNearbyBreakOffRange(Thing, float r) { range = r; rangeSet = 1; add("R"); }
    void ResetStealStealableItems(Thing) { steal = false; add("s"); }
    void ResetCombatNearbyBreakOffRange(Thing) { rangeSet = 0; add("r"); }
};
struct MeanBandit {
    Game* Game_; Thing Me; int predicateSet;
    bool IsActiveThreadTerminating() const { return Game_->frames >= Game_->terminateAt; }
    void SetPredicate(const Thing& t) { predicateSet = t.id; }
    void Main()
    {
        SetPredicate(Me);
        Game_->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!IsActiveThreadTerminating()) {
            Game_->EntitySetAsHavingBoundHands(Me, true);
            while (!Game_->IsHeroNaked()) {
                Game_->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game_->EntitySetAsHavingBoundHands(Me, false);
            Game_->SetStealStealableItems(Me, true);
            Game_->SetCombatNearbyBreakOffRange(Me, 0.0f);
            while (Game_->IsHeroNaked()) {
                Game_->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game_->ResetStealStealableItems(Me);
            Game_->ResetCombatNearbyBreakOffRange(Me);
            Game_->NewScriptFrame();
        }
    }
};
static int run(int stripFrom, int stripTo, int terminateAt, const char* expect, bool bound, bool steal, int rangeSet)
{
    Game g; g.stripFrom = stripFrom; g.stripTo = stripTo; g.terminateAt = terminateAt;
    MeanBandit b; b.Game_ = &g; b.Me.id = 7; b.predicateSet = 0;
    b.Main();
    if (b.predicateSet != 7) return 1;
    if (std::strcmp(g.log, expect) != 0) { std::printf("got %s want %s\n", g.log, expect); return 1; }
    if (g.bound != bound || g.steal != steal || g.rangeSet != rangeSet) return 1;
    if (rangeSet && g.range != 0.0f) return 1;
    return 0;
}
int main()
{
    int bad = 0;
    bad |= run(100, 200, 1, "F", false, false, 0);                 // terminates on the first frame: nothing else happens
    bad |= run(3, 5, 4, "FBFFbSRF", false, true, 1);               // bound, hero strips at frame 3, terminated while he is unclothed
    bad |= run(3, 5, 7, "FBFFbSRFFsrFBF", true, false, 0);         // full cycle: reset, frame, bound again, terminated in the clothed wait
    bad |= run(100, 200, 3, "FBFF", true, false, 0);               // never unclothed: stays bound until the thread ends
    bad |= run(0, 6, 6, "FBbSRFFFFF", false, true, 1);             // unclothed from the start: the clothed wait is skipped
    if (bad) { std::printf("00ef4320_TEST FAIL\n"); return 1; }
    std::printf("00ef4320_TEST PASS\n");
    return 0;
}