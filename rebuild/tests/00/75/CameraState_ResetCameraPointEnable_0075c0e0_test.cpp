#include <stdio.h>
// CameraState::ResetCameraPointEnable() -- Clear the camera state flag.
// Leaf: ?ResetCameraPointEnable@CameraState@@YAXXZ
// Identity: descriptive. Only true caller004ee281 resets during level transition; read0075d8f0 gates camera-point activation. Existing descriptive global spelling retained. Original function name/scope unproven; descriptive namespace/free-function spelling. Propagated $E2 label is not an atexit destructor: own callers use it at runtime.
// Layout: one global boolean, no receiver or engine header.
extern bool g_cameraPointsEnabled;
namespace CameraState{void ResetCameraPointEnable(){g_cameraPointsEnabled=false;}}
bool g_cameraPointsEnabled;int main(){g_cameraPointsEnabled=true;CameraState::ResetCameraPointEnable();bool ok=!g_cameraPointsEnabled;CameraState::ResetCameraPointEnable();ok=ok&&!g_cameraPointsEnabled;g_cameraPointsEnabled=true;CameraState::ResetCameraPointEnable();ok=ok&&!g_cameraPointsEnabled;puts(ok?"0075c0e0_TEST PASS":"0075c0e0_TEST FAIL");return ok?0:1;}