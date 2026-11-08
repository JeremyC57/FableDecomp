// Behaviour test for 0x00C51A20 CEAXManager::QueueCommit: every node of the change list is forwarded with the right
// property set, id, voice, field address and size; an environment-size change is bracketed by flags 0 / the cached flags.
#include <cstdio>
#include <cstring>

struct GUID { unsigned int a, b, c, d; };
static const GUID DSPROPSETID_EAX30_ListenerProperties = { 0xa8fa6882u, 0, 0, 0 };
static const GUID DSPROPSETID_EAX30_BufferProperties = { 0xa8fa6881u, 0, 0, 0 };
struct EAXVECTOR { float x, y, z; };
struct EAX30LISTENERPROPERTIES {
    unsigned int ulEnvironment; float flEnvironmentSize; float flEnvironmentDiffusion; int lRoom; int lRoomHF; int lRoomLF;
    float flDecayTime; float flDecayHFRatio; float flDecayLFRatio; int lReflections; float flReflectionsDelay;
    EAXVECTOR vReflectionsPan; int lReverb; float flReverbDelay; EAXVECTOR vReverbPan; float flEchoTime; float flEchoDepth;
    float flModulationTime; float flModulationDepth; float flAirAbsorptionHF; float flHFReference; float flLFReference;
    float flRoomRolloffFactor; unsigned int ulFlags;
};
struct EAX30BUFFERPROPERTIES {
    int lDirect; int lDirectHF; int lRoom; int lRoomHF; int lObstruction; float flObstructionLFRatio; int lOcclusion;
    float flOcclusionLFRatio; float flOcclusionRoomRatio; float flOcclusionDirectRatio; int lExclusion; float flExclusionLFRatio;
    int lOutsideVolumeHF; float flDopplerFactor; float flRolloffFactor; float flRoomRolloffFactor; float flAirAbsorptionFactor;
    unsigned int ulFlags;
};
struct BUFFERPROPERTIES { unsigned char head[0x44]; EAX30BUFFERPROPERTIES eax30BP; };
enum IDENTIFIER { EAX3LISTENER = 0, EAX3BUFFER = 1, LISTENERUPDATE = 4, BUFFERUPDATE = 5 };
struct _CHANGES {
    IDENTIFIER id; int property;
    union { EAX30LISTENERPROPERTIES eax30LP; BUFFERPROPERTIES Buffer; };
    _CHANGES* next;
};
class CListenerState;
struct CEAXVoice { int dummy; };
struct Call { const GUID* set; unsigned int id; CEAXVoice* voice; void* data; unsigned int size; unsigned int value; };
static Call g_calls[64]; static int g_n = 0;
struct CEAXManager {
    CListenerState* m_pListenerState;
    EAX30LISTENERPROPERTIES m_listenerProperties;
    long SetEAXProperty(const GUID* set, unsigned int id, CEAXVoice* voice, void* data, unsigned int size)
    {
        Call c = { set, id, voice, data, size, *(unsigned int*)data }; g_calls[g_n++] = c; return 0;
    }
    void QueueCommit(_CHANGES* changes, CEAXVoice* voice);
};

void CEAXManager::QueueCommit(_CHANGES* changes, CEAXVoice* voice)
{
    unsigned long savedFlags = 0;
    unsigned long noFlags = 0;
    for (; changes; changes = changes->next)
    {
        switch (changes->id)
        {
        case EAX3BUFFER:
        {
            switch (changes->property)
            {
            case 1:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 1, voice, &changes->Buffer.eax30BP, sizeof(EAX30BUFFERPROPERTIES)); break;
            case 2:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 2, voice, &changes->Buffer.eax30BP.lObstruction, 8); break;
            case 3:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 3, voice, &changes->Buffer.eax30BP.lOcclusion, 16); break;
            case 4:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 4, voice, &changes->Buffer.eax30BP.lExclusion, 8); break;
            case 5:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 5, voice, &changes->Buffer.eax30BP.lDirect, 4); break;
            case 6:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 6, voice, &changes->Buffer.eax30BP.lDirectHF, 4); break;
            case 7:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 7, voice, &changes->Buffer.eax30BP.lRoom, 4); break;
            case 8:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 8, voice, &changes->Buffer.eax30BP.lRoomHF, 4); break;
            case 9:  SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 9, voice, &changes->Buffer.eax30BP.lObstruction, 4); break;
            case 10: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 10, voice, &changes->Buffer.eax30BP.flObstructionLFRatio, 4); break;
            case 11: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 11, voice, &changes->Buffer.eax30BP.lOcclusion, 4); break;
            case 12: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 12, voice, &changes->Buffer.eax30BP.flOcclusionLFRatio, 4); break;
            case 13: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 13, voice, &changes->Buffer.eax30BP.flOcclusionRoomRatio, 4); break;
            case 14: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 14, voice, &changes->Buffer.eax30BP.flOcclusionDirectRatio, 4); break;
            case 15: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 15, voice, &changes->Buffer.eax30BP.lExclusion, 4); break;
            case 16: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 16, voice, &changes->Buffer.eax30BP.flExclusionLFRatio, 4); break;
            case 17: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 17, voice, &changes->Buffer.eax30BP.lOutsideVolumeHF, 4); break;
            case 18: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 18, voice, &changes->Buffer.eax30BP.flDopplerFactor, 4); break;
            case 19: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 19, voice, &changes->Buffer.eax30BP.flRolloffFactor, 4); break;
            case 20: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 20, voice, &changes->Buffer.eax30BP.flRoomRolloffFactor, 4); break;
            case 21: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 21, voice, &changes->Buffer.eax30BP.flAirAbsorptionFactor, 4); break;
            case 22: SetEAXProperty(&DSPROPSETID_EAX30_BufferProperties, 22, voice, &changes->Buffer.eax30BP.ulFlags, 4); break;
            }
            break;
        }
        case EAX3LISTENER:
        {
            switch (changes->property)
            {
            case 1:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 1, 0, &changes->eax30LP, sizeof(EAX30LISTENERPROPERTIES)); break;
            case 2:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 2, 0, &changes->eax30LP.ulEnvironment, 4); break;
            case 3:
                if (m_pListenerState)
                    savedFlags = m_listenerProperties.ulFlags;
                SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 25, 0, &noFlags, 4);
                SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 3, 0, &changes->eax30LP.flEnvironmentSize, 4);
                SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 25, 0, &savedFlags, 4);
                break;
            case 4:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 4, 0, &changes->eax30LP.flEnvironmentDiffusion, 4); break;
            case 5:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 5, 0, &changes->eax30LP.lRoom, 4); break;
            case 6:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 6, 0, &changes->eax30LP.lRoomHF, 4); break;
            case 7:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 7, 0, &changes->eax30LP.lRoomLF, 4); break;
            case 8:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 8, 0, &changes->eax30LP.flDecayTime, 4); break;
            case 9:  SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 9, 0, &changes->eax30LP.flDecayHFRatio, 4); break;
            case 10: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 10, 0, &changes->eax30LP.flDecayLFRatio, 4); break;
            case 11: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 11, 0, &changes->eax30LP.lReflections, 4); break;
            case 12: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 12, 0, &changes->eax30LP.flReflectionsDelay, 4); break;
            case 13: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 13, 0, &changes->eax30LP.vReflectionsPan, sizeof(EAXVECTOR)); break;
            case 14: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 14, 0, &changes->eax30LP.lReverb, 4); break;
            case 15: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 15, 0, &changes->eax30LP.flReverbDelay, 4); break;
            case 16: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 16, 0, &changes->eax30LP.vReverbPan, sizeof(EAXVECTOR)); break;
            case 17: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 17, 0, &changes->eax30LP.flEchoTime, 4); break;
            case 18: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 18, 0, &changes->eax30LP.flEchoDepth, 4); break;
            case 19: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 19, 0, &changes->eax30LP.flModulationTime, 4); break;
            case 20: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 20, 0, &changes->eax30LP.flModulationDepth, 4); break;
            case 21: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 21, 0, &changes->eax30LP.flAirAbsorptionHF, 4); break;
            case 22: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 22, 0, &changes->eax30LP.flHFReference, 4); break;
            case 23: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 23, 0, &changes->eax30LP.flLFReference, 4); break;
            case 24: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 24, 0, &changes->eax30LP.flRoomRolloffFactor, 4); break;
            case 25: SetEAXProperty(&DSPROPSETID_EAX30_ListenerProperties, 25, 0, &changes->eax30LP.ulFlags, 4); break;
            }
            break;
        }
        }
    }
}

static int fail(int n) { std::printf("00c51a20_TEST FAIL %d\n", n); return 1; }

int main()
{
    CEAXManager m; std::memset(&m, 0, sizeof(m));
    CEAXVoice v;
    static const unsigned int bufOff[23] = { 0, 0, 16, 24, 40, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68 };
    static const unsigned int bufSize[23] = { 0, 0x48, 8, 16, 8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
    _CHANGES n[23];
    for (int p = 1; p <= 22; ++p) { n[p].id = EAX3BUFFER; n[p].property = p; n[p].next = p < 22 ? &n[p + 1] : 0; }
    m.QueueCommit(&n[1], &v);
    if (g_n != 22) return fail(1);
    for (int p = 1; p <= 22; ++p)
    {
        Call& c = g_calls[p - 1];
        if (c.set != &DSPROPSETID_EAX30_BufferProperties || c.id != (unsigned)p || c.voice != &v || c.size != bufSize[p]
            || (char*)c.data != (char*)&n[p].Buffer.eax30BP + bufOff[p]) return fail(2);
    }
    static const unsigned int lisOff[26] = { 0, 0, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 56, 60, 64, 76, 80, 84, 88, 92, 96, 100, 104, 108 };
    _CHANGES l[26];
    for (int p = 1; p <= 25; ++p) { std::memset(&l[p], 0, sizeof(l[p])); l[p].id = EAX3LISTENER; l[p].property = p; l[p].next = p < 25 ? &l[p + 1] : 0; }
    l[3].eax30LP.flEnvironmentSize = 7.5f;
    g_n = 0;
    m.m_pListenerState = (CListenerState*)&m; m.m_listenerProperties.ulFlags = 0x3f;
    m.QueueCommit(&l[1], &v);
    if (g_n != 27) return fail(3);
    int k = 0;
    for (int p = 1; p <= 25; ++p)
    {
        if (p == 3)
        {
            if (g_calls[k].id != 25 || g_calls[k].value != 0 || g_calls[k].size != 4) return fail(4);
            if (g_calls[k + 1].id != 3 || g_calls[k + 1].data != &l[3].eax30LP.flEnvironmentSize) return fail(5);
            if (g_calls[k + 2].id != 25 || g_calls[k + 2].value != 0x3f || g_calls[k + 2].voice != 0) return fail(6);
            k += 3; continue;
        }
        Call& c = g_calls[k++];
        unsigned int size = p == 1 ? 0x70 : (p == 13 || p == 16) ? 12 : 4;
        if (c.set != &DSPROPSETID_EAX30_ListenerProperties || c.id != (unsigned)p || c.voice != 0 || c.size != size
            || (char*)c.data != (char*)&l[p].eax30LP + lisOff[p]) return fail(7);
    }
    _CHANGES other; other.id = LISTENERUPDATE; other.property = 2; other.next = 0;
    _CHANGES bad; bad.id = EAX3BUFFER; bad.property = 23; bad.next = &other;
    g_n = 0; m.QueueCommit(&bad, &v); m.QueueCommit(0, &v);
    if (g_n != 0) return fail(8);
    std::printf("00c51a20_TEST PASS\n");
    return 0;
}