// CSoundEmitterLH::vfunc_15(const C3DVector& position) -- move the emitter: when the sound system is up and the emitter is active, store the new position and pass a copy of it to the player for every active sound that is playing and not stopping
// Leaf: ?vfunc_15@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AABBB0, 146 bytes (+ int3 padding), thiscall `ret 4`. Vtable 0x0129E6EC
//   (CSoundEmitterLH) slot 15 (rtti-vtables.tsv); no other reference.
// Identity: the audio layer is not in Ego_r.pdb, so the method keeps the slot name as its landed siblings do (vfunc_11
//   0x00AABA80, vfunc_13 0x00AABB60); it is the emitter's SetPosition. The catalog's "Particle_Or_Emitter_SetPosition" is
//   descriptive.
// Callees: the sound system's slot 6 (+0x18, GetPlayer), the player's slot 27 (+0x6c: handle, position).
// Layout: as the landed siblings -- +0x04 the owning manager (its +0x40 is the sound system), +0x08 the C3DVector position,
//   +0x14 the VC6 list sentinel of counted active sounds (the node's +8 is the CActiveSound*), +0x18 the active flag;
//   CActiveSound m_handle +0x08, m_playing +0x14, m_stopping +0x15.
// Shape: the two guards (system, active flag) share the early exit; the 1-aligned C3DVector member assigned through a
//   pointer kept in ebp; per sound a named C3DVector built component-wise from the member (X through ebp, Y/Z through `this`) in the frame
//   before GetPlayer(), then the
//   slot-27 call with the sound's handle and the copy's address; the list sentinel re-read for every loop test.
#pragma pack(push, 1)
class C3DVector {                                       // 1-aligned in retail (GOTCHAS: CProng::Prong sits at +1)
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(pop)

class CSoundPlayer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual void SetPosition(long handle, const C3DVector& position);   // slot 27 (+0x6c)
};

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundPlayer* GetPlayer();                  // slot 6 (+0x18)
};

class CSoundManagerLH {
public:
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                            // +0x40
};

class CSoundEmitterLH {                                 // vtable 0x0129E6EC
public:
    class CActiveSound {                                // vtable 0x0129E67C
    public:
        virtual void v00();
        long  m_state;                                  // +0x04
        long  m_handle;                                 // +0x08
        long  m_elapsed;                                // +0x0c
        float m_volume;                                 // +0x10
        bool  m_playing;                                // +0x14
        bool  m_stopping;                               // +0x15
    };
    struct SSoundNode {                                 // VC6 list node of CCountedPointer<CActiveSound>
        SSoundNode*   next;                             // +0x00
        SSoundNode*   prev;                             // +0x04
        CActiveSound* sound;                            // +0x08 (the counted pointer's object)
        void*         count;                            // +0x0c
    };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void vfunc_15(const C3DVector& position);   // slot 15
    CSoundManagerLH* m_pManager;                        // +0x04
    C3DVector        m_position;                        // +0x08
    SSoundNode*      m_pSounds;                         // +0x14 list sentinel
    bool             m_active;                          // +0x18
};

void CSoundEmitterLH::vfunc_15(const C3DVector& position)
{
    if (m_pManager->m_pSystem && m_active) {
        m_position = position;
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_playing && !sound->m_stopping) {
                C3DVector soundPosition(m_position.X, m_position.Y, m_position.Z);
                m_pManager->m_pSystem->GetPlayer()->SetPosition(sound->m_handle, soundPosition);
            }
        }
    }
}