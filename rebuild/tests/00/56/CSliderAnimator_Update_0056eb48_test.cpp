// Behaviour test for CSliderAnimator::Update @ 0x0056EB48: the animation step / clamp / output logic over model components.
#include <cstdio>
#include <math.h>
#include <string.h>

class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct CComponent {
    int lookups; float zoomX; long shownValue; int texts;
    CComponent() : lookups(0), zoomX(-1), shownValue(-999), texts(0) {}
};
struct CSlider {
    CComponent fill, number; int lookups;
    CComponent* FindChild(const char* name) { ++lookups; return strcmp(name, "UI_LEVEL_SLIDER_NUMBER") == 0 ? &number : &fill; }
};

class CSliderAnimator { public:
    CSlider* pSlider; float StartValue, EndValue, MaxValue, a, Overall; bool Play, Finished;
    void Update()
    {
        if ((EndValue - StartValue) / Overall == -1.0f) { Play = false; Finished = false; return; }
        float speed = fabs((EndValue - StartValue) / Overall);
        CComponent* fill = pSlider->FindChild("UI_LEVEL_SLIDER_FILL_SPRITE");
        CComponent* number = pSlider->FindChild("UI_LEVEL_SLIDER_NUMBER");
        if (!Play)
            a = Finished ? 1.0f : 0.0f;
        else {
            a += 1.0f / (speed * 15.0f);
            if (a > 1.0f) { Play = false; Finished = true; a = 1.0f; }
        }
        long value = (long)((EndValue - StartValue) * a + StartValue);
        number->shownValue = value; ++number->texts;
        fill->zoomX = C2DVector(value / MaxValue * 32.0f, 1.0f).X;
    }
};

int main()
{
    CSlider s; s.lookups = 0;
    CSliderAnimator an = { &s, 10.0f, 40.0f, 100.0f, 0.0f, 1.0f, true, false };
    bool ok = true;
    an.Update();                                   // speed 30 -> a = 1/450
    ok = ok && an.Play && fabs(an.a - 1.0f / 450.0f) < 1e-6f && s.number.shownValue == 10 && s.lookups == 2;
    an.a = 0.99999f; an.Update();                  // overshoots -> clamped, finished
    ok = ok && !an.Play && an.Finished && an.a == 1.0f && s.number.shownValue == 40 && s.fill.zoomX == 40.0f / 100.0f * 32.0f;
    an.Update();                                   // not playing, finished -> a stays 1
    ok = ok && an.a == 1.0f;
    an.Finished = false; an.Update();              // not playing, not finished -> 0
    ok = ok && an.a == 0.0f && s.number.shownValue == 10;
    CSliderAnimator stop = { &s, 5.0f, 4.0f, 1.0f, 0.5f, 1.0f, true, true };
    int before = s.lookups; stop.Update();         // step -1 -> stopped, nothing looked up
    ok = ok && !stop.Play && !stop.Finished && s.lookups == before;
    if (!ok) { std::printf("0056eb48_TEST FAIL\n"); return 1; }
    std::printf("0056eb48_TEST PASS\n");
    return 0;
}