// CEngineOcclusionManager::EndOcclusionTest(unsigned long) -- empty on the PC build (`ret 4`) -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CEngineOcclusionManager.h.
// Leaf: ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z
// Evidence: Ego_r.pdb 0x00A347B0 ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z (3 bytes) + FableWin.pdb; the header OCCLUSIONTEST opt-in names 0x00B8D0A0.
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#include "engine/CEngineOcclusionManager.h"

void CEngineOcclusionManager::EndOcclusionTest(unsigned long handle)
{
}
