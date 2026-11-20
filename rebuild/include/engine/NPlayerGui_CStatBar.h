#ifndef FABLE_ENGINE_NPLAYERGUI_CSTATBAR_H
#define FABLE_ENGINE_NPLAYERGUI_CSTATBAR_H
#include "engine/NPlayerGui_CStatBase.h"
#ifdef FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGE_TEXT_PASS23
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
#endif
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
#ifdef FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGETEXTS
 // hand3c opt-in (row 0x00439BB0 CPlayerGui::UpdateMoney): Ego_r.pdb / FableWin.pdb ?UpdateChangeTexts@CStatBar@NPlayerGui@@QAEXXZ
 // (out of line in retail at 0x00646B12) and ?AddChangeText@CStatBar@NPlayerGui@@QAEXABVCRGBColour@@ABVCWideString@@J_N@Z (0x0064DAE0).
 // Declarations only: no layout change.
 void UpdateChangeTexts();                                            // 0x00646B12
 void AddChangeText(const CRGBColour& colour,const CWideString& text,long value,bool start_to_end);   // 0x0064DAE0
#endif
protected:
 virtual void DrawStatBox(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,long,long,long,const CWideString&,unsigned char,bool,bool);
#ifdef FABLE_RETAIL_NPLAYERGUI_CSTATBAR_DRAWCHANGETEXT
 // Opt-in (hand3c2, row 0x00646762 CStatBar::DrawStatBarOnLeft): Ego_r.pdb / Ego_d.pdb ?DrawChangeText@CStatBar@NPlayerGui@@IAEXABVC2DVector@@ABVCWideString@@ABVCRGBColour@@@Z
 // (protected; out of line in retail at 0x00645F2E, landed). Declaration only.
 void DrawChangeText(const C2DVector&,const CWideString&,const CRGBColour&);                    // 0x00645F2E
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGE_TEXT_PASS23
 // Both original nested class records; native size36, needed for normal list lifetimes.
 class CChangeText {public:
  C2DVector OldOffset,Offset;
  CWideString Text;
  long Value;
  CRGBColour Colour;
  bool StartToEnd;
  float PercentageThrough;
 };
#else
 class CChangeText;
#endif
 std::list<CChangeText> ChangeTexts;
};
}
FABLE_STATIC_ASSERT_AT(21, sizeof(NPlayerGui::CStatBar)==104);
#endif
