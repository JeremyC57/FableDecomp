// behaviour test for 0x00681610 COpinionTransientOffset::COpinionTransientOffset
#include <cstdio>
struct COpinionTransientOffset {
    long Opinion; float OffsetPerFrameRunIn; long FramesToRunIn; long FramesOfCappedPeak; float OffsetPerFrameRunOut; long FramesToRunOut;
    COpinionTransientOffset(long opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset)
    {
        Opinion = opinion;
        OffsetPerFrameRunIn = peakOffset;
        if (framesToRunIn == 0) framesToRunIn = 1;
        else OffsetPerFrameRunIn = peakOffset / framesToRunIn;
        FramesToRunIn = framesToRunIn;
        FramesOfCappedPeak = 0;
        float runOut = finalOffset - peakOffset;
        OffsetPerFrameRunOut = runOut;
        if (framesToRunOut == 0) framesToRunOut = 1;
        else OffsetPerFrameRunOut = runOut / framesToRunOut;
        FramesToRunOut = framesToRunOut;
    }
};
int main()
{
    COpinionTransientOffset a(3, 10.0f, 4, 5, 0.0f);
    COpinionTransientOffset b(2, 6.0f, 0, 0, 1.0f);
    if (a.Opinion != 3 || a.OffsetPerFrameRunIn != 2.5f || a.FramesToRunIn != 4 || a.FramesOfCappedPeak != 0
        || a.OffsetPerFrameRunOut != -2.0f || a.FramesToRunOut != 5
        || b.OffsetPerFrameRunIn != 6.0f || b.FramesToRunIn != 1 || b.OffsetPerFrameRunOut != -5.0f || b.FramesToRunOut != 1)
    { std::printf("00681610_TEST FAIL\n"); return 1; }
    std::printf("00681610_TEST PASS\n");
    return 0;
}