// Test for 0x00C3AA40 CSegmentFile::GetPosition: cursor plus range start.
#include <cstdio>
struct CSegmentFile { unsigned int RangeFirst, RangeCount, RangeCursor;
    unsigned int GetPosition() const { return RangeCursor + RangeFirst; } };
int main()
{
    CSegmentFile f; f.RangeFirst = 1000; f.RangeCount = 50; f.RangeCursor = 12;
    if (f.GetPosition() != 1012) { std::printf("00c3aa40_TEST FAIL\n"); return 1; }
    std::printf("00c3aa40_TEST PASS\n");
    return 0;
}