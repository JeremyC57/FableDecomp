// Behaviour test for 0x008956B0 (parked row): the row's logic over a flat model.
#include <cstdio>

struct V { float X, Y, Z; };
struct Thing { V Pos; bool Dead; bool HasNav; };
static bool Teleport(Thing* entity, Thing* hero, V wander) {
    if (!entity || entity->Dead) return false;
    if (!hero || hero->Dead) return false;
    float dx = hero->Pos.X - entity->Pos.X, dy = hero->Pos.Y - entity->Pos.Y;
    if (dx * dx + dy * dy > 4.0f && entity->HasNav) { entity->Pos = wander; return true; }
    return false;
}
int main() {
    Thing e = { { 0, 0, 0 }, false, true }, h = { { 5, 0, 0 }, false, true }; V w = { 4.5f, 0.2f, 0 };
    bool ok = Teleport(&e, &h, w) && e.Pos.X == 4.5f;
    ok = ok && !Teleport(&e, &h, w);                         // now within 2 units
    Thing far = { { -10, 0, 0 }, false, false }; ok = ok && !Teleport(&far, &h, w);
    h.Dead = true; far.HasNav = true; ok = ok && !Teleport(&far, &h, w);
    std::printf(ok ? "008956b0_TEST PASS\n" : "008956b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
