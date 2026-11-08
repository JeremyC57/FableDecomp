// CEAXManager::ApplyListenerProperties(const EAXLISTENERPROPERTIES* properties) -- translate a set of EAX 3.0 listener properties into the software listener block (+0x08): zero the block's two vectors, copy each translated property clamped to its range (the air absorption converted to a whole mB level), hand the block to the listener state (+0x04) and refresh it; 0 when there are no properties or no listener state yet, else 1
// Leaf: ?ApplyListenerProperties@CEAXManager@@QAEHPBU_EAXLISTENERPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4E3C0, 875 bytes, thiscall `ret 4`.
// Identity (descriptive; libpk has no PDB / RTTI): the one caller (0x00C51919) is the manager's listener-property writer,
//   which passes the manager's own EAX 3.0 listener copy at +0x74 (it writes flHFReference +0xD4 / flLFReference +0xD8 of
//   that copy just before); +0x04 / +0x08 are the listener state and block of the landed CEAXManager::SetListenerProperty
//   0x00C52980 (callees SetBlock 0x00C40830 = the landed 27-word copy, Refresh 0x00C40D10). The source offsets and ranges are
//   the EAX 3.0 EAXLISTENERPROPERTIES members (lRoom +0x0C -10000..0, flDecayTime +0x18 0.1..20, lReflections +0x24
//   -10000..1000, lReverb +0x38 -10000..2000, flAirAbsorptionHF +0x5C -100..0, flRoomRolloffFactor +0x68 0..10); the block
//   defaults of the landed 0x00C40750 (Room / Reflections / Reverb -10000, decay 1.0 / 0.5, delays 0.02 / 0.04) are the
//   I3DL2 ones, so the block members are named after the EAX properties they receive. The catalog's
//   "CShaderRenderManager_SetupProjection" is propagated.
// Shape: a statement macro `if (in range) dst = value; else dst = value > max ? max : min;` per member (each branch stores;
//   the in-range float is copied as a word); the air absorption is one conditional expression with (long) on the in-range
//   value only (__ftol2 0x00BFEA70; 0 / -100 folded, one shared store); the zero vector is a local initialised before the
//   tests; the result word starts at 0 (`xor eax,eax` ahead of the tests, shared epilogue). Constants checked by relchk.py.
struct EAXVECTOR {
    float x, y, z;
};
typedef struct _EAXLISTENERPROPERTIES {
    unsigned long ulEnvironment;                        // +0x00
    float         flEnvironmentSize;                    // +0x04
    float         flEnvironmentDiffusion;               // +0x08
    long          lRoom;                                // +0x0c
    long          lRoomHF;                              // +0x10
    long          lRoomLF;                              // +0x14
    float         flDecayTime;                          // +0x18
    float         flDecayHFRatio;                       // +0x1c
    float         flDecayLFRatio;                       // +0x20
    long          lReflections;                         // +0x24
    float         flReflectionsDelay;                   // +0x28
    EAXVECTOR     vReflectionsPan;                      // +0x2c
    long          lReverb;                              // +0x38
    float         flReverbDelay;                        // +0x3c
    EAXVECTOR     vReverbPan;                           // +0x40
    float         flEchoTime;                           // +0x4c
    float         flEchoDepth;                          // +0x50
    float         flModulationTime;                     // +0x54
    float         flModulationDepth;                    // +0x58
    float         flAirAbsorptionHF;                    // +0x5c
    float         flHFReference;                        // +0x60
    float         flLFReference;                        // +0x64
    float         flRoomRolloffFactor;                  // +0x68
    unsigned long ulFlags;                              // +0x6c
} EAXLISTENERPROPERTIES;

struct SListenerBlock {                                 // 0x6c bytes (27 words, copied by SetBlock)
    EAXVECTOR m_vector0;                                // +0x00
    EAXVECTOR m_vector1;                                // +0x0c
    long      m_room;                                   // +0x18
    long      m_roomHF;                                 // +0x1c
    long      m_reserved20;                             // +0x20
    float     m_decayTime;                              // +0x24
    float     m_decayHFRatio;                           // +0x28
    float     m_reserved2c;                             // +0x2c
    long      m_reflections;                            // +0x30
    float     m_reflectionsDelay;                       // +0x34
    long      m_reverb;                                 // +0x38
    float     m_reverbDelay;                            // +0x3c
    float     m_environmentSize;                        // +0x40
    float     m_environmentDiffusion;                   // +0x44
    unsigned long m_reserved48[7];                      // +0x48
    float     m_roomRolloffFactor;                      // +0x64
    long      m_airAbsorption;                          // +0x68 mB
};
struct CListenerState {
    void SetBlock(const SListenerBlock* block);         // 0x00C40830
    void Refresh();                                     // 0x00C40D10
};

#define EAX_CLAMP_SET(dst, value, lo, hi) if ((value) <= (hi) && (value) >= (lo)) (dst) = (value); else (dst) = (value) > (hi) ? (hi) : (lo)

struct CEAXManager {
    unsigned char   _pad0[4];
    CListenerState* m_pListenerState;                   // +0x04
    SListenerBlock  m_listenerBlock;                    // +0x08
    int ApplyListenerProperties(const EAXLISTENERPROPERTIES* properties);
};

int CEAXManager::ApplyListenerProperties(const EAXLISTENERPROPERTIES* properties)
{
    EAXVECTOR zero = { 0.0f, 0.0f, 0.0f };
    int result = 0;
    if (properties && m_pListenerState) {
        m_listenerBlock.m_vector0 = zero;
        m_listenerBlock.m_vector1 = zero;
        EAX_CLAMP_SET(m_listenerBlock.m_room, properties->lRoom, -10000, 0);
        EAX_CLAMP_SET(m_listenerBlock.m_roomHF, properties->lRoomHF, -10000, 0);
        EAX_CLAMP_SET(m_listenerBlock.m_decayTime, properties->flDecayTime, 0.1f, 20.0f);
        EAX_CLAMP_SET(m_listenerBlock.m_decayHFRatio, properties->flDecayHFRatio, 0.1f, 2.0f);
        EAX_CLAMP_SET(m_listenerBlock.m_reflections, properties->lReflections, -10000, 1000);
        EAX_CLAMP_SET(m_listenerBlock.m_reflectionsDelay, properties->flReflectionsDelay, 0.0f, 0.3f);
        EAX_CLAMP_SET(m_listenerBlock.m_reverb, properties->lReverb, -10000, 2000);
        EAX_CLAMP_SET(m_listenerBlock.m_reverbDelay, properties->flReverbDelay, 0.0f, 0.1f);
        EAX_CLAMP_SET(m_listenerBlock.m_environmentSize, properties->flEnvironmentSize, 0.1f, 100.0f);
        EAX_CLAMP_SET(m_listenerBlock.m_environmentDiffusion, properties->flEnvironmentDiffusion, 0.0f, 1.0f);
        EAX_CLAMP_SET(m_listenerBlock.m_roomRolloffFactor, properties->flRoomRolloffFactor, 0.0f, 10.0f);
        m_listenerBlock.m_airAbsorption = properties->flAirAbsorptionHF <= 0.0f && properties->flAirAbsorptionHF >= -100.0f
                                          ? (long)properties->flAirAbsorptionHF
                                          : properties->flAirAbsorptionHF > 0.0f ? 0 : -100;
        m_pListenerState->SetBlock(&m_listenerBlock);
        m_pListenerState->Refresh();
        result = 1;
    }
    return result;
}