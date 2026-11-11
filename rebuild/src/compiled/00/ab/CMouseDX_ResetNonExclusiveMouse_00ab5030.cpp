// 00ab5030 CMouseDX::ResetNonExclusiveMouse
// Flags: none
// Retail: 00ab5030,342 bytes.
// Leaf: ?ResetNonExclusiveMouse@CMouseDX@@IAEXXZ
#define FABLE_RETAIL_CMOUSE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#define FABLE_RETAIL_WIN32_MOUSE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/C2DExtentsI.h"
#include "engine/CMouseDX.h"
#include "engine/CSystemManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

void CMouseDX::ResetNonExclusiveMouse()
{
    C2DBoxF box;
    if (GFGetSystemManager()->GetDisplayManager()) {
        POINT mouse_pos;
        long mouse_x, mouse_y;
        POINT window_pos;
        RECT window_rect;
        HWND win_handle = GFGetSystemManager()->GetApplicationWinHandle();
        GetCursorPos(&mouse_pos);
        window_pos.x = 0;
        window_pos.y = 0;
        ClientToScreen(win_handle, &window_pos);
        GetClientRect(win_handle, &window_rect);
        if (mouse_pos.x >= window_pos.x && mouse_pos.x < window_rect.right + window_pos.x
            && mouse_pos.y >= window_pos.y && mouse_pos.y < window_rect.bottom + window_pos.y) {
            mouse_pos.x -= window_pos.x;
            mouse_pos.y -= window_pos.y;
            mouse_x = mouse_pos.x;
            mouse_y = mouse_pos.y;
            C2DExtentsI screen_dims = GFGetSystemManager()->GetDisplayManager()->GetBackBuffer().GetDimensions();
            if (mouse_x >= 0 && mouse_x < screen_dims.X && mouse_y >= 0 && mouse_y < screen_dims.Y) {
                NonExclusiveOldMouseX = (float)mouse_x;
                NonExclusiveOldMouseY = (float)mouse_y;
                Pos.X = (float)mouse_x;
                Pos.Y = (float)mouse_y;
            }
            NonExclusiveOldButton0Pressed = GFGetSystemManager()->IsMouseButton0Pressed();
            NonExclusiveOldButton1Pressed = GFGetSystemManager()->IsMouseButton1Pressed();
            NonExclusiveOldButton2Pressed = GFGetSystemManager()->IsMouseButton2Pressed();
            NonExclusiveOldButton3Pressed = GFGetSystemManager()->IsMouseButton3Pressed();
            NonExclusiveOldButton4Pressed = GFGetSystemManager()->IsMouseButton4Pressed();
        }
    }
}