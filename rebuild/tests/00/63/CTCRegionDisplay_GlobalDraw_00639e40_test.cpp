#include <stdio.h>
#include <stdlib.h>
static void ck(bool v){if(!v)exit(2);}
struct CWideString{int id;bool operator!=(const wchar_t* other)const{ck(!*other);return id!=0;}};
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
struct CRGBColour{unsigned char r,g,b,a;CRGBColour(unsigned char rr,unsigned char gg,unsigned char bb,unsigned char aa):r(rr),g(gg),b(bb),a(aa){}};
struct CInterpolationInfo{struct{float WFInterpolate;}Current;};
float GFGetInterpolatedValue(float old,float now,float t){return (now-old)*t+old;}
float GFAXToVX(float n){return n*2.0f;}float GFAYToVY(float n){return n*3.0f;}
struct CFontBank{float height;int widths[3];float GetVStringWidth(const CWideString& s)const{return (float)widths[s.id];}float GetFontMaxHeight()const{return height;}float GetVFontMaxHeight()const{return GFAYToVY(height);}};
struct CPlayerGuiDef{C2DVector InfoDisplayPos,RegionDisplayPos;};
struct CT{int present;bool open;bool IsOpen()const{return open;}};
struct CTCQuestCompletionUI:CT{};struct CTCInGameMenu:CT{};
struct CThing{CTCQuestCompletionUI quest;CTCInGameMenu menu;bool PeekTC(const CTCQuestCompletionUI** out)const{if(quest.present==2)*out=&quest;return quest.present!=0;}bool PeekTC(const CTCInGameMenu** out)const{if(menu.present==2)*out=&menu;return menu.present!=0;}};
struct World{bool loading;bool IsPerformingRegionLoadReally()const{return loading;}};
struct Subtitle{bool cut,text;bool IsInCutscene()const{return cut;}bool IsDisplayingText()const{return text;}};
struct Hero{bool open;bool IsOpen()const{return open;}};
struct CPlayerGui{CPlayerGuiDef def;Subtitle sub;Hero hero;bool draw;bool GetDrawGuiFlag()const{return draw;}Hero* GetPHeroInfoScreensManager(){return &hero;}Subtitle& GetSubtitleRenderer(){return sub;}static CPlayerGui& Get();static const CPlayerGuiDef& PeekGuiDef();} gui;
CPlayerGui& CPlayerGui::Get(){return gui;}const CPlayerGuiDef& CPlayerGui::PeekGuiDef(){return gui.def;}
struct Display{bool fade;bool IsScreenFadeOut()const{return fade;}};
struct CMainGameComponent{Display display;CFontBank mainFont,cutFont;static CMainGameComponent* Get();Display& GetDisplayEngine(){return display;}const CFontBank& PeekMainFont()const{return mainFont;}const CFontBank& PeekCutSceneMainFont()const{return cutFont;}} game;
CMainGameComponent* CMainGameComponent::Get(){return &game;}
struct Ext{int X,Y;};struct CDisplayManager{Ext size;int reads;Ext GetRenderTargetDimensions(){++reads;return size;}} dm;static CDisplayManager* GPDisplayManager=&dm;
struct Recorded{C2DVector position;int text;const CFontBank* font;int alpha;};static Recorded records[2];static int nr;
namespace NInventory{struct CTCInventoryBase{static void Draw2DText(const C2DVector& pos,const CWideString& text,const CFontBank& font,const CRGBColour& col,float scale,bool shadow){ck(nr<2&&scale==1&&shadow&&col.r==255&&col.g==255&&col.b==255);records[nr].position=pos;records[nr].font=&font;records[nr].text=text.id;records[nr++].alpha=col.a;}};}
struct CTCRegionDisplay{CWideString CurrentAreaName,CurrentAreaNameExtra;long CurrentAlpha,LastAlpha;bool InfoDisplay;World world;CThing parent;bool IsDrawing()const{return CurrentAlpha>0&&CurrentAreaName!=L"";}const World& PeekWorld()const{return world;}const CThing& GetParentThing()const{return parent;}const CMainGameComponent& PeekComponent()const{return game;}void GlobalDraw(const CInterpolationInfo&)const;};
void CTCRegionDisplay::GlobalDraw(const CInterpolationInfo& interp) const
{
    if (!IsDrawing() || !CPlayerGui::Get().GetDrawGuiFlag()) return;
    long current_alpha = (long)GFGetInterpolatedValue((float)LastAlpha, (float)CurrentAlpha, interp.Current.WFInterpolate);
    if (!(CurrentAreaName != L"" || CurrentAreaNameExtra != L"") || current_alpha <= 0) return;
    if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen() && !PeekWorld().IsPerformingRegionLoadReally()) return;
    if (CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) return;
    const CTCQuestCompletionUI* ptc_quest_ui = 0;
    GetParentThing().PeekTC(&ptc_quest_ui);
    if (ptc_quest_ui && ptc_quest_ui->IsOpen()) return;
    const CTCInGameMenu* ptc_in_game_menu = 0;
    GetParentThing().PeekTC(&ptc_in_game_menu);
    if (ptc_in_game_menu && ptc_in_game_menu->IsOpen()) return;
    if (InfoDisplay) {
        if (CMainGameComponent::Get()->GetDisplayEngine().IsScreenFadeOut()) return;
        const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
        const CFontBank& font=PeekComponent().PeekMainFont();
        C2DVector display_pos(gui_def.InfoDisplayPos);
        float x=GFAXToVX(display_pos.X)-(font.GetVStringWidth(CurrentAreaName));
        float y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
        NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaName, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
    } else {
        if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) return;
        const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
        const CFontBank& font=PeekComponent().PeekCutSceneMainFont();
        C2DVector display_pos(gui_def.RegionDisplayPos);
        display_pos.X=GPDisplayManager->GetRenderTargetDimensions().X/2.0f;
        display_pos.Y=GPDisplayManager->GetRenderTargetDimensions().Y-(480.0f-display_pos.Y);
        float x=GFAXToVX(display_pos.X)-font.GetVStringWidth(CurrentAreaName)/2.0f;
        float y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
        NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaName, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
        if (CurrentAreaNameExtra != L"") {
            display_pos.Y+=font.GetFontMaxHeight();
            x=GFAXToVX(display_pos.X)-font.GetVStringWidth(CurrentAreaNameExtra)/2.0f;
            y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
            NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaNameExtra, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
        }
    }
}
int main(){
 CTCRegionDisplay r;CInterpolationInfo interp;
 gui.def.InfoDisplayPos=C2DVector(97.5f,58.0f);gui.def.RegionDisplayPos=C2DVector(7,250.0f);
 game.mainFont.height=12;game.cutFont.height=20;
 game.mainFont.widths[1]=29;game.cutFont.widths[1]=43;game.cutFont.widths[2]=22;
 unsigned long count=0;
 for(unsigned state=0;state<512;++state)
 for(unsigned qc=0;qc<4;++qc)for(unsigned menu=0;menu<4;++menu)
 for(unsigned nm=0;nm<4;++nm)for(unsigned al=0;al<4;++al){
  gui.draw=(state&1)!=0;gui.hero.open=(state&2)!=0;r.world.loading=(state&4)!=0;
  gui.sub.cut=(state&8)!=0;gui.sub.text=(state&16)!=0;game.display.fade=(state&32)!=0;
  r.InfoDisplay=(state&64)!=0;dm.size.X=(state&128)?801:640;dm.size.Y=(state&256)?601:480;
  r.parent.quest.present=qc<2?qc:2;r.parent.quest.open=qc==3;
  r.parent.menu.present=menu<2?menu:2;r.parent.menu.open=menu==3;
  r.CurrentAreaName.id=(nm&1)?1:0;r.CurrentAreaNameExtra.id=(nm&2)?2:0;
  r.CurrentAlpha=al?170:0;r.LastAlpha=al==1?-180:al==3?28:100;interp.Current.WFInterpolate=al==1?0.25f:0.5f;
  nr=0;dm.reads=0;r.GlobalDraw(interp);
  bool common=nm%2&&al>=2&&gui.draw&&(!gui.hero.open||r.world.loading)&&!gui.sub.cut&&qc!=3&&menu!=3;
  int expected=common?(r.InfoDisplay?(!game.display.fade):gui.sub.text?0:1+(r.CurrentAreaNameExtra.id!=0)):0;
  ck(nr==expected&&dm.reads==(expected&&!r.InfoDisplay?2:0));
  for(int i=0;i<nr;++i){
   const CFontBank* font=r.InfoDisplay?&game.mainFont:&game.cutFont;ck(records[i].font==font&&records[i].text==i+1);
   float wantedX=r.InfoDisplay?195.0f-font->widths[1]:(float)dm.size.X-font->widths[i+1]*0.5f;
   float wantedY=(r.InfoDisplay?58.0f:(float)dm.size.Y-230.0f+i*font->height)*3-font->height*1.5f;
   ck(records[i].position.X==wantedX&&records[i].position.Y==wantedY&&records[i].alpha==(al==3?99:135));
  }
  ++count;
 }ck(count==131072UL);puts("00639e40_TEST PASS");return 0;
}