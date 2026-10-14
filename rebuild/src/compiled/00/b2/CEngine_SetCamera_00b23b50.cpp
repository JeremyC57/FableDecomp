// CEngine::SetCamera(CCamera*) -- make the camera the engine camera's exclusive observer, process the engine camera once, and remember it as the current camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23B50, length 39 bytes.
// Callees: CObservable::AddExclusiveObserver(CObserver*) @ 0x00B2FBF0 and CEngineCamera::Process(bool) @ 0x00B314E0, both on the engine camera singleton
// @ 0x01436EA0 (re-read for the second call). Layout from engine/CEngine.h (PDB): CurrentCamera +0x14. Shape: the camera stays in edi for the push and the
// final member store.
#include "engine/CEngine.h"

struct CEngineCamera {
    void AddExclusiveObserver(CCamera* observer);  // 0x00B2FBF0 (CObservable)
    void Process(bool force);                      // 0x00B314E0
};
extern CEngineCamera* g_pEngineCamera_01436ea0;    // 0x01436EA0

struct CEngine_Methods : CEngine {
    void SetCamera(CCamera* camera);
};

void CEngine_Methods::SetCamera(CCamera* camera)
{
    g_pEngineCamera_01436ea0->AddExclusiveObserver(camera);
    g_pEngineCamera_01436ea0->Process(true);
    CurrentCamera = camera;
}