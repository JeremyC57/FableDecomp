// CAudioSource::CreateProcessor(unsigned long mode) -- create the voice's 3D sound processor for the speaker mode: detect the CPU once, then build the SSE2 or the MMX/C variant of the HRTF processor (0x100000), the four-tap delay panner (0x200000), the gain-ramp panner (0x400000) or the plain stereo panner (anything else); null when the allocation fails
// Leaf: ?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55270, 587 bytes, thiscall `ret 4` (`this` unused), EH-framed (states 0..7: one per
//   `new`, the class operator delete runs if a constructor throws).
// Identity (descriptive; libpk software mixer, no PDB / RTTI): the only caller is the landed CAudioSource::Open 0x00C55680, which
//   stores the result as the voice's processor (+0x220) and primes it; it binds the address as ?CreateStream@CAudioSource@@ (a
//   descriptive guess: the body builds the effect objects, not a stream; NAME_FIXES). The catalog label
//   "CTCHeroStats_CreateSpellEffect" is propagated.
// Callees: DetectCpu 0x00C52E40 (bound spelling) behind g_cpuDetected 0x0143BC78, g_cpuHasSSE2 0x0143BC7D; CodaMalloc 0x00C3C360
//   (the CCodaObjectBase operator new); the constructors CVtable_012bede0 0x00C70B00, CVtable_012bedcc 0x00C70120,
//   CVtable_012bed7c 0x00C6CA20 (base of the SSE2 / MMX four-tap panners, whose own constructors 0x00C55070 / 0x00C550D0 are
//   expanded here: base call + vtable 0x012B8A64 / 0x012B8AA0), CVtable_012b8a78 0x00C55090, CVtable_012b8a8c 0x00C550B0,
//   CVtable_012b8ab4 0x00C550F0, CVtable_012b8ac8 0x00C55110.
// Sizes (the CodaMalloc arguments): 0xC6C, 0x214, 0x78, 0x38 (SSE2) and 0x66C, 0x214, 0x78, 0x38 (MMX / C).
#include "engine/CCodaObjectBase.h"

extern bool g_cpuDetected;                                                  // 0x0143BC78
extern bool g_cpuHasSSE2;                                                   // 0x0143BC7D
void DetectCpu();                                                           // 0x00C52E40

struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CCodaObjectBase {                          // 0x28 bytes (ctor 0x00C6E6A0 landed)
public:
    CVtable_012beda4();                                                     // 0x00C6E6A0
    virtual ~CVtable_012beda4();
    virtual void Clear();
    virtual void Process(const short*, long* const*);
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void Update(float, float, float, float, const float*, bool, bool);
    CUnprovenLock_012B8FC8 Lock;                                            // +0x04 (its CRITICAL_SECTION at +0x08)
    float m_volume;                                                         // +0x20
    bool  m_crossfade;                                                      // +0x24
};
class CVtable_012bed7c : public CVtable_012beda4 {                         // four-tap delay panner, 0x214 bytes
public:
    CVtable_012bed7c();                                                     // 0x00C6CA20
    virtual void SetVolume(float, unsigned long, bool);
    unsigned char m_state[0x214 - 0x28];
};
class CVtable_012bed90 : public CVtable_012beda4 {                         // gain-ramp panner, 0x78 bytes
public:
    CVtable_012bed90();                                                     // 0x00C6D9D0
    virtual void SetVolume(float, unsigned long, bool);
    unsigned char m_state[0x78 - 0x28];
};
class CVtable_012bedb8 : public CVtable_012beda4 {                         // plain stereo panner, 0x38 bytes
public:
    CVtable_012bedb8();                                                     // 0x00C6E760
    virtual void SetVolume(float, unsigned long, bool);
    unsigned char m_state[0x38 - 0x28];
};
class CVtable_012bede0 : public CVtable_012beda4 {                         // HRTF processor (SSE2), 0xC6C bytes
public:
    CVtable_012bede0();                                                     // 0x00C70B00
    virtual void SetVolume(float, unsigned long, bool);
    unsigned char m_state[0xc6c - 0x28];
};
class CVtable_012bedcc : public CVtable_012beda4 {                         // HRTF processor (C), 0x66C bytes
public:
    CVtable_012bedcc();                                                     // 0x00C70120
    virtual void SetVolume(float, unsigned long, bool);
    unsigned char m_state[0x66c - 0x28];
};
class CVtable_012b8a64 : public CVtable_012bed7c {                         // SSE2 four-tap panner (vtable 0x012B8A64)
public:
    CVtable_012b8a64() {}                                                   // 0x00C55070
    virtual void Process(const short*, long* const*);
};
class CVtable_012b8aa0 : public CVtable_012bed7c {                         // MMX four-tap panner (vtable 0x012B8AA0)
public:
    CVtable_012b8aa0() {}                                                   // 0x00C550D0
    virtual void Process(const short*, long* const*);
};
class CVtable_012b8a78 : public CVtable_012bed90 {                         // SSE2 gain-ramp panner (vtable 0x012B8A78)
public:
    CVtable_012b8a78();                                                     // 0x00C55090
    virtual void Process(const short*, long* const*);
};
class CVtable_012b8ab4 : public CVtable_012bed90 {                         // MMX gain-ramp panner (vtable 0x012B8AB4)
public:
    CVtable_012b8ab4();                                                     // 0x00C550F0
    virtual void Process(const short*, long* const*);
};
class CVtable_012b8a8c : public CVtable_012bedb8 {                         // SSE2 stereo panner (vtable 0x012B8A8C)
public:
    CVtable_012b8a8c();                                                     // 0x00C550B0
    virtual void Process(const short*, long* const*);
};
class CVtable_012b8ac8 : public CVtable_012bedb8 {                         // MMX stereo panner (vtable 0x012B8AC8)
public:
    CVtable_012b8ac8();                                                     // 0x00C55110
    virtual void Process(const short*, long* const*);
};

class CAudioSource {
public:
    CVtable_012beda4* CreateProcessor(unsigned long mode);
};

CVtable_012beda4* CAudioSource::CreateProcessor(unsigned long mode)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        switch (mode) {
        case 0x100000:
            return new CVtable_012bede0;
        case 0x200000:
            return new CVtable_012b8a64;
        case 0x400000:
            return new CVtable_012b8a78;
        default:
            return new CVtable_012b8a8c;
        }
    } else {
        switch (mode) {
        case 0x100000:
            return new CVtable_012bedcc;
        case 0x200000:
            return new CVtable_012b8aa0;
        case 0x400000:
            return new CVtable_012b8ab4;
        default:
            return new CVtable_012b8ac8;
        }
    }
}