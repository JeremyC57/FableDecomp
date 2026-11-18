#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWQUICKACCESSICONSPC_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWQUICKACCESSICONSPC_H
#include "engine/NPlayerGui_CDrawQuickAccessIcons.h"
namespace NPlayerGui {
// Ego_d record 0x6113C; native Draw reads the handle vector at +0x94.
class CDrawQuickAccessIconsPC : public CDrawQuickAccessIcons
{
public:
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);
protected:
    std::vector<CEnginePrimitiveHandle> HotBarPrimitiveHandles;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawQuickAccessIconsPC) == 0xA0);
#endif
