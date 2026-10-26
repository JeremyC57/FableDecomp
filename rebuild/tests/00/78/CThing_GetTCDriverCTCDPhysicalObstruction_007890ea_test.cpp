// Behaviour test for CThing::GetTCDriver<CTCDPhysicalObstruction> @ 0x007890EA: bitset bit 0xed gates the driver hand-back.
#include <cstdio>
class CTCDriverBase { public: int id; };
class CTCDPhysicalObstruction;
struct CTCBitset {
    unsigned long m_words[9];
    bool test(unsigned long position) const { return (m_words[position >> 5] >> (position & 31)) & 1; }
};
class CThing { public:
    CTCBitset      UsingComponents;
    CTCDriverBase* PTCDriver;
    template <class T> bool GetTCDriver(T** out)
    {
        if (UsingComponents.test(0xed)) { *out = (T*)PTCDriver; return true; }
        return false;
    }
};
int main()
{
    CTCDriverBase d; CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents.m_words[i] = 0; t.PTCDriver = &d;
    CTCDPhysicalObstruction* out = 0;
    if (t.GetTCDriver(&out) || out) { std::printf("007890ea_TEST FAIL\n"); return 1; }
    t.UsingComponents.m_words[0xed >> 5] = 1UL << (0xed & 31);
    if (!t.GetTCDriver(&out) || (void*)out != (void*)&d) { std::printf("007890ea_TEST FAIL\n"); return 1; }
    std::printf("007890ea_TEST PASS\n");
    return 0;
}