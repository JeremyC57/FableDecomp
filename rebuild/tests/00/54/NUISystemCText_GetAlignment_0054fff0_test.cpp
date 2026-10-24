// Behaviour test for 0x0054FFF0 NUISystem::CText::GetAlignment: centred flag (0x10) wins, then right-aligned (0x20), else left
#include <cstdio>
enum ETextAlignement { TEXT_ALIGN_LEFT = 0, TEXT_ALIGN_CENTRE = 1, TEXT_ALIGN_RIGHT = 2 };
class CText {
public:
    virtual ETextAlignement GetAlignment() const;
    virtual ~CText() { }
    unsigned char FlagsSetTwo;
    bool IsTextCentred() const { return (FlagsSetTwo & 0x10) != 0; }
    bool IsTextRightAligned() const { return (FlagsSetTwo & 0x20) != 0; }
};
ETextAlignement CText::GetAlignment() const
{
    if (IsTextCentred())
        return TEXT_ALIGN_CENTRE;
    if (IsTextRightAligned())
        return TEXT_ALIGN_RIGHT;
    return TEXT_ALIGN_LEFT;
}
int main()
{
    CText t;
    t.FlagsSetTwo = 0x00; if (t.GetAlignment() != TEXT_ALIGN_LEFT) { std::printf("0054fff0_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x10; if (t.GetAlignment() != TEXT_ALIGN_CENTRE) { std::printf("0054fff0_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x20; if (t.GetAlignment() != TEXT_ALIGN_RIGHT) { std::printf("0054fff0_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0x30; if (t.GetAlignment() != TEXT_ALIGN_CENTRE) { std::printf("0054fff0_TEST FAIL\n"); return 1; }
    t.FlagsSetTwo = 0xcf; if (t.GetAlignment() != TEXT_ALIGN_LEFT) { std::printf("0054fff0_TEST FAIL\n"); return 1; }
    std::printf("0054fff0_TEST PASS\n");
    return 0;
}