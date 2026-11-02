// Behaviour test for 0x0121E3A0: CTCTrophy::InstructionsText[2] built in place from the two tutorial text keys (length -1 = to the terminator).
#include <cstdio>
#include <cstring>
class CCharString {
public:
    CCharString(const char* text, long length = -1) : Text(text), Length(length < 0 ? (long)std::strlen(text) : length) { }
    const char* Text;
    long Length;
};
class CTCTrophy { public: static CCharString InstructionsText[2]; };
CCharString CTCTrophy::InstructionsText[2] = {
    "TEXT_QST_TUTORIAL_TROPHYRULES_PC_10",
    "TEXT_QST_TUTORIAL_TROPHYRULES_PC_20"
};
int main()
{
    if (sizeof(CTCTrophy::InstructionsText) / sizeof(CCharString) != 2
        || std::strcmp(CTCTrophy::InstructionsText[0].Text, "TEXT_QST_TUTORIAL_TROPHYRULES_PC_10") != 0
        || std::strcmp(CTCTrophy::InstructionsText[1].Text, "TEXT_QST_TUTORIAL_TROPHYRULES_PC_20") != 0
        || CTCTrophy::InstructionsText[1].Length != 35) { std::printf("0121e3a0_TEST FAIL\n"); return 1; }
    std::printf("0121e3a0_TEST PASS\n");
    return 0;
}