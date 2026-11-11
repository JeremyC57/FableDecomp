#ifndef FABLE_ENGINE_NPLAYERGUI_CSTATBAR_H
#define FABLE_ENGINE_NPLAYERGUI_CSTATBAR_H
#include "engine/NPlayerGui_CStatBase.h"
class CFontBank; class CWideString;
namespace NPlayerGui {
// Layout: both PDBs; base72, six scalar words, FontBank+96, STLport list+100.
class CStatBar:public CStatBase {
public:
 unsigned long GraphicFrame,OldGraphicFrame;
 long GraphicIndex,BorderGraphicIndex,EffectIndex,OldEffectIndex;
 const CFontBank& FontBank;
 CStatBar(const CPlayerGuiDef&); CStatBar(const CStatBar&); ~CStatBar();
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool);
 virtual void ClearPrimitiveHandles();
protected:
 virtual void DrawStatBox(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,long,long,long,const CWideString&,unsigned char,bool,bool);
 class CChangeText;
 std::list<CChangeText> ChangeTexts;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CStatBar)==104);
#endif
