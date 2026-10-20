// Behaviour test for CTCLifeTimer::FrameUpdate (0x007E59B0).
#include <cstdio>
#include <cstring>

struct CTCFadeOutAndIn {
    bool FadingIn, FadingOut;
    int Fades; float LastTime; bool LastFadeIn;
    void Fade(float time, bool fadeIn) { ++Fades; LastTime = time; LastFadeIn = fadeIn; FadingOut = !fadeIn; }
};
struct CThing {
    CTCFadeOutAndIn* Fade;     // the 0xf9 component, or 0
    CTCFadeOutAndIn Added;
    int Adds, Kills; bool LastKill;
    bool HasTC(long kind) const { return kind == 0xf9 && Fade != 0; }
    CTCFadeOutAndIn* GetTC() const { return Fade; }
    CTCFadeOutAndIn* AddTC(const char* name, bool, void*) { if (std::strcmp(name, "CTCFadeOutAndIn") == 0) { ++Adds; Fade = &Added; } return Fade; }
    void Kill(bool performOnDie) { ++Kills; LastKill = performOnDie; }
};
struct CTCLifeTimer {
    CThing* ParentThing;
    long    FramesLeft;
    bool    FadeOutBeforeKill;
    bool    PerformOnDie;
    void FrameUpdate()
    {
        if (FramesLeft > 0) {
            if (--FramesLeft == 0) {
                if (FadeOutBeforeKill) {
                    CTCFadeOutAndIn* fade;
                    if (ParentThing->HasTC(0xf9)) {
                        fade = ParentThing->GetTC();
                    } else {
                        CThing* thing = ParentThing;
                        fade = thing->AddTC("CTCFadeOutAndIn", false, 0);
                    }
                    fade->Fade(2.0f, false);
                    return;
                }
                ParentThing->Kill(PerformOnDie);
            }
        } else if (FramesLeft == 0) {
            CThing* thing = ParentThing;
            const CTCFadeOutAndIn* fade = 0;
            if (thing->HasTC(0xf9))
                fade = thing->GetTC();
            if (!fade->FadingOut && !fade->FadingIn)
                thing->Kill(PerformOnDie);
        }
    }
};

int main()
{
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    CTCLifeTimer t; t.ParentThing = &thing; t.FramesLeft = 2; t.FadeOutBeforeKill = true; t.PerformOnDie = true;
    bool ok = true;
    t.FrameUpdate();                                   // 2 -> 1: nothing
    ok = ok && t.FramesLeft == 1 && thing.Adds == 0 && thing.Kills == 0;
    t.FrameUpdate();                                   // 1 -> 0: add the fade component, fade out over 2 s
    ok = ok && t.FramesLeft == 0 && thing.Adds == 1 && thing.Added.Fades == 1 && thing.Added.LastTime == 2.0f && !thing.Added.LastFadeIn;
    t.FrameUpdate();                                   // expired, still fading: no kill
    ok = ok && thing.Kills == 0;
    thing.Added.FadingOut = false;
    t.FrameUpdate();                                   // fade done: kill with PerformOnDie
    ok = ok && thing.Kills == 1 && thing.LastKill;
    CThing other; std::memset(&other, 0, sizeof(other));
    CTCLifeTimer k; k.ParentThing = &other; k.FramesLeft = 1; k.FadeOutBeforeKill = false; k.PerformOnDie = false;
    k.FrameUpdate();                                   // no fade: kill at once
    ok = ok && other.Kills == 1 && !other.LastKill && other.Adds == 0;
    k.FramesLeft = -1; k.FrameUpdate();                // infinite life: nothing
    ok = ok && other.Kills == 1 && k.FramesLeft == -1;
    if (!ok) { std::printf("007e59b0_TEST FAIL\n"); return 1; }
    std::printf("007e59b0_TEST PASS\n");
    return 0;
}