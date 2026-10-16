#include <cstdio>
// Behaviour model of CVirtualKeyboard::Confirm: a non-empty name on a keyboard that is not done marks it done (not
// cancelled) and finalises an active IME composition with commit.
struct IME { bool active; int finalised; bool committed; bool IsIMEActive() { return active; } void FinaliseString(bool c) { ++finalised; committed = c; } };
static IME g_ime;
struct Keyboard {
    long nameLength; bool Canceled; bool Done;
    void Confirm()
    {
        bool accept = !Done && nameLength > 0;
        if (accept) { Done = true; Canceled = false; if (g_ime.IsIMEActive()) g_ime.FinaliseString(true); }
    }
};
int main()
{
    g_ime.active = true; g_ime.finalised = 0; g_ime.committed = false;
    Keyboard k; k.nameLength = 0; k.Canceled = true; k.Done = false;
    k.Confirm();
    if (k.Done || g_ime.finalised) { std::printf("00851920_TEST FAIL\n"); return 1; }
    k.nameLength = 4; k.Confirm();
    if (!k.Done || k.Canceled || g_ime.finalised != 1 || !g_ime.committed) { std::printf("00851920_TEST FAIL\n"); return 1; }
    k.Confirm();
    if (g_ime.finalised != 1) { std::printf("00851920_TEST FAIL\n"); return 1; }
    std::printf("00851920_TEST PASS\n");
    return 0;
}
