extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x0088E540 CGameScriptInterface::EntityTeleportToThing: when both script things are valid, the entity is
// teleported to the target's position and XY angle (third flag true, the caller's flag passed on); otherwise nothing.
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public: bool valid; C3DVector pos; float angle; bool IsValid() const { return valid; } C3DVector GetPos() const { return pos; } float GetAngleXY() const { return angle; } };
static int calls = 0; static C3DVector gotPos; static float gotAngle; static bool gotA, gotB;
static void EntityTeleportToPosition(const CScriptThing&, const C3DVector& p, float a, bool x, bool y) { ++calls; gotPos = p; gotAngle = a; gotA = x; gotB = y; }
static void EntityTeleportToThing(const CScriptThing& e, const CScriptThing& t, bool b) { if (e.IsValid() && t.IsValid()) EntityTeleportToPosition(e, t.GetPos(), t.GetAngleXY(), true, b); }
int main()
{
    CScriptThing e = { true, { 0, 0, 0 }, 0 }, t = { true, { 1, 2, 3 }, 1.5f }, bad = { false, { 0, 0, 0 }, 0 };
    EntityTeleportToThing(e, t, false);
    CHECK(calls == 1 && gotPos.X == 1 && gotPos.Y == 2 && gotPos.Z == 3 && gotAngle == 1.5f && gotA && !gotB);
    EntityTeleportToThing(e, t, true); CHECK(calls == 2 && gotB);
    EntityTeleportToThing(bad, t, true); EntityTeleportToThing(e, bad, true); CHECK(calls == 2);
    if (fails == 0) printf("0088e540_TEST PASS\n"); else printf("0088e540_TEST FAIL\n");
    return fails;
}