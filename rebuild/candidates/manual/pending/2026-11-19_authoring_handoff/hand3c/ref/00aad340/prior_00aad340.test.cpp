// Behaviour test for CWaveFile::ConvertSamplingRate: the step and its 16.16 fraction come from the floored rate ratio (parked row 0x00AAD340).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static unsigned long Step(unsigned long from, unsigned long to) { return GFFloatToLongFloor((float)from / (float)to); }
int main()
{
    bool ok = Step(44100, 22050) == 2 && Step(22050, 44100) == 0 && Step(48000, 44100) == 1;
    printf(ok ? "00aad340_TEST PASS\n" : "00aad340_TEST FAIL\n");
    return ok ? 0 : 1;
}
