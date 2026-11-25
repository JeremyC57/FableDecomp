// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of the HRTF
// renderer constructor 0x00C70120 (CVtable_012bedcc): the CSalRender base is constructed once, m_pAudio is 16-byte aligned inside
// m_audioBuffer + 0x80, the four block pointers are aligned inside their raw buffers, both blocks are otherwise zero, front / back point at
// the blocks, the counters are zero, and the object carries this class's vtable.
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

struct CVtable_012bedcc : CSalRender {
    unsigned char     m_audioBuffer[0x190];        // +0x28 (SDK CSalRenderHrtf / HrtfMMX names: m_audioBuffer, then m_pAudio = 16-byte aligned m_audioBuffer + 0x80, computed before the vptr store)
    short*            m_pAudio;                    // +0x1B8
    SDoubleBlock<64>  Block0;                      // +0x1BC
    SDoubleBlock<64>  Block1;                      // +0x404
    SDoubleBlock<64>* PFront;                      // +0x64C
    SDoubleBlock<64>* PBack;                       // +0x650
    long              Count0;                      // +0x654
    long              Count1;                      // +0x658
    long              Unset65C;                    // +0x65C
    long              Unset660;                    // +0x660
    long              Count2;                      // +0x664
    long              Count3;                      // +0x668
    CVtable_012bedcc();
    virtual ~CVtable_012bedcc();
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDCC slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDCC (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

CVtable_012bedcc::~CVtable_012bedcc() { ++g_dtors; g_derivedAt = ++g_order; }
void CVtable_012bedcc::ClearFilterHistory() { ++g_clear; }
void CVtable_012bedcc::Process(const short*, int*(&)[4]) { }
void CVtable_012bedcc::SetGain(float, const float*, bool) { }
void CVtable_012bedcc::SetPosition(float, float, float, float, const float*, bool, bool) { }
template <int N> static bool AlignedInside(const SAlignedFloats<N>& f)
{
    const unsigned char* p = (const unsigned char*)f.Aligned;
    return ((unsigned long)p & 15) == 0 && p >= f.Raw && p + N * 4 <= f.Raw + sizeof(f.Raw);
}
int main()
{
    bool ok = true;
    for (int round = 0; round < 3; ++round) {
        CVtable_012bedcc* obj = new CVtable_012bedcc;                                  // heap objects land at varying 16-byte phases
        const unsigned char* audio = (const unsigned char*)obj->m_pAudio;
        ok = ok && g_baseCtors == round + 1 && ((unsigned long)audio & 15) == 0 && audio >= obj->m_audioBuffer + 0x80 &&
             audio < obj->m_audioBuffer + 0x90 && AlignedInside<64>(obj->Block0.A) && AlignedInside<64>(obj->Block0.B) &&
             AlignedInside<64>(obj->Block1.A) && AlignedInside<64>(obj->Block1.B) &&
             obj->PFront == &obj->Block0 && obj->PBack == &obj->Block1 &&
             obj->Count0 == 0 && obj->Count1 == 0 && obj->Count2 == 0 && obj->Count3 == 0 &&
             obj->Block0.GapA[3] == 0 && obj->Block1.B.Raw[5] == 0 && obj->Block1.GapB[15] == 0;
        int clears = g_clear; obj->ClearFilterHistory(); ok = ok && g_clear == clears + 1;   // the row stored this class's vtable
        CSalRender* base = obj; delete base;                                                // ??_G: derived, then base dtor, then free
        ok = ok && g_dtors == round + 1 && g_baseDtors == round + 1 && g_lastFree == (void*)obj;
    }
    if (!ok) { std::printf("00c70120_TEST FAIL\n"); return 1; }
    std::printf("00c70120_TEST PASS\n");
    return 0;
}
