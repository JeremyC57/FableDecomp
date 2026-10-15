// CSoundEmitterLH::vfunc_9(long handle) -- when the sound system is up: stop every active sound of this emitter playing `handle` through the system's player (slot 6 -> slot 22), mark it -1, then prune the list (0x00AABCA0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC400, length 87 bytes (96 with padding).
// Vtable 0x0129E6EC (CSoundEmitterLH, rtti-vtables.tsv) slot 9.
// Callees: slot 6 (+0x18) of the sound system (the manager's +0x40, as in 0x00AABC50), slot 22 (+0x58) of
// the object it returns with the handle, 0x00AABCA0 (a thiscall on `this`, no arguments: the inactive-sound
// prune; manifest label CModeHoverBase::RemoveInactiveHoverAnims is propagated).
// Layout as 0x00AABC50: +0x04 the owning manager, +0x14 the VC6 list of CActiveSound pointers; the
// CActiveSound handle word is +0x08 (landed constructor 0x00AAB8B0).
// Shape: `this` in edi, the system null test skipping everything (the prune too: it is inside the `if`), the
// node in ebx, the handle in ebp, the sound in esi; the virtual calls inside the loop force the manager,
// the system and the list sentinel to be re-read (`cmp ebx,[edi+14h]` per iteration); the handle pushed
// to slot 22 is the argument (the member could have changed across the slot-6 call).
struct CSoundPlayer_00aac400 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void Stop(long handle);         // slot 22 (+0x58)
};

struct CSoundSystem_00aac400 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundPlayer_00aac400* GetPlayer();   // slot 6 (+0x18)
};

struct CSoundManagerLH {
    unsigned char          _00[0x40];
    CSoundSystem_00aac400* m_pSystem;       // +0x40
};

struct CActiveSound_00aac400 {              // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long m_references;                      // +0x04
    long m_handle;                          // +0x08
};

struct SSoundNode_00aac400 {                // VC6 list node, 16 bytes
    SSoundNode_00aac400*   next;
    SSoundNode_00aac400*   prev;
    CActiveSound_00aac400* sound;           // +0x08
    long                   extra;           // +0x0c
};

struct CSoundEmitterLH {                    // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH*     m_pManager;        // +0x04
    float                m_position[3];     // +0x08
    SSoundNode_00aac400* m_pSounds;         // +0x14: list sentinel
    void RemoveInactiveSounds();            // 0x00AABCA0
    virtual void vfunc_9(long handle);
};

void CSoundEmitterLH::vfunc_9(long handle)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode_00aac400* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound_00aac400* active = node->sound;
            if (active->m_handle == handle) {
                m_pManager->m_pSystem->GetPlayer()->Stop(handle);
                active->m_handle = -1;
            }
        }
        RemoveInactiveSounds();
    }
}