// CEAXManager::QueueCommit(_CHANGES* changes, CEAXVoice* voice) -- walk the EAX translation's change list and push each changed EAX 3.0 property through the manager's property setter: buffer changes (DSPROPSETID_EAX30_BufferProperties, on the voice) and listener changes (DSPROPSETID_EAX30_ListenerProperties); an environment-size change is written with the listener flags cleared and then restored, so the size does not rescale the other reverb properties
// Leaf: ?QueueCommit@CEAXManager@@QAEXPAU_CHANGES@@PAUCEAXVoice@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C51A20; true length 0x48C = 1164 bytes (0x3CD of code to the `ret 8` at 0x00C51DEA,
// `lea ecx,[ecx]` pad, the 22-entry buffer and 25-entry listener jump tables to 0x00C51EAC). The catalogue extent (1248)
// also holds, after one int3, the separate function 0x00C51EB0..0x00C51EF9 (thiscall `ret 14h`: calls 0x00C68FE0 on the
// +0xE4 listener with a local out word, then this QueueCommit -- an EAX 4 property setter).
// Identity: the landed callers CEAXManager::SetVoiceProperty 0x00C51F00 and SetListenerProperty 0x00C52980 (and
// 0x00C52044 / 0x00C529FA) pass the change list the CEAXListener / CEAXBuffer property setters return and then call this
// (they spell the first parameter `unsigned long dirty`; it is the _CHANGES list: the body reads id +0, property +4, the
// union +8 and the next link +0x94 -- NAME_FIXES.tsv). Name kept as landed (descriptive, libpk/CODA, no PDB/RTTI).
// GUIDs: 0x012B9418 a8fa6882-b476-11d3-bdb9-00c0f02ddf87 and 0x012B9428 a8fa6881-... are EAX 3.0 eax.h's
// DSPROPSETID_EAX30_ListenerProperties / _BufferProperties (DEFINE_GUID, extern "C", in the EAX translation's .rdata next
// to _EAX_NULL_GUID 0x012B9438). Property numbers are the 1-based DSPROPERTY_EAX30LISTENER_* / EAX30BUFFER_* ids.
// Callee: the manager's property setter 0x00C4F600 (thiscall: property set, id, voice, data, size; it builds the EAX
// environment preset table on a 0xAFC frame; catalog "PropertySerializer_GetPropertyTypeCode" is propagated, descriptive
// name SetEAXProperty here).
// Shape: two zeroed dword locals (the cached listener flags, read from the manager's EAX30LISTENERPROPERTIES +0xE0 when the
// listener state +4 exists, and an all-clear flags word); buffer arms push their GUID before the shared call, listener
// arms share the `push GUID` (cross-jumped), the third environment-size call shares case FLAGS's tail. Member expressions
// at every call (a local reference to the union member hoists one `lea` for all arms and moves the node out of esi).
#include "engine/EAXNativeTypes.h"

extern "C" const GUID DSPROPSETID_EAX30_ListenerProperties;   // 0x012B9418
extern "C" const GUID DSPROPSETID_EAX30_BufferProperties;     // 0x012B9428

class CListenerState;
struct CEAXVoice;

struct CEAXManager {
    unsigned char           _pad0[4];
    CListenerState*         m_pListenerState;           // +0x04
    unsigned char           _pad8[0x74 - 0x08];
    EAX30LISTENERPROPERTIES m_listenerProperties;       // +0x74 .. +0xE3
    long SetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size);   // 0x00C4F600
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