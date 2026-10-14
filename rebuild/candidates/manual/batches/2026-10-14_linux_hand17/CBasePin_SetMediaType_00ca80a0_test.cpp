#include <cstdio>
static long g_setResult = 0; static const void* g_setFrom = 0;
class CMediaType { public: unsigned char m_mediaType[0x48]; long Set(const CMediaType& other) { g_setFrom = &other; return g_setResult; } };
class CBasePin {
public:
    unsigned char _pad_0x00[0x34]; CMediaType m_mt;
    long SetMediaType(const CMediaType* pmt) { long hr = m_mt.Set(*pmt); if (hr < 0) return hr; return 0; }
};
int main()
{
    CBasePin pin; CMediaType mt;
    g_setResult = 0; if (pin.SetMediaType(&mt) != 0 || g_setFrom != &mt) { std::printf("00ca80a0_TEST FAIL\n"); return 1; }
    g_setResult = 1; if (pin.SetMediaType(&mt) != 0) { std::printf("00ca80a0_TEST FAIL\n"); return 1; }
    g_setResult = (long)0x8007000e; if (pin.SetMediaType(&mt) != (long)0x8007000e) { std::printf("00ca80a0_TEST FAIL\n"); return 1; }
    g_setResult = -1; if (pin.SetMediaType(&mt) != -1) { std::printf("00ca80a0_TEST FAIL\n"); return 1; }
    std::printf("00ca80a0_TEST PASS\n"); return 0;
}
