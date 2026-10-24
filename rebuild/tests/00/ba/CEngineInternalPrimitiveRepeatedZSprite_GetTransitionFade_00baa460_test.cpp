// behaviour test for 0x00BAA460 CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade() const
#include <cstdio>

struct FadeDistance { float Begin; float End; FadeDistance(float b, float e) : Begin(b), End(e) { } };
struct SharedData { float Factor; float MinMesh; float MaxMesh; };
static SharedData gShared;

template <class T> T Clamp(T value, T lo, T hi) { return value < lo ? lo : (value > hi ? hi : value); }

struct ZSprite {
    float FadeInStart, FadeInEnd;
    FadeDistance GetTransitionFade() const
    {
        FadeDistance fade(FadeInStart, FadeInEnd);
        float end = Clamp(fade.End, gShared.MinMesh, gShared.MaxMesh);
        fade.Begin -= fade.End - end;
        fade.End = end;
        return fade;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    gShared.Factor = 1.0f; gShared.MinMesh = 20.0f; gShared.MaxMesh = 50.0f;
    ZSprite s; s.FadeInStart = 30.0f; s.FadeInEnd = 40.0f;           // inside the window: unchanged
    FadeDistance f = s.GetTransitionFade();
    if (!Near(f.Begin, 30.0f) || !Near(f.End, 40.0f)) { std::printf("00baa460_TEST FAIL\n"); return 1; }
    s.FadeInStart = 60.0f; s.FadeInEnd = 70.0f;                      // above: shifted down by 20
    f = s.GetTransitionFade();
    if (!Near(f.Begin, 40.0f) || !Near(f.End, 50.0f)) { std::printf("00baa460_TEST FAIL\n"); return 1; }
    s.FadeInStart = 5.0f; s.FadeInEnd = 10.0f;                       // below: shifted up by 10
    f = s.GetTransitionFade();
    if (!Near(f.Begin, 15.0f) || !Near(f.End, 20.0f)) { std::printf("00baa460_TEST FAIL\n"); return 1; }
    std::printf("00baa460_TEST PASS\n");
    return 0;
}