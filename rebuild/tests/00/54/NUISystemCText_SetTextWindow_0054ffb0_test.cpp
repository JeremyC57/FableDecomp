// Behaviour test for 0x0054FFB0 NUISystem::CText::SetTextWindow: copies the 2D box into the text's Window
#include <cstdio>
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
class CText {
public:
    virtual void SetTextWindow(C2DBoxF& window);
    virtual ~CText() { }
    C2DBoxF Window;
};
void CText::SetTextWindow(C2DBoxF& window)
{
    Window = window;
}
int main()
{
    CText t;
    C2DBoxF b = { 1.0f, 2.0f, 3.0f, 4.0f };
    t.SetTextWindow(b);
    b.TLX = 9.0f;
    if (t.Window.TLX != 1.0f || t.Window.TLY != 2.0f || t.Window.BRX != 3.0f || t.Window.BRY != 4.0f)
        { std::printf("0054ffb0_TEST FAIL\n"); return 1; }
    std::printf("0054ffb0_TEST PASS\n");
    return 0;
}