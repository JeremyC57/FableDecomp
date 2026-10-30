// CVtable_012bed90::SetParameters(float pan, long a1, long a2, float volume, long a4, bool alternate, bool force) -- vtable slot 4 of the libpk panning gain effect: turn the pan angle (degrees) into per-channel scales (four-speaker equal-sum quadrants when the audio master runs four or more output channels, else a left/right pair), store them when they changed (or when forced), then store a changed (or forced) volume and rebuild the gain ramps -- or rebuild for changed scales alone
// Grade: DIFFER(367v369) -- frame layout only: retail reserves a 0x20-byte frame and homes the six stored pan temporaries in distinct slots (c +4, 0.5-s +8, three gains +0xC/+0x10/+0x14, the c+0.5 CSE in the dead pan argument slot); VC7.1 packs ours into 0xC bytes (slot reuse, the CSE in a local), every instruction otherwise in retail order | tried: named intermediates, expression form (closest), function-scope gains, `pan *= k` in place, hoisted sin, QFE-4035 (same) | next: find the declaration shape that gives eight unshared float slots (an inline helper with by-value float parameters, or more function-scope locals)
// Leaf: ?SetParameters@CVtable_012bed90@@UAEXMJJMJ_N0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E050, 369 bytes, thiscall `ret 1Ch` (seven stack arguments; a1 / a2 / a4 are not
//   read), 0x20-byte frame. Slot 4 of the vtable 0x012BED90 (word at 0x012BEDA0) and of its MMX / SSE2 derivatives
//   (0x012B8A88 / 0x012B8AC4) -- the twin of the landed CVtable_012bedb8::SetParameters 0x00C6EC80 (same seven-argument
//   slot; there the first argument is unread and modelled long).
// Identity (descriptive; libpk software mixer, no PDB / RTTI). The catalog label sub_00c6e050 is a placeholder.
// Callees: RebuildGain 0x00C6DEA0 (the landed binding, this chunk's retail_asm row); g_pAudioMaster 0x0143BC2C (`class
//   CAudioMaster`, as the landed mixer rows) +0x1BC output channels; fsin / fcos intrinsics; pi/180 at 0x012B8FBC, 0.5f
//   at 0x0122F59C.
// Shape: x87 compares written `m_scales[i] != value` / `m_volume != volume` (the new value loaded first: `fucompp ; test
//   ah,44h ; jp/jnp`); the quadrant products as sequenced float locals.
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(sin, cos)

class CUnprovenAudioMasterBase_012B5B80 { public: virtual void v00(); unsigned char UnprovenBase04[0x50]; };
class CAudioMaster : public CUnprovenAudioMasterBase_012B5B80 {
public:
    unsigned char UnprovenFields54[356];
    void* TapTable;                                // +0x1b8
    unsigned long m_outputChannels;                // +0x1bc
    unsigned char UnprovenTail1C0[24];
};
extern CAudioMaster* g_pAudioMaster;               // 0x0143BC2C

struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CVtable_012beda4 {                           // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
public:
    virtual void v00();
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear
    bool             m_alternate;                  // +0x24
};
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
class CVtable_012bed90 : public CVtable_012beda4 { // vtable 0x012BED90 (ctor 0x00C6D9D0)
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void SetParameters(float pan, long a1, long a2, float volume, long a4, bool alternate, bool force);   // slot 4
    SGainRamp  m_first[6];                         // +0x28
    SGainRamp  m_second[6];                        // +0x40
    SGainRamp* m_pCurrent;                         // +0x58
    SGainRamp* m_pPrevious;                        // +0x5c
    float      m_scales[6];                        // +0x60 front left/right, rear left/right
    void RebuildGain(bool alternate);              // 0x00C6DEA0
};

void CVtable_012bed90::SetParameters(float pan, long a1, long a2, float volume, long a4, bool alternate, bool force)
{
    bool changed = false;
    if (g_pAudioMaster->m_outputChannels >= 4) {
        float s = (float)sin(pan * 0.0174532924f) * 0.5f;
        float c = (float)cos(pan * 0.0174532924f) * 0.5f;
        float frontLeft = (c + 0.5f) * (0.5f - s);
        float frontRight = (c + 0.5f) * (s + 0.5f);
        float backLeft = (0.5f - s) * (0.5f - c);
        float backRight = (0.5f - c) * (s + 0.5f);
        if (force || m_scales[0] != frontLeft || m_scales[1] != frontRight || m_scales[2] != backLeft
            || m_scales[3] != backRight) {
            m_scales[0] = frontLeft;
            m_scales[1] = frontRight;
            m_scales[2] = backLeft;
            m_scales[3] = backRight;
            changed = true;
        }
    } else {
        float s = (float)sin(pan * 0.0174532924f) * 0.5f;
        float left = 0.5f - s;
        float right = s + 0.5f;
        if (force || m_scales[0] != left || m_scales[1] != right) {
            m_scales[0] = left;
            m_scales[1] = right;
            changed = true;
        }
    }
    if (force || m_volume != volume) {
        m_volume = volume;
        RebuildGain(alternate);
    } else if (changed) {
        RebuildGain(alternate);
    }
}
