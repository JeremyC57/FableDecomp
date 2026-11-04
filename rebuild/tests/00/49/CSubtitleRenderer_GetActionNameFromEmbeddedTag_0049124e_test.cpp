// Behaviour test for CSubtitleRenderer::GetActionNameFromEmbeddedTag (0x0049124E): mapped tags use their action, movement
// tags map their UP/RIGHT/DOWN/LEFT suffix to a direction, anything else fails.
#include <stdio.h>
#include <string.h>
struct Dir { float x, y; };
static bool StartsWith(const char* s, const char* p) { return strncmp(s, p, strlen(p)) == 0; }
static bool EndsWith(const char* s, const char* x) { long o = (long)strlen(s) - (long)strlen(x); return o >= 0 && strcmp(s + o, x) == 0; }
static int lastAction; static Dir lastDir;
static bool Lookup(const char* tag, int* action) { if (strcmp(tag, "GAME_ACTION_JUMP") == 0) { *action = 7; return true; } return false; }
static bool Assigned(int action) { lastAction = action; return true; }
static bool AssignedMove(Dir d) { lastDir = d; return true; }
static bool Resolve(const char* tag)
{
    int action;
    if (Lookup(tag, &action))
        return Assigned(action);
    else if (StartsWith(tag, "GAME_ACTION_MOVEMENT_")) {
        Dir d = { 0.0f, 0.0f };
        if (EndsWith(tag, "UP")) { d.x = 0.0f; d.y = 1.0f; }
        else if (EndsWith(tag, "RIGHT")) { d.x = 1.0f; d.y = 0.0f; }
        else if (EndsWith(tag, "DOWN")) { d.x = 0.0f; d.y = -1.0f; }
        else if (EndsWith(tag, "LEFT")) { d.x = -1.0f; d.y = 0.0f; }
        if (d.x * d.x + d.y * d.y > 1e-4f * 1e-4f)
            return AssignedMove(d);
    }
    return false;
}
int main()
{
    bool ok = Resolve("GAME_ACTION_JUMP") && lastAction == 7;
    ok = ok && Resolve("GAME_ACTION_MOVEMENT_LEFT") && lastDir.x == -1.0f && lastDir.y == 0.0f;
    ok = ok && Resolve("GAME_ACTION_MOVEMENT_DOWN") && lastDir.y == -1.0f;
    ok = ok && !Resolve("GAME_ACTION_MOVEMENT_SIDEWAYS") && !Resolve("UNKNOWN") && !Resolve("P");
    printf(ok ? "0049124e_TEST PASS\n" : "0049124e_TEST FAIL\n");
    return ok ? 0 : 1;
}