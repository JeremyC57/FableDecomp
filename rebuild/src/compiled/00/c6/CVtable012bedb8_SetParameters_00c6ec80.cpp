// CVtable_012bedb8::SetParameters(long a0, long a1, long a2, float volume, long a4, bool alternate, bool force) -- vtable slot 4 of the libpk gain-ramp mixer effect: unless forced, ignore an unchanged volume; else store the new volume (+0x20) and rebuild the gain ramp for the selected record (0x00C6EBE0)
// Leaf: ?SetParameters@CVtable_012bedb8@@UAEXJJJMJ_N0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6EC80, 44 bytes (+ int3 padding), thiscall `ret 1Ch` (seven stack
//   arguments; only the volume, the record selector and the force flag are read); slot 4 of the vtable 0x012BEDB8 (word
//   at 0x012BEDC8) and of its MMX / SSE2 derivatives 0x012B8A8C / 0x012B8AC8 (ctors 0x00C550B0 / 0x00C55110 over
//   0x00C6E760).
// Callee: 0x00C6EBE0 (thiscall `ret 4`: under the effect's lock, m_volume * 32767 rounded by libpk's inline-asm helper,
//   clamped to 0x7FFF, halved into the current (+0x30) or previous (+0x34) gain record, its step >> 7; catalog
//   "Audio_SetVolume_Channel", descriptive name UpdateGainRamp; the twin of the landed ComputeGainRamp 0x00C6EBA0).
// Name: descriptive (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c6ec80` is a placeholder). The volume at
//   +0x20 is the landed ComputeGainRamp model's m_volume (base CVtable_012beda4).
// Shape: `m_volume != volume` loads the argument first for `fucompp ; test ah,44h ; jnp` (equal -> return); the float
//   store is an integer move; the call shares the `ret 1Ch` epilogue.
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
class CVtable_012beda4 {                           // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
public:
    virtual void v00();
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear
    bool             m_alternate;                  // +0x24
};
class CVtable_012bedb8 : public CVtable_012beda4 { // vtable 0x012BEDB8 (ctor 0x00C6E760)
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void SetParameters(long a0, long a1, long a2, float volume, long a4, bool alternate, bool force);   // slot 4
    SGainRamp  m_first;                            // +0x28
    SGainRamp  m_second;                           // +0x2c
    SGainRamp* m_pCurrent;                         // +0x30
    SGainRamp* m_pPrevious;                        // +0x34
    void UpdateGainRamp(bool alternate);           // 0x00C6EBE0
};

void CVtable_012bedb8::SetParameters(long a0, long a1, long a2, float volume, long a4, bool alternate, bool force)
{
    if (force || m_volume != volume) {
        m_volume = volume;
        UpdateGainRamp(alternate);
    }
}