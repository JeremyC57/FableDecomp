// 00624d90 behaviour test for CTCInventoryQuests::OnItemSelected(long, bool&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class CTCInventoryQuests {
public:
    virtual ~CTCInventoryQuests() { }
    virtual void OnItemSelected(long item, bool& handled);
};
void CTCInventoryQuests::OnItemSelected(long item, bool& handled)
{
    (void)item;
    handled = true;
}
static void run()
{
    CTCInventoryQuests q;
    bool handled = false;
    q.OnItemSelected(3, handled);
    CHECK(handled);
    q.OnItemSelected(-1, handled);
    CHECK(handled);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00624d90_TEST FAIL\n");
        return 1;
    }
    std::printf("00624d90_TEST PASS\n");
    return 0;
}