#ifndef FABLE_ENGINE_CAGRAPHICAPPEARANCECOMPONENT_H
#define FABLE_ENGINE_CAGRAPHICAPPEARANCECOMPONENT_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
// Full original Ego_r 0x29e64 interface; all five virtual slots retained.
namespace NGraphicAppearance {
class CTCGraphicAppearance; class CGraphicAppearanceDrawInfo; class CMeshInfo;
class CAGraphicAppearanceComponent {
public:
 ~CAGraphicAppearanceComponent();
 virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance & p0, const NGraphicAppearance::CGraphicAppearanceDrawInfo & p1) = 0;
 virtual void OnAppearanceClear() = 0;
 virtual void GetMeshesUsed(std::vector<NGraphicAppearance::CMeshInfo,std::allocator<NGraphicAppearance::CMeshInfo> > & p0) const;
 virtual void GetTexturesUsed(std::vector<long,std::allocator<long> > & p0) const;
 virtual bool IsAppearanceSentEveryRenderFrame();
 CAGraphicAppearanceComponent(const NGraphicAppearance::CAGraphicAppearanceComponent & p0);
 CAGraphicAppearanceComponent();
 NGraphicAppearance::CAGraphicAppearanceComponent & operator=(const NGraphicAppearance::CAGraphicAppearanceComponent & p0);
};
}
#endif
