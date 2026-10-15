// CSoundEmitterLH::vfunc_23(long sound, long instance) -- true when the sound system is up and one of this emitter's active sounds matches both the sound (+0x0c) and the instance word (+0x18)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AABC50, length 65 bytes (80 with padding).
// Vtable 0x0129E6EC (CSoundEmitterLH, rtti-vtables.tsv) slot 23.
// Layout: +0x04 is the owning sound manager (its +0x40 is the sound-system pointer that CSoundManagerLH
// slot 49, 0x00A3E480, null-tests the same way; the landed constructor 0x00AAC580 calls the manager method
// 0x00A3E420 on it), +0x14 the VC6 list of CActiveSound pointers (16-byte nodes: next, prev, sound, word).
// CActiveSound (landed 0x00AAB8B0): +0x0c the sound word, +0x18 the instance word.
// Shape: ONE `xor al,al` exit shared by the system null test and the empty/exhausted walk (the loop nested in
// `if (system)`; an early `return false` gets its own exit), the sentinel cached in edx (no calls in the
// loop), the two arguments loaded once into esi/edi before the aligned loop head, `return true` inside the
// loop with the `&&` test as the separate `mov al,1` exit.
struct CSoundSystem_00aabc50;

struct CSoundManagerLH {
    unsigned char          _00[0x40];
    CSoundSystem_00aabc50* m_pSystem;       // +0x40
};

struct CActiveSound_00aabc50 {              // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long m_references;                      // +0x04
    long m_handle;                          // +0x08
    long m_sound;                           // +0x0c
    long m_10;                              // +0x10
    bool m_e, m_f;                          // +0x14, +0x15
    long m_instance;                        // +0x18
};

struct SSoundNode_00aabc50 {                // VC6 list node, 16 bytes
    SSoundNode_00aabc50*   next;
    SSoundNode_00aabc50*   prev;
    CActiveSound_00aabc50* sound;           // +0x08
    long                   extra;           // +0x0c
};

struct CSoundEmitterLH {                    // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH*     m_pManager;        // +0x04
    float                m_position[3];     // +0x08
    SSoundNode_00aabc50* m_pSounds;         // +0x14: list sentinel
    virtual bool vfunc_23(long sound, long instance);
};

bool CSoundEmitterLH::vfunc_23(long sound, long instance)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode_00aabc50* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound_00aabc50* active = node->sound;
            if (active->m_sound == sound && active->m_instance == instance)
                return true;
        }
    }
    return false;
}