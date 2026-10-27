// Test for 0x00A44130 CCountedPointer<CLipSyncSampleData>::DeleteData: deletes the sample data -- every frame's phoneme
// array is destroyed and both vectors' storage freed; a null pointer does nothing.
#include <stdio.h>
#include <stdlib.h>
static int g_frameDtors = 0, g_freed = 0;
class CLipSyncPhoneme { public: const char* Symbol; unsigned int ID; float Weight; };
class CLipSyncFrameData { public:
    CLipSyncPhoneme* Start; CLipSyncPhoneme* Finish; CLipSyncPhoneme* End;
    ~CLipSyncFrameData() { ++g_frameDtors; if (Start) { free(Start); ++g_freed; } }
};
class CLipSyncSampleData { public:
    CLipSyncFrameData* Start; CLipSyncFrameData* Finish; CLipSyncFrameData* End; float FramesPerSecond;
    ~CLipSyncSampleData() { for (CLipSyncFrameData* p = Start; p != Finish; ++p) p->~CLipSyncFrameData(); if (Start) { free(Start); ++g_freed; } }
};
template <class T> class CCountedPointer { public: static void DeleteData(void* p) { delete (T*)p; } };
int main()
{
    CLipSyncSampleData* d = new CLipSyncSampleData;
    d->Start = (CLipSyncFrameData*)malloc(2 * sizeof(CLipSyncFrameData)); d->Finish = d->Start + 2; d->End = d->Finish;
    d->Start[0].Start = (CLipSyncPhoneme*)malloc(sizeof(CLipSyncPhoneme)); d->Start[1].Start = 0;
    CCountedPointer<CLipSyncSampleData>::DeleteData(d);
    bool ok = g_frameDtors == 2 && g_freed == 2;
    CCountedPointer<CLipSyncSampleData>::DeleteData(0);
    if (g_frameDtors != 2) ok = false;
    puts(ok ? "00a44130_TEST PASS" : "00a44130_TEST FAIL");
    return ok ? 0 : 1;
}