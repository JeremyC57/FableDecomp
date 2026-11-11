#ifndef FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#define FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
namespace NPlayerGui {
// Original Ego00686ac7/FableWin020d908c: PC mapping copies the input vector.
inline C2DVector FABLE_FASTCALL GFToScreen(const C2DVector& pos) { return pos; }
}
#endif
