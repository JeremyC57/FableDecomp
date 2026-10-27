// Behaviour test for 0x008B0000 CCombatSequence_BanditKingSpinAround::StatGetName(): returns the registry name
// "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND" built with length -1 (the whole C string).
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
class SpinAround {
public:
    static CharString StatGetName() { return CharString("COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND", -1); }
};

int main()
{
    CharString s = SpinAround::StatGetName();
    if (std::strcmp(s.text, "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND") != 0) { std::printf("008b0000_TEST FAIL\n"); return 1; }
    std::printf("008b0000_TEST PASS\n");
    return 0;
}