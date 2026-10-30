// Behaviour test for NUISystem::CIconText::GetAlignment (retail 0x00543B10): centred bit 0x10 wins, then right bit 0x20, else left.
#include <cstdio>
enum ETextAlignement { TEXT_ALIGN_LEFT = 0, TEXT_ALIGN_CENTRE = 1, TEXT_ALIGN_RIGHT = 2 };
struct CIconText {
    unsigned char FlagsSetTwo;
    bool IsTextCentred() const { return (FlagsSetTwo & 0x10) != 0; }
    bool IsTextRightAligned() const { return (FlagsSetTwo & 0x20) != 0; }
    ETextAlignement GetAlignment() const
    {
        if (IsTextCentred()) return TEXT_ALIGN_CENTRE;
        if (IsTextRightAligned()) return TEXT_ALIGN_RIGHT;
        return TEXT_ALIGN_LEFT;
    }
};
int main()
{
    CIconText t;
    t.FlagsSetTwo = 0x00; if (t.GetAlignment() != 0) { std::printf("00543b10_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x20; if (t.GetAlignment() != 2) { std::printf("00543b10_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x30; if (t.GetAlignment() != 1) { std::printf("00543b10_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x1f; if (t.GetAlignment() != 1) { std::printf("00543b10_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0xef; if (t.GetAlignment() != 2) { std::printf("00543b10_TEST FAIL\n"); return 1; }
    std::printf("00543b10_TEST PASS\n");
    return 0;
}