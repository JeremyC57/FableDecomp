// Behaviour test for CTCCreatureGenerator::UpdateUndead (0x0074ACE0): with no triggerers the undead flag drops; outside
// the radius a surfaced undead returns to earth and a buried one (free, with a generation point) is moved and rises with a
// strike; inside the radius only a buried undead rises.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
enum { ACT_NONE, ACT_RETURN, ACT_RISE_STRIKE };
struct CThing { C3DVector Pos; bool Underground; bool Free; int Action; int Moves; C3DVector MovedTo; };
static bool g_pointOk = true;
struct CTCCreatureGenerator {
    CThing** TriggerersStart;
    CThing** TriggerersFinish;
    CThing** Created; int NumCreated;
    float TriggerRadius;
    bool UndeadTriggered;
    bool GetUndeadGenerationPoint(CThing* t, C3DVector* out)
    { if (!g_pointOk) return false; out->X = t->Pos.X + 1; out->Y = t->Pos.Y; out->Z = 0; return true; }
    void UpdateUndead()
    {
        if (TriggerersStart == TriggerersFinish)
            UndeadTriggered = false;
        for (CThing** it = TriggerersStart; it != TriggerersFinish; ++it) {
            CThing* triggerer = *it;
            for (int i = 0; i < NumCreated; ++i) {
                CThing* creature = Created[i];
                C3DVector delta;
                delta.X = triggerer->Pos.X - creature->Pos.X;
                delta.Y = triggerer->Pos.Y - creature->Pos.Y;
                bool underground = creature->Underground;
                if (delta.Y * delta.Y + delta.X * delta.X > TriggerRadius * TriggerRadius) {
                    if (underground) {
                        bool free = creature->Free;
                        C3DVector pos;
                        if (free && GetUndeadGenerationPoint(triggerer, &pos)) {
                            creature->MovedTo = pos; ++creature->Moves;
                            creature->Action = ACT_RISE_STRIKE;
                        }
                    } else {
                        creature->Action = ACT_RETURN;
                    }
                } else if (underground) {
                    creature->Action = ACT_RISE_STRIKE;
                }
            }
        }
    }
};
static int fail() { std::printf("0074ace0_TEST FAIL\n"); return 1; }
int main()
{
    CThing hero; std::memset(&hero, 0, sizeof(hero));
    CThing a, b, c, d;
    std::memset(&a, 0, sizeof(a)); std::memset(&b, 0, sizeof(b)); std::memset(&c, 0, sizeof(c)); std::memset(&d, 0, sizeof(d));
    a.Pos.X = 20;                                   // far, surfaced -> return to earth
    b.Pos.X = 20; b.Underground = true; b.Free = true;   // far, buried, free -> moved + rise
    c.Pos.X = 2; c.Underground = true;              // near, buried -> rise
    d.Pos.X = 2;                                    // near, surfaced -> nothing
    CThing* created[4] = { &a, &b, &c, &d };
    CThing* trig[1] = { &hero };
    CTCCreatureGenerator g;
    g.TriggerersStart = trig; g.TriggerersFinish = trig;
    g.Created = created; g.NumCreated = 4; g.TriggerRadius = 10.0f; g.UndeadTriggered = true;
    g.UpdateUndead();
    if (g.UndeadTriggered || a.Action != ACT_NONE) return fail();
    g.TriggerersFinish = trig + 1; g.UndeadTriggered = true;
    g.UpdateUndead();
    if (!g.UndeadTriggered) return fail();
    if (a.Action != ACT_RETURN || b.Action != ACT_RISE_STRIKE || b.Moves != 1 || b.MovedTo.X != 1.0f) return fail();
    if (c.Action != ACT_RISE_STRIKE || c.Moves != 0 || d.Action != ACT_NONE) return fail();
    b.Action = ACT_NONE; g_pointOk = false;
    g.UpdateUndead();
    if (b.Action != ACT_NONE || b.Moves != 1) return fail();
    std::printf("0074ace0_TEST PASS\n");
    return 0;
}
