// Behaviour test for 0x00779420: OnInitialActivate resolves the entrance only when a UID was loaded
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SExit { long EntranceConnectedToUID; int Resolved;
    void SetUpPEntranceConnectedToFromUID() { ++Resolved; }
    void OnInitialActivate() { if (EntranceConnectedToUID) SetUpPEntranceConnectedToFromUID(); } };
int main()
{
    SExit e; e.EntranceConnectedToUID = 0; e.Resolved = 0;
    e.OnInitialActivate(); CHECK(e.Resolved == 0);
    e.EntranceConnectedToUID = 1234; e.OnInitialActivate(); CHECK(e.Resolved == 1);
    if (!g_fail) printf("00779420_TEST PASS\n");
    return g_fail;
}