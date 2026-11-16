#ifndef FABLE_NPLAYERGUI_CMULTISTATBAR_H
#define FABLE_NPLAYERGUI_CMULTISTATBAR_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CMULTISTATBAR_PASS23
#include "engine/NPlayerGui_CStatBar.h"
#include "engine/EStatUpdateType.h"
namespace NPlayerGui {
// Both original complete class records; native STLport list is four bytes.
class CMultiStatBar:public CStatBar {
public:
 CMultiStatBar(const CPlayerGuiDef&);
 ~CMultiStatBar();
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool);
 void AddStatUpdate(EStatUpdateType,long);
 EStatUpdateType GetCurrentType()const;
 long GetCurrentChange()const;
 unsigned char GetFadeAlpha()const{return (unsigned char)(FadeInTimer*255.0f);}
 class CStatUpdate {public:EStatUpdateType Type;long Change;float TimeRemaining;};
 std::list<CStatUpdate> Updates;
 EStatUpdateType CurrentType;
 CRGBColour CurrentColour;
 long CurrentGraphic,CurrentEffect;
 float CurrentTimeLeft,WaitingTimeLeft;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CMultiStatBar)==132);
#endif
#endif
