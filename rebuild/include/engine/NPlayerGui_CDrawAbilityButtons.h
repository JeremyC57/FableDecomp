#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWABILITYBUTTONS_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWABILITYBUTTONS_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CHeroAbilityDef;class CExpressionDef;class CAbilityButtonGuiDef;class C2DVector;class CEnginePrimitivePrepareInfo;
namespace NParticleEngine {class CParticleEmitter;}
namespace NPlayerGui {
// Original complete Ego_r/FableWin records: native STLport vectors12, sizeof84.
class CDrawAbilityButtons:public CDrawBase {
public:
 CDrawAbilityButtons(CPlayerGui&,std::vector<CDrawBase*>&);
 ~CDrawAbilityButtons();
 virtual void Draw(const CEnginePrimitivePrepareInfo&);
 void ClearEffects();
 virtual void ClearPrimitivesIfNecessary();
protected:
 void DrawButton(const CEnginePrimitivePrepareInfo&,const C2DVector&,float,long,long,bool,EEngine2DLayer);
 void DrawEffects(long,const C2DVector&,const CHeroAbilityDef&);
 void UpdateWeaponAbilities();
 virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef&)const;
 virtual void DrawAbilityList(const CEnginePrimitivePrepareInfo&,float);
 void DrawAbilityButton(const CEnginePrimitivePrepareInfo&,std::vector<CDefPointer<const CHeroAbilityDef> >&,long,long,long,float);
 virtual void GetAbilityButtonMapping(std::vector<long>&);
 virtual void GetAbilityPositionMapping(std::vector<long>&);
 virtual void DrawExpressionList(const CEnginePrimitivePrepareInfo&,float);
 virtual void DrawExpressionButton(const CEnginePrimitivePrepareInfo&,std::vector<CDefPointer<const CExpressionDef> >&,long,long,long,float);
 long Columns;
 CEnginePrimitiveHandle BackgroundPrimitiveHandle,InteractBorderPrimitiveHandle;
 std::vector<CEnginePrimitiveHandle> SpecialPrimitiveHandles;
 std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawAbilityButtons)==84);
#endif
#endif
