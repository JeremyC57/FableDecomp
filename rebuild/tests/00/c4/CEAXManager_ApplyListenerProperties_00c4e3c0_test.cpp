// Behaviour test for CEAXManager::ApplyListenerProperties (0x00C4E3C0): in-range EAX 3.0 values pass through, out-of-range
// ones clamp to the nearest bound, the air absorption becomes a truncated whole level in -100..0, the block's two vectors are
// zeroed, the listener state receives the block and is refreshed; 0 without properties or without a listener state.
#include <cstdio>
#include <cstring>
struct EAXVECTOR { float x, y, z; };
struct EAXLISTENERPROPERTIES {
    unsigned long ulEnvironment; float flEnvironmentSize, flEnvironmentDiffusion; long lRoom, lRoomHF, lRoomLF;
    float flDecayTime, flDecayHFRatio, flDecayLFRatio; long lReflections; float flReflectionsDelay; EAXVECTOR vReflectionsPan;
    long lReverb; float flReverbDelay; EAXVECTOR vReverbPan; float flEchoTime, flEchoDepth, flModulationTime, flModulationDepth;
    float flAirAbsorptionHF, flHFReference, flLFReference, flRoomRolloffFactor; unsigned long ulFlags;
};
struct SListenerBlock {
    EAXVECTOR m_vector0, m_vector1; long m_room, m_roomHF, m_reserved20; float m_decayTime, m_decayHFRatio, m_reserved2c;
    long m_reflections; float m_reflectionsDelay; long m_reverb; float m_reverbDelay, m_environmentSize, m_environmentDiffusion;
    unsigned long m_reserved48[7]; float m_roomRolloffFactor; long m_airAbsorption;
};
struct CListenerState {
    SListenerBlock m_block; int m_refreshed;
    void SetBlock(const SListenerBlock* block) { m_block = *block; }
    void Refresh() { ++m_refreshed; }
};
#define EAX_CLAMP_SET(dst, value, lo, hi) if ((value) <= (hi) && (value) >= (lo)) (dst) = (value); else (dst) = (value) > (hi) ? (hi) : (lo)
struct CEAXManager {
    CListenerState* m_pListenerState; SListenerBlock m_listenerBlock;
    int ApplyListenerProperties(const EAXLISTENERPROPERTIES* properties)
    {
        EAXVECTOR zero = { 0.0f, 0.0f, 0.0f };
        int result = 0;
        if (properties && m_pListenerState) {
            m_listenerBlock.m_vector0 = zero; m_listenerBlock.m_vector1 = zero;
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
                ? (long)properties->flAirAbsorptionHF : properties->flAirAbsorptionHF > 0.0f ? 0 : -100;
            m_pListenerState->SetBlock(&m_listenerBlock);
            m_pListenerState->Refresh();
            result = 1;
        }
        return result;
    }
};
int main()
{
    bool ok = true;
    CListenerState state; std::memset(&state, 0, sizeof state);
    CEAXManager m; std::memset(&m, 0, sizeof m); m.m_listenerBlock.m_vector0.x = 5.0f;
    EAXLISTENERPROPERTIES p; std::memset(&p, 0, sizeof p);
    ok = ok && m.ApplyListenerProperties(&p) == 0 && m.m_listenerBlock.m_vector0.x == 5.0f;   // no listener state
    m.m_pListenerState = &state;
    ok = ok && m.ApplyListenerProperties(0) == 0 && state.m_refreshed == 0;
    p.lRoom = -1000; p.lRoomHF = 50; p.flDecayTime = 30.0f; p.flDecayHFRatio = 0.05f; p.lReflections = -20000; p.flReflectionsDelay = 0.2f;
    p.lReverb = 2500; p.flReverbDelay = 0.05f; p.flEnvironmentSize = 7.5f; p.flEnvironmentDiffusion = 1.5f; p.flRoomRolloffFactor = -1.0f;
    p.flAirAbsorptionHF = -5.7f;
    ok = ok && m.ApplyListenerProperties(&p) == 1 && state.m_refreshed == 1;
    const SListenerBlock& b = state.m_block;
    ok = ok && b.m_vector0.x == 0.0f && b.m_room == -1000 && b.m_roomHF == 0 && b.m_decayTime == 20.0f && b.m_decayHFRatio == 0.1f;
    ok = ok && b.m_reflections == -10000 && b.m_reflectionsDelay == 0.2f && b.m_reverb == 2000 && b.m_reverbDelay == 0.05f;
    ok = ok && b.m_environmentSize == 7.5f && b.m_environmentDiffusion == 1.0f && b.m_roomRolloffFactor == 0.0f && b.m_airAbsorption == -5;
    p.flAirAbsorptionHF = 3.0f; m.ApplyListenerProperties(&p); ok = ok && state.m_block.m_airAbsorption == 0;
    p.flAirAbsorptionHF = -300.0f; m.ApplyListenerProperties(&p); ok = ok && state.m_block.m_airAbsorption == -100;
    if (!ok) { std::printf("00c4e3c0_TEST FAIL\n"); return 1; }
    std::printf("00c4e3c0_TEST PASS\n");
    return 0;
}