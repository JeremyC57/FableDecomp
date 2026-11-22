// probe (NF224 form 3): header opt-in inline Clear, scaffold calls it
// Leaf: ??_GCManager@NUISystem@@IAEPAXI@Z
#define FABLE_RETAIL_CMANAGER_CLEAR_INLINE_PROBE
#include "engine/NUISystem_CManager.h"
namespace {
void ReferenceManagerClear()
{
    NUISystem::CManager::Clear();
}
}
