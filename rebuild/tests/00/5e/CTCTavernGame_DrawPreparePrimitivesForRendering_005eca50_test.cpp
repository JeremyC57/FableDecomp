// VC7.1 /Od fixture. The production members run verbatim against observable draw and service interfaces.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#define FABLE_RETAIL_ASSERT(c) ((void)0)

class CWideString {
public:
    explicit CWideString(const wchar_t* value) : value(value) {}
    bool operator!=(const wchar_t* rhs) const { return wcscmp(value,rhs) != 0; }
    const wchar_t* value;
};
struct CRGBColour {
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
        : r(r),g(g),b(b),a(a) {}
    unsigned char r,g,b,a;
};
struct CFontBank {
    explicit CFontBank(float height) : height(height),reads(0) {}
    float GetFontMaxHeight() { ++reads; return height; }
    float height;
    int reads;
};
struct CFontManager {
    CFontManager() : font(13),requests(0) {}
    CFontBank* GetFont(const char* name) {
        assert(strcmp(name,"ENG_ARIAL_16")==0);
        ++requests;
        return &font;
    }
    CFontBank font;
    int requests;
};
static CFontManager font_manager;
static CFontManager* PFontManager=&font_manager;

struct C2DVector {
    C2DVector(float x,float y) : x(x),y(y) {}
    float x,y;
};
struct C2DBoxF {
    C2DBoxF(float left,float top,float right,float bottom) : left(left),top(top),right(right),bottom(bottom) {}
    float left,top,right,bottom;
};
struct C2DExtentsI { int X,Y; };
struct CDisplayManager {
    CDisplayManager() : queries(0) {}
    C2DExtentsI GetRenderTargetDimensions() {
        ++queries;
        C2DExtentsI extents = { 1280, 720 };
        return extents;
    }
    unsigned queries;
};
static CDisplayManager display;
static CDisplayManager* GPDisplayManager=&display;
enum { ENGINE_2D_LAYER_6=0x4a, ENGINE_PRIMITIVE_BLEND_ALPHA=2, ENGINE_INTERPOLATION_SET_0=0 };

struct CEnginePrimitive2DSprite {
    CEnginePrimitive2DSprite(unsigned __int64 texture,const C2DBoxF& clip,int layer,unsigned long left,unsigned long top,
        int blend,int interpolation,float u,float v,unsigned char alpha,unsigned char red,unsigned char green,
        unsigned char blue,unsigned long flags,const C2DVector& start,const C2DVector& end)
        : clip(clip) {
        assert(!texture && layer==0x4a && !left && !top && blend==2 && !interpolation);
        assert(u==0 && v==0 && alpha==255 && !red && !green && !blue && !flags);
        assert(start.x==0 && start.y==0 && end.x==0 && end.y==0);
    }
    C2DBoxF clip;
};
struct HandleList {
    HandleList() : count(2),clears(0),item(55) {}
    void clear() { count=0; ++clears; }
    void push_back() { assert(count==0); ++count; item=0; }
    unsigned long& back() { assert(count==1); return item; }
    unsigned count,clears;
    unsigned long item;
};
struct FixtureEngine {
    FixtureEngine() : calls(0) {}
    void SetPersistentPrimitive(unsigned long& handle,const CEnginePrimitive2DSprite& sprite,unsigned long flags) {
        assert(sprite.clip.left==0 && sprite.clip.top==0 && sprite.clip.right==1280 && sprite.clip.bottom==720);
        assert(flags==0x40 && handle==0);
        ++calls;
        handle=301;
    }
    unsigned calls;
};
struct CEnginePrimitivePrepareInfo {
    explicit CEnginePrimitivePrepareInfo(FixtureEngine& engine) : Engine(engine) {}
    FixtureEngine& Engine;
};

struct CTavernGameDef {
    float ScoreX,ScoreY,TargetX,TargetY,BestX,BestY,AdditionalX,AdditionalY;
    float BetX,BetY,MoneyX,MoneyY,WinningsX,WinningsY;
    float MainBetX,MainBetY,MainMoneyX,MainMoneyY;
    bool DisplayErrata;
};
template<class T> struct CDefPointer {
    CDefPointer() : value(0) {}
    const T* operator->() const { assert(value); return value; }
    const T* value;
};

static double tick;
static unsigned time_reads;
double GFGetTime() { ++time_reads; return ++tick; }

class CTCTavernGame {
public:
    enum ETavernGameStates {
        ETGS_NONE=0,ETGS_GREETING=1,ETGS_OPTIONS=2,ETGS_INSTRUCTIONS=3,ETGS_BETTING=4,ETGS_GAME=5,
        ETGS_UPDATE_VALUES=9,ETGS_FAREWELL=10,ETGS_QUIT_MID_GAME=11,ETGS_WAIT_FOR_CLICK=13,
        ETGS_CARDS_MAIN_GAME=23,ETGS_WAIT_FOR_YES_NO=45,ETGS_LEAVE=47
    };
    CTCTavernGame(const CTavernGameDef& def,const CEnginePrimitivePrepareInfo& info)
      : definition(def), expected_info(info), LastRenderTime(7), IsBeingPlayed(true),
        PScoreFont(&score), PTargetFont(&target), PStatsFont(&stats), score(10), target(11), stats(12),
        TavernGameState(ETGS_BETTING), PreviousTavernGameState(ETGS_NONE), YesState(ETGS_NONE), AltState(ETGS_NONE),
        string_reads(0), texts(0), draw_bet(0), clear_bet(0), boxes(0), clear_boxes(0), tail(0) {
        memset(coordinates,0,sizeof(coordinates));
    }
    bool IsCurrentOrUpcomingState(ETavernGameStates state);
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
    void GetDef(CDefPointer<const CTavernGameDef>* pdef) { pdef->value=&definition; }
    ETavernGameStates GetGameState() { return TavernGameState; }
    virtual CWideString GetCurrentScore() { assert(string_reads++==0); return CWideString(L"score"); }
    virtual CWideString GetBestScore() { assert(string_reads++==1); return CWideString(L"best"); }
    virtual CWideString GetRequiredScore() { assert(string_reads++==2); return CWideString(L"required"); }
    virtual CWideString GetAdditionalInfo() { assert(string_reads++==3); return CWideString(L"additional"); }
    virtual CWideString GetBet() { assert(string_reads++==4); return CWideString(L"bet"); }
    virtual CWideString GetPlayersMoney() { assert(string_reads++==5); return CWideString(L"money"); }
    virtual CWideString GetTotalWinnings() { assert(string_reads++==6); return CWideString(L"winnings"); }
    virtual CWideString GetWinPhrase() { ++tail; return CWideString(L"winner"); }

    void RenderText(const CEnginePrimitivePrepareInfo& info,float x,float y,CWideString text,const CFontBank& font,CRGBColour color) {
        assert(&info==&expected_info && color.r==255 && color.g==255 && color.b==255 && color.a==255);
        static const wchar_t* names[]={L"score",L"required",L"best",L"additional"};
        assert(texts<4 && !(text!=names[texts]));
        assert(x==1+2*texts && y==2+2*texts);
        assert(&font==(texts==0 ? PScoreFont : (texts==1 ? PTargetFont : PStatsFont)));
        ++texts;
    }
    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info,float bx,float by,float mx,float my,float wx,float wy) {
        assert(&info==&expected_info && draw_bet==0);
        ++draw_bet;
        coordinates[0]=bx; coordinates[1]=by; coordinates[2]=mx;
        coordinates[3]=my; coordinates[4]=wx; coordinates[5]=wy;
    }
    void ClearBetAndMoney() { ++clear_bet; }
    void DrawBox(const CEnginePrimitivePrepareInfo& info) { assert(&info==&expected_info); ++boxes; }
    void ClearBox() { ++clear_boxes; }

    const CTavernGameDef& definition;
    const CEnginePrimitivePrepareInfo& expected_info;
    HandleList Handles;
    double LastRenderTime;
    bool IsBeingPlayed;
    CFontBank* PScoreFont;
    CFontBank* PTargetFont;
    CFontBank* PStatsFont;
    CFontBank score,target,stats;
    ETavernGameStates TavernGameState,PreviousTavernGameState,YesState,AltState;
    unsigned string_reads,texts,draw_bet,clear_bet,boxes,clear_boxes,tail;
    float coordinates[6];
};

bool CTCTavernGame::IsCurrentOrUpcomingState(ETavernGameStates state)
{
    return TavernGameState == state || YesState == state || AltState == state;
}

void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<const CTavernGameDef> pdef;
    GetDef(&pdef);
    Handles.clear();
    float time_step = (float)(GFGetTime() - LastRenderTime);
    LastRenderTime = GFGetTime();
    if (IsBeingPlayed) {
        CWideString current_score = GetCurrentScore();
        CWideString best_score = GetBestScore();
        CWideString required_score = GetRequiredScore();
        CWideString additional_info = GetAdditionalInfo();
        CWideString bet = GetBet();
        CWideString players_money = GetPlayersMoney();
        CWideString total_winnings = GetTotalWinnings();
        if (!(IsCurrentOrUpcomingState(ETGS_GREETING) || IsCurrentOrUpcomingState(ETGS_OPTIONS) ||
              IsCurrentOrUpcomingState(ETGS_INSTRUCTIONS) || IsCurrentOrUpcomingState(ETGS_FAREWELL) ||
              IsCurrentOrUpcomingState(ETGS_LEAVE) || IsCurrentOrUpcomingState(ETGS_GAME)) ||
            PreviousTavernGameState == ETGS_UPDATE_VALUES) {
            bool betting = TavernGameState == ETGS_WAIT_FOR_YES_NO && YesState == ETGS_GAME;
            RenderText(info, pdef->ScoreX, pdef->ScoreY, current_score, *PScoreFont, CRGBColour(255, 255, 255, 255));
            RenderText(info, pdef->TargetX, pdef->TargetY, required_score, *PTargetFont, CRGBColour(255, 255, 255, 255));
            RenderText(info, pdef->BestX, pdef->BestY, best_score, *PStatsFont, CRGBColour(255, 255, 255, 255));
            if (!betting)
                RenderText(info, pdef->AdditionalX, pdef->AdditionalY, additional_info, *PStatsFont, CRGBColour(255, 255, 255, 255));
            if (pdef->DisplayErrata || GetGameState() != ETGS_CARDS_MAIN_GAME)
                DrawBetAndMoney(info, pdef->BetX, pdef->BetY, pdef->MoneyX, pdef->MoneyY, pdef->WinningsX, pdef->WinningsY);
            else
                ClearBetAndMoney();
            DrawBox(info);
        } else {
            if (YesState == ETGS_GAME) {
                DrawBetAndMoney(info, pdef->MainBetX, pdef->MainBetY, pdef->MainMoneyX, pdef->MainMoneyY, 0.0f, 0.0f);
                ClearBox();
            } else {
                ClearBetAndMoney();
                ClearBox();
            }
        }
        if (GetGameState() == ETGS_WAIT_FOR_YES_NO && YesState == ETGS_QUIT_MID_GAME) {
            C2DExtentsI extents = GPDisplayManager->GetRenderTargetDimensions();
            C2DBoxF clip_box(0.0f, 0.0f, (float)extents.X, (float)extents.Y);
            CEnginePrimitive2DSprite background_primitive(0, clip_box, ENGINE_2D_LAYER_6, 0, 0,
                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 0, 0, 0, 0,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            Handles.push_back();
            info.Engine.SetPersistentPrimitive(Handles.back(), background_primitive, 0x40);
        }
        float boundary = PFontManager->GetFont("ENG_ARIAL_16")->GetFontMaxHeight();
        float dist_between_boxes = (float)(boundary * 4.0);
        float base_dist = (float)(boundary * 6.0);
        CWideString game_state = GetWinPhrase();
        game_state != L"";
        GetGameState();
        float control_box_y = (float)(480.0 - base_dist);
        switch (GetGameState()) {
        case ETGS_WAIT_FOR_CLICK:
            FABLE_RETAIL_ASSERT(0);
            break;
        }
    }
}

int main()
{
    FixtureEngine engine;
    const CEnginePrimitivePrepareInfo info(engine);
    CTavernGameDef definition={1,2,3,4,5,6,7,8,11,12,13,14,15,16,21,22,23,24,false};
    typedef CTCTavernGame Game;
    {
        Game inactive(definition,info);
        inactive.IsBeingPlayed=false;
        inactive.DrawPreparePrimitivesForRendering(info);
        assert(inactive.Handles.clears==1 && inactive.Handles.count==0);
        assert(time_reads==2 && inactive.LastRenderTime==2);
        assert(!inactive.string_reads && !inactive.tail && !font_manager.requests);
    }
    {
        Game ordinary(definition,info);
        ordinary.DrawPreparePrimitivesForRendering(info);
        assert(ordinary.texts==4 && ordinary.draw_bet==1 && ordinary.boxes==1);
        assert(ordinary.coordinates[0]==11 && ordinary.coordinates[5]==16 && !ordinary.clear_bet);
        assert(ordinary.string_reads==7 && ordinary.tail==1 && ordinary.LastRenderTime==4);
    }
    {
        Game betting(definition,info);
        betting.TavernGameState=Game::ETGS_WAIT_FOR_YES_NO;
        betting.YesState=Game::ETGS_GAME;
        betting.PreviousTavernGameState=Game::ETGS_UPDATE_VALUES;
        betting.DrawPreparePrimitivesForRendering(info);
        assert(betting.texts==3 && betting.draw_bet==1 && betting.boxes==1 && !betting.clear_boxes);
    }
    {
        Game greeting(definition,info);
        greeting.TavernGameState=Game::ETGS_GREETING;
        greeting.YesState=Game::ETGS_GAME;
        greeting.DrawPreparePrimitivesForRendering(info);
        assert(greeting.texts==0 && greeting.draw_bet==1 && greeting.clear_boxes==1 && !greeting.boxes);
        assert(greeting.coordinates[0]==21 && greeting.coordinates[3]==24 && greeting.coordinates[4]==0 && greeting.coordinates[5]==0);
    }
    {
        Game instructions(definition,info);
        instructions.AltState=Game::ETGS_INSTRUCTIONS;
        instructions.DrawPreparePrimitivesForRendering(info);
        assert(instructions.texts==0 && instructions.draw_bet==0 && instructions.clear_bet==1 && instructions.clear_boxes==1);
    }
    {
        Game cards(definition,info);
        cards.TavernGameState=Game::ETGS_CARDS_MAIN_GAME;
        cards.DrawPreparePrimitivesForRendering(info);
        assert(cards.texts==4 && cards.draw_bet==0 && cards.boxes==1 && cards.clear_bet==1);
        definition.DisplayErrata=true;
        Game errata(definition,info);
        errata.TavernGameState=Game::ETGS_CARDS_MAIN_GAME;
        errata.DrawPreparePrimitivesForRendering(info);
        assert(errata.texts==4 && errata.draw_bet==1 && errata.boxes==1 && !errata.clear_bet);
    }
    {
        Game quit(definition,info);
        quit.TavernGameState=Game::ETGS_WAIT_FOR_YES_NO;
        quit.YesState=Game::ETGS_QUIT_MID_GAME;
        quit.DrawPreparePrimitivesForRendering(info);
        assert(quit.Handles.clears==1 && quit.Handles.count==1 && quit.Handles.item==301);
        assert(engine.calls==1 && display.queries==1 && quit.tail==1);
    }
    assert(time_reads==16 && font_manager.requests==7 && font_manager.font.reads==7);
    puts("005eca50_TEST PASS");
    return 0;
}