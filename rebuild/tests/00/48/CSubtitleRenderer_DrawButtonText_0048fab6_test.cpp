#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>
static void check(bool b){if(!b)abort();}
typedef int ESubtitleFlag;
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
static float GFAXToVX(float f){return f*2;}static float GFAYToVY(float f){return f*3;}
static void GFACoordToVCoord(const C2DVector&in,C2DVector&out){out=C2DVector(in.X*2,in.Y*3);}
struct CRGBColour{unsigned char B,G,R,A;};
struct CCharString{const char*p;CCharString(const char*s):p(s){}};
struct CWideString{std::wstring s;CWideString(const wchar_t*t=L""):s(t){}bool operator!=(const wchar_t*t)const{return s!=t;}};
static CWideString operator+(const CWideString&a,const CWideString&b){CWideString r;r.s=a.s+b.s;return r;}
struct CFontBank{C2DVector GetVStringExtents(const CWideString&t)const{return C2DVector((float)t.s.size()*3+1,5);}float GetVStringWidth(const CWideString&t)const{return GetVStringExtents(t).X;}}font;
struct CPlayerGuiDef{long TextBoxButtonTextFontName;float TextBoxButtonInBoxOffsetY,TextBoxButtonOffsetY,TextBoxButtonGapWidth;C2DVector TextBoxTextAreaBRPos,TextBoxFishingReelTextPos;CRGBColour TextBoxTextColour;}def;
struct FontManager{const CFontBank*GetFont(long x){check(x==17);return &font;}}fm,*PFontManager=&fm;
struct Display{C2DVector GetRenderTargetDimensions()const{return C2DVector(320,180);}}display,*GPDisplayManager=&display;
static bool fishing,respawn,skip,ready;enum {MINIGAME_FISHING=1};
struct Quest{int GetMiniGameType()const{return fishing?MINIGAME_FISHING:0;}}quest;
struct CPlayerGui{static CPlayerGui&Get(){static CPlayerGui p;return p;}static const CPlayerGuiDef&PeekGuiDef(){return def;}const Quest&GetQuestInfo(){return quest;}};
namespace NPlayerGui{C2DVector GFAnchorToTopRight(const C2DVector&p){return C2DVector(p.X+20,p.Y+10);}}
struct TextBank{CWideString GetTextBySymbol(const CCharString&c){check(!strcmp(c.p,"TEXT_GUI_CONTROL_ESC_ABBREVIATED"));return CWideString(L"ESC");}}bank;
struct CPlayer{long GetPlayerRespawnDelay()const{return respawn?8:0;}}player;
struct PlayerManager{CPlayer&GetMainPlayer(){return player;}}pm;
struct CMainGameComponent{static CMainGameComponent*Get(){static CMainGameComponent c;return &c;}TextBank&GetTextBank(){return bank;}PlayerManager&GetPlayerManager(){return pm;}};
struct Call{C2DVector pos;std::wstring text;unsigned char alpha;float scale;};static Call calls[8],wanted[8];static int count,wcount;
struct CSubtitleRenderer{CWideString Button1Text,Button2Text,Button3Text,Button2OverrideText,Button1OverrideText;unsigned char Button1Alpha,Button2Alpha;
 bool InCutscene,Button2TextOverridden,Button1TextOverridden,action;int flags;
 bool IsInCutscene()const{return InCutscene;}bool IsCutsceneActionModeEnabled()const{return action;}bool IsFlagSet(ESubtitleFlag n)const{return (flags&n)!=0;}bool IsCutsceneSkippable()const{return skip;}bool IsTextReadyForClickPast()const{return ready;}
 float GetTextWidthFromText(const CWideString&t,const CFontBank&f)const{check(&f==&font);return f.GetVStringWidth(t);}
 void Draw2DText(const C2DVector&p,const CWideString&t,const CFontBank&f,const CRGBColour&c,bool shadow,float x,float y,bool centre){check(&f==&font&&shadow&&x==1&&!centre&&c.R==9&&c.G==27&&c.B==15&&count<8);Call& a=calls[count++];a.pos=p;a.text=t.s;a.alpha=c.A;a.scale=y;}
 bool DrawButtonText(const C2DVector&,bool);
};
static void expect(float x,float y,const std::wstring&s,unsigned char a,float scale=1){check(wcount<8);Call& c=wanted[wcount++];c.pos=C2DVector(x,y);c.text=s;c.alpha=a;c.scale=scale;}
bool CSubtitleRenderer::DrawButtonText(const C2DVector& br_pos, bool drawn_box)
{
    C2DVector vbr_pos;
    GFACoordToVCoord(br_pos, vbr_pos);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CFontBank& font = *PFontManager->GetFont(gui_def.TextBoxButtonTextFontName);
    C2DVector cancel_pos(0.0f, vbr_pos.Y);
    C2DVector confirm_pos(0.0f, vbr_pos.Y);
    C2DVector third_pos(0.0f, vbr_pos.Y);
    if (drawn_box) {
        cancel_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
        confirm_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
        third_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
    } else {
        cancel_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
        confirm_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
        third_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
    }
    float cancel_text_width = GetTextWidthFromText(Button1Text, font);
    float confirm_text_width = GetTextWidthFromText(Button2Text, font);
    float third_text_width = GetTextWidthFromText(Button3Text, font);
    if (IsCutsceneActionModeEnabled())
        cancel_pos.X = GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X + gui_def.TextBoxTextAreaBRPos.X - 640.0f) + GFAXToVX(16.0f);
    else
        cancel_pos.X = GFAXToVX(16.0f) + vbr_pos.X - cancel_text_width;
    confirm_pos.X = cancel_pos.X - confirm_text_width - GFAXToVX(gui_def.TextBoxButtonGapWidth);
    third_pos.X = confirm_pos.X - third_text_width - GFAXToVX(gui_def.TextBoxButtonGapWidth);
    CRGBColour text_colour = gui_def.TextBoxTextColour;
    bool drawn = false;
    bool drawn_button_1 = false;
    if (IsCutsceneActionModeEnabled()) {
        drawn = true;
    } else if (IsFlagSet((ESubtitleFlag)4)) {
        text_colour.A = Button1Alpha;
        Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
        drawn_button_1 = true;
        text_colour.A = Button2Alpha;
        Draw2DText(confirm_pos, Button2Text, font, text_colour, true, 1.0f, 1.0f, false);
        text_colour.A = 0xff;
        Draw2DText(third_pos, Button3Text, font, text_colour, true, 1.0f, 1.0f, false);
        drawn = true;
    } else if ((IsInCutscene() || Button2TextOverridden) && !(IsFlagSet((ESubtitleFlag)0x40) || IsFlagSet((ESubtitleFlag)8))) {
        if (IsCutsceneSkippable() || Button2TextOverridden) {
            CWideString button_2_override_text(Button2OverrideText);
            if (button_2_override_text != L"" && !IsCutsceneActionModeEnabled()) {
                button_2_override_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CONTROL_ESC_ABBREVIATED") + L" - " + button_2_override_text;
            }
            // Retail expands the two width accessors here and calls extents at 0048FDD8.
            cancel_pos.X = vbr_pos.X - font.GetVStringExtents(button_2_override_text).X;
            text_colour.A = Button2Alpha;
            float scale = 1.0f;
            if (IsInCutscene()) {
                text_colour.A = 0xd8;
                cancel_pos.X += GFAXToVX(55.0f);
                cancel_pos.Y = cancel_pos.Y - GFAXToVX(2.0f);
                scale = 0.75f;
            }
            if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType() == MINIGAME_FISHING)
                cancel_pos.X += GFAXToVX(55.0f);
            Draw2DText(cancel_pos, button_2_override_text, font, text_colour, true, 1.0f, scale, false);
            drawn = true;
        }
    } else if (IsFlagSet((ESubtitleFlag)2)) {
        if ((!IsInCutscene() && IsTextReadyForClickPast()) || (IsFlagSet((ESubtitleFlag)0x40) || IsFlagSet((ESubtitleFlag)8))) {
            text_colour.A = Button1Alpha;
            Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
            drawn = true;
            drawn_button_1 = true;
        }
    } else if (!IsInCutscene()) {
        const CPlayer& player_hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();
        if (player_hero.GetPlayerRespawnDelay() == 0) {
            if (Button1Text != L"") {
                text_colour.A = Button1Alpha;
                Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
                drawn_button_1 = true;
            }
            if (Button2Text != L"") {
                text_colour.A = Button2Alpha;
                Draw2DText(confirm_pos, Button2Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
            }
            if (Button3Text != L"") {
                text_colour.A = 0xff;
                Draw2DText(third_pos, Button3Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
            }
        }
    }
    if (!drawn_button_1 && Button1TextOverridden) {
        confirm_pos.X = cancel_pos.X - GetTextWidthFromText(Button1OverrideText, font) - GFAXToVX(gui_def.TextBoxButtonGapWidth);
        if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType() == MINIGAME_FISHING) {
            C2DVector top_right_pos = NPlayerGui::GFAnchorToTopRight(gui_def.TextBoxFishingReelTextPos);
            confirm_pos.X = GFAXToVX(top_right_pos.X);
            confirm_pos.Y = GFAXToVX(top_right_pos.Y);
            float actual_width = GetTextWidthFromText(Button1OverrideText, font);
            if ((float)(confirm_pos.X + actual_width) > GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X)) {
                float diff = confirm_pos.X + actual_width - GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X);
                confirm_pos.X -= diff + GFAXToVX(4.0f);
            }
        }
        text_colour.A = Button1Alpha;
        Draw2DText(confirm_pos, Button1OverrideText, font, text_colour, true, 1.0f, 1.0f, false);
        drawn = true;
        drawn_button_1 = true;
    }
    return drawn;
}
static unsigned rng=71563;static unsigned next(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}static long width(const std::wstring&s){return 1+3*(long)s.length();}
int main(){
 def.TextBoxButtonTextFontName=17;def.TextBoxButtonInBoxOffsetY=9;def.TextBoxButtonOffsetY=4;def.TextBoxButtonGapWidth=6;def.TextBoxTextAreaBRPos=C2DVector(544,0);
 def.TextBoxTextColour.R=9;def.TextBoxTextColour.G=27;def.TextBoxTextColour.B=15;CSubtitleRenderer r;r.Button1Alpha=83;r.Button2Alpha=114;
 unsigned hits[10]={0};
 for(unsigned run=0;run<65536;++run){unsigned bits=next(); r.action=(bits&1)!=0;r.InCutscene=(bits&2)!=0;r.Button1TextOverridden=(bits&4)!=0;r.Button2TextOverridden=(bits&8)!=0;ready=(bits&16)!=0;skip=(bits&32)!=0;fishing=(bits&64)!=0;respawn=(bits&128)!=0;bool panel=(bits&256)!=0;r.flags=(bits>>9)&0x4e;
 r.Button1Text=CWideString(bits&0x20000?L"one":L"");r.Button2Text=CWideString(bits&0x40000?L"second":L"");r.Button3Text=CWideString(bits&0x80000?L"third":L"");r.Button1OverrideText=CWideString(L"alternate");r.Button2OverrideText=CWideString(bits&0x100000?L"skip":L"");
 def.TextBoxFishingReelTextPos=C2DVector(bits&0x200000?305.0f:170.0f,21);
 C2DVector br(266,151);float y=453+(panel?27:12);float x1=r.action?480.0f:564.0f-width(r.Button1Text.s);float x2=x1-width(r.Button2Text.s)-12;float x3=x2-width(r.Button3Text.s)-12;
 count=wcount=0;bool drawn=r.action,first=false;
 if(r.action)++hits[0];else if(r.flags&4){++hits[1];expect(x1,y,r.Button1Text.s,83);expect(x2,y,r.Button2Text.s,114);expect(x3,y,r.Button3Text.s,255);drawn=first=true;}
 else if((r.InCutscene||r.Button2TextOverridden)&&!(r.flags&72)){++hits[2];if(skip||r.Button2TextOverridden){++hits[3];std::wstring label=r.Button2OverrideText.s;if(!label.empty())label=L"ESC - "+label;x1=532.0f-width(label);float sy=y;if(r.InCutscene){x1+=110;sy-=4;y=sy;}if(fishing)x1+=110;expect(x1,sy,label,r.InCutscene?216:114,r.InCutscene?0.75f:1.0f);drawn=true;}}
 else if(r.flags&2){++hits[4];if((!r.InCutscene&&ready)||(r.flags&72)){expect(x1,y,r.Button1Text.s,83);drawn=first=true;}}
 else if(!r.InCutscene&&!respawn){++hits[5];if(r.Button1Text.s.size()){expect(x1,y,r.Button1Text.s,83);drawn=first=true;}if(r.Button2Text.s.size()){expect(x2,y,r.Button2Text.s,114);drawn=true;}if(r.Button3Text.s.size()){expect(x3,y,r.Button3Text.s,255);drawn=true;}}
 if(!first&&r.Button1TextOverridden){++hits[6];float w=(float)width(r.Button1OverrideText.s);float x=x1-w-12;float ey=453+(panel?27:12);if(fishing){++hits[7];x=2*(def.TextBoxFishingReelTextPos.X+20);ey=62;if(x+w>640){++hits[8];x=640-w-8;}else ++hits[9];}expect(x,ey,r.Button1OverrideText.s,83);drawn=true;}
 check(r.DrawButtonText(br,panel)==drawn);check(count==wcount);for(int i=0;i<count;++i){check(calls[i].pos.X==wanted[i].pos.X&&calls[i].pos.Y==wanted[i].pos.Y&&calls[i].text==wanted[i].text&&calls[i].alpha==wanted[i].alpha&&calls[i].scale==wanted[i].scale);}
 }
 for(int i=0;i<10;++i)check(hits[i]!=0);puts("0048fab6_TEST PASS");return 0;
}