// CAudioDevice_00c3e760::BeginFrame() -- start an audio frame: with a primary output (+0x180) the shared "active" flag (0x0143BC14) becomes the primary's ready state, and-ed with the secondary output's (+0x184) when there is one; an inactive frame returns at once; an active one locks the mixer (+0x58, slot 2) and clears the scratch block (+0x17c: its dirty byte and its 0x200-byte sample buffer). Without a primary output the flag toggles. An active flag then commits the listener (+0x1d4, slot 10). Returns the flag
// Leaf: ?BeginFrame@CAudioDevice_00c3e760@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3E760, 156 bytes (+ int3 padding), __fastcall, plain `ret`, bool result.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the device object at +4 of the audio system
//   singleton 0x0143BB58; callers are the system's frame update 0x00C3D4D9 / 0x00C3D5F3 (which goes on with
//   UpdateMeters 0x00C3E800 and 0x00C3E890). The manifest's "CCombatRenderer_UpdateRenderState" is propagated.
// Callees: virtual slot 6 (+0x18, IsReady) on both outputs, the mixer's slot 2 (+8), the listener's slot 10 (+0x28).
// Shape: `s_active = s_active && secondary->IsReady();` then `if (s_active) {...}` gives the `mov [g],1 ; jmp` /
//   `xor al,al ; mov [g],al ; ret` arms (the following tests are threaded); the flag is a FILE STATIC (address never
//   taken), so VC7.1 hoists the +0x184 load above the flag store (an `extern` flag keeps the load after it); the scratch
//   block's inline Clear() (`m_dirty = 0; memset(m_pSamples, ...)` on its own `this`) lets the same-base +0x21c load
//   rise above the byte store, which then lands among the `rep stosd` set-up moves (written on m_pScratch-> directly,
//   the member is re-read after the byte store); the flag is re-read after every call.
#include <string.h>

struct COutput_00c3e760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual bool IsReady();                     // slot 6 (+0x18)
};

struct CMixer_00c3e760 {
    virtual void v00(); virtual void v01();
    virtual void Lock();                        // slot 2 (+8)
};

struct CListener_00c3e760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Commit();                      // slot 10 (+0x28)
};

struct CScratchBlock_00c3e760 {
    unsigned char _pad_0x000[8];
    unsigned char m_dirty;                      // +0x008
    unsigned char _pad_0x009[0x213];
    long* m_pSamples;                           // +0x21c (0x80 words)
    void Clear()
    {
        m_dirty = 0;
        memset(m_pSamples, 0, 0x80 * sizeof(long));
    }
};

static bool s_audioActive_0143bc14;             // 0x0143BC14 (file static: its address is never taken)

struct CAudioDevice_00c3e760 {
    unsigned char _pad_0x000[0x58];
    CMixer_00c3e760* m_pMixer;                  // +0x058
    unsigned char _pad_0x05c[0x120];
    CScratchBlock_00c3e760* m_pScratch;         // +0x17c
    COutput_00c3e760* m_pPrimary;               // +0x180
    COutput_00c3e760* m_pSecondary;             // +0x184
    unsigned char _pad_0x188[0x4c];
    CListener_00c3e760* m_pListener;            // +0x1d4
    bool BeginFrame();
};

bool CAudioDevice_00c3e760::BeginFrame()
{
    if (m_pPrimary) {
        s_audioActive_0143bc14 = m_pPrimary->IsReady();
        if (m_pSecondary)
            s_audioActive_0143bc14 = s_audioActive_0143bc14 && m_pSecondary->IsReady();
        if (s_audioActive_0143bc14) {
            m_pMixer->Lock();
            if (m_pScratch)
                m_pScratch->Clear();
        }
    } else
        s_audioActive_0143bc14 = !s_audioActive_0143bc14;
    if (s_audioActive_0143bc14 && m_pListener)
        m_pListener->Commit();
    return s_audioActive_0143bc14;
}