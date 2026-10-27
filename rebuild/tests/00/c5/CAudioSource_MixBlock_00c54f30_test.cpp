// Behaviour test for 0x00C54F30 CAudioSource::MixBlock
#include <cstdio>
#include <cstring>
struct Block { unsigned char m_bytes[0x100]; };
struct Ring { unsigned char* m_pData; unsigned int m_frameBytes; unsigned int m_readPos; unsigned int m_read;
    void CommitRead(unsigned int n) { m_read += n; m_readPos += n; } };
static int g_effect = 0, g_volume = 0, g_filter = 0, g_write = 0; static unsigned char g_written0 = 0;
struct Scaler { bool m_bEnabled; };
static Scaler* g_pScaler = 0; static int g_eaxVersion = 0; static bool g_haveEAX = false;
struct Src {
    void MixBlock(Ring* ring, bool effects)
    {
        Block block, processed;
        block = *(Block*)(ring->m_readPos * ring->m_frameBytes + ring->m_pData);
        if (effects) {
            Scaler* scaler = g_pScaler;
            if (scaler) { ++g_effect; processed = block; if (scaler->m_bEnabled) ++g_volume; }
            else if (g_haveEAX && g_eaxVersion == 1) { ++g_effect; processed = block; ++g_volume; }
        }
        ++g_filter; block.m_bytes[0] ^= 0xff;
        ++g_write; g_written0 = block.m_bytes[0];
        ring->CommitRead(0x80);
    }
};
static int Fail() { std::printf("00c54f30_TEST FAIL\n"); return 1; }
int main()
{
    static unsigned char data[0x400]; for (int i = 0; i < 0x400; ++i) data[i] = (unsigned char)i;
    Ring r; r.m_pData = data; r.m_frameBytes = 2; r.m_readPos = 0x80; r.m_read = 0;
    Src s; s.MixBlock(&r, false);
    if (g_written0 != (unsigned char)(0x00 ^ 0xff) || r.m_readPos != 0x100 || g_effect != 0 || g_filter != 1) return Fail();
    Scaler sc; sc.m_bEnabled = false; g_pScaler = &sc;
    s.MixBlock(&r, true);
    if (g_effect != 1 || g_volume != 0) return Fail();
    g_pScaler = 0; g_haveEAX = true; g_eaxVersion = 1;
    s.MixBlock(&r, true);
    if (g_effect != 2 || g_volume != 1 || g_write != 3 || r.m_read != 0x180) return Fail();
    std::printf("00c54f30_TEST PASS\n");
    return 0;
}