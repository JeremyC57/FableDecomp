#ifndef FABLE_ENGINE_CRENDERSTATECAPTUREBLOCK_RENDER2D_H
#define FABLE_ENGINE_CRENDERSTATECAPTUREBLOCK_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CRenderStateManager.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CRenderStateScopedCaptureBlock;
class CRenderStateCaptureBlock { friend class CRenderStateScopedCaptureBlock;
public: CRenderStateCaptureBlock();~CRenderStateCaptureBlock();void Capture();void Restore();static void __fastcall SetStateManager(CRenderStateManager*);
private: bool Open;static CRenderStateManager* StateManager;
};
class CRenderStateScopedCaptureBlock {public:
 CRenderStateScopedCaptureBlock() {CRenderStateCaptureBlock::StateManager->StartCaptureBlock();}
 ~CRenderStateScopedCaptureBlock() {CRenderStateCaptureBlock::StateManager->RestoreCaptureBlock();}
};
#endif
