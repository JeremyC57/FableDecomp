// CSoundEmitterLH::StopSoundsContaining(const CCharString&) -- when the sound system is up: stop (player slot 22) every still-active sound whose name contains the text and mark its handle -1, then prune the list
// Leaf: StopSoundsContaining
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC460, 133 bytes (`ret 4`). CSoundEmitterLH vtable 0x0129E6EC slot 12.
// Label: the catalog's "CModeHoverBase::RemoveHoverAnimsBySubstring" is propagated (the vtable slot names the class);
// the method name is descriptive.
// Callees: strstr @ 0x00BFEB3A (`__cdecl`, the sound's name first), the system's slot 6 (GetPlayer), the player's
// slot 22 (+0x58, Stop(handle)), CSoundEmitterLH::RemoveInactiveSounds @ 0x00AABCA0.
// Layout: as 0x00AABCA0; CActiveSound's name (a CCharString) at +0x1c.
// Shape: the -1 lives in ebp for the compare and the store; both texts through one null-case-first accessor.
struct CSoundPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void Stop(long handle);         // slot 22 (+0x58)
};

struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundPlayer* GetPlayer();      // slot 6 (+0x18)
};

struct CSoundManagerLH {
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                // +0x40
};

struct CCharStringData { const char* m_pText; unsigned long m_length; };
struct CCharString {
    CCharStringData* m_pData;
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pText; }   // "" = 0x0129AAF4
};
extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A

struct CActiveSound {                       // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long          m_references;             // +0x04
    long          m_handle;                 // +0x08
    unsigned char _pad0c[0x10];
    CCharString   m_name;                   // +0x1c
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
    void RemoveInactiveSounds();            // 0x00AABCA0
    void StopSoundsContaining(const CCharString& text);
};

void CSoundEmitterLH::StopSoundsContaining(const CCharString& text)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_handle != -1 && strstr(sound->m_name.GetPString(), text.GetPString())) {
                m_pManager->m_pSystem->GetPlayer()->Stop(sound->m_handle);
                sound->m_handle = -1;
            }
        }
        RemoveInactiveSounds();
    }
}