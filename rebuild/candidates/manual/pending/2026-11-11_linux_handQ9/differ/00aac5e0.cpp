// CSoundEmitterLH::GetActiveSoundInfo(long, long, vector<CActiveSoundInfo>*) -- when the sound system is up: for every active sound whose +0x0c / +0x18 words equal the two keys, append a CActiveSoundInfo {+0x18, +0x0c, +0x10} to the vector
// Grade: DIFFER(ours v retail) -- DIFFER(130v136): regraded as it sits with FABLE_HDR_OVERLAY (2026-11-11), residue unchanged from the prior draft: DIFFER(130v136) (try k): retail loads the out-vector argument into ebp once, after the empty-list test (shrink-wrapped `push ebp`), and spills the record's +0x10 word into that dead argument slot across the CBaseClass call (node ebx, keys esi/edi); VC7.1 here keeps the +0x10 word in a register, reloads the argument at the call and aligns the loop head (`lea ecx,[ecx]`).
// Earlier grade (prior draft): DIFFER(ours v retail) -- DIFFER(130v136) (try k): retail loads the out-vector argument into ebp once, after the empty-list test (shrink-wrapped `push ebp`), and spills the record's +0x10 word into that dead argument slot across the CBaseClass call (node ebx, keys esi/edi); VC7.1 here keeps the +0x10 word in a register, reloads the argument at the call and aligns the loop head (`lea ecx,[ecx]`).
// Leaf: GetActiveSoundInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC5E0, 136 bytes (`ret 0Ch`). CSoundEmitterLH vtable 0x0129E6EC slot 24.
// Label: the catalog's "CModeHoverBase::PushAnimEntry" is propagated (the vtable slot names the class); the method
// name is descriptive.
// Callees: CBase::CBase @ 0x0099A2D0 / CBase::~CBase @ 0x0099A2E0 (the info record's base), the vector's
// push_back(const CActiveSoundInfo&) @ 0x00AACEF0 (catalog "Vector_PushBack_SingleElement").
// Layout: CActiveSoundInfo (vtable 0x0129E674, rtti-vtables.tsv): CBase + three words; the active sound's keys at +0x0c
// and +0x18, its third word at +0x10.
// Shape: the info record is a temporary built by an inline three-argument constructor (arguments right to left:
// the +0x10 word is read first and parked in the dead third-argument slot across the CBase call); `this` itself
// is parked in the frame word (register pressure: node ebx, out vector ebp, keys esi/edi).
#include "engine/CBaseClass.h"                 // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CActiveSoundInfo : CBaseClass {      // vtable 0x0129E674
    long m_04;
    long m_08;
    long m_0c;
    CActiveSoundInfo(long a, long b, long c) : m_04(a), m_08(b), m_0c(c) { }
};
struct CActiveSoundInfoVector {
    void push_back(const CActiveSoundInfo& info);   // 0x00AACEF0
};

struct CSoundSystem;
struct CSoundManagerLH {
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                // +0x40
};

struct CActiveSound {                       // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long m_references;                      // +0x04
    long m_handle;                          // +0x08
    long m_0c;                              // +0x0c
    long m_10;                              // +0x10
    long m_14;                              // +0x14
    long m_18;                              // +0x18
};

struct SSoundNode {                         // VC6 list node, 16 bytes
    SSoundNode*   next;
    SSoundNode*   prev;
    CActiveSound* sound;                    // +0x08
    long          extra;                    // +0x0c
};

struct CSoundEmitterLH {                    // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH* m_pManager;            // +0x04
    float            m_position[3];         // +0x08
    SSoundNode*      m_pSounds;             // +0x14: list sentinel
    void GetActiveSoundInfo(long key0c, long key18, CActiveSoundInfoVector* out);
};

void CSoundEmitterLH::GetActiveSoundInfo(long key0c, long key18, CActiveSoundInfoVector* out)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            long type = sound->m_0c;
            long id = sound->m_18;
            if (type == key0c && id == key18)
                out->push_back(CActiveSoundInfo(id, type, sound->m_10));
        }
    }
}
