// CEAXManager::SetVoiceSourceProperty(unsigned long id, CEAXVoice* voice, void* data, unsigned long size, unsigned long reserved) -- EAX 4.0 source-property setter: store the new value(s) into the voice's cached EAXSOURCEPROPERTIES / send / active-slot blocks, and when anything changed forward the property to the voice's effect through SetVoiceProperty 0x00C51F00
// Residue: DIFFER 2388v2288 (+100 bytes): retail hoists `data` into edi before the jump table (`mov edi,[esp+28h]` ahead of `ja`) and each simple case does `mov edi,[edi] ; cmp [ebx+N],edi ; setne dl ; mov [ebx+N],edi`; VC7.1 gives the data parameter (c2prio candidate #2, lines 34..240) priority -179 with allowed set .....bp (live across the rep cmps/movs blocks and the calls) and splits it per case into eax/ecx pieces reloaded from the argument slot (+4..6 bytes per case), so `changed` moves to ebx and voice loses its ebx. Probe: 30 grades -- a 12-point structural sweep (source as pointer / no alias / declared late, changed declared late / initialised false, ternary / if-not tail, const void* data, a named property local: all but two compile to the identical 414-line IL, the other two are worse), data copies (h/i/j: forwarded back to the identical IL), value-first / store-first case bodies (m/n), changed as bool/long (o/p), no-cases-2-4 and tail-less experiments (x1/x3).
// Next: give the split piece at the dispatch a reason to exist -- e.g. read `*(const long*)data` once before the switch for the simple cases (`long value = *(const long*)data;` hoisted, cases use it) or move the ALLPARAMETERS/send/slot cases (the rep users) behind a separate `if` so data is not live across the string ops in the switch.
// Leaf: ?SetVoiceSourceProperty@CEAXManager@@QAEJKPAUCEAXVoice@@PAXKK@Z
#include "engine/CEAXBuffer.h"

extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

struct CEAXVoice {
    SOURCEBP      m_Source;                             // +0x000 .. +0x1ef
    CEAXBuffer*   m_pEffect;                            // +0x1f0
};

struct CEAXManager {
    void QueueCommit(_CHANGES* changes, CEAXVoice* voice);                                  // 0x00C51A20
    long SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice);   // 0x00C51F00
    long SetVoiceSourceProperty(unsigned long id, CEAXVoice* voice, void* data, unsigned long size, unsigned long reserved);
};

long CEAXManager::SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice)
{
    long result = -4;
    if (voice) {
        _CHANGES* changes;
        result = voice->m_pEffect->SetSourceProperty_00c68510(id, data, size, &changes);
        if (!result)
            QueueCommit(changes, voice);
    }
    return result;
}

long CEAXManager::SetVoiceSourceProperty(unsigned long id, CEAXVoice* voice, void* data, unsigned long size, unsigned long reserved)
{
    EAXSOURCEPROPERTIES& source = voice->m_Source.eax4BP;
    unsigned char changed = true;
    switch (id & 0x7fffffff)
    {
    case EAXSOURCE_NONE:
        changed = true;
        break;
    case EAXSOURCE_ALLPARAMETERS:
        changed = memcmp(&voice->m_Source.eax4BP, data, sizeof(EAXSOURCEPROPERTIES));
        voice->m_Source.eax4BP = *(const EAXSOURCEPROPERTIES*)data;
        if (!changed)
            return 0;
        return SetVoiceProperty(id, data, size, voice);
    case EAXSOURCE_OBSTRUCTIONPARAMETERS:
        changed = memcmp(&source.lObstruction, data, sizeof(EAXOBSTRUCTIONPROPERTIES));
        *(EAXOBSTRUCTIONPROPERTIES*)&source.lObstruction = *(const EAXOBSTRUCTIONPROPERTIES*)data;
        break;
    case EAXSOURCE_OCCLUSIONPARAMETERS:
        changed = memcmp(&source.lOcclusion, data, sizeof(EAXOCCLUSIONPROPERTIES));
        *(EAXOCCLUSIONPROPERTIES*)&source.lOcclusion = *(const EAXOCCLUSIONPROPERTIES*)data;
        break;
    case EAXSOURCE_EXCLUSIONPARAMETERS:
        changed = memcmp(&source.lExclusion, data, sizeof(EAXEXCLUSIONPROPERTIES));
        *(EAXEXCLUSIONPROPERTIES*)&source.lExclusion = *(const EAXEXCLUSIONPROPERTIES*)data;
        break;
    case EAXSOURCE_DIRECT:
        changed = source.lDirect != *(const long*)data;
        source.lDirect = *(const long*)data;
        break;
    case EAXSOURCE_DIRECTHF:
        changed = source.lDirectHF != *(const long*)data;
        source.lDirectHF = *(const long*)data;
        break;
    case EAXSOURCE_ROOM:
        changed = source.lRoom != *(const long*)data;
        source.lRoom = *(const long*)data;
        break;
    case EAXSOURCE_ROOMHF:
        changed = source.lRoomHF != *(const long*)data;
        source.lRoomHF = *(const long*)data;
        break;
    case EAXSOURCE_OBSTRUCTION:
        changed = source.lObstruction != *(const long*)data;
        source.lObstruction = *(const long*)data;
        break;
    case EAXSOURCE_OBSTRUCTIONLFRATIO:
        changed = source.flObstructionLFRatio != *(const float*)data;
        source.flObstructionLFRatio = *(const float*)data;
        break;
    case EAXSOURCE_OCCLUSION:
        changed = source.lOcclusion != *(const long*)data;
        source.lOcclusion = *(const long*)data;
        break;
    case EAXSOURCE_OCCLUSIONLFRATIO:
        changed = source.flOcclusionLFRatio != *(const float*)data;
        source.flOcclusionLFRatio = *(const float*)data;
        break;
    case EAXSOURCE_OCCLUSIONROOMRATIO:
        changed = source.flOcclusionRoomRatio != *(const float*)data;
        source.flOcclusionRoomRatio = *(const float*)data;
        break;
    case EAXSOURCE_OCCLUSIONDIRECTRATIO:
        changed = source.flOcclusionDirectRatio != *(const float*)data;
        source.flOcclusionDirectRatio = *(const float*)data;
        break;
    case EAXSOURCE_EXCLUSION:
        changed = source.lExclusion != *(const long*)data;
        source.lExclusion = *(const long*)data;
        break;
    case EAXSOURCE_EXCLUSIONLFRATIO:
        changed = source.flExclusionLFRatio != *(const float*)data;
        source.flExclusionLFRatio = *(const float*)data;
        break;
    case EAXSOURCE_OUTSIDEVOLUMEHF:
        changed = source.lOutsideVolumeHF != *(const long*)data;
        source.lOutsideVolumeHF = *(const long*)data;
        break;
    case EAXSOURCE_DOPPLERFACTOR:
        changed = source.flDopplerFactor != *(const float*)data;
        source.flDopplerFactor = *(const float*)data;
        break;
    case EAXSOURCE_ROLLOFFFACTOR:
    {
        bool rolloffChanged = voice->m_Source.eax4BP.flRolloffFactor != *(const float*)data;
        voice->m_Source.eax4BP.flRolloffFactor = *(const float*)data;
        if (!rolloffChanged)
            return 0;
        return SetVoiceProperty(id, data, size, voice);
    }
    case EAXSOURCE_ROOMROLLOFFFACTOR:
        changed = source.flRoomRolloffFactor != *(const float*)data;
        source.flRoomRolloffFactor = *(const float*)data;
        break;
    case EAXSOURCE_AIRABSORPTIONFACTOR:
        changed = source.flAirAbsorptionFactor != *(const float*)data;
        source.flAirAbsorptionFactor = *(const float*)data;
        break;
    case EAXSOURCE_FLAGS:
        changed = source.ulFlags != *(const unsigned long*)data;
        source.ulFlags = *(const unsigned long*)data;
        break;
    case EAXSOURCE_SENDPARAMETERS:
    {
        const EAXSOURCESENDPROPERTIES* sends = (const EAXSOURCESENDPROPERTIES*)data;
        unsigned long count = size / sizeof(EAXSOURCESENDPROPERTIES);
        unsigned long i;
        for (i = 0; i < count; ++i)
        {
            changed = false;
            for (i = 0; i < count; ++i)
            {
                if (voice->m_Source.eax4BPSend[0].lSend != sends[i].lSend || voice->m_Source.eax4BPSend[0].lSendHF != sends[i].lSendHF)
                {
                    voice->m_Source.eax4BPSend[0].lSend = sends[i].lSend;
                    voice->m_Source.eax4BPSend[0].lSendHF = sends[i].lSendHF;
                    changed = true;
                }
            }
        }
        break;
    }
    case EAXSOURCE_ALLSENDPARAMETERS:
    {
        const EAXSOURCEALLSENDPROPERTIES* sends = (const EAXSOURCEALLSENDPROPERTIES*)data;
        unsigned long count = size / sizeof(EAXSOURCEALLSENDPROPERTIES);
        changed = false;
        for (unsigned long i = 0; i < count; ++i)
        {
            EAXSOURCEOCCLUSIONSENDPROPERTIES& occlusion = voice->m_Source.eax4BPOcclusionSend[0];
            EAXSOURCEEXCLUSIONSENDPROPERTIES& exclusion = voice->m_Source.eax4BPExclusionSend[0];
            EAXSOURCESENDPROPERTIES& send = voice->m_Source.eax4BPSend[0];
            if (occlusion.lOcclusion != sends[i].lOcclusion
                || occlusion.flOcclusionLFRatio != sends[i].flOcclusionLFRatio
                || occlusion.flOcclusionRoomRatio != sends[i].flOcclusionRoomRatio
                || occlusion.flOcclusionDirectRatio != sends[i].flOcclusionDirectRatio
                || exclusion.lExclusion != sends[i].lExclusion
                || exclusion.flExclusionLFRatio != sends[i].flExclusionLFRatio
                || send.lSend != sends[i].lSend
                || send.lSendHF != sends[i].lSendHF)
            {
                occlusion.lOcclusion = sends[i].lOcclusion;
                occlusion.flOcclusionLFRatio = sends[i].flOcclusionLFRatio;
                occlusion.flOcclusionRoomRatio = sends[i].flOcclusionRoomRatio;
                occlusion.flOcclusionDirectRatio = sends[i].flOcclusionDirectRatio;
                exclusion.lExclusion = sends[i].lExclusion;
                exclusion.flExclusionLFRatio = sends[i].flExclusionLFRatio;
                send.lSend = sends[i].lSend;
                send.lSendHF = sends[i].lSendHF;
                changed = true;
            }
        }
        break;
    }
    case EAXSOURCE_OCCLUSIONSENDPARAMETERS:
    {
        const EAXSOURCEOCCLUSIONSENDPROPERTIES* sends = (const EAXSOURCEOCCLUSIONSENDPROPERTIES*)data;
        long count = size / sizeof(EAXSOURCEOCCLUSIONSENDPROPERTIES);
        changed = false;
        for (long i = 0; i < count; ++i)
        {
            EAXSOURCEOCCLUSIONSENDPROPERTIES& occlusion = voice->m_Source.eax4BPOcclusionSend[0];
            if (occlusion.lOcclusion != sends[i].lOcclusion
                || occlusion.flOcclusionLFRatio != sends[i].flOcclusionLFRatio
                || occlusion.flOcclusionRoomRatio != sends[i].flOcclusionRoomRatio
                || occlusion.flOcclusionDirectRatio != sends[i].flOcclusionDirectRatio)
            {
                occlusion.lOcclusion = sends[i].lOcclusion;
                occlusion.flOcclusionLFRatio = sends[i].flOcclusionLFRatio;
                occlusion.flOcclusionRoomRatio = sends[i].flOcclusionRoomRatio;
                occlusion.flOcclusionDirectRatio = sends[i].flOcclusionDirectRatio;
                changed = true;
            }
        }
        break;
    }
    case EAXSOURCE_EXCLUSIONSENDPARAMETERS:
    {
        const EAXSOURCEEXCLUSIONSENDPROPERTIES* sends = (const EAXSOURCEEXCLUSIONSENDPROPERTIES*)data;
        long count = size / sizeof(EAXSOURCEEXCLUSIONSENDPROPERTIES);
        for (long i = 0; i < count; ++i)
        {
            EAXSOURCEEXCLUSIONSENDPROPERTIES& exclusion = voice->m_Source.eax4BPExclusionSend[0];
            if (exclusion.lExclusion != sends[i].lExclusion || exclusion.flExclusionLFRatio != sends[i].flExclusionLFRatio)
            {
                exclusion.lExclusion = sends[i].lExclusion;
                exclusion.flExclusionLFRatio = sends[i].flExclusionLFRatio;
            }
        }
        break;
    }
    case EAXSOURCE_ACTIVEFXSLOTID:
        if (data && size >= sizeof(GUID))
        {
            const GUID* slots = (const GUID*)data;
            unsigned long count = size / sizeof(GUID);
            changed = false;
            for (unsigned long i = 0; i < (count < EAX_MAX_ACTIVE_FXSLOTS ? count : EAX_MAX_ACTIVE_FXSLOTS); ++i)
            {
                if (memcmp(&slots[i], &voice->m_Source.eax4BPActiveFXSlotID[i], sizeof(GUID)) != 0)
                {
                    voice->m_Source.eax4BPActiveFXSlotID[i] = slots[i];
                    changed = true;
                }
            }
        }
        break;
    }
    if (changed)
        return SetVoiceProperty(id, data, size, voice);
    return 0;
}
