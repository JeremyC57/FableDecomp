// CTCChest::FrameUpdate() -- advance the lid's open/close transition by one frame; on the frame the chest becomes open the hero gets its reward items, on the frame it becomes closed the chest is reset; the lid animation follows; an idle chest stops forcing frame updates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750780, 205 bytes (plain `ret`).
// Callees: the CTCBase accessor @ 0x00686D60 (`[0x013B8A1C]+0x1c`, object unidentified; named GetFrameSource as two landed
// callers do; the result is unused, as in the landed CTCNoise::ResetLifeTimer 0x007497C0, which calls it GetWorld),
// COpenCloseHelper::FrameUpdate(float) @ 0x0074FF60, CTCContainerRewardHero::RewardHeroWithItems() @ 0x0074EE90 (on `this`),
// CTCChest::UpdateAnimation(float) @ 0x00750470, CTCChest::InitClosed() @ 0x007502D0, CThing::SetForceFrameUpdate(bool)
// @ 0x004C7C60. Globals: g_framesPerSecond @ 0x01375550 (`fild`), 1.0f @ 0x0122DED8.
// Layout: CTCChest ChestOpener (COpenCloseHelper, Ego_r.pdb +0x20) at retail +0x1c: Opened +0x1c, Closed +0x1d, Opening +0x1e,
// Closing +0x1f, OpenCloseTransitionAmount +0x20, LastOpenCloseTransitionAmount +0x24.
// Shape: the frame time is an inline static reached through PeekComponent() (the accessor is still called, twice);
// the previous Opened/Closed states are read before the helper update (Closed parked in a frame byte, Opened in bl); the
// idle test is an inline IsIdle() (`!(Last != Transition)` = `fld ; fld ; fucompp ; test ah,44h ; jnp`, the idle block
// placed last); the reward/reset arms are an `if / else if` whose shared UpdateAnimation tail VC7.1 duplicates into the
// reward arm (a separate `return` there picks edx for the reset arm's reload).
extern long g_framesPerSecond;                          // 0x01375550
class CMainGameComponent { public:
    static float GetSecondsPerFrame() { return 1.0f / g_framesPerSecond; }
};
struct COpenCloseHelper {
    bool  Opened;                                       // +0x00
    bool  Closed;                                       // +0x01
    bool  Opening;                                      // +0x02
    bool  Closing;                                      // +0x03
    float OpenCloseTransitionAmount;                    // +0x04
    float LastOpenCloseTransitionAmount;                // +0x08
    void FrameUpdate(float time);                       // 0x0074FF60
    bool IsIdle() const
    {
        return !Opening && !Closing && !(LastOpenCloseTransitionAmount != OpenCloseTransitionAmount);
    }
};
class CThing { public:
    void SetForceFrameUpdate(bool force);               // 0x004C7C60
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60
};
class CTCContainer : public CTCBase
{
public:
};

class CTCContainerRewardHero : public CTCContainer { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    bool RewardHeroWithItems();                         // 0x0074EE90
};
class CTCChest : public CTCContainerRewardHero { public:
    COpenCloseHelper ChestOpener;                       // +0x1c
    void UpdateAnimation(float time);                   // 0x00750470
    void InitClosed();                                  // 0x007502D0
    void FrameUpdate();
};

void CTCChest::FrameUpdate()
{
    if (!ChestOpener.IsIdle()) {
        bool wasClosed = ChestOpener.Closed;
        bool wasOpened = ChestOpener.Opened;
        ChestOpener.FrameUpdate(PeekComponent().GetSecondsPerFrame());
        float time = PeekComponent().GetSecondsPerFrame();
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