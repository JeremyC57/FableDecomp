#include <cstdio>
// Behaviour model of CTCAssassinRush::CreateTrailingEffect: with a special-effects sub-def, the low startup effect
// (scaled by spell level / max level) and the displacement effect are created and attached; the radial blur is always
// started, centred BlurHeight above the rusher.
struct Result { bool trail; float scale; bool displacement; float blurZ; };
static Result Create(bool hasSubDef, bool hasLow, bool hasDisplacement, bool hasAbilities, long level, long maxLevel,
                     float rusherZ, float blurHeight)
{
    Result r = { false, 0.0f, false, 0.0f };
    if (hasSubDef) {
        if (hasLow) {
            float scale = 0.0f;
            if (hasAbilities) scale = (float)level / maxLevel;
            r.trail = true; r.scale = scale;
        }
        if (hasDisplacement) r.displacement = true;
    }
    r.blurZ = rusherZ + blurHeight;
    return r;
}
int main()
{
    Result a = Create(true, true, true, true, 2, 4, 1.0f, 0.5f);
    Result b = Create(false, true, true, true, 2, 4, 3.0f, 1.0f);
    Result c = Create(true, true, false, false, 2, 4, 0.0f, 2.0f);
    int ok = a.trail && a.scale == 0.5f && a.displacement && a.blurZ == 1.5f
          && !b.trail && !b.displacement && b.blurZ == 4.0f
          && c.trail && c.scale == 0.0f && !c.displacement && c.blurZ == 2.0f;
    std::printf(ok ? "007a9fe0_TEST PASS\n" : "007a9fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
