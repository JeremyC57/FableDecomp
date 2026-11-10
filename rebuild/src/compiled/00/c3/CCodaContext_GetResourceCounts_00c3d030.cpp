// CCodaContext::GetResourceCounts(CRESOURCEDESC* desc) const -- report the four available resource counts (descriptive name: GameCODA has no PDB)
// Leaf: ?GetResourceCounts@CCodaContext@@QBEPAU_CRESOURCEDESC@@PAU2@@Z
// Flags:
// Retail: 0x00c3d030, 94 bytes, thiscall.
// Naming: vendor descriptive method; exact declaration absent from SDK 2.10.
#include "engine/CCodaContext.h"
CRESOURCEDESC* CCodaContext::GetResourceCounts(CRESOURCEDESC* pResourceDesc) const
{
    int count = m_nMax2DSources;
    if (m_pSal) {
        if (count > 0) pResourceDesc->n2DSources = m_2DSources.GetNumFree();
        else pResourceDesc->n2DSources = count;
        count = m_nMax3DSources;
        if (count > 0) pResourceDesc->n3DSources = m_3DSources.GetNumFree();
        else pResourceDesc->n3DSources = count;
        count = m_nMaxListeners;
        if (count > 0) pResourceDesc->nListeners = m_Listeners.GetNumFree();
        else pResourceDesc->nListeners = count;
        count = m_nMaxEnvironments;
        if (count > 0) pResourceDesc->nEnvironments = m_Environments.GetNumFree();
        else pResourceDesc->nEnvironments = count;
    } else {
        pResourceDesc->n2DSources = count;
        pResourceDesc->n3DSources = m_nMax3DSources;
        pResourceDesc->nListeners = m_nMaxListeners;
        pResourceDesc->nEnvironments = m_nMaxEnvironments;
    }
    return pResourceDesc;
}