// CSoundEmitterLH::vfunc_8(void* source, long id) -- when the sound system is up: stop every idle-flagged sound of this emitter started by (source, id) and mark it -1, prune the list, then query the player once more for each remaining match
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC340, 182 bytes (+ int3 padding), `ret 8`. Vtable 0x0129E6EC (CSoundEmitterLH,
//   rtti-vtables.tsv) slot 8. The audio layer is not in Ego_r.pdb: method and member names are descriptive.
// Leaf: vfunc_8
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 22 (+0x58, Stop(handle)) and slot 28 (+0x70, a
//   float-returning query taking the handle and an out word; the result is discarded with `fstp st(0)`),
//   CSoundEmitterLH::RemoveInactiveSounds @ 0x00AABCA0 (landed).
// Shape: the sibling of the landed vfunc_9 0x00AAC400 / StopSoundsContaining 0x00AAC460: `this` in edi, the system null
//   test wrapping both walks and the prune, the node in ebx and the sound in esi; the three-way match (+0x04 clear, +0x0C
//   == source, +0x18 == id) is one `&&` chain per walk; the virtual calls force the manager, system and list sentinel to
//   be re-read. The query's out word is the function's single `push ecx` frame slot.
// Layout as 0x00AABCA0: +0x04 the owning manager (system +0x40), +0x14 the VC6 list of CActiveSound pointers;
//   CActiveSound (landed ctor 0x00AAB8B0): +0x04 m_references, +0x08 handle, +0x0C source, +0x18 id word.
struct CSoundPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void Stop(long handle);                    // slot 22 (+0x58)
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual float Query(long handle, long* out);       // slot 28 (+0x70)
};

struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundPlayer* GetPlayer();                 // slot 6 (+0x18)
};

struct CSoundManagerLH {
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                           // +0x40
};

struct CActiveSound {                                  // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long          m_references;                        // +0x04
    long          m_handle;                            // +0x08
    void*         m_pSource;                           // +0x0c
    void*         m_pD;                                // +0x10
    bool          m_e;                                 // +0x14
    bool          m_f;                                 // +0x15
    unsigned char _pad16[2];
    long          m_id;                                // +0x18
};

struct SSoundNode {                                    // VC6 list node, 16 bytes
    SSoundNode*   next;
    SSoundNode*   prev;
    CActiveSound* sound;                               // +0x08
    long          extra;                               // +0x0c
};

struct CSoundEmitterLH {                               // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH* m_pManager;                       // +0x04
    float            m_position[3];                    // +0x08
    SSoundNode*      m_pSounds;                        // +0x14: list sentinel
    void RemoveInactiveSounds();                       // 0x00AABCA0
    virtual void vfunc_8(void* source, long id);
};

void CSoundEmitterLH::vfunc_8(void* source, long id)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_references == 0 && sound->m_pSource == source && sound->m_id == id) {
                m_pManager->m_pSystem->GetPlayer()->Stop(sound->m_handle);
                sound->m_handle = -1;
            }
        }
        RemoveInactiveSounds();
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_references == 0 && sound->m_pSource == source && sound->m_id == id) {
                long out;
                m_pManager->m_pSystem->GetPlayer()->Query(sound->m_handle, &out);
            }
        }
    }
}
