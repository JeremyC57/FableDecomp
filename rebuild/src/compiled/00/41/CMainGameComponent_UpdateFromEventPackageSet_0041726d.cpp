// CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet*) -- process new input packages and report the world state
// Leaf: ?UpdateFromEventPackageSet@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z
// Retail: 0x0041726D,189 bytes,thiscall
// Flags: /O1 /Oy-
#define FABLE_RETAIL_STLP_MAX_FASTCALL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CGAMEEVENTPACKAGE
#define FABLE_RETAIL_CGAMEEVENTPACKAGESET_ACCESSORS
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CWORLD_EVENTPACKAGES
#define FABLE_RETAIL_CDISPLAYENGINE_EVENTPACKAGES
#include "engine/CMainGameComponent.h"
#include "engine/CGameEventPackageSet.h"
#include "engine/CGameEventPackage.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "stlport/fable_containers.h"
void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* event_package_set) {
    for(long c1=0;c1<event_package_set->GetNoPackages();++c1) {
        const CGameEventPackage* event_package=event_package_set->GetPackage(c1);
        if(event_package->GetTimeStamp()>InputServerFrame) {
            ProcessEventPackage(event_package);
            bool do_update=false;
            if(InputServerFrame==CurrentServerFrame) {
                do_update=true;
                AllowRender=true;
            }
            PWorld->Update(*event_package,do_update);
            PDisplayEngine->ConfirmFeedbackGameEventPackage(event_package);
            InputServerFrame=event_package->GetTimeStamp();
            CurrentServerFrame=std::max(event_package->GetTimeStamp(),CurrentServerFrame);
            Client.Update(GetCurrentServerFrame(),PeekWorldChecksum(),PeekWorldSeed());
            PDisplayEngine->WorldUpdate();
        }
    }
}