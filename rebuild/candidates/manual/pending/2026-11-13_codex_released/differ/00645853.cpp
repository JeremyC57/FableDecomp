// NPlayerGui::CStatBar::DrawStatBox(CEnginePrimitivePrepareInfo const &,std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> > &,C2DVector const &,long,CWideString const &,long,unsigned char,bool,bool,bool) -- draw the player statistic box and its text and graphic indicators.
// Leaf: ?DrawStatBox@CStatBar@NPlayerGui@@KIXABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@JABVCWideString@@JE_N44@Z
// Grade: DIFFER(1160v1174) -- scalar/temporary slot ranking under /O1 biased ebp; merged fudge guards; x87 middle-box scheduling
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00645853/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p05; 00645853 probes in probe/00645853.log
// Flags: /O1
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STATBOX
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_RETAIL_CAMERA_DRAW
#define FABLE_RETAIL_CENTERED_TEXT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitive.h"
#include "engine/NPlayerGui_CStatBar.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
#pragma auto_inline(off)
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){return RenderTargetDimensions;}
#pragma auto_inline(on)

void __fastcall NPlayerGui::CStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info,std::vector<CEnginePrimitiveHandle>& handles,const C2DVector& tl_pos,long icon_graphic,const CWideString& stat_text,long border_graphic,unsigned char alpha,bool mirror,bool centre,bool fudge)
{
 const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
 const CGraphicDataBank& graphic_bank=CPlayerGui::Get().PeekGraphicBank();
 const CFontBank& font=CPlayerGui::Get().PeekFontBank();
 C2DVector draw_pos=tl_pos;
 if(mirror) draw_pos.X=GPDisplayManager->GetRenderTargetDimensions().X-draw_pos.X;
 float mid_width=gui_def.StatBoxMaxWidth;
 float text_width=font.GetStringWidth(stat_text);
 if(!mirror) {
  if(text_width>33.0f)mid_width+=text_width-33.0f;
 } else {
  if(fudge)mid_width+=text_width-37.0f;
 }
 if(mirror && fudge)draw_pos.X-=mid_width+graphic_bank.GetGraphicMaxWidth(gui_def.StatBoxStartGraphic)+6.0f;
 if(centre)draw_pos.X=(GPDisplayManager->GetRenderTargetDimensions().X-16.0f-mid_width)*0.5f;
 C2DVector mid_pos;
 if(mirror || icon_graphic==0) {
  draw_pos+=gui_def.StatBoxLeftOffset;
  mid_pos=draw_pos;
  CEnginePrimitive2DSprite start_sprite(0,draw_pos.ToC2DCoordF(),ENGINE_2D_LAYER_3,gui_def.StatBoxStartGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[0],start_sprite,0xc0);
 } else {
  mid_pos=draw_pos+gui_def.StatBoxLeftOffset;
  CEnginePrimitive2DSprite icon_sprite(0,draw_pos.ToC2DCoordF(),ENGINE_2D_LAYER_2,icon_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[0],icon_sprite,0xc0);

 }
 float icon_width;
 if(icon_graphic>0)icon_width=(float)graphic_bank.GetGraphicMaxWidth(icon_graphic);
 else icon_width=(float)graphic_bank.GetGraphicMaxWidth(gui_def.StatBoxStartGraphic);
 float mid_height=(float)graphic_bank.GetGraphicMaxHeight(gui_def.StatBoxMiddleGraphic);
 float mid_coord_x=mid_pos.X+icon_width*0.5f;
 if(icon_graphic==0)mid_coord_x+=icon_width*0.5f;
 float end_width=(float)graphic_bank.GetGraphicMaxWidth(gui_def.StatBoxEndGraphic);
 draw_pos.X=mid_width-end_width+mid_coord_x;
 C2DBoxF middle_box(C2DVector(mid_coord_x,mid_pos.Y),C2DVector(draw_pos.X,mid_pos.Y+mid_height));
 CEnginePrimitive2DSprite middle_sprite(0,middle_box,ENGINE_2D_LAYER_3,gui_def.StatBoxMiddleGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[1],middle_sprite,0xc0);
 if(mirror && icon_graphic>0) {
  C2DVector end_pos(draw_pos.X,mid_pos.Y);
  end_pos+=gui_def.StatBoxRightOffset;
  CEnginePrimitive2DSprite icon_sprite(0,end_pos.ToC2DCoordF(),ENGINE_2D_LAYER_2,icon_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[2],icon_sprite,0xc0);
 }else{
  C2DVector end_pos(draw_pos.X,mid_pos.Y);
  CEnginePrimitive2DSprite end_sprite(0,end_pos.ToC2DCoordF(),ENGINE_2D_LAYER_3,gui_def.StatBoxEndGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[2],end_sprite,0xc0);
 }
}
