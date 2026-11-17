// CSoundSample::LoadData(CSoundEventManager* manager, CStreamBuffer* buffer) -- hand the stream buffer's pool data to the sample's
//   virtual slot 2 and return its status
// Leaf: ?LoadData@CSoundSample@@QAEJPAUCSoundEventManager@@PAUCStreamBuffer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EA60, 31 bytes, thiscall, `ret 8`.
// Identity (NF53): the catalog label CSoundSample::LoadData (chunk 750: vtable/caller/string evidence) and the landed caller
//   CSoundEvent::Play 0x00C21780, which binds ?LoadData@CSoundSample@@QAEJPAUCSoundEventManager@@PAUCStreamBuffer@@@Z with the
//   manager first and the buffer second (retail: [esp+4] is pushed as GetPoolData's argument, [esp+8] is its `this`). The earlier
//   source spelled it CTCCreatureNavigation::IsPosNavLineOfSightTo(C3DVector*, Thing*) (catalog residue). Descriptive libpk names
//   (no PDB covers this library): CSoundSample and its slot-2 ReadPoolData are descriptive.
// Callees: CStreamBuffer::GetPoolData 0x00C26260 (libpk/CStreamBuffer.h), the sample's virtual slot 2 (+0x08).
#define FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
#include "libpk/CStreamBuffer.h"

struct CSoundSample {                            // no shared header: the libpk sample (vtable at +0)
    virtual void v00();
    virtual long StartStreaming(CSoundEventManager* manager, CStreamBuffer* buffer);   // slot 1 (+0x04), as CSoundEvent::Play 0x00C21780
    virtual long ReadPoolData(void* data);       // slot 2 (+0x08), descriptive
    long LoadData(CSoundEventManager* manager, CStreamBuffer* buffer);
};

long CSoundSample::LoadData(CSoundEventManager* manager, CStreamBuffer* buffer)
{
    return ReadPoolData(buffer->GetPoolData(manager));
}
