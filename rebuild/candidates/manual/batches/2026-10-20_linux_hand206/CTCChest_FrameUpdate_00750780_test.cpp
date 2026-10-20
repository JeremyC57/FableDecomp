// Behaviour test for CTCChest::FrameUpdate (0x00750780): idle chests stop forcing updates; otherwise the helper advances by
// 1/fps, a newly opened chest rewards the hero, a newly closed one is reset and animated with time 0.
#include <cstdio>
long g_framesPerSecond = 20;
static int g_world = 0;
struct CFrameSource {
    static float GetSecondsPerFrame() { return 1.0f / g_framesPerSecond; }
};
struct COpenCloseHelper {
    bool  Opened, Closed, Opening, Closing;
    float OpenCloseTransitionAmount;
    float LastOpenCloseTransitionAmount;
    bool  OpenNext, CloseNext; float LastTime;
    void FrameUpdate(float time) { LastTime = time; if (OpenNext) Opened = true; if (CloseNext) Closed = true; }
    bool IsIdle() const
    {
        return !Opening && !Closing && !(LastOpenCloseTransitionAmount != OpenCloseTransitionAmount);
    }
};
struct CThing {
    int ForceCalls; bool Force;
    void SetForceFrameUpdate(bool force) { ++ForceCalls; Force = force; }
};
struct CTCContainerRewardHero {
    virtual void v00() {}
    CThing* ParentThing;
    int Rewards;
    bool RewardHeroWithItems() { ++Rewards; return true; }
    CFrameSource* GetFrameSource() { ++g_world; return 0; }
};
struct CTCChest : CTCContainerRewardHero {
    COpenCloseHelper ChestOpener;
    int Anims, Inits; float AnimTime;
    void UpdateAnimation(float time) { ++Anims; AnimTime = time; }
    void InitClosed() { ++Inits; }
    void FrameUpdate();
};

void CTCChest::FrameUpdate()
{
    if (!ChestOpener.IsIdle()) {
        bool wasClosed = ChestOpener.Closed;
        bool wasOpened = ChestOpener.Opened;
        ChestOpener.FrameUpdate(GetFrameSource()->GetSecondsPerFrame());
        float time = GetFrameSource()->GetSecondsPerFrame();
        if (!wasOpened && ChestOpener.Opened) {
            RewardHeroWithItems();
        } else if (!wasClosed && ChestOpener.Closed) {
            InitClosed();
            time = 0.0f;
        }
        UpdateAnimation(time);
        return;
    }
    ParentThing->SetForceFrameUpdate(false);
}

int main()
{
    int bad = 0;
    CThing thing; thing.ForceCalls = 0; thing.Force = true;
    CTCChest c; c.ParentThing = &thing; c.Rewards = c.Anims = c.Inits = 0; c.AnimTime = -1.0f;
    COpenCloseHelper z = { false, true, false, false, 0.0f, 0.0f, false, false, 0.0f };
    c.ChestOpener = z;
    float frame = 1.0f / 20;
    c.FrameUpdate();                                 // idle
    if (thing.ForceCalls != 1 || thing.Force || c.Anims != 0 || g_world != 0) bad |= 1;
    c.ChestOpener.Opening = true; c.ChestOpener.Closed = false; c.ChestOpener.OpenNext = true;
    c.FrameUpdate();                                 // becomes open: reward + anim(frame)
    if (c.Rewards != 1 || c.Anims != 1 || c.AnimTime != frame || c.ChestOpener.LastTime != frame || g_world != 2) bad |= 2;
    c.FrameUpdate();                                 // already open: anim only
    if (c.Rewards != 1 || c.Anims != 2 || c.Inits != 0) bad |= 4;
    c.ChestOpener.Opening = false; c.ChestOpener.Closing = true; c.ChestOpener.OpenNext = false; c.ChestOpener.CloseNext = true;
    c.FrameUpdate();                                 // becomes closed: reset + anim(0)
    if (c.Inits != 1 || c.Anims != 3 || c.AnimTime != 0.0f || c.Rewards != 1) bad |= 8;
    c.ChestOpener.Closing = false; c.ChestOpener.LastOpenCloseTransitionAmount = 0.5f;
    c.FrameUpdate();                                 // transition amounts differ: still updated
    if (c.Anims != 4 || thing.ForceCalls != 1 || c.Inits != 1) bad |= 16;
    if (bad) { std::printf("00750780_TEST FAIL\n"); return 1; }
    std::printf("00750780_TEST PASS\n");
    return 0;
}
