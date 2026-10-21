#include <cstdio>
#include <string>
// behaviour test: OnHeroAttackCrowdMember -- always reschedules the opinion-source clear ((r + 0.5) * fps * 60 frames ahead);
// unless already hostile (state 10): pops one pushed source, drops a pending deed (only from a living hero with a deed log),
// attaches the "hero attacked crowd" source (counting it on success) and enters state 10
struct Crowd {
    int CrowdState, DeedHandle, SourcesPushed, FrameToClear;
    bool HeroAlive, HeroHasLog, AttachOk; int Cleared, DeedsRemoved; std::string Attached;
    void OnHeroAttackCrowdMember(int now, float r, int fps)
    {
        { float delay = (r + 0.5f) * fps * 60.0f; FrameToClear = (int)(now + delay); }
        if (CrowdState == 10) return;
        if (SourcesPushed > 0) { Cleared++; --SourcesPushed; }
        if (DeedHandle != -1) { if (HeroAlive && HeroHasLog) DeedsRemoved++; DeedHandle = -1; }
        { std::string name = ""; name = "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD";
          if (name != "") { Attached = name; if (AttachOk) ++SourcesPushed; }
          CrowdState = 10; }
    }
};
int main()
{
    Crowd c = { 2, 7, 2, 0, true, true, true, 0, 0, "" };
    c.OnHeroAttackCrowdMember(100, 0.25f, 30);
    bool ok = c.FrameToClear == 100 + 1350 && c.CrowdState == 10 && c.Cleared == 1 && c.DeedsRemoved == 1 && c.DeedHandle == -1
              && c.SourcesPushed == 2 && c.Attached == "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD";
    c.OnHeroAttackCrowdMember(200, 0.0f, 30);       // already hostile: only the reschedule
    ok = ok && c.FrameToClear == 200 + 900 && c.Cleared == 1 && c.SourcesPushed == 2;
    Crowd d = { 1, 3, 0, 0, false, true, false, 0, 0, "" };
    d.OnHeroAttackCrowdMember(0, 0.0f, 30);         // dead hero: deed dropped without the log call; attach fails
    ok = ok && d.DeedsRemoved == 0 && d.DeedHandle == -1 && d.SourcesPushed == 0 && d.Cleared == 0 && d.CrowdState == 10;
    if (!ok) { std::printf("007849c0_TEST FAIL\n"); return 1; }
    std::printf("007849c0_TEST PASS\n");
    return 0;
}
