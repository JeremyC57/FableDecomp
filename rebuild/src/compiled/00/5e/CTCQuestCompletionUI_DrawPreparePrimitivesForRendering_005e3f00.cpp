// CTCQuestCompletionUI::DrawPreparePrimitivesForRendering -- update the quest UI and fade its backdrop.
// Retail: 0x005E3F00, 295 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?DrawPreparePrimitivesForRendering@CTCQuestCompletionUI@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW
#define FABLE_RETAIL_CUSERPROFILEMANAGER_UIBACKGROUND
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/GFGetTime.h"
void CTCQuestCompletionUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if(Active&&!Quitting){
        float time_step=float(GFGetTime()-LastRenderTime);
        LastRenderTime=GFGetTime();
        PBlack->Update(time_step);
        PBlack->Draw(*info.Engine,0,0,0,0);
        for(unsigned long count=0;count<Components.size();++count){
            Components[count]->Update(time_step);
            Components[count]->Draw(*info.Engine,0,0,0,0);
        }
        AccumFadeTime+=time_step;
        float lerp=AccumFadeTime/NUISystem::CManager::GetInstance()->GetMiscThingsDef()->TimeInSecsForFade;
        if(lerp>1.0f)lerp=1.0f;
        CUserProfileManager::Get().RenderUIBackground(lerp);
    }
}