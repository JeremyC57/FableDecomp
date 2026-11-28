#ifdef FABLE_RETAIL_CDRAWBETTINGHUD_PASS30
#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWBETTINGHUD_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWBETTINGHUD_H

#include "engine/NPlayerGui_CDrawBase.h"

class CEnginePrimitivePrepareInfo;

namespace NPlayerGui {
class CDrawBettingHUD : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};
}

#endif
#endif
