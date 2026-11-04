// Behaviour test for 0x008956B0 CGameScriptInterface::EntityTeleportToHeroPosition: a portable model of the retail logic
// (alive entity, live hero, squared XY distance > 4, navigation component, wander position found, physics present).
#include <cstdio>

struct V { float X, Y, Z; };
struct Thing { V Pos; bool Dead; bool HasNav; bool HasPhysics; };
static bool WanderFound = true;
static bool Teleport(Thing* entity, Thing* hero, V wander) {
    if (!entity || entity->Dead) return false;
    if (!hero || hero->Dead) return false;
    float heroX = hero->Pos.X, heroY = hero->Pos.Y;
    float posX = entity->Pos.X, posY = entity->Pos.Y;
    float dx = heroX - posX, dy = heroY - posY;
    if (dx * dx + dy * dy > 4.0f && entity->HasNav) {
        V target = wander;
        if (WanderFound && entity->HasPhysics) { entity->Pos = target; return true; }
    }
    return false;
}
int main() {
    Thing e = { { 0, 0, 0 }, false, true, true }, h = { { 5, 0, 0 }, false, true, true }; V w = { 4.5f, 0.2f, 0 };
    bool ok = Teleport(&e, &h, w) && e.Pos.X == 4.5f;
    ok = ok && !Teleport(&e, &h, w);                         // now within 2 units
    Thing far = { { -10, 0, 0 }, false, false, true }; ok = ok && !Teleport(&far, &h, w);
    far.HasNav = true; far.HasPhysics = false; ok = ok && !Teleport(&far, &h, w);
    far.HasPhysics = true; WanderFound = false; ok = ok && !Teleport(&far, &h, w);
    WanderFound = true; h.Dead = true; ok = ok && !Teleport(&far, &h, w);
    h.Dead = false; ok = ok && Teleport(&far, &h, w);
    Thing edge = { { 3, 0, 0 }, false, true, true }; ok = ok && !Teleport(&edge, &h, w);   // exactly 2 units: not > 4
    std::printf(ok ? "008956b0_TEST PASS\n" : "008956b0_TEST FAIL\n");
    return ok ? 0 : 1;
}