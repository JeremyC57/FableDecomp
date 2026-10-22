// Behaviour test for NUISystem::CManager::CreateStaticSlider (0x00427F31): place the slider at the position in states 0/1/3,
// caption the text child, tint the button sprite grey (0.2 * 255 = 51, alpha 255), move the button to value% and label it.
#include <cstdio>
#include <cstring>

struct State { float px, py, sx; unsigned char r, g, b, a; State() : px(0), py(0), sx(0), r(0), g(0), b(0), a(0) {} };
struct Comp {
    const char* name; State states[4]; Comp* kids[4]; int nkids; char text[32];
    Comp(const char* n) : name(n), nkids(0) { text[0] = 0; }
    Comp* FindChild(const char* n) { for (int i = 0; i < nkids; ++i) if (std::strcmp(kids[i]->name, n) == 0) return kids[i]; return 0; }
};
static unsigned char ToByte(float f) { return (unsigned char)(int)(f * 255.0f); }

static Comp* CreateStaticSlider(Comp* slider, const char* text, float value, int x, int y)
{
    slider->states[0].px = (float)x; slider->states[0].py = (float)y;
    slider->states[1].px = (float)x; slider->states[1].py = (float)y;
    slider->states[3].px = (float)x; slider->states[3].py = (float)y;
    float scale = value * 0.01f;
    float colour[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    Comp* caption = slider->FindChild("UI_STATIC_SLIDER_TEXT");
    if (caption) std::strcpy(caption->text, text);
    Comp* button = slider->FindChild("UI_STATIC_SLIDER_BUTTON");
    Comp* sprite = slider->FindChild("UI_STATIC_SLIDER_BUTTON_SPRITE");
    if (button) {
        sprite->states[0].r = ToByte(colour[0]); sprite->states[0].g = ToByte(colour[1]);
        sprite->states[0].b = ToByte(colour[2]); sprite->states[0].a = ToByte(colour[3]);
        float w = scale * 100.0f;
        button->states[1].px = w; button->states[3].px = w;
        Comp* number = button->FindChild("UI_STATIC_SLIDER_BUTTON_TEXT");
        if (number) std::sprintf(number->text, "%d", (int)value);
    }
    return slider;
}

int main()
{
    Comp slider("UI_STATIC_SLIDER"), cap("UI_STATIC_SLIDER_TEXT"), btn("UI_STATIC_SLIDER_BUTTON"), spr("UI_STATIC_SLIDER_BUTTON_SPRITE"), num("UI_STATIC_SLIDER_BUTTON_TEXT");
    slider.kids[0] = &cap; slider.kids[1] = &btn; slider.kids[2] = &spr; slider.nkids = 3;
    btn.kids[0] = &num; btn.nkids = 1;
    CreateStaticSlider(&slider, "Volume", 40.0f, 12, 34);
    bool ok = slider.states[3].px == 12.0f && slider.states[1].py == 34.0f && std::strcmp(cap.text, "Volume") == 0
        && spr.states[0].r == 51 && spr.states[0].a == 255 && btn.states[1].px > 39.9f && btn.states[3].px < 40.1f
        && std::strcmp(num.text, "40") == 0;
    std::printf(ok ? "00427f31_TEST PASS\n" : "00427f31_TEST FAIL\n");
    return ok ? 0 : 1;
}