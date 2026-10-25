// Behaviour test for 0x00A441E0 (source body copied from src/00a441e0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
#include <math.h>
#pragma intrinsic(fmod)

float __fastcall GFGetSignedAngleDifference(float from, float to)
{
    float diff;
    if ((diff = to - from) < 0.0f || diff >= 1.0f) {
        diff = (float)fmod((double)(to - from), 1.0);
        if (diff < 0.0f)
            diff += 1.0f;
    }
    return diff >= 0.5f ? -(1.0f - diff) : diff;
}

// ---- test harness ----
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    if (!Near(GFGetSignedAngleDifference(0.1f, 0.3f), 0.2f)) ok = 0;     // in range
    if (!Near(GFGetSignedAngleDifference(0.9f, 0.1f), 0.2f)) ok = 0;     // negative difference wraps up
    if (!Near(GFGetSignedAngleDifference(0.1f, 0.8f), -0.3f)) ok = 0;    // >= 0.5 folds to the negative side
    if (!Near(GFGetSignedAngleDifference(0.0f, 2.25f), 0.25f)) ok = 0;   // several turns
    if (!Near(GFGetSignedAngleDifference(0.25f, 0.25f), 0.0f)) ok = 0;   // equal
    printf("00a441e0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}