// CStartupDefinitionManager::Set(CStartupDefinitionManager* manager) -- hand the startup definition manager to the class's counted pointer
// Leaf: ?Set@CStartupDefinitionManager@@SIXPAV1@@Z
// Retail: Fable.exe @ 0x004336B0, 12 bytes: `push ecx ; mov ecx,0x013B8760 ; call 0x00433D3A ; ret`. Public static __fastcall
// (Ego_r.pdb 0x00441A90 and FableWin.pdb 0x01B31560 ?Set@CStartupDefinitionManager@@SIXPAV1@@Z). The pointer goes to
// CCountedPointer<CStartupDefinitionManager>::Reset (FableWin.pdb ?Reset@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXPAVCStartupDefinitionManager@@@Z,
// out of line at 0x00433D3A) on the protected static PClass 0x013B8760 (FableWin.pdb dynamic initializer / atexit destructor
// for 'CStartupDefinitionManager::PClass'). Binders: CNewFrontendGameComponent::CompileDefsPlatformSpecific 0x0042F5A9 (`Set(new ...)`)
// and Shutdown 0x0042EBB6 (`Set(0)`).
// Catalog label CEngineSubPrimitiveBase::operator_new (and the earlier `@OperatorNew@4` self leaf) was a stand-in.
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
#include "engine/CStartupDefinitionManager.h"

void __fastcall CStartupDefinitionManager::Set(CStartupDefinitionManager* manager)
{
    PClass.Reset(manager);
}
