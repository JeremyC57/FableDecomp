#include <cstdio>
long EncodeFixed16(float value)
{
    float scaled = value * 65535.0f;
    return (long)(scaled * 0.0001f);
}
int main()
{
    if (EncodeFixed16(0.0f) != 0 || EncodeFixed16(10000.0f) != 65534 && EncodeFixed16(10000.0f) != 65535) { std::printf("00c1ba20_TEST FAIL\n"); return 1; }
    if (EncodeFixed16(1.0f) != 6 || EncodeFixed16(5000.0f) < 32766 || EncodeFixed16(5000.0f) > 32768) { std::printf("00c1ba20_TEST FAIL\n"); return 1; }
    std::printf("00c1ba20_TEST PASS\n");
    return 0;
}