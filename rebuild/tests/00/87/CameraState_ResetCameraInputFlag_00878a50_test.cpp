#include <stdio.h>
// CameraState::ResetCameraInputFlag() -- Clear the camera state flag.
// Leaf: ?ResetCameraInputFlag@CameraState@@YAXXZ
// Identity: descriptive. Only true caller0069b515 resets during camera manager setup; reader008799c0 scales one input component when the flag is set. Original flag identity unproven. Original function name/scope unproven; descriptive namespace/free-function spelling. Propagated $E2 label is not an atexit destructor: own callers use it at runtime.
// Layout: one global boolean, no receiver or engine header.
extern bool g_CUnprovenCameraInputFlag_013baf98;
namespace CameraState{void ResetCameraInputFlag(){g_CUnprovenCameraInputFlag_013baf98=false;}}
bool g_CUnprovenCameraInputFlag_013baf98;int main(){g_CUnprovenCameraInputFlag_013baf98=true;CameraState::ResetCameraInputFlag();bool ok=!g_CUnprovenCameraInputFlag_013baf98;CameraState::ResetCameraInputFlag();ok=ok&&!g_CUnprovenCameraInputFlag_013baf98;g_CUnprovenCameraInputFlag_013baf98=true;CameraState::ResetCameraInputFlag();ok=ok&&!g_CUnprovenCameraInputFlag_013baf98;puts(ok?"00878a50_TEST PASS":"00878a50_TEST FAIL");return ok?0:1;}