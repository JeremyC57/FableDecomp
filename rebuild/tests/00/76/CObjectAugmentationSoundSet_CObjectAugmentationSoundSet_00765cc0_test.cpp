#include <cstdio>
#include <cstring>
// Behaviour test for CObjectAugmentationSoundSet constructor @ 0x00765CC0: not played, empty sound name, handle -1
class CCharString {
public:
    CCharString(const char* text, long length) : m_pText(text), m_length(length) {}
    const char* m_pText;
    long m_length;
};
class CObjectAugmentationSoundSet { public:
    bool Played;
    CCharString Sound;
    unsigned long Handle;
    CObjectAugmentationSoundSet() : Played(false), Sound("", -1), Handle(0xFFFFFFFF) {}
};
int main()
{
    CObjectAugmentationSoundSet s;
    if (s.Played || std::strcmp(s.Sound.m_pText, "") != 0 || s.Sound.m_length != -1 || s.Handle != 0xFFFFFFFF) { std::printf("00765cc0_TEST FAIL\n"); return 1; }
    std::printf("00765cc0_TEST PASS\n");
    return 0;
}