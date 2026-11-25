// Behaviour test of CSalSw3DVoice::Create (0x00C556E0), linked against the row object, over the shared headers: a failing base create
// is returned unchanged; otherwise the frame buffer is set up over the scratch block (128 frames of 2 bytes, 1 channel), becomes the mix
// buffer, and Initialize gets the 3D source's HRTF mode: 0 when it succeeds, -5 when it fails. A 2D source (Is3D false) is not used.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE        // test only: declares the CCoda3DSource constructor the test defines
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#define FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE
#include "engine/CSalSw3DVoice.h"
#include "engine/CCoda3DSource.h"
#include <cstdio>
#include <cstring>
// The callee spellings the row binds (declaration-only shells, as in the row).
struct CVoiceDesc_00c556e0;
struct CSoftwareVoice_00c5a1d0 { virtual long Setup(CVoiceDesc_00c556e0* desc); };
struct CAudioRingBuffer {
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);
};
unsigned char g_scratchRing_0143bcb0[0x100];
static long g_baseResult; static int g_baseCalls; static void* g_baseThis; static void* g_baseDesc;
static int g_initCalls; static void* g_initThis; static unsigned long g_initArgs[5]; static unsigned char* g_initData;
static int g_initializeCalls; static bool g_initializeResult; static CHRTFMODE g_mode;
static const void* g_mixBuffer; static const void* g_frameBuffer;
static CHRTFMODE g_ctorMode; static bool g_is3D;
long CSoftwareVoice_00c5a1d0::Setup(CVoiceDesc_00c556e0* desc) { ++g_baseCalls; g_baseThis = this; g_baseDesc = desc; return g_baseResult; }
void CAudioRingBuffer::Init(unsigned char* data, unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long e)
{ ++g_initCalls; g_initThis = this; g_initData = data; g_initArgs[0] = a; g_initArgs[1] = b; g_initArgs[2] = c; g_initArgs[3] = d; g_initArgs[4] = e; }
bool CSalSw3DVoice::Initialize(CHRTFMODE mode)
{ ++g_initializeCalls; g_mode = mode; g_mixBuffer = m_pMixBuffer; g_frameBuffer = &m_FrameBuffer; return g_initializeResult; }
// The source: the shared CCoda3DSource, its out-of-line members defined here (the test is their only user).
CCoda3DSource::CCoda3DSource() { m_HrtfMode = g_ctorMode; }
CCoda3DSource::~CCoda3DSource() { }
bool CCoda3DSource::Is3D() const { return g_is3D; }
bool CCoda3DSource::Create(unsigned int) { return true; }
CCodaSource::~CCodaSource() { }
bool CCodaSource::Is3D() const { return false; }
bool CCodaSource::IsCreated() const { return false; }
bool CCodaSource::Create(unsigned int) { return true; }
void CCodaSource::Release() { }
CCodaObject::~CCodaObject() { }
CCodaObjectBase::~CCodaObjectBase() { }
extern "C" void __cdecl CodaFree(void*) { }
static double g_voice[0x248 / sizeof(double) + 1];
int main()
{
    bool ok = sizeof(CSalSw3DVoice) == 0x248;
    CSalSw3DVoice* v = reinterpret_cast<CSalSw3DVoice*>(g_voice);
    g_ctorMode = C_HRTF_DELAY; g_is3D = true;
    CCoda3DSource src;
    // 1. the base create fails: its result comes back, nothing else runs
    g_baseResult = -7;
    ok = ok && v->CSalSw3DVoice::Create(&src) == -7 && g_baseCalls == 1 && g_baseThis == v && g_baseDesc == static_cast<CCodaSource*>(&src)
            && g_initCalls == 0 && g_initializeCalls == 0;
    // 2. success: the frame buffer over the scratch block becomes the mix buffer, Initialize gets the source's mode
    g_baseResult = 0; g_initializeResult = true;
    ok = ok && v->CSalSw3DVoice::Create(&src) == 0 && g_initCalls == 1 && g_initThis == g_frameBuffer && g_initData == g_scratchRing_0143bcb0
            && g_initArgs[0] == 0x80 && g_initArgs[1] == 2 && g_initArgs[2] == 1 && g_initArgs[3] == 0 && g_initArgs[4] == 0
            && g_mixBuffer == g_frameBuffer && g_initializeCalls == 1 && g_mode == C_HRTF_DELAY
            && reinterpret_cast<const unsigned char*>(g_frameBuffer) == reinterpret_cast<unsigned char*>(g_voice) + 0x1f0;
    // 3. Initialize fails: -5; a positive base result is not an error either
    g_initializeResult = false; g_baseResult = 3;
    ok = ok && v->CSalSw3DVoice::Create(&src) == -5 && g_initializeCalls == 2;
    if (!ok) { std::printf("00c556e0_TEST FAIL\n"); return 1; }
    std::printf("00c556e0_TEST PASS\n");
    return 0;
}
