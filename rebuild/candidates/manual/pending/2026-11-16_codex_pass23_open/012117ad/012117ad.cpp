// 012117ad CSoundEventManager::PlayStreamed parent00c12e50 and generated cleanup
// Grade: DIFFER(741v741) -- required parent has13 nonrelocated byte differences: one offset-store order
// Leaf: ?PlayStreamed@CSoundEventManager@@UAEJPAVCStreamSoundBank@@JKPBUInput@@@Z
// Flags: /EHsc
#define FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
#define FABLE_RETAIL_STREAMED_TYPED_MANAGER_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#define FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
#include "libpk/CSoundEventManager.h"
#include "libpk/CPooledResource.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;
void* __cdecl operator new(unsigned int);
void __cdecl operator delete(void*);

long CSoundEventManager::PlayStreamed(CStreamSoundBank* bank, long position, unsigned long size, const Input* settings)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (bank->m_pStream == 0)
        return -1;
    void* sample = bank->m_samples.FindByKey(&position);
    if (sample) {
        void* driver = bank->m_drivers.FindByKey(&sample);
        if (driver) {
            CPKVoiceParams params;
            params.Load(settings);
            CSoundEvent* entry = PlayDriver(driver, 0, &params, 0);
            if (entry)
                return entry->Index;
            return -1;
        }
    }
    long padding = bank->m_padding;
    unsigned long offset = GetStreamBlockOffset(bank->m_pStream, position);
    long tail = GetStreamBlockTail(bank->m_pStream, position, size);
    long handle = m_events.AllocateHandle(this, tail + offset + size, padding);
    if (handle < 0)
        return -1;
    m_events.Pool.Lock(handle);
    CPooledResource* request = new CPooledResource;
    long id = m_nextId++;
    request->m_id = id;
    request->m_offset = offset;
    request->m_tail = tail;
    request->m_handle = handle;
    request->m_flag = 0;
    request->m_pBank = bank;
    request->m_size = size;
    request->m_key = position;
    request->m_padding = padding;
    request->m_params.Load(settings);
    request->Activate(this);
    if (!bank->m_pStream->Read(request->GetFirstBlock(), request->GetBlockCount(), offset + size,
                               m_events.Pool.GetPointerFromHandle(handle), request, StreamOwner)) {
        m_events.Pool.Unlock(handle);
        request->Release(this);
        delete request;
        return -1;
    }
    return id;
}

