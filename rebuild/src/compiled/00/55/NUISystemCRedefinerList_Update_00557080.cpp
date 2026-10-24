// NUISystem::CRedefinerList::Update(float deltaTime) -- the scrollable-list update, then run the fade-out timer; when it expires either rebuild the list or clear it
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00557080, 101 bytes (`ret 4`).
// Callees: NUISystem::CScrollableList::Update(float) @ 0x00555220 (landed), CRedefinerList::Refresh @ 0x00557000 (landed;
// clear, rebuild and re-lay the rows), CRedefinerList::ClearList @ 0x005567B0.
// Label: CRedefinerList vtable 0x0124A994 slot 1 (rtti-vtables.tsv); Ego_r.pdb names ?Update@CRedefinerList@NUISystem@@UAEXM@Z
// and ?FadeList@CRedefinerList@NUISystem@@QAEXM_N@Z (which arms the timer and the refresh flag).
// Layout: Ego_r.pdb IsActivated +0x1ac, RefreshAfterFade +0x1ad, FadeOutTime +0x1b0; retail 0x30 lower (+0x17c, +0x17d,
// +0x180; the landed OnActivate 0x005570F0 uses the same offsets). 0x0122DEDC is the 0.0f pool constant.
// Shape: `test ah,41h ; jne` = `FadeOutTime > 0.0f`; the decrement is `fst` reused by the `<= 0.0f` test (`test ah,41h ;
// jp` skips); the timer is zeroed before the flag branch, each arm a call with its own `ret 4`.
extern const float g_zero_0122dedc;              // 0x0122DEDC (0.0f)

namespace NUISystem {
class CScrollableList {
public:
    virtual void v000();
    virtual void Update(float deltaTime);          // slot 1; 0x00555220
};
class CRedefinerList : public CScrollableList {
public:
    unsigned char _pad04[0x17c - 4];
    bool  IsActivated;                             // +0x17c
    bool  RefreshAfterFade;                        // +0x17d
    float FadeOutTime;                             // +0x180
    virtual void Update(float deltaTime);
    void Refresh();                                // 0x00557000
    void ClearList();                              // 0x005567B0
};

void CRedefinerList::Update(float deltaTime)
{
    CScrollableList::Update(deltaTime);
    if (FadeOutTime > g_zero_0122dedc) {
        FadeOutTime -= deltaTime;
        if (FadeOutTime <= g_zero_0122dedc) {
            FadeOutTime = 0.0f;
            if (RefreshAfterFade)
                Refresh();
            else
                ClearList();
        }
    }
}
}