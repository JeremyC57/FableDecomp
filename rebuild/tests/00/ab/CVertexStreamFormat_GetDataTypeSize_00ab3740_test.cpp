// Behaviour test for CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType) @ 0x00AB3740
#include <cstdio>

static unsigned int GetDataTypeSize(int type)
{
    switch (type) {
    case 0:  return 4;
    case 1:
    case 6:
    case 11: return 8;
    case 2:  return 12;
    case 3:  return 16;
    case 4:  return 4;
    case 5:  return 4;
    case 7:  return 4;
    case 8:  return 2;
    case 9:  return 4;
    case 10: return 6;
    case 12: return 4;
    case 13: return 4;
    }
    return 0;
}

int main()
{
    // Retail jump table at 0x00AB3774, read entry by entry.
    static const unsigned int expected[14] = { 4, 8, 12, 16, 4, 4, 8, 4, 2, 4, 6, 8, 4, 4 };
    int ok = 1;
    for (int i = 0; i < 14; ++i)
        if (GetDataTypeSize(i) != expected[i]) ok = 0;
    if (GetDataTypeSize(14) != 0 || GetDataTypeSize(-1) != 0 || GetDataTypeSize(100) != 0) ok = 0;
    if (!ok) { std::printf("00ab3740_TEST FAIL\n"); return 1; }
    std::printf("00ab3740_TEST PASS\n");
    return 0;
}