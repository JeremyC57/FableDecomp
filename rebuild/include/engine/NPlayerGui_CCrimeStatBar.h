#ifndef FABLE_ENGINE_NPLAYERGUI_CCRIMESTATBAR_H
#define FABLE_ENGINE_NPLAYERGUI_CCRIMESTATBAR_H
#include "engine/NPlayerGui_CStatBar.h"
namespace NPlayerGui {
// Layout: original PDB CCrimeStatBar; native current fine+68, time left+6c.
class CCrimeStatBar:public CStatBar {
public:
 CCrimeStatBar(const CPlayerGuiDef&); ~CCrimeStatBar();
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool);
 void SetFine(long); void SetNormalisedTimeLeft(float);
protected:
 virtual void DrawStatBox(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,long,long,long,const CWideString&,unsigned char,bool,bool);
private:
 long CurrentFine;
 float NormalisedTimeLeft;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CCrimeStatBar)==112);
#endif
