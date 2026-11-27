#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWLETTERBOXBLACKBARS_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWLETTERBOXBLACKBARS_H
#ifdef FABLE_RETAIL_DRAW_LETTERBOX_PASS29
#include "engine/NPlayerGui_CDrawBase.h"
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
// Ego_r layout has no additional fields. Retail Draw addresses only inherited members.
class CDrawLetterBoxBlackBars : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};
}
#endif
#endif
