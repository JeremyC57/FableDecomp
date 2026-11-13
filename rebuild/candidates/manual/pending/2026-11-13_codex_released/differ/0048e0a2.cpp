// CSubtitleRenderer::DrawBox(CEnginePrimitivePrepareInfo const &,std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> > &,C2DBoxF const &,EEngine2DLayer) -- draw a subtitle box using the supplied primitive handles and screen bounds.
// Leaf: ?DrawBox@CSubtitleRenderer@@SIXABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DBoxF@@W4EEngine2DLayer@@@Z
// Grade: DIFFER(1760v1772) -- frame 0x2b8 vs retail 0x2c4 (12 bytes of locals missing) -- every ebp slot and the sprite argument scheduling differ
// Context: grades against the Codex row context $S/codex11_dl/x/rows/0048e0a2/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p05; 0048e0a2 probes in probe/0048e0a2.log
// Flags: /O1
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STATBOX
#define FABLE_RETAIL_SUBTITLE_BOX
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_RETAIL_CAMERA_DRAW
#define FABLE_RETAIL_CENTERED_TEXT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;

inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 uid,const C2DVector& screen_pos,EEngine2DLayer layer,unsigned long bank_index,unsigned long frame,EEnginePrimitiveBlendMode blend_mode,EEngineInterpolationSet interp_set,float start_angle,float end_angle,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long sprite_2d_flags,const C2DVector& uv_offsets_start,const C2DVector& uv_offsets_end)
:CEnginePrimitive(ENGINE_PRIMITIVE_2D_SPRITE,uid,interp_set),ClipBox(screen_pos,screen_pos),Layer(layer),BankIndex(bank_index),Frame(frame),StartAngle(start_angle),EndAngle(end_angle),BlendMode(blend_mode),Colour(r,g,b,alpha),ClipBoxIsPos(true),Flags(sprite_2d_flags),Texture(0),UVOffsetsStart(uv_offsets_start),UVOffsetsEnd(uv_offsets_end){}
inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 uid,const C2DBoxF& clip_box,EEngine2DLayer layer,unsigned long bank_index,unsigned long frame,EEnginePrimitiveBlendMode blend_mode,EEngineInterpolationSet interp_set,float start_angle,float end_angle,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long sprite_2d_flags,const C2DVector& uv_offsets_start,const C2DVector& uv_offsets_end)
:CEnginePrimitive(ENGINE_PRIMITIVE_2D_SPRITE,uid,interp_set),ClipBox(clip_box),Layer(layer),BankIndex(bank_index),Frame(frame),StartAngle(start_angle),EndAngle(end_angle),BlendMode(blend_mode),Colour(r,g,b,alpha),ClipBoxIsPos(false),Flags(sprite_2d_flags),Texture(0),UVOffsetsStart(uv_offsets_start),UVOffsetsEnd(uv_offsets_end){}

void __fastcall CSubtitleRenderer::DrawBox(const CEnginePrimitivePrepareInfo& info,std::vector<CEnginePrimitiveHandle>& handles,const C2DBoxF& original_text_box,EEngine2DLayer layer)
{
 const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
 const CGraphicDataBank& graphics=CPlayerGui::Get().PeekGraphicBank();
 C2DBoxF text_box=original_text_box;
 float offset=GPDisplayManager->GetRenderTargetDimensions().X*0.0125f;
 text_box.TLX-=offset; text_box.TLY-=offset; text_box.BRX+=offset; text_box.BRY+=offset;
 C2DVector tl_pos=text_box.GetTLPos();
 float text_box_width=text_box.GetWidth();
 float text_box_height=text_box.GetHeight();
 CEnginePrimitive2DSprite tl_sprite(0,tl_pos,layer,gui_def.TextBoxTLGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[0],tl_sprite,0xc0);
 float tl_width=(float)graphics.GetGraphicMaxWidth(gui_def.TextBoxTLGraphic);
 float tr_width=(float)graphics.GetGraphicMaxWidth(gui_def.TextBoxTRGraphic);
 float tm_width=text_box_width-tr_width-tl_width;
 float tm_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxTMGraphic);
 C2DBoxF tm_box(C2DVector(text_box.TLX+tl_width,text_box.TLY),C2DExtentsF(tm_width,tm_height));
 CEnginePrimitive2DSprite tm_sprite(0,tm_box,layer,gui_def.TextBoxTMGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[1],tm_sprite,0xc0);
 float tr_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxTRGraphic);
 float br_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxBRGraphic);
 C2DVector tr_pos(text_box.TLX+tl_width+tm_width,text_box.TLY);
 CEnginePrimitive2DSprite tr_sprite(0,tr_pos,layer,gui_def.TextBoxTRGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[2],tr_sprite,0xc0);
 float tl_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxTLGraphic);
 float bl_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxBLGraphic);
 float ml_height=text_box_height-tl_height-bl_height;
 C2DVector ml_pos(text_box.TLX,text_box.TLY+tl_height);
 float ml_width=(float)graphics.GetGraphicMaxWidth(gui_def.TextBoxMLGraphic);
 C2DBoxF ml_box(ml_pos,C2DExtentsF(ml_width,ml_height));
 if(ml_height>0.0f) {
 CEnginePrimitive2DSprite ml_sprite(0,ml_box,layer,gui_def.TextBoxMLGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[3],ml_sprite,0xc0);
 }
 float mr_width=(float)graphics.GetGraphicMaxWidth(gui_def.TextBoxMRGraphic);
 float mr_height=ml_height;
 float mr_coord_x=text_box_width+text_box.TLX;
 C2DBoxF mr_box(C2DVector(mr_coord_x-mr_width,ml_pos.Y),C2DExtentsF(mr_width,mr_height));
 if(mr_height>0.0f) {
 CEnginePrimitive2DSprite mr_sprite(0,mr_box,layer,gui_def.TextBoxMRGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[5],mr_sprite,0xc0);
 }
 C2DVector bl_pos(text_box.TLX,ml_height+tl_height+text_box.TLY);
 CEnginePrimitive2DSprite bl_sprite(0,bl_pos,layer,gui_def.TextBoxBLGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[6],bl_sprite,0xc0);
 float bm_height=(float)graphics.GetGraphicMaxHeight(gui_def.TextBoxBMGraphic);
 float bm_coord_y=text_box_height+text_box.TLY;
 C2DBoxF bm_box(C2DVector(text_box.TLX+tl_width,bm_coord_y-bm_height),C2DExtentsF(tm_width,bm_height));
 CEnginePrimitive2DSprite bm_sprite(0,bm_box,layer,gui_def.TextBoxBMGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[7],bm_sprite,0xc0);
 float br_width=(float)graphics.GetGraphicMaxWidth(gui_def.TextBoxBRGraphic);
 C2DVector br_pos(mr_coord_x-br_width,bm_coord_y-br_height);
 CEnginePrimitive2DSprite br_sprite(0,br_pos,layer,gui_def.TextBoxBRGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[8],br_sprite,0xc0);
 if(ml_height>0.0f) {
  C2DBoxF back_box(C2DVector(text_box.TLX+tl_width,ml_pos.Y),C2DExtentsF(text_box_width-tl_width*2.0f,text_box_height-tl_height*2.0f));
 CEnginePrimitive2DSprite back_sprite(0,back_box,(EEngine2DLayer)(layer+1),gui_def.TextBoxBackGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0.0f,0.0f,255,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
 info.Engine.SetPersistentPrimitive(handles[4],back_sprite,0xc0);
 }
}
