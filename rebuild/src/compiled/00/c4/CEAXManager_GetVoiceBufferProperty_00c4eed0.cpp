// CEAXManager::GetVoiceBufferProperty(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size) -- read one EAX 3.0 buffer property (or a parameter group, or all of them) from a voice's cached EAX30BUFFERPROPERTIES block; -3 when the voice is null, the version is not 2 or 4, the id is unknown or the buffer too small
// Leaf: ?GetVoiceBufferProperty@CEAXManager@@QAEJPAUCEAXVoice@@KKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4EED0, 816 bytes = 0x2C8 of code + 23-entry jump table (one function); thiscall
// `ret 14h` with `this` unused.
// Identity: the voice property-get dispatcher 0x00C4F470 (thiscall on the EAX manager, ret 14h) calls it at 0x00C4F5DD with
// ecx = the manager and (voice, 2, id, data, size) when the caller's property-set GUID maps to class 2; class 4 goes to the
// landed CEAXManager::GetVoiceProperty 0x00C4E380 (the EAX 4 source through the voice's effect). The ids are the EAX 3.0
// DSPROPERTY_EAX30BUFFER_* numbers: 0 NONE, 1 ALLPARAMETERS (0x48 bytes = EAX30BUFFERPROPERTIES), 2/3/4 the obstruction /
// occlusion / exclusion parameter groups, 5 DIRECT .. 22 FLAGS, read from the voice's block at +0x1A8 (it ends at +0x1F0,
// where the landed rows keep the voice's CEAXBuffer). Descriptive name (libpk/CODA, no PDB/RTTI), the cached-EAX3 twin of
// GetVoiceProperty and of this chunk's GetListenerProperty 0x00C4E730.
// Shape: the result accumulator (`result = -3` before the tests, each arm `result = 0; break`) as in 0x00C4E730; the
// version test `version == 2 || version == 4` leaves the constant 4 in ecx, reused by the 4-byte size tests.
// The parameter-group copies store their fields in retail order (obstruction/exclusion LF ratio first, occlusion
// direct, LF, room, level); member order gave +0,+4 (grade 1).
#include "engine/EAXNativeTypes.h"

class CEAXBuffer;

struct CEAXVoice {
    unsigned char         _pad[0x1a8];
    EAX30BUFFERPROPERTIES m_eax3BufferProperties;       // +0x1a8 .. +0x1ef
    CEAXBuffer*           m_pEffect;                    // +0x1f0
};

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
    case 5:  if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lDirect; result = 0; } break;
    case 6:  if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lDirectHF; result = 0; } break;
    case 7:  if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lRoom; result = 0; } break;
    case 8:  if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lRoomHF; result = 0; } break;
    case 9:  if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lObstruction; result = 0; } break;
    case 10: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flObstructionLFRatio; result = 0; } break;
    case 11: if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lOcclusion; result = 0; } break;
    case 12: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionLFRatio; result = 0; } break;
    case 13: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionRoomRatio; result = 0; } break;
    case 14: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flOcclusionDirectRatio; result = 0; } break;
    case 15: if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lExclusion; result = 0; } break;
    case 16: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flExclusionLFRatio; result = 0; } break;
    case 17: if (size >= 4) { *(long*)data = voice->m_eax3BufferProperties.lOutsideVolumeHF; result = 0; } break;
    case 18: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flDopplerFactor; result = 0; } break;
    case 19: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flRolloffFactor; result = 0; } break;
    case 20: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flRoomRolloffFactor; result = 0; } break;
    case 21: if (size >= 4) { *(float*)data = voice->m_eax3BufferProperties.flAirAbsorptionFactor; result = 0; } break;
    case 22: if (size >= 4) { *(unsigned long*)data = voice->m_eax3BufferProperties.ulFlags; result = 0; } break;
    }
    }
    return result;
}