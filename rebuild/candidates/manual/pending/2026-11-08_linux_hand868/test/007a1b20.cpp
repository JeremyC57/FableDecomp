// Behaviour test for 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate: a portable model of the beam state machine --
// frame count and clock advance; 1 -> 2 after the delay (looping sound); 2 -> 3 after the activation time while home and target
// live (frame count -1, children activated) else deactivate; 3: deactivate on an invalid target (or a temporary beam after its
// life), else re-arm the electrocuted effect after a successful hit and every OnhitEveryFrame frames hit the target (decapitating
// once the decapitation clock passes its time), recording whether the hit landed; 4 -> 5 once every child is inactive (end sound);
// 5 -> 6 after the deactivation time; finished children are erased.
#include <cstdio>
#include <vector>

struct Beam {
    int state; float time, timeBase, delay, activation, deactivation, decapTime, decapClock; long frameCount, onHitEvery;
    bool targetAlive, targetLightningable, successfulHit, blocked; int hits, decapHits, sounds, effectsReset, deactivations;
    std::vector<Beam*> children; bool activated;
    void Deactivate() { ++deactivations; state = children.empty() ? 5 : 4; timeBase = time; }
    void FrameUpdate(float dt)
    {
        ++frameCount; time += dt;
        switch (state) {
        case 1: if (time - timeBase >= delay) { timeBase = time; state = 2; ++sounds; } break;
        case 2:
            if (targetAlive) {
                if (time - timeBase >= activation) { timeBase = time; state = 3; frameCount = -1;
                    for (size_t i = 0; i < children.size(); ++i) children[i]->activated = true; }
            } else Deactivate();
            break;
        case 3:
            if (!targetAlive || !targetLightningable) Deactivate();
            else {
                if (successfulHit) { successfulHit = false; ++effectsReset; }
                decapClock += dt;
                if (decapClock >= decapTime && decapTime != -1.0f && frameCount % onHitEvery == 0) { ++decapHits; successfulHit = !blocked; }
                else if (frameCount % onHitEvery == 0) { ++hits; successfulHit = !blocked; }
            }
            break;
        case 4: { bool all = true; for (size_t i = 0; i < children.size(); ++i) all = all && children[i]->state == 6;
                  if (all) { timeBase = time; state = 5; ++sounds; } } break;
        case 5: if (time - timeBase >= deactivation) { timeBase = time; state = 6; } break;
        }
        if (state != 0 && state != 6)
            for (size_t i = 0; i < children.size(); ) {
                children[i]->FrameUpdate(dt);
                if (children[i]->state == 6) children.erase(children.begin() + i); else ++i;
            }
    }
};

int main()
{
    Beam b = Beam();
    b.state = 1; b.delay = 0.2f; b.activation = 0.2f; b.deactivation = 0.2f; b.decapTime = 0.5f; b.onHitEvery = 2;
    b.targetAlive = b.targetLightningable = true;
    Beam c = Beam(); c.state = 0; b.children.push_back(&c);
    for (int i = 0; i < 4; ++i) b.FrameUpdate(0.1f);           // delay passes (2), activation passes (3)
    bool ok = b.state == 3 && c.activated && b.sounds == 1;
    for (int i = 0; i < 4; ++i) b.FrameUpdate(0.1f);           // frames 0..3: hits on 0 and 2
    ok = ok && b.hits + b.decapHits == 2 && !b.successfulHit && b.effectsReset == 2;
    for (int i = 0; i < 4; ++i) b.FrameUpdate(0.1f);           // decapitation clock passes 0.5
    ok = ok && b.decapHits >= 1;
    b.targetAlive = false; b.FrameUpdate(0.1f);                // deactivate: child pending -> 4
    ok = ok && b.state == 4 && b.deactivations == 1;
    c.state = 6; b.FrameUpdate(0.1f);                          // all children done -> 5, child erased
    ok = ok && b.state == 5 && b.sounds == 2 && b.children.empty();
    b.FrameUpdate(0.1f); b.FrameUpdate(0.1f);
    ok = ok && b.state == 6;
    if (!ok) { std::printf("007a1b20_TEST FAIL\n"); return 1; }
    std::printf("007a1b20_TEST PASS\n");
    return 0;
}
