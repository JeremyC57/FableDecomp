// Behaviour test for 0x00540120 NUISystem::CEditBox::ProcessEvent: ignored while inactive; event 0x22 types the manager's last key (AddCharacter), event 0x21 passes it to KeyPressed, any other event does nothing
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static unsigned short g_key = 0; static int g_mgr = 0;
static unsigned short GetKeyPressed() { ++g_mgr; return g_key; }
struct CEditBox {
    bool Active; int added; int pressed; unsigned short lastAdd, lastPress;
    void AddCharacter(unsigned short c) { ++added; lastAdd = c; }
    void KeyPressed(unsigned short c) { ++pressed; lastPress = c; }
    void ProcessEvent(int e) { if (Active) { if (e == 0x22) AddCharacter(GetKeyPressed()); else if (e == 0x21) KeyPressed(GetKeyPressed()); } }
};
int main()
{
    CEditBox b; b.Active = true; b.added = b.pressed = 0;
    g_key = 'q'; b.ProcessEvent(0x22); CHECK(b.added == 1 && b.lastAdd == 'q' && b.pressed == 0 && g_mgr == 1);
    g_key = 8;   b.ProcessEvent(0x21); CHECK(b.pressed == 1 && b.lastPress == 8 && g_mgr == 2);
    b.ProcessEvent(0x19); CHECK(b.added == 1 && b.pressed == 1 && g_mgr == 2);
    b.Active = false; b.ProcessEvent(0x22); b.ProcessEvent(0x21); CHECK(b.added == 1 && b.pressed == 1 && g_mgr == 2);
    if (fails == 0) printf("00540120_TEST PASS\n"); else printf("00540120_TEST FAIL\n");
    return fails;
}