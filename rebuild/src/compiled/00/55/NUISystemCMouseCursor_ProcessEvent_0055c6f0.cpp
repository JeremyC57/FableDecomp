// 0055c6f0 NUISystem::CMouseCursor::ProcessEvent
// Leaf: ?ProcessEvent@CMouseCursor@NUISystem@@UAEXW4EEvent@2@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMOUSECURSOR_PROCESSEVENT_PASS27
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_SETMOUSEMOVEMENT
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/NUISystem_CMouseCursor.h"
#include "engine/NUISystem_CManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"

namespace NUISystem {
void CMouseCursor::ProcessEvent(EEvent event)
{
    switch (event) {
    case UI_EVENT_MOUSE_MOVEMENT: {
        C2DVector delta(Position);
        ChangePositionDelta(CManager::GetInstance()->GetMouseMovement(), 0.0f, false);
        if (Position.X >= GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X)
            Position.X = (float)GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X;
        if (Position.X < 0.0f)
            Position.X = 0.0f;
        if (Position.Y >= GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y)
            Position.Y = (float)GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y;
        if (Position.Y < 0.0f)
            Position.Y = 0.0f;
        delta = Position - delta;
        CManager::GetInstance()->SetMouseMovement(delta);
        break;
    }
    }
}
}