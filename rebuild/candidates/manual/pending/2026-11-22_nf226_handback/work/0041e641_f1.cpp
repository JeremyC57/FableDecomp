// Leaf: ??1CManager@NUISystem@@IAE@XZ
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_ERRORMESSAGE
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CManager::~CManager()
{
    if (m_bHasErrorMessage)
        ClearErrorMessage();
    GetInstance()->SetMeshBank(CCountedPointer<CMeshDataBank>());
    GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());
}
}
