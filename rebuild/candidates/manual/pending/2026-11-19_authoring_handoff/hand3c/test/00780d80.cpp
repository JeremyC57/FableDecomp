// Behaviour test for CTCFlammable::FrameUpdate (0x00780D80), a portable model. Without a def nothing happens. With fire points, each
// point whose resistance is used up and is not burning starts burning, and any such point sets the component on fire (resistance -1).
// On catching fire (once): the FIRE_DEFAULT sound criteria plays when the thing has a sound player and a sound def with that criteria,
// and the fire opinion source is attached. While on fire and not damaged per point, once the frame passes the next damage frame, a game
// object takes the per-cycle damage; the fire goes out when |caused + per cycle| reaches the cap, or when health falls below 1e-4 (a
// creature then starts dying); the next damage frame is now + period * FPS.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Point { long resistance; bool burning; };
struct Def { float max_damage, per_cycle, period; };
struct Thing { bool game_object, creature, has_sound, has_criteria; float health; int dying; int sounds; int opinion_sources; };
struct Flammable {
    const Def* def; std::vector<Point> points; bool burning; long next_damage_frame, resistance; float caused;
    bool per_point, has_point; int extinguished;
    bool IsOnFire() const { return resistance <= 0; }
    void Extinguish() { ++extinguished; resistance = 1; }
    void FrameUpdate(Thing& t, long frame, long fps)
    {
        if (!def) return;
        if (has_point)
            for (size_t i = 0; i < points.size(); ++i) {
                if (points[i].resistance <= 0 && !points[i].burning) points[i].burning = true;
                if (points[i].resistance <= 0) resistance = -1;
            }
        if (IsOnFire() && !burning) {
            burning = true;
            if (t.has_sound && t.has_criteria) ++t.sounds;
            ++t.opinion_sources;
        }
        if (IsOnFire() && !per_point && frame > next_damage_frame) {
            if (t.game_object) {
                bool out = false;
                if (fabsf(caused + def->per_cycle) >= def->max_damage) { Extinguish(); out = true; }
                caused += def->per_cycle;
                t.health += def->per_cycle;
                if (t.health < 1e-4f) { if (t.creature) ++t.dying; if (!out) Extinguish(); }
            }
            next_damage_frame = (long)(frame + def->period * fps);
        }
    }
};
int main()
{
    int bad = 0;
    Def d = { 30.0f, -10.0f, 2.0f };
    Thing t = { true, true, true, true, 25.0f, 0, 0, 0 };
    Flammable f; f.def = &d; f.burning = false; f.next_damage_frame = 0; f.resistance = 5; f.caused = 0; f.per_point = false; f.has_point = true; f.extinguished = 0;
    Point p1 = { 0, false }, p2 = { 3, false }; f.points.push_back(p1); f.points.push_back(p2);
    f.FrameUpdate(t, 10, 30);
    if (!f.points[0].burning || f.points[1].burning || f.resistance != -1 || !f.burning || t.sounds != 1 || t.opinion_sources != 1) ++bad;
    if (t.health != 15.0f || f.caused != -10.0f || f.next_damage_frame != 70 || f.extinguished) ++bad;
    f.FrameUpdate(t, 20, 30);                                    // before the next damage frame: no damage, no second sound
    if (t.health != 15.0f || t.sounds != 1) ++bad;
    f.FrameUpdate(t, 71, 30);                                    // health 5
    f.FrameUpdate(t, 200, 30);                                   // |-20 - 10| reaches 30: out; health -5 -> dies, no second extinguish
    if (f.extinguished != 1 || t.dying != 1 || t.health != -5.0f) ++bad;
    Flammable none; none.def = 0; none.FrameUpdate(t, 1000, 30);
    printf(bad ? "00780d80_TEST FAIL\n" : "00780d80_TEST PASS\n");
    return bad ? 1 : 0;
}
