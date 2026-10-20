// Behaviour test for CTCLook::ImplementHeadSettings @ 0x00753BA0 (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
static char g_log[64];
struct CAnimateThing { void RemoveCombinationAnimsOfType(long t) { char b[8]; sprintf(b, "R%ld", t); strcat(g_log, b); } };
struct CTCAnimation { CAnimateThing anim; CAnimateThing& GetAnimation() { return anim; } };
static long g_animV, g_animH;
struct Model {
    CTCAnimation* component; float HeadAngleRelToBodyXY, HeadAngleRelToBodyYZ; bool FirstTimeThrough; bool inside;
    void InitialiseHeadAnims() { strcat(g_log, "I"); }
    bool IsInsideAnimationRange() { return inside; }
    void UpdateHeadCreatureAnim(long* a, long type, CAnimateThing& anim) { char b[8]; sprintf(b, "U%ld", type); strcat(g_log, b); if (a == &g_animV) strcat(g_log, "v"); if (a == &g_animH) strcat(g_log, "h"); }
    void ImplementHeadSettings()
    {
        if (FirstTimeThrough) { FirstTimeThrough = false; InitialiseHeadAnims(); }
        if (!IsInsideAnimationRange()) return;
        CAnimateThing& animation = component->GetAnimation();
        if (HeadAngleRelToBodyXY == 0.0f && HeadAngleRelToBodyYZ == 0.0f) { animation.RemoveCombinationAnimsOfType(12); animation.RemoveCombinationAnimsOfType(11); }
        else { UpdateHeadCreatureAnim(&g_animV, 12, animation); UpdateHeadCreatureAnim(&g_animH, 11, animation); }
    }
};
int main()
{
    bool ok = true; CTCAnimation comp;
    Model m = { &comp, 0.0f, 0.0f, true, true };
    m.ImplementHeadSettings(); ok = ok && strcmp(g_log, "IR12R11") == 0 && !m.FirstTimeThrough;
    g_log[0] = 0; m.HeadAngleRelToBodyXY = 0.25f; m.ImplementHeadSettings(); ok = ok && strcmp(g_log, "U12vU11h") == 0;
    g_log[0] = 0; m.HeadAngleRelToBodyXY = 0.0f; m.HeadAngleRelToBodyYZ = -0.5f; m.ImplementHeadSettings(); ok = ok && strcmp(g_log, "U12vU11h") == 0;
    g_log[0] = 0; m.inside = false; m.FirstTimeThrough = true; m.ImplementHeadSettings(); ok = ok && strcmp(g_log, "I") == 0;   // out of range: only the first-time init
    if (!ok) { printf("00753ba0_TEST FAIL\n"); return 1; }
    printf("00753ba0_TEST PASS\n"); return 0;
}