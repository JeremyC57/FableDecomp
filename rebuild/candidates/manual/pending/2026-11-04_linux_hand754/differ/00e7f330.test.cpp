// Behaviour test for CV_FishingCompetitionScript::Init (0x00E7F330): a model of the row.
#include <stdio.h>

struct Script { bool running, attacked; long attacks; float biggest, competitor; long sizeComment[3]; int nComments; };
static void Init(Script& s, long roll50, float base)
{
    s.attacked = false; s.running = true; s.attacks = 0;
    s.competitor = (float)roll50 * 0.1f + base * 0.25f;
    s.biggest = 0.0f; s.nComments = 3; for (int i = 0; i < 3; ++i) s.sizeComment[i] = -1;
}
static int run()
{
    Script s; s.attacked = true; s.attacks = 4; s.biggest = 3; Init(s, 10, 4.0f);
    return s.running && !s.attacked && s.attacks == 0 && s.biggest == 0 && s.competitor > 1.99f && s.competitor < 2.01f
        && s.nComments == 3 && s.sizeComment[2] == -1;
}
int main()
{
    int ok = run();
    printf(ok ? "00e7f330_TEST PASS\n" : "00e7f330_TEST FAIL\n");
    return ok ? 0 : 1;
}
