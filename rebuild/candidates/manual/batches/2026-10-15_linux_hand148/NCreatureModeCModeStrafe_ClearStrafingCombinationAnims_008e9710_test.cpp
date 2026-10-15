#include <cstdio>
// CModeStrafe::ClearStrafingCombinationAnims model: fade out the two combination anims when present and running.
struct CAnimSet {
    float CurrentAlpha; float ChangeAlphaBy; bool Finished;
    void FadeOut() { if (!Finished) { Finished = true; ChangeAlphaBy = CurrentAlpha; } }
};
struct CAnimateThing {
    CAnimSet a1b; CAnimSet* p19;
    CAnimSet* FindCombinationAnim(long type) { return type == 0x1b ? &a1b : p19; }
};
struct CThing { bool dead; bool hasAnim; CAnimateThing anim; };
struct CModeStrafe {
    CThing* ParentThing;
    void ClearStrafingCombinationAnims(bool)
    {
        if (ParentThing && !ParentThing->dead && ParentThing->hasAnim) {
            CAnimateThing& animator = ParentThing->anim;
            CAnimSet* set = animator.FindCombinationAnim(0x1b);
            if (set) set->FadeOut();
            set = animator.FindCombinationAnim(0x19);
            if (set) set->FadeOut();
        }
    }
};
int main()
{
    CThing t; t.dead = false; t.hasAnim = true; t.anim.a1b.CurrentAlpha = 0.5f; t.anim.a1b.ChangeAlphaBy = 0.0f; t.anim.a1b.Finished = false; t.anim.p19 = 0;
    CModeStrafe m; m.ParentThing = &t; m.ClearStrafingCombinationAnims(false);
    CModeStrafe n; n.ParentThing = 0; n.ClearStrafingCombinationAnims(true);
    if (!(t.anim.a1b.Finished && t.anim.a1b.ChangeAlphaBy == 0.5f)) { std::printf("008e9710_TEST FAIL\n"); return 1; }
    std::printf("008e9710_TEST PASS\n");
    return 0;
}
