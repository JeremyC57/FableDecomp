// Behaviour test for 0x0053D000 NUISystem::CMenuEntry::MakeUnavailableDynamically: only when available -- the base step, then the text child (type 6, else the child named MENU_ENTRY_TEXT, else a direct type-6 child) gets the same call, and IsAvailable is cleared
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_baseCalls = 0, g_childCalls = 0, g_mode = 0, g_strings = 0;
struct CComponent {
    CComponent* typeChild; CComponent* namedChild; CComponent* directChild; int forwarded;
    CComponent() : typeChild(0), namedChild(0), directChild(0), forwarded(0) { }
    CComponent* FindFirstChildOfType(int t) { return t == 6 ? typeChild : 0; }
    CComponent* FindChildForward(const char* name) { ++g_strings; return (name[0] == 'M' && name[11] == 'T') ? namedChild : 0; }
    CComponent* FindFirstChildOfTypeNonRecursive(int t) { return t == 6 ? directChild : 0; }
    void BaseStep() { ++g_baseCalls; }
    void ChildStep() { ++forwarded; ++g_childCalls; }
};
struct CMenuEntry : CComponent {
    bool IsAvailable;
    void Run()
    {
        if (IsAvailable) {
            BaseStep();
            CComponent* text = FindFirstChildOfType(6);
            if (text) text->ChildStep();
            else if ((text = FindChildForward("MENU_ENTRY_TEXT")) != 0) text->ChildStep();
            else if ((text = FindFirstChildOfTypeNonRecursive(6)) != 0) text->ChildStep();
            IsAvailable = false;
        }
    }
};
int main()
{
    CMenuEntry e; CComponent t1, t2, t3;
    e.IsAvailable = true; e.typeChild = &t1; e.namedChild = &t2; e.directChild = &t3;
    e.Run();
    CHECK(g_baseCalls == 1 && t1.forwarded == 1 && t2.forwarded == 0 && g_strings == 0 && e.IsAvailable == false);
    e.Run();                                                    // already in the target state: nothing happens
    CHECK(g_baseCalls == 1 && g_childCalls == 1);
    e.IsAvailable = true; e.typeChild = 0; e.Run();
    CHECK(t2.forwarded == 1 && g_strings == 1 && t3.forwarded == 0);
    e.IsAvailable = true; e.namedChild = 0; e.Run();
    CHECK(t3.forwarded == 1 && g_strings == 2);
    e.IsAvailable = true; e.directChild = 0; e.Run();
    CHECK(g_baseCalls == 4 && g_childCalls == 3 && e.IsAvailable == false);
    if (fails == 0) printf("0053d000_TEST PASS\n"); else printf("0053d000_TEST FAIL\n");
    return fails;
}