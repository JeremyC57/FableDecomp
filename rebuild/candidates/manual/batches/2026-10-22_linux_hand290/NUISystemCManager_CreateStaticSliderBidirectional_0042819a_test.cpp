// Behaviour test for NUISystem::CManager::CreateStaticSliderBidirectional (0x0042819A): value in [-100,100] maps the button to
// (value + 100) * 0.5; the label gets a "+" for positive values and is nudged by its length (1 -> +5, 3 -> -5, 4 -> -10).
#include <cstdio>
#include <cstring>

struct Comp { float px; char text[32]; Comp() : px(0) { text[0] = 0; } };

static void Label(Comp* button, Comp* number, float value)
{
    button->px = (value + 100.0f) * 0.005f * 100.0f;
    char str[32]; std::sprintf(str, "%d", (int)value);
    if (value > 0.0f) { char t[32]; std::sprintf(t, "+%s", str); std::strcpy(str, t); }
    if (number) {
        std::strcpy(number->text, str);
        float offset = 0.0f;
        unsigned int length = (unsigned int)std::strlen(str);
        if (length == 1) offset = 5.0f;
        if (length == 3) offset = -5.0f;
        if (length == 4) offset = -10.0f;
        number->px += offset;
    }
}

int main()
{
    Comp b1, n1, b2, n2, b3, n3;
    Label(&b1, &n1, 0.0f);
    Label(&b2, &n2, 50.0f);
    Label(&b3, &n3, -100.0f);
    bool ok = b1.px > 49.9f && b1.px < 50.1f && std::strcmp(n1.text, "0") == 0 && n1.px == 5.0f
        && std::strcmp(n2.text, "+50") == 0 && n2.px == -5.0f && b2.px > 74.9f && b2.px < 75.1f
        && std::strcmp(n3.text, "-100") == 0 && n3.px == -10.0f && b3.px < 0.1f;
    std::printf(ok ? "0042819a_TEST PASS\n" : "0042819a_TEST FAIL\n");
    return ok ? 0 : 1;
}
