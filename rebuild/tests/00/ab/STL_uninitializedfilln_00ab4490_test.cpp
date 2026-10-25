// Behaviour test for std::__uninitialized_fill_n<CVertexShaderInput::CStreamInfo*> @ 0x00AB4490
// Copy-constructs n copies of x into raw storage and returns the end pointer (first when n == 0).
#include <cstdio>
#include <new>

static int gCopies = 0;

struct CStreamInfoModel {
    unsigned int StreamIndex;
    unsigned int ElementCount;
    CStreamInfoModel(unsigned int index, unsigned int count) : StreamIndex(index), ElementCount(count) { }
    CStreamInfoModel(const CStreamInfoModel& other) : StreamIndex(other.StreamIndex), ElementCount(other.ElementCount) { ++gCopies; }
};

static CStreamInfoModel* uninitialized_fill_n(CStreamInfoModel* first, unsigned int n, const CStreamInfoModel& x)
{
    CStreamInfoModel* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) CStreamInfoModel(x);
    return cur;
}

union Storage { double align; unsigned char bytes[8 * sizeof(CStreamInfoModel)]; };

int main()
{
    Storage storage;
    CStreamInfoModel* first = (CStreamInfoModel*)storage.bytes;
    CStreamInfoModel value(3, 7);
    CStreamInfoModel* end = uninitialized_fill_n(first, 5, value);
    int ok = end == first + 5 && gCopies == 5;
    for (int i = 0; i < 5 && ok; ++i)
        if (first[i].StreamIndex != 3 || first[i].ElementCount != 7) ok = 0;
    if (uninitialized_fill_n(first, 0, value) != first || gCopies != 5) ok = 0;
    if (!ok) { std::printf("00ab4490_TEST FAIL\n"); return 1; }
    std::printf("00ab4490_TEST PASS\n");
    return 0;
}