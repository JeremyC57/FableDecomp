// Behaviour test for 0x0066F060 CInputTypeMouseButtonEvent::GetEventTypeFromGeneralButtonType.
#include <cstdio>
static long Map(long type, long button)
{
    switch (type) {
    case 1: switch (button) { case 1: return 4; case 3: return 7; case 2: return 10; case 8: return 22; case 9: return 23; case 10: return 24; case 11: return 25; case 12: return 26; } break;
    case 2: switch (button) { case 1: return 5; case 3: return 8; case 2: return 11; case 8: return 27; case 9: return 28; case 10: return 29; case 11: return 30; case 12: return 31; } break;
    case 3: switch (button) { case 1: return 6; case 3: return 9; case 2: return 12; case 8: return 32; case 9: return 33; case 10: return 34; case 11: return 35; case 12: return 36; } break;
    }
    return 0;
}
int main()
{
    if (Map(1, 1) != 4 || Map(2, 2) != 11 || Map(3, 3) != 9 || Map(3, 12) != 36 || Map(1, 8) != 22 ||
        Map(2, 5) != 0 || Map(4, 1) != 0 || Map(1, 13) != 0 || Map(0, 2) != 0) { std::printf("0066f060_TEST FAIL\n"); return 1; }
    std::printf("0066f060_TEST PASS\n");
    return 0;
}
