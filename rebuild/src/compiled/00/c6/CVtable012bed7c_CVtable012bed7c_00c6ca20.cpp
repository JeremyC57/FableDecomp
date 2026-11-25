// CVtable_012bed7c::CVtable_012bed7c() -- constructor: the critical-section-owning base (0x00C6E6A0), own vtable, a 16-byte aligned pointer into an inline scratch buffer, two zeroed 0x30-byte records with pointers to them, two zeroed six-word arrays
// Leaf: ??0CVtable_012bed7c
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CA20, 150 bytes (+ int3 padding).
// Label correction: the manifest's "CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface" is propagated and
//   wrong -- that class (Ego_r.pdb) is 0x104 bytes with no vtable; this object stores vtable 0x012BED7C (not in
//   rtti-vtables.tsv: a no-RTTI script/engine module) and reaches +0x210. Named by vtable like its landed sibling
//   CVtable_012bed90 (0x00C6D9D0), which has the same base and the same two-record/two-pointer shape.
// Callees: CVtable_012beda4::CVtable_012beda4 @ 0x00C6E6A0 (0x28 bytes: vptr, a critical-section owner at +4, a dword
//   +0x20, a byte +0x24).
// Shape: after the vtable store, in declaration order: the aligned pointer `(this + 0x68 + 15) & ~15` (`lea eax,[esi+77h] ;
//   and eax,0FFFFFFF0h`) into the 0x110-byte buffer at +0x68; two 12-word records that clear themselves with an intrinsic
//   memset (`rep stosd`, one shared zero register); the two record pointers; then a body loop clearing two six-word
//   arrays, which VC7.1 unrolls into interleaved stores (+0x1e4/+0x1fc, +0x1e8/+0x200, ...).
// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base constructor it calls is respelled (??0CVtable_012beda4@@QAE@XZ -> ??0CSalRender@@QAE@XZ, unit 0x00C6E630).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4, 0x28 bytes; constructor 0x00C6E6A0, destructor 0x00C6E630)
#include <string.h>


struct SRecord_00c6ca20 {                              // twelve words, cleared as a block
    long m_words[12];
    SRecord_00c6ca20() { memset(m_words, 0, sizeof(m_words)); }
};

struct CVtable_012bed7c : CSalRender {           // vtable 0x012BED7C
    unsigned char     m_state[0x40];                   // +0x28 (not touched by the constructor)
    unsigned char     m_buffer[0x110];                 // +0x68 scratch storage, used 16-byte aligned
    unsigned char*    m_pAlignedBuffer;                // +0x178
    SRecord_00c6ca20  m_first;                         // +0x17c
    SRecord_00c6ca20  m_second;                        // +0x1ac
    SRecord_00c6ca20* m_pCurrent;                      // +0x1dc
    SRecord_00c6ca20* m_pPrevious;                     // +0x1e0
    long              m_valuesA[6];                    // +0x1e4
    long              m_valuesB[6];                    // +0x1fc
    CVtable_012bed7c();
    virtual ~CVtable_012bed7c();                      // 0x00C6CAC0 (slot 0 of the vtable is its scalar deleting destructor)
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BED7C slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BED7C (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

CVtable_012bed7c::CVtable_012bed7c()
    : m_pAlignedBuffer(reinterpret_cast<unsigned char*>((reinterpret_cast<unsigned long>(m_buffer) + 15) & ~15UL)),
      m_first(), m_second(), m_pCurrent(&m_first), m_pPrevious(&m_second)
{
    for (long i = 0; i < 6; ++i) {
        m_valuesA[i] = 0;
        m_valuesB[i] = 0;
    }
}