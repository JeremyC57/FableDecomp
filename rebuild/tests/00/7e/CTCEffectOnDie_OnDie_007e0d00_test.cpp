// Behaviour test for CTCEffectOnDie::OnDie (0x007E0D00): a model of the row -- paused: nothing; otherwise the death sound
// criteria is played (once, with true/false/false/-1) when the parent has a sound player, and the effect is created at the
// parent's position when one is set.
#include <stdio.h>
struct C3DVector { float X, Y, Z; };
static int g_played = 0, g_created = 0; static long g_lastEffect = 0; static const char* g_lastCriteria = 0;
struct CTCSoundPlayer { unsigned long PlayAttachedCriteria(const char* c, bool a, bool b, bool d, long e) { if (a && !b && !d && e == -1) { ++g_played; g_lastCriteria = c; } return 0; } };
struct CThing { C3DVector Pos; CTCSoundPlayer* Sound; const C3DVector& GetPos() const { return Pos; }
    bool GetTC(CTCSoundPlayer** p) { if (Sound) { *p = Sound; return true; } return false; } };
static CThing* Create(long effect, const C3DVector& pos, bool attached) { if (!attached && pos.X == 1.0f) { ++g_created; g_lastEffect = effect; } return 0; }
struct CTCEffectOnDie {
    CThing* ParentThing; long Effect; const char* DeathSoundCriteria; bool Paused;
    void OnDie() {
        if (!Paused) {
            CTCSoundPlayer* s;
            if (ParentThing->GetTC(&s)) s->PlayAttachedCriteria(DeathSoundCriteria, true, false, false, -1);
            if (Effect) Create(Effect, ParentThing->GetPos(), false);
        }
    }
};
int main()
{
    CTCSoundPlayer sp; CThing t; t.Pos.X = 1.0f; t.Pos.Y = t.Pos.Z = 0.0f; t.Sound = &sp;
    CTCEffectOnDie e; e.ParentThing = &t; e.Effect = 7; e.DeathSoundCriteria = "DIE"; e.Paused = true;
    e.OnDie(); int ok = g_played == 0 && g_created == 0;
    e.Paused = false; e.OnDie(); ok = ok && g_played == 1 && g_created == 1 && g_lastEffect == 7 && g_lastCriteria == e.DeathSoundCriteria;
    t.Sound = 0; e.Effect = 0; e.OnDie(); ok = ok && g_played == 1 && g_created == 1;
    printf(ok ? "007e0d00_TEST PASS\n" : "007e0d00_TEST FAIL\n");
    return ok ? 0 : 1;
}