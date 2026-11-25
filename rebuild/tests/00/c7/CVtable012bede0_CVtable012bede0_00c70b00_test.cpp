// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of the HRTF
// renderer constructor 0x00C70B00 (CVtable_012bede0, SSE2 twin of 0x00C70120 with 160-float blocks): as 0x00C70120.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_baseDtors = 0, g_dtors = 0, g_frees = 0, g_order = 0, g_baseAt = 0, g_derivedAt = 0, g_clear = 0;
static void* g_lastFree = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { return std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_lastFree = p; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; g_baseAt = ++g_order; }
void CSalRender::ClearFilterHistory() { }
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)


template <int N> struct SAlignedFloats {           // N floats, 16-byte aligned inside a padded raw block
    unsigned char Raw[N * 4 + 16];
    float*        Aligned;
};


template <int N> struct SDoubleBlock {
    SAlignedFloats<N> A;
    unsigned char     GapA[0x10];
    SAlignedFloats<N> B;
    unsigned char     GapB[0x10];
};

struct CVtable_012bede0 : CSalRender {
    unsigned char     m_audioBuffer[0x190];        // +0x28 (SDK CSalRenderHrtf / HrtfMMX names: m_audioBuffer, then m_pAudio = 16-byte aligned m_audioBuffer + 0x80, computed before the vptr store)
    short*            m_pAudio;                    // +0x1B8
    SDoubleBlock<160>  Block0;                      // +0x1BC
    SDoubleBlock<160>  Block1;                      // +0x704
    SDoubleBlock<160>* PFront;                      // +0xC4C
    SDoubleBlock<160>* PBack;                       // +0xC50
    long              Count0;                      // +0xC54
    long              Count1;                      // +0xC58
    long              UnsetC5C;                    // +0xC5C
    long              UnsetC60;                    // +0xC60
    long              Count2;                      // +0xC64
    long              Count3;                      // +0xC68
    CVtable_012bede0();
    virtual ~CVtable_012bede0();
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDE0 slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDE0 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

CVtable_012bede0::~CVtable_012bede0() { ++g_dtors; g_derivedAt = ++g_order; }
void CVtable_012bede0::ClearFilterHistory() { ++g_clear; }
void CVtable_012bede0::Process(const short*, int*(&)[4]) { }
void CVtable_012bede0::SetGain(float, const float*, bool) { }
void CVtable_012bede0::SetPosition(float, float, float, float, const float*, bool, bool) { }
template <int N> static bool AlignedInside(const SAlignedFloats<N>& f)
{
    const unsigned char* p = (const unsigned char*)f.Aligned;
    return ((unsigned long)p & 15) == 0 && p >= f.Raw && p + N * 4 <= f.Raw + sizeof(f.Raw);
}
int main()
{
    bool ok = true;
    for (int round = 0; round < 3; ++round) {
        CVtable_012bede0* obj = new CVtable_012bede0;                                  // heap objects land at varying 16-byte phases
        const unsigned char* audio = (const unsigned char*)obj->m_pAudio;
        ok = ok && g_baseCtors == round + 1 && ((unsigned long)audio & 15) == 0 && audio >= obj->m_audioBuffer + 0x80 &&
             audio < obj->m_audioBuffer + 0x90 && AlignedInside<160>(obj->Block0.A) && AlignedInside<160>(obj->Block0.B) &&
             AlignedInside<160>(obj->Block1.A) && AlignedInside<160>(obj->Block1.B) &&
             obj->PFront == &obj->Block0 && obj->PBack == &obj->Block1 &&
             obj->Count0 == 0 && obj->Count1 == 0 && obj->Count2 == 0 && obj->Count3 == 0 &&
             obj->Block0.GapA[3] == 0 && obj->Block1.B.Raw[5] == 0 && obj->Block1.GapB[15] == 0;
        int clears = g_clear; obj->ClearFilterHistory(); ok = ok && g_clear == clears + 1;   // the row stored this class's vtable
        CSalRender* base = obj; delete base;                                                // ??_G: derived, then base dtor, then free
        ok = ok && g_dtors == round + 1 && g_baseDtors == round + 1 && g_lastFree == (void*)obj;
    }
    if (!ok) { std::printf("00c70b00_TEST FAIL\n"); return 1; }
    std::printf("00c70b00_TEST PASS\n");
    return 0;
}
