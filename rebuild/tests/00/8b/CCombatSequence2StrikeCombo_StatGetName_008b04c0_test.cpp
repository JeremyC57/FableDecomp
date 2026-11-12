// Behaviour test for 0x008B04C0 CCombatSequence2StrikeCombo::StatGetName(): returns the registry name "COMBAT_SEQUENCE_TWO_STRIKE" built with length -1.
#include <cstdio>
#include <cstring>

class CharString {
public:
    char text[64];
    CharString(const char* s, long length)
    {
        unsigned int n = length < 0 ? (unsigned int)std::strlen(s) : (unsigned int)length;
        std::memcpy(text, s, n); text[n] = 0;
    }
};
class Sequence {
public:
    static CharString StatGetName() { return CharString("COMBAT_SEQUENCE_TWO_STRIKE", -1); }
};

int main()
{
    CharString s = Sequence::StatGetName();
    if (std::strcmp(s.text, "COMBAT_SEQUENCE_TWO_STRIKE") != 0) { std::printf("008b04c0_TEST FAIL\n"); return 1; }
    std::printf("008b04c0_TEST PASS\n");
    return 0;
}
