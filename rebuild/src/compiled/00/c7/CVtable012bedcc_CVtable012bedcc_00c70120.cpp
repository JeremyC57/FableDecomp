// CVtable_012bedcc::CVtable_012bedcc() -- constructor of a no-RTTI processing object: polymorphic base, a 16-byte-aligned 64-float scratch buffer, two zeroed blocks of two aligned 64-float buffers each, front/back pointers to the blocks and four zeroed counters
// Leaf: ??0CVtable_012bedcc@@QAE@XZ
// Name (sweep1, 2026-10-25): spelled `CVtable_012bedcc` / base `CVtable_012beda4` like the class's other landed members
//   (UpdateVoices, the scalar deleting destructor renamed this pass) and the landed base ctor/dtor 0x00C6E6A0 /
//   0x00C6E630, so the base-ctor call binds `??0CVtable_012beda4@@QAE@XZ` (was the upper-case spelling, a second name).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70120, 167 bytes (+9 int3 padding), returns `this`.
// Callee: 0x00C6E6A0 the base constructor (stores two vptrs 0x012BEDA4/0x012B8FC8, InitializeCriticalSection on +8,
// zeroes +0x20 and the byte +0x24; catalogued as the propagated `CConsoleCommandParameters`). Vtable 0x012BEDCC (slot 0
// the vector deleting destructor 0x00C70A30); called once, from 0x00C55490.
// Identity: the manifest's `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize` is propagated. The
// vtable has no complete-object locator (no-RTTI module), so the class is named after it (GOTCHAS family-landing rule);
// the members are descriptive. Its twin 0x00C70B00 (vtable 0x012BEDE0) is the same class shape with 160-float buffers.
// Shape: the scratch buffer's pointer store ((this+0xB7) & ~15 -> +0x1B8) precedes the derived vptr store, so it is a
// second, non-polymorphic base with an inline constructor (GOTCHAS wave 24o); the two blocks are POD members zeroed by
// intrinsic memsets (`rep stosd` of 0x92 dwords from this+0x1BC and this+0x404) and their four aligned pointers set
// from the derived constructor (addressed off `this`, not off a block pointer -- a block constructor addresses them
// off edx/ebx); the counters are assigned +0x664/+0x668 before +0x654/+0x658 (retail store order); +0x65C/+0x660 stay
// unset.
// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base constructor it calls is respelled (??0CVtable_012beda4@@QAE@XZ -> ??0CSalRender@@QAE@XZ, unit 0x00C6E630).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4, 0x28 bytes; constructor 0x00C6E6A0, destructor 0x00C6E630)
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

CVtable_012bedcc::CVtable_012bedcc()
{
    m_pAudio = (short*)(((unsigned long)m_audioBuffer + 0x80 + 15) & ~15);
    memset(&Block0, 0, sizeof(Block0));
    memset(&Block1, 0, sizeof(Block1));
    Block0.A.Aligned = (float*)(((unsigned long)Block0.A.Raw + 15) & ~15);
    Block0.B.Aligned = (float*)(((unsigned long)Block0.B.Raw + 15) & ~15);
    Block1.A.Aligned = (float*)(((unsigned long)Block1.A.Raw + 15) & ~15);
    Block1.B.Aligned = (float*)(((unsigned long)Block1.B.Raw + 15) & ~15);
    PFront = &Block0;
    PBack = &Block1;
    Count2 = 0;
    Count3 = 0;
    Count0 = 0;
    Count1 = 0;
}