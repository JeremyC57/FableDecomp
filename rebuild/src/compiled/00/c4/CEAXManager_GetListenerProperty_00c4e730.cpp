// CEAXManager::GetListenerProperty(unsigned long id, void* data, unsigned long size) -- read one EAX 3.0 listener property (or all of them) from the manager's cached EAX30LISTENERPROPERTIES block; -9 when the listener state is missing, the buffer is null or too small, or the id is unknown
// Leaf: ?GetListenerProperty@CEAXManager@@QAEJKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4E730, 768 bytes = 0x28D of code + `lea ecx,[ecx]` pad + 26-entry jump table (one
// function); thiscall `ret 0Ch`.
// Identity: the EAX property-get dispatcher 0x00C4F2BF.. calls it (0x00C4F3BA, ecx = the manager) once the caller's
// property-set GUID compares equal to the listener set (its 0x10000 class bit), and the manager's commit 0x00C519F0.. calls it
// too. The ids are the EAX 3.0 DSPROPERTY_EAX30LISTENER_* numbers: 0 NONE, 1 ALLPARAMETERS (0x70 bytes = the whole
// EAX30LISTENERPROPERTIES), 2 ENVIRONMENT .. 25 FLAGS, each read from the block at +0x74 (+0x74 + field offset; the two pan
// vectors are 12 bytes). +0x04 is the listener state the landed CEAXManager::SetListenerProperty 0x00C52980 creates on
// first use, +0xE4 the CEAXListener that follows the block. Descriptive name (libpk/CODA, no PDB/RTTI), the Get twin of
// the landed SetListenerProperty / GetVoiceProperty.
// Shape: the result accumulator (`result = -9` ahead of the null tests, each arm `result = 0; break`) keeps `mov eax,-9`
// live into every size-test exit and shares one `pop edi ; ret 0Ch`; early `return -9` / `return 0` arms move the jumps (grade 1).
#include "engine/EAXNativeTypes.h"

class CListenerState;

struct CEAXManager {
    unsigned char           _pad0[4];
    CListenerState*         m_pListenerState;           // +0x04
    unsigned char           _pad8[0x74 - 0x08];
    EAX30LISTENERPROPERTIES m_listenerProperties;       // +0x74 .. +0xE3
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
    case 2:  if (size >= 4) { *(unsigned long*)data = m_listenerProperties.ulEnvironment; result = 0; } break;
    case 3:  if (size >= 4) { *(float*)data = m_listenerProperties.flEnvironmentSize; result = 0; } break;
    case 4:  if (size >= 4) { *(float*)data = m_listenerProperties.flEnvironmentDiffusion; result = 0; } break;
    case 5:  if (size >= 4) { *(long*)data = m_listenerProperties.lRoom; result = 0; } break;
    case 6:  if (size >= 4) { *(long*)data = m_listenerProperties.lRoomHF; result = 0; } break;
    case 7:  if (size >= 4) { *(long*)data = m_listenerProperties.lRoomLF; result = 0; } break;
    case 8:  if (size >= 4) { *(float*)data = m_listenerProperties.flDecayTime; result = 0; } break;
    case 9:  if (size >= 4) { *(float*)data = m_listenerProperties.flDecayHFRatio; result = 0; } break;
    case 10: if (size >= 4) { *(float*)data = m_listenerProperties.flDecayLFRatio; result = 0; } break;
    case 11: if (size >= 4) { *(long*)data = m_listenerProperties.lReflections; result = 0; } break;
    case 12: if (size >= 4) { *(float*)data = m_listenerProperties.flReflectionsDelay; result = 0; } break;
    case 13: if (size >= sizeof(EAXVECTOR)) { *(EAXVECTOR*)data = m_listenerProperties.vReflectionsPan; result = 0; } break;
    case 14: if (size >= 4) { *(long*)data = m_listenerProperties.lReverb; result = 0; } break;
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
    case 25: if (size >= 4) { *(unsigned long*)data = m_listenerProperties.ulFlags; result = 0; } break;
    }
    }
    return result;
}