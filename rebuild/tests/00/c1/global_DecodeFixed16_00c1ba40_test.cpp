// Behaviour test for 00c1ba40: DecodeFixed16 -- a 16-bit fixed value back to 0..10000
#include <cstdio>

float DecodeFixed16(unsigned short value)
{
    float scaled = (float)value * 10000.0f;
    return scaled * (1.0f / 65535.0f);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

int main()
{
    if (!Near(DecodeFixed16(0), 0.0f)) { std::printf("00c1ba40_TEST FAIL\n"); return 1; }
    if (!Near(DecodeFixed16(65535), 10000.0f)) { std::printf("00c1ba40_TEST FAIL\n"); return 1; }
    if (!Near(DecodeFixed16(32768), 32768.0f * 10000.0f / 65535.0f)) { std::printf("00c1ba40_TEST FAIL\n"); return 1; }
    std::printf("00c1ba40_TEST PASS\n");
    return 0;
}