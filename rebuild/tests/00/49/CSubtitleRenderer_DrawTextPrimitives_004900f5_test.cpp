#define private public
#define protected public
// 004900f5 CSubtitleRenderer::DrawTextPrimitives
// Leaf: ?DrawTextPrimitives@CSubtitleRenderer@@AAE?AVC2DBoxF@@ABVCEnginePrimitivePrepareInfo@@ABV2@1@Z
// Flags:
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_SUBTITLE_WRAP_BODY_PASS29
#define FABLE_RETAIL_CWSTRINGPARSER_INIT
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_SUBTITLE_WIDTH_DRAW_PASS29
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_CORNERPOS
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_C2DEXTENTSF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE
#define FABLE_RETAIL_CDISPLAYMANAGER
#include "engine/GFLimit.h"
#include "engine/Direct3D9Types.h"
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CWStringParser.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/SubtitleDrawGlobals.h"
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif

C2DBoxF CSubtitleRenderer::DrawTextPrimitives(const CEnginePrimitivePrepareInfo& info,const C2DBoxF& v_full_window,const C2DBoxF& v_television_window)
{
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    CRGBColour text_colour(255,255,255);
    if (IsFlagSet((ESubtitleFlag)8)) text_colour=gui_def.TextBoxTextGameInfoColour;
    if (Text.ToCharString().StartsWith("(HC)")) text_colour=CRGBColour(128,128,128);
    CWideString formatted_text;
    C2DBoxF text_window;
    float screen_width=(float)GPDisplayManager->GetRenderTargetDimensions().X;
    C2DBoxF text_area(GFAXToVX(gui_def.TextBoxTextAreaTLPos.X),
                      ((float)(VIRTUAL_SCREEN_HEIGHT-GFAYToVY(GetBlackBarHeight())-GFAYToVY(gui_def.TextBoxTextAreaTLPos.Y))),
                      GFAXToVX((float)(gui_def.TextBoxTextAreaBRPos.X+screen_width-640.0f)),
                      (float)(VIRTUAL_SCREEN_HEIGHT-GFAYToVY(GetBlackBarHeight())-GFAYToVY(gui_def.TextBoxTextAreaBRPos.Y)));
    FormatTextForVWindow(text_area,*PFont,Text,formatted_text,text_window);
    if (IsFlagSet((ESubtitleFlag)8)) {
        float text_window_height=text_window.GetHeight();
        text_window.TLY=(float)((VIRTUAL_SCREEN_HEIGHT-text_window.GetHeight())/2.0f);
        text_window.BRY=text_window.TLY+text_window_height;
    }
    GPRenderManager->SetVWindow(text_window);
    CWideString visible_text;
    GetVisibleText(formatted_text,visible_text);
    C2DVector text_pos(0,0);
    CWStringParser parser(formatted_text),visible_parser(visible_text);
    CWideString text_line=parser.ReadAsStringUntilCharacter(10);
    while(text_line!=L"") {
        CWideString visible_line=visible_parser.ReadAsStringUntilCharacter(10);
        if(visible_line==L"") break;
        text_pos.X=0;
        if(!GLeftAlignText) text_pos.X=(float)((text_window.GetWidth()-PFont->GetVStringWidth(text_line))/2.0f);
        C2DVector draw_pos=text_pos+text_window.GetTLPos();
        Draw2DText(draw_pos,visible_line,*PFont,text_colour,true,2.0f,1.0f,false);
        text_pos.Y+=PFont->GetVFontMaxHeight();
        parser.SkipWhitespace();visible_parser.SkipWhitespace();
        text_line=parser.ReadAsStringUntilCharacter(10);
    }
    DrawTextEmbeddedGraphics(info,text_window);DrawTextEmbeddedHighlights(info,text_window);
    GPRenderManager->SetVWindow(v_full_window);
    return text_window;
}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#undef private
#undef protected

#include "engine/CEnginePrimitivePrepareInfo.h"
#include <assert.h>
#include <stdio.h>
#include <wchar.h>

CDisplayManager* GPDisplayManager;
CRenderManager* GPRenderManager;
double VIRTUAL_SCREEN_HEIGHT;
bool GLeftAlignText;
const char CCharString::NullChar = 0;
template<class T> union FixtureStorage {
    double aligned;
    unsigned char bytes[sizeof(T)];
    T* ptr() { return reinterpret_cast<T*>(bytes); }
};
static FixtureStorage<CPlayerGuiDef> gui;
static FixtureStorage<CSubtitleRenderer> captions;
static FixtureStorage<CDisplayManager> display;
static FixtureStorage<CRenderManager> renderer;
static FixtureStorage<CEnginePrimitivePrepareInfo> info;
static C2DBoxF fullBox(0,0,512,384), lastWindow;
static int drawCount, windows, graphics, highlights, formattedCount, visibleCount, stage;
static int screenWidth, shade, flag, prefix, lineWidth, lineHeight, bar;
static unsigned int dataCompare;
static bool dataEmpty;
static const wchar_t* fullText;
static const wchar_t* visibleText;
static FixtureStorage<CWideStringData> widePool[40];
static const wchar_t* wideText[40];
static int wideUsed;
static CWideStringData* intern(const wchar_t* s) {
    if (!*s && !dataEmpty) return 0;
    assert(wideUsed < 40);
    wideText[wideUsed] = s;
    return widePool[wideUsed++].ptr();
}
static const wchar_t* view(const CWideStringData* p) {
    if (!p) return L"";
    for (int i=0;i<wideUsed;++i) if (widePool[i].ptr()==p) return wideText[i];
    assert(false); return L"";
}
static FixtureStorage<CCharStringData> narrowPool[12];
static int narrowUsed;
CCharString::CCharString(): PStringData(0) {}
CCharString::CCharString(const char* s,long n): PStringData(0) {
    if (!*s) return;
    assert(narrowUsed<12);
    PStringData=narrowPool[narrowUsed++].ptr();
    PStringData->Data.PData=const_cast<char*>(s);
    PStringData->Data.StringLength=n<0 ? (unsigned long)strlen(s):(unsigned long)n;
}
CCharString::CCharString(const CCharString& s): PStringData(s.PStringData) {}
CCharString::~CCharString() {}
CWideString::CWideString():PStringData(0){}
CWideString::CWideString(const wchar_t* s):PStringData(intern(s)){}
CWideString::CWideString(const CWideString& s):PStringData(s.PStringData){}
CWideString::~CWideString(){}
const CWideString& CWideString::operator=(const CWideString& s) {PStringData=s.PStringData;return *this;}
CWideString::operator const wchar_t*() const {return view(PStringData);}
const wchar_t* CWideString::GetAsCharArray() const {return view(PStringData);}
bool CWideString::operator!=(const wchar_t* other) const {return wcscmp(view(PStringData),other)!=0;}
CCharString CWideString::ToCharString() const {assert(this==&captions.ptr()->Text);return CCharString(prefix==2?"(HC)Voice":prefix==1?"(H)Voice":"");}
namespace std {
template<> bool operator==(const basic_string<unsigned short>& s,const unsigned short* t) {
    ++dataCompare;
    return wcscmp(view(reinterpret_cast<const CWideStringData*>(&s)),t)==0;
}
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CWStringParser::CWStringParser(const unsigned short* s):String(s),StringPos(0),Line(0) {}
CWideString CWStringParser::ReadAsStringUntilCharacter(unsigned short ch) {
    assert(ch==10);
    if (String==fullText && Line<(unsigned long)formattedCount) return CWideString(Line?L"word":L"abcdef");
    if (String==visibleText && Line<(unsigned long)visibleCount) return CWideString(Line?L"wo":L"ab");
    return CWideString(L"");
}
long CWStringParser::SkipWhitespace() {++Line;return 0;}
CFontBank::~CFontBank() {}
float FABLE_FASTCALL GFAXToVX(float x){return x*2;}
float FABLE_FASTCALL GFAYToVY(float y){return y*0.5f;}
bool CFontBank::Preload(const CWideString&,bool) const{return true;}
class FixtureFont : public CFontBank {
public:
    virtual C2DExtentsF GetStringExtents(const wchar_t* s,bool option) const {
        assert(!option);return C2DExtentsF((float)wcslen(s)*lineWidth,3);
    }
    virtual float GetFontMaxHeight() const {return (float)lineHeight;}
    virtual void PrepareToRender(bool) const {}
    virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*) const {}
};
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){return *gui.ptr();}
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){assert(this==GPDisplayManager);return C2DExtentsI(screenWidth,99);}
float CSubtitleRenderer::GetBlackBarHeight() const {return (float)bar;}
static bool sameBox(const C2DBoxF& a,const C2DBoxF& b){return a.TLX==b.TLX && a.TLY==b.TLY && a.BRX==b.BRX && a.BRY==b.BRY;}
static C2DBoxF expectedWindow(){
    return flag? C2DBoxF(10,(float)((VIRTUAL_SCREEN_HEIGHT-30)*0.5),90,(float)((VIRTUAL_SCREEN_HEIGHT+30)*0.5)):C2DBoxF(10,20,90,50);
}
void CSubtitleRenderer::FormatTextForVWindow(const C2DBoxF& area,const CFontBank& f,const CWideString& text,CWideString& formatted,C2DBoxF& box)const{
    assert(stage++==0 && &f==PFont && &text==&Text);
    assert(area.TLX==4 && area.BRX==(22+screenWidth-640)*2);
    assert(area.TLY==VIRTUAL_SCREEN_HEIGHT-(bar+4)*0.5f && area.BRY==VIRTUAL_SCREEN_HEIGHT-(bar+24)*0.5f);
    formatted=CWideString(fullText);box=C2DBoxF(10,20,90,50);
}
bool CSubtitleRenderer::GetVisibleText(const CWideString& formatted,CWideString& out){
    assert(stage++==2 && formatted.GetAsCharArray()==fullText);out=CWideString(visibleText);return visibleCount>0;
}
void CRenderManagerCore::SetVWindow(const C2DBoxF& b){
    if(!windows) {assert(stage++==1 && sameBox(b,expectedWindow()));}
    else {assert(stage==5 && sameBox(b,fullBox));++stage;}
    ++windows;lastWindow=b;
}
void CSubtitleRenderer::Draw2DText(const C2DVector& position,const CWideString& line,const CFontBank& font,const CRGBColour& c,bool on,float scale,float opacity,bool end){
    assert(stage==3 && &font==PFont && on && scale==2 && opacity==1 && !end);
    assert(!wcscmp(line.GetAsCharArray(),drawCount?L"wo":L"ab"));
    const unsigned int selected=prefix==2?128:flag?shade:255;
    assert(c.R==selected && c.G==(prefix==2?128:flag?shade+1:255) && c.B==(prefix==2?128:flag?shade+2:255) && c.A==255);
    const int units=drawCount?4:6;
    assert(position.X==10+(GLeftAlignText?0:(80-2*lineWidth*units)*0.5f));
    assert(position.Y==expectedWindow().TLY+drawCount*lineHeight*0.5f);
    ++drawCount;
}
void CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& i,const C2DBoxF& b){
    assert(stage++==3 && &i==info.ptr() && sameBox(b,expectedWindow()));++graphics;
}
void CSubtitleRenderer::DrawTextEmbeddedHighlights(const CEnginePrimitivePrepareInfo& i,const C2DBoxF& b){
    assert(stage++==4 && &i==info.ptr() && sameBox(b,expectedWindow()));++highlights;
}
int main(){
    FixtureFont font;
    fullText=L"full sentinel";visibleText=L"visible sentinel";
    GPDisplayManager=display.ptr();GPRenderManager=renderer.ptr();
    CSubtitleRenderer* row=captions.ptr();row->PFont=&font;
    CPlayerGuiDef* g=gui.ptr();g->TextBoxTextAreaTLPos=C2DVector(2,4);g->TextBoxTextAreaBRPos=C2DVector(22,24);
    C2DBoxF television(100,100,200,200);
    for(flag=0;flag<2;++flag) for(prefix=0;prefix<3;++prefix)
    for(formattedCount=0;formattedCount<3;++formattedCount) for(visibleCount=0;visibleCount<3;++visibleCount)
    for(int align=0;align<2;++align) for(int dims=0;dims<2;++dims) for(int empty=0;empty<2;++empty){
        windows=graphics=highlights=drawCount=stage=wideUsed=narrowUsed=0;
        GLeftAlignText=align!=0;dataEmpty=empty!=0;shade=dims?9:31;
        g->TextBoxTextGameInfoColour=CRGBColour((unsigned char)shade,(unsigned char)(shade+1),(unsigned char)(shade+2));
        screenWidth=dims?800:640;VIRTUAL_SCREEN_HEIGHT=dims?270:384;
        bar=dims?8:0;lineWidth=dims?3:2;lineHeight=dims?6:10;
        row->DisplayFlags=flag?8:0x20;
        C2DBoxF got=row->DrawTextPrimitives(*info.ptr(),fullBox,television);
        assert(stage==6 && windows==2 && graphics==1 && highlights==1 && drawCount==(formattedCount<visibleCount?formattedCount:visibleCount));
        assert(sameBox(got,expectedWindow()) && sameBox(lastWindow,fullBox));
    }
    assert(dataCompare!=0);
    puts("004900f5_TEST PASS");return 0;
}