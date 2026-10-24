// Behaviour test for 0x0080C460 CTCDExplosion::Create(long, const C3DVector&) (model copy with stubbed engine calls).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCDExplosion { C3DVector StartPos, EndPos; C3DVector constructedAt; int constructed; };
struct CThing { unsigned int UsingComponents[9]; CTCDExplosion* PTCDriver; int activateOk; int killed; int activated; };
static int g_inMap = 1, g_createOk = 1, g_lastMap = -1, g_lastPlayer = -1, g_lastDef = -1; static bool g_lastPersist = false;
static CThing g_thing; static CTCDExplosion g_expl;
static bool IsPosInMap(const C3DVector&) { return g_inMap != 0; }
static long GetMapNumberAt(const C3DVector& p) { return (long)p.Z; }
static CThing* ThingCreate(long def, long player, long map, bool persist)
{ g_lastDef = (int)def; g_lastPlayer = (int)player; g_lastMap = (int)map; g_lastPersist = persist; return g_createOk ? &g_thing : 0; }
static CThing* create_model(long defIndex, const C3DVector& position)
{
    CThing* result = 0;
    if (IsPosInMap(position)) {
        CThing* thing = ThingCreate(defIndex, 4, GetMapNumberAt(position), true);
        if (thing) {
            if (thing->UsingComponents[2] & 0x2) {
                CTCDExplosion* e = thing->PTCDriver;
                e->StartPos = position; e->EndPos = position;
                e->constructedAt = position; e->constructed++;
                thing->activated++;
                if (thing->activateOk) result = thing;
            }
            else
                thing->killed++;
        }
    }
    return result;
}
static void reset(unsigned int bits, int act)
{ for (int i = 0; i < 9; ++i) g_thing.UsingComponents[i] = 0; g_thing.UsingComponents[2] = bits; g_thing.PTCDriver = &g_expl;
  g_thing.activateOk = act; g_thing.killed = 0; g_thing.activated = 0; g_expl.constructed = 0; g_expl.StartPos.X = 0; g_expl.EndPos.Y = 0; }
int main()
{
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 7.0f;
    int ok = 1;
    reset(0x2, 1);
    if (create_model(55, p) != &g_thing || g_lastDef != 55 || g_lastPlayer != 4 || g_lastMap != 7 || !g_lastPersist) ok = 0;
    if (g_expl.StartPos.X != 1.0f || g_expl.EndPos.Y != 2.0f || g_expl.constructed != 1 || g_expl.constructedAt.Z != 7.0f) ok = 0;
    reset(0x2, 0);
    if (create_model(55, p) != 0 || g_thing.activated != 1 || g_thing.killed != 0) ok = 0;
    reset(0x1, 1);
    if (create_model(55, p) != 0 || g_thing.killed != 1 || g_expl.constructed != 0) ok = 0;
    reset(0x2, 1); g_createOk = 0;
    if (create_model(55, p) != 0) ok = 0;
    g_createOk = 1; g_inMap = 0; g_lastDef = -1;
    if (create_model(56, p) != 0 || g_lastDef != -1) ok = 0;
    if (!ok) { std::printf("0080c460_TEST FAIL\n"); return 1; }
    std::printf("0080c460_TEST PASS\n");
    return 0;
}