// 00c26260 CStreamBuffer::GetPoolData
// Flags: none
// Leaf: ?GetPoolData@CStreamBuffer@@QAEPAXPAUCSoundEventManager@@@Z
// Names are descriptive libpk identities; NAMING_EVIDENCE.md records all searches.
#define FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
#include "libpk/CStreamBuffer.h"
#include "libpk/CSoundEventManager.h"

void* CStreamBuffer::GetPoolData(CSoundEventManager* manager)
{
    return manager->m_pool.GetPointerFromHandle(m_handle);
}