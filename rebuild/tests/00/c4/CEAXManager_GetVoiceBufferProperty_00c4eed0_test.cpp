// Behaviour test for 0x00C4EED0 CEAXManager::GetVoiceBufferProperty: each EAX 3.0 buffer id / parameter group reads the
// voice's cached block; null data, a bad version, an unknown id or a short buffer give -3 and write nothing.
#include <cstdio>
#include <cstring>

// 32-bit field widths (int, not long) so the g++ x64 build keeps the retail 0x48-byte layout
struct EAX30BUFFERPROPERTIES {
    int lDirect; int lDirectHF; int lRoom; int lRoomHF; int lObstruction; float flObstructionLFRatio; int lOcclusion;
    float flOcclusionLFRatio; float flOcclusionRoomRatio; float flOcclusionDirectRatio; int lExclusion; float flExclusionLFRatio;
    int lOutsideVolumeHF; float flDopplerFactor; float flRolloffFactor; float flRoomRolloffFactor; float flAirAbsorptionFactor;
    unsigned int ulFlags;
};
struct EAXOBSTRUCTIONPROPERTIES { int lObstruction; float flObstructionLFRatio; };
struct EAXOCCLUSIONPROPERTIES { int lOcclusion; float flOcclusionLFRatio; float flOcclusionRoomRatio; float flOcclusionDirectRatio; };
struct EAXEXCLUSIONPROPERTIES { int lExclusion; float flExclusionLFRatio; };
struct CEAXVoice { EAX30BUFFERPROPERTIES m_eax3BufferProperties; };
struct CEAXManager {
    long GetVoiceBufferProperty(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size);
};

long CEAXManager::GetVoiceBufferProperty(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size)
{
    long result = -3;
    if (data && (version == 2 || version == 4))
    {
    switch (id)
    {
    case 0:  result = 0; break;
    case 1:  if (size >= sizeof(EAX30BUFFERPROPERTIES)) { *(EAX30BUFFERPROPERTIES*)data = voice->m_eax3BufferProperties; result = 0; } break;
    case 2:  if (size >= sizeof(EAXOBSTRUCTIONPROPERTIES)) {
                 EAXOBSTRUCTIONPROPERTIES* p = (EAXOBSTRUCTIONPROPERTIES*)data;
                 p->flObstructionLFRatio = voice->m_eax3BufferProperties.flObstructionLFRatio;
                 p->lObstruction = voice->m_eax3BufferProperties.lObstruction;
                 result = 0; } break;
    case 3:  if (size >= sizeof(EAXOCCLUSIONPROPERTIES)) {
                 EAXOCCLUSIONPROPERTIES* p = (EAXOCCLUSIONPROPERTIES*)data;
                 p->flOcclusionDirectRatio = voice->m_eax3BufferProperties.flOcclusionDirectRatio;
                 p->flOcclusionLFRatio = voice->m_eax3BufferProperties.flOcclusionLFRatio;
                 p->flOcclusionRoomRatio = voice->m_eax3BufferProperties.flOcclusionRoomRatio;
                 p->lOcclusion = voice->m_eax3BufferProperties.lOcclusion;
                 result = 0; } break;
    case 4:  if (size >= sizeof(EAXEXCLUSIONPROPERTIES)) {
                 EAXEXCLUSIONPROPERTIES* p = (EAXEXCLUSIONPROPERTIES*)data;
                 p->flExclusionLFRatio = voice->m_eax3BufferProperties.flExclusionLFRatio;
                 p->lExclusion = voice->m_eax3BufferProperties.lExclusion;
                 result = 0; } break;
    case 5:  if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lDirect; result = 0; } break;
    case 6:  if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lDirectHF; result = 0; } break;
    case 7:  if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lRoom; result = 0; } break;
    case 8:  if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lRoomHF; result = 0; } break;
    case 9:  if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lObstruction; result = 0; } break;
    case 10: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flObstructionLFRatio; result = 0; } break;
    case 11: if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lOcclusion; result = 0; } break;
    case 12: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionLFRatio; result = 0; } break;
    case 13: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionRoomRatio; result = 0; } break;
    case 14: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionDirectRatio; result = 0; } break;
    case 15: if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lExclusion; result = 0; } break;
    case 16: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flExclusionLFRatio; result = 0; } break;
    case 17: if (size >= 4) { *(int*)data = voice->m_eax3BufferProperties.lOutsideVolumeHF; result = 0; } break;
    case 18: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flDopplerFactor; result = 0; } break;
    case 19: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flRolloffFactor; result = 0; } break;
    case 20: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flRoomRolloffFactor; result = 0; } break;
    case 21: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flAirAbsorptionFactor; result = 0; } break;
    case 22: if (size >= 4) { *(unsigned int*)data = voice->m_eax3BufferProperties.ulFlags; result = 0; } break;
    }
    }
    return result;
}

static int fail() { std::printf("00c4eed0_TEST FAIL\n"); return 1; }

int main()
{
    CEAXVoice v; CEAXManager m;
    unsigned int* w = (unsigned int*)&v.m_eax3BufferProperties;
    for (int i = 0; i < 18; ++i) w[i] = 0x200u + (unsigned int)i;
    for (unsigned long version = 2; version <= 4; version += 2)
    {
        for (int id = 5; id <= 22; ++id)
        {
            unsigned int out = 0;
            if (m.GetVoiceBufferProperty(&v, version, id, &out, 4) != 0 || out != w[id - 5]) return fail();
            out = 9;
            if (m.GetVoiceBufferProperty(&v, version, id, &out, 3) != -3 || out != 9) return fail();
        }
        unsigned int g[4] = { 0, 0, 0, 0 };
        if (m.GetVoiceBufferProperty(&v, version, 2, g, 8) != 0 || g[0] != w[4] || g[1] != w[5]) return fail();
        if (m.GetVoiceBufferProperty(&v, version, 3, g, 16) != 0 || g[0] != w[6] || g[1] != w[7] || g[2] != w[8] || g[3] != w[9]) return fail();
        if (m.GetVoiceBufferProperty(&v, version, 4, g, 8) != 0 || g[0] != w[10] || g[1] != w[11]) return fail();
        if (m.GetVoiceBufferProperty(&v, version, 3, g, 15) != -3) return fail();
        EAX30BUFFERPROPERTIES all; std::memset(&all, 0, sizeof(all));
        if (m.GetVoiceBufferProperty(&v, version, 1, &all, sizeof(all)) != 0 || std::memcmp(&all, w, sizeof(all)) != 0) return fail();
        if (m.GetVoiceBufferProperty(&v, version, 0, g, 0) != 0) return fail();
        if (m.GetVoiceBufferProperty(&v, version, 23, g, 4) != -3) return fail();
    }
    unsigned int out = 1;
    if (m.GetVoiceBufferProperty(&v, 3, 5, &out, 4) != -3 || out != 1) return fail();
    if (m.GetVoiceBufferProperty(&v, 1, 5, &out, 4) != -3 || out != 1) return fail();
    if (m.GetVoiceBufferProperty(&v, 2, 5, 0, 4) != -3) return fail();
    std::printf("00c4eed0_TEST PASS\n");
    return 0;
}