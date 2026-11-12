// Behaviour test for NScript::CGlobal_TeleportToHeroGuildScript::Main (0x00CDD6B0): with a teleport under way and the
// guild marker alive, a hero at the marker or inside the guild goes back to the remembered recall spot (only when one is
// remembered) and the spot is cleared; a hero away from the guild makes his followers wait, his spot is remembered and he
// is teleported to the marker. The script then deactivates itself, except when the thread terminates.
#include <cstdio>

struct CVec { float x, y, z; };
class CGame { public:
    bool teleporting, markerAlive, atMarker, insideLoaded;
    int  terminateAtCall, calls;
    CVec recallPos; float recallAngle;
    int  followers, waits, teleToPos, teleToMarker, facing, deactivated, recallSets;
    bool Terminating() { return ++calls >= terminateAtCall; }
};
static CGame* G;

static void Main()
{
    if (G->Terminating()) return;
    if (G->teleporting) {
        if (G->Terminating()) return;
        if (G->markerAlive) {
            if (G->Terminating()) return;
            if (G->atMarker) {
                if (G->Terminating()) return;
            } else {
                if (G->Terminating()) return;
                if (G->insideLoaded) {
                    if (G->Terminating()) return;
                } else {
                    if (G->Terminating()) return;
                    for (int i = 0; i < G->followers; ++i) {
                        if (G->Terminating()) return;
                        ++G->waits;
                    }
                    if (G->Terminating()) return;
                    G->recallPos.x = 5.0f; G->recallAngle = 1.0f; ++G->recallSets;
                    ++G->teleToMarker;
                    goto done;
                }
            }
            if (G->Terminating()) return;
            if (G->recallPos.x * G->recallPos.x + G->recallPos.y * G->recallPos.y + G->recallPos.z * G->recallPos.z > 1e-12f) {
                if (G->Terminating()) return;
                ++G->teleToPos; ++G->facing;
            }
            G->recallPos.x = G->recallPos.y = G->recallPos.z = 0.0f; G->recallAngle = 0.0f; ++G->recallSets;
        }
    done:;
    }
    ++G->deactivated;
}

static CGame Make(bool atMarker, bool inside, float rx, int followers, int termAt)
{
    CGame g = { true, true, atMarker, inside, termAt, 0, { rx, 0.0f, 0.0f }, 0.0f, followers, 0, 0, 0, 0, 0, 0 };
    return g;
}

int main()
{
    bool ok = true;
    CGame a = Make(true, false, 3.0f, 2, 1000); G = &a; Main();            // at the marker, spot remembered
    ok = ok && a.teleToPos == 1 && a.facing == 1 && a.recallSets == 1 && a.recallPos.x == 0.0f && a.deactivated == 1 && a.waits == 0;
    CGame b = Make(false, true, 0.0f, 2, 1000); G = &b; Main();            // inside the guild, no spot
    ok = ok && b.teleToPos == 0 && b.recallSets == 1 && b.deactivated == 1;
    CGame c = Make(false, false, 0.0f, 3, 1000); G = &c; Main();           // away: followers wait, spot saved, to marker
    ok = ok && c.waits == 3 && c.teleToMarker == 1 && c.recallPos.x == 5.0f && c.teleToPos == 0 && c.deactivated == 1;
    CGame d = Make(false, false, 0.0f, 3, 7); G = &d; Main();              // terminates in the follower loop
    ok = ok && d.waits == 1 && d.teleToMarker == 0 && d.deactivated == 0;
    if (!ok) { std::printf("00cdd6b0_TEST FAIL\n"); return 1; }
    std::printf("00cdd6b0_TEST PASS\n");
    return 0;
}