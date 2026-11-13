// DisplayCenteredText(CFontBank const &,CWideString const &,C2DBoxF const &) -- draw the supplied text centered within the given box.
// Leaf: ?DisplayCenteredText@@YIXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z
// Grade: DIFFER(1104v1104) -- one instruction: at the third Draw2DText the receiver reload mov ecx,[esp+3c] precedes lea/push in retail, follows it here
// Context: grades against the Codex row context $S/codex11_dl/x/rows/009bca20/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 009bca20 probes in probe/009bca20.log
// Flags: /Gr
#define FABLE_RETAIL_CENTERED_TEXT
#define FABLE_RETAIL_CWIDESTRING_COPY
#define FABLE_RETAIL_CWIDESTRING_CTOR
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CARRAY_DISPLAY
#include "stlport/display_containers.h"
#include "engine/CFontBank.h"
#include "engine/CWStringParser.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/CRenderManager.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
float __fastcall GFRoundVXToNearestPixel(double);
float __fastcall GFRoundVYToNearestPixel(double);
void __fastcall DisplayCenteredText(const CFontBank& font,const CWideString& text,const C2DBoxF& vbox)
{
 CWStringParser parser(text);
 CDisplayManager* pdisplay=GFGetSystemManager()->GetDisplayManager();
 float boxlength=vbox.BRX-vbox.TLX;
 long MaxNumLines=0;
 CWStringParser counter(text);
 CWideString text_line=counter.ReadAsStringUntilCharacter(10);
 while(text_line!=L"") {
  counter.SkipWhitespace();
  text_line=counter.ReadAsStringUntilCharacter(10);
  ++MaxNumLines;
 }
 text_line=parser.ReadAsStringUntilCharacter(10);
 if(text_line!=L"") {
 long NumLines=MaxNumLines;
 do {
  C2DExtentsF extents=font.GetStringExtents(text_line,false);
  extents.X=GFAXToVX(extents.X);
  extents.Y=GFAYToVY(extents.Y);
  C2DVector start_draw_pos((boxlength-extents.X)*0.5f+vbox.TLX,vbox.BRY-font.GetVFontMaxHeight()*NumLines);
  C2DVector draw_pos;
  draw_pos=start_draw_pos+C2DVector(GFAXToVX(1.0f),GFAYToVY(1.0f));
  draw_pos.X=GFRoundVXToNearestPixel(draw_pos.X);
  draw_pos.Y=GFRoundVYToNearestPixel(draw_pos.Y);
  pdisplay->GetRenderManager().Draw2DText(draw_pos,text_line,font,CRGBColour(0,0,0,255),0);
  draw_pos=start_draw_pos+C2DVector(GFAXToVX(1.0f),GFAYToVY(-1.0f));
  draw_pos.X=GFRoundVXToNearestPixel(draw_pos.X);
  draw_pos.Y=GFRoundVYToNearestPixel(draw_pos.Y);
  pdisplay->GetRenderManager().Draw2DText(draw_pos,text_line,font,CRGBColour(0,0,0,255),0);
  draw_pos=start_draw_pos+C2DVector(GFAXToVX(-1.0f),GFAYToVY(1.0f));
  draw_pos.X=GFRoundVXToNearestPixel(draw_pos.X);
  draw_pos.Y=GFRoundVYToNearestPixel(draw_pos.Y);
  pdisplay->GetRenderManager().Draw2DText(draw_pos,text_line,font,CRGBColour(0,0,0,255),0);
  draw_pos=start_draw_pos;
  draw_pos.X=GFRoundVXToNearestPixel(draw_pos.X);
  draw_pos.Y=GFRoundVYToNearestPixel(draw_pos.Y);
  pdisplay->GetRenderManager().Draw2DText(draw_pos,text_line,font,CRGBColour(255,255,255,255),0);
  parser.SkipWhitespace();
  text_line=parser.ReadAsStringUntilCharacter(10);
  --NumLines;
 } while(text_line!=L"");
 }
}
