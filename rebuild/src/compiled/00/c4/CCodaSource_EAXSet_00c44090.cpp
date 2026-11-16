// CCodaSource::EAXSet(const _GUID* guid, int property, void* data, int size) -- set an EAX property on the source's voice: a source without a voice first allocates one for its buffer (the set buffer, else the queue's play buffer, else none); any error from the allocation or from the voice goes to the context's error latch
// Leaf: ?EAXSet@CCodaSource@@QAEXPBU_GUID@@HPAXH@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44090, 99 bytes (+ int3 padding), thiscall `ret 10h`, no result.
// Identity: GameCODA source (shared engine/CCodaSource.h: m_pVoice +0x50, m_Queue +0x54, m_pBuffer +0x78). The four arguments go
//   unchanged to voice slot 15 (`call [eax+3Ch]`), which engine/CSalVoice.h (SDK 2.10 CodeView) names EAXSet(const _GUID*, int, void*, int);
//   the error goes to CCodaContext::SetError 0x00C3D090 on CODA 0x0143BB58. The member name on the source is taken from the voice
//   slot it forwards to (no SDK symbol list for CCodaSource on hand): descriptive, as are the vtable slots 5..8 of the opt-in.
//   Second source: the only caller, 0x00C52A10 (call at 0x00C52A38), an unlanded five-argument routine of the EAX layer that takes the
//   source and the same (guid, property, data, size) and, on its other arm, compares the guid with 0x012B9468 =
//   {C4D79F1E-F1AC-436B-A81D-A738E7045469} (EAXPROPERTYID_EAX40_FXSlot0): the first argument is an EAX property-set GUID.
//   The catalog's _gapscan::sub_00c44090 has no name.
// Shape: two SetError calls in the source (the allocation failure returns at once); VC7.1 merges them into the one tail, which
//   is what puts `mov ecx, offset CODA` ahead of `push eax` (0x00C440E4). A single shared `if (error) SetError` pushes first.
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODASOURCE_EAX
#include "engine/CCodaSource.h"
#include "engine/CCodaContext.h"
#include "engine/CSalVoice.h"

void CCodaSource::EAXSet(const _GUID* guid, int property, void* data, int size)
{
    int error;
    if (!m_pVoice) {
        error = AllocateVoice(m_pBuffer ? m_pBuffer : m_Queue.IsCreated() ? m_Queue.GetPlayBuffer() : 0, &m_pVoice);
        if (error) {
            CODA.SetError(error);
            return;
        }
    }
    error = m_pVoice->EAXSet(guid, property, data, size);
    if (error)
        CODA.SetError(error);
}