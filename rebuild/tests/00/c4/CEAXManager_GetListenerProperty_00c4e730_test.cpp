// Behaviour test for 0x00C4E730 CEAXManager::GetListenerProperty: every EAX 3.0 listener id reads its field from the
// cached block, the size / null / missing-state checks return -9, id 0 returns 0 without writing.
#include <cstdio>
#include <cstring>

struct EAXVECTOR { float x, y, z; };
// 32-bit field widths (int, not long) so the g++ x64 build keeps the retail 0x70-byte layout
struct EAX30LISTENERPROPERTIES {
    unsigned int ulEnvironment; float flEnvironmentSize; float flEnvironmentDiffusion; int lRoom; int lRoomHF; int lRoomLF;
    float flDecayTime; float flDecayHFRatio; float flDecayLFRatio; int lReflections; float flReflectionsDelay;
    EAXVECTOR vReflectionsPan; int lReverb; float flReverbDelay; EAXVECTOR vReverbPan; float flEchoTime; float flEchoDepth;
    float flModulationTime; float flModulationDepth; float flAirAbsorptionHF; float flHFReference; float flLFReference;
    float flRoomRolloffFactor; unsigned int ulFlags;
};
class CListenerState;
struct CEAXManager {
    CListenerState*         m_pListenerState;
    EAX30LISTENERPROPERTIES m_listenerProperties;
    long GetListenerProperty(unsigned long id, void* data, unsigned long size);
};

long CEAXManager::GetListenerProperty(unsigned long id, void* data, unsigned long size)
{
    long result = -9;
    if (data && m_pListenerState)
    {
    switch (id)
    {
    case 0:  result = 0; break;
    case 1:  if (size >= sizeof(EAX30LISTENERPROPERTIES)) { *(EAX30LISTENERPROPERTIES*)data = m_listenerProperties; result = 0; } break;
    case 2:  if (size >= 4) { *(unsigned int*)data = m_listenerProperties.ulEnvironment; result = 0; } break;
    case 3:  if (size >= 4) { *(float*)data = m_listenerProperties.flEnvironmentSize; result = 0; } break;
    case 4:  if (size >= 4) { *(float*)data = m_listenerProperties.flEnvironmentDiffusion; result = 0; } break;
    case 5:  if (size >= 4) { *(int*)data = m_listenerProperties.lRoom; result = 0; } break;
    case 6:  if (size >= 4) { *(int*)data = m_listenerProperties.lRoomHF; result = 0; } break;
    case 7:  if (size >= 4) { *(int*)data = m_listenerProperties.lRoomLF; result = 0; } break;
    case 8:  if (size >= 4) { *(float*)data = m_listenerProperties.flDecayTime; result = 0; } break;
    case 9:  if (size >= 4) { *(float*)data = m_listenerProperties.flDecayHFRatio; result = 0; } break;
    case 10: if (size >= 4) { *(float*)data = m_listenerProperties.flDecayLFRatio; result = 0; } break;
    case 11: if (size >= 4) { *(int*)data = m_listenerProperties.lReflections; result = 0; } break;
    case 12: if (size >= 4) { *(float*)data = m_listenerProperties.flReflectionsDelay; result = 0; } break;
    case 13: if (size >= sizeof(EAXVECTOR)) { *(EAXVECTOR*)data = m_listenerProperties.vReflectionsPan; result = 0; } break;
    case 14: if (size >= 4) { *(int*)data = m_listenerProperties.lReverb; result = 0; } break;
    case 15: if (size >= 4) { *(float*)data = m_listenerProperties.flReverbDelay; result = 0; } break;
    case 16: if (size >= sizeof(EAXVECTOR)) { *(EAXVECTOR*)data = m_listenerProperties.vReverbPan; result = 0; } break;
    case 17: if (size >= 4) { *(float*)data = m_listenerProperties.flEchoTime; result = 0; } break;
    case 18: if (size >= 4) { *(float*)data = m_listenerProperties.flEchoDepth; result = 0; } break;
    case 19: if (size >= 4) { *(float*)data = m_listenerProperties.flModulationTime; result = 0; } break;
    case 20: if (size >= 4) { *(float*)data = m_listenerProperties.flModulationDepth; result = 0; } break;
    case 21: if (size >= 4) { *(float*)data = m_listenerProperties.flAirAbsorptionHF; result = 0; } break;
    case 22: if (size >= 4) { *(float*)data = m_listenerProperties.flHFReference; result = 0; } break;
    case 23: if (size >= 4) { *(float*)data = m_listenerProperties.flLFReference; result = 0; } break;
    case 24: if (size >= 4) { *(float*)data = m_listenerProperties.flRoomRolloffFactor; result = 0; } break;
    case 25: if (size >= 4) { *(unsigned int*)data = m_listenerProperties.ulFlags; result = 0; } break;
    }
    }
    return result;
}

static int fail() { std::printf("00c4e730_TEST FAIL\n"); return 1; }

int main()
{
    CEAXManager m;
    unsigned int* words = (unsigned int*)&m.m_listenerProperties;
    for (int i = 0; i < 28; ++i) words[i] = 0x100u + (unsigned int)i;
    m.m_pListenerState = (CListenerState*)&m;
    // each scalar id 2..25 maps to its word index in the block (pans take 3 words)
    static const int wordOf[26] = { -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25, 26, 27 };
    for (int id = 2; id <= 25; ++id)
    {
        unsigned int out[3] = { 0, 0, 0 };
        unsigned long need = (id == 13 || id == 16) ? 12 : 4;
        if (m.GetListenerProperty(id, out, need) != 0 || out[0] != words[wordOf[id]]) return fail();
        if (need == 12 && (out[1] != words[wordOf[id] + 1] || out[2] != words[wordOf[id] + 2])) return fail();
        unsigned int untouched[3] = { 7, 7, 7 };
        if (m.GetListenerProperty(id, untouched, need - 1) != -9 || untouched[0] != 7) return fail();
    }
    EAX30LISTENERPROPERTIES all;
    std::memset(&all, 0, sizeof(all));
    if (m.GetListenerProperty(1, &all, sizeof(all)) != 0 || std::memcmp(&all, &m.m_listenerProperties, sizeof(all)) != 0) return fail();
    if (m.GetListenerProperty(1, &all, sizeof(all) - 1) != -9) return fail();
    unsigned int w = 5;
    if (m.GetListenerProperty(0, &w, 0) != 0 || w != 5) return fail();
    if (m.GetListenerProperty(26, &w, 4) != -9) return fail();
    if (m.GetListenerProperty(2, 0, 4) != -9) return fail();
    m.m_pListenerState = 0;
    if (m.GetListenerProperty(2, &w, 4) != -9 || w != 5) return fail();
    std::printf("00c4e730_TEST PASS\n");
    return 0;
}