// CEngineOcclusionManager::StartOcclusionTest() -- empty on the PC build (`ret`) -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CEngineOcclusionManager.h.
// Leaf: ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ
// Evidence: Ego_r.pdb 0x00A347A0 ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ (1 byte) + FableWin.pdb; the header OCCLUSIONTEST opt-in names 0x00B8D090.
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#include "engine/CEngineOcclusionManager.h"

void CEngineOcclusionManager::StartOcclusionTest()
{
}
