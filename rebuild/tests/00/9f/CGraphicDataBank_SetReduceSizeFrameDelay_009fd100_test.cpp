// Behaviour test for CGraphicDataBank::SetReduceSizeFrameDelay at 0x009FD100: the delay lands in both retail fields.
#include <cstdio>
#include <cstring>
class CGraphicDataBank {
public:
    unsigned char _pad000[0x19c];
    unsigned int m_19c;
    unsigned char _pad1a0[0x26c - 0x1a0];
    unsigned int m_26c;
    void SetReduceSizeFrameDelay(unsigned int delay);
};
void CGraphicDataBank::SetReduceSizeFrameDelay(unsigned int delay)
{
    m_26c = delay;
    m_19c = delay;
}
int main() {
    static CGraphicDataBank bank;
    std::memset(&bank, 0, sizeof(bank));
    bank.SetReduceSizeFrameDelay(3);
    if (bank.m_26c != 3 || bank.m_19c != 3) { std::printf("BAD\n"); return 1; }
    std::printf("OPPR_009fd100_OK\n"); return 0;
}
