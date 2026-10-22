// CSoundEmitterLH::vfunc_7(long a, long b) -- when the sound system is up, forwards both arguments to the sound player's slot 25
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AABA40, 25 bytes, `ret 8`. Vtable 0x0129E6EC (CSoundEmitterLH, rtti-vtables.tsv) slot 7.
// The audio layer is not in Ego_r.pdb: method and member names are descriptive.
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 25 (+0x64) as a sibling-call `jmp` (arguments forwarded in place).
// Shape: the system read into a named local (a re-read member gives `mov ecx,eax ; mov ecx,[ecx+40h]`).
// Layout as the landed 0x00AABC50 / 0x00AAC340: +0x04 the owning manager (system at +0x40).
struct CSoundPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void Update(long handle);                  // slot 23 (+0x5c)
    virtual void v24();
    virtual void SetPairB(long a, long b);             // slot 25 (+0x64)
    virtual void SetPairA(long a, long b);             // slot 26 (+0x68)
    virtual void v27(); virtual void v28();
    virtual long GetState(long handle);                // slot 29 (+0x74)
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
};

struct SSoundNode {                                    // VC6 list node, 16 bytes
    SSoundNode*   next;
    SSoundNode*   prev;
    CActiveSound* sound;                               // +0x08
    long          extra;                               // +0x0c
};

struct C3DVector { float X, Y, Z; };

struct CSoundEmitterLH {                               // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH* m_pManager;                       // +0x04: the owning sound manager (its +0x40 is the sound system)
    C3DVector        m_position;                       // +0x08
    SSoundNode*      m_pSounds;                        // +0x14: list sentinel
    bool             m_active;                         // +0x18
    bool             m_paused;                         // +0x19
    bool             m_finished;                       // +0x1a
    void vfunc_7(long a, long b);
};

void CSoundEmitterLH::vfunc_7(long a, long b)
{
    CSoundSystem* system = m_pManager->m_pSystem;
    if (system)
        system->GetPlayer()->SetPairB(a, b);
}
