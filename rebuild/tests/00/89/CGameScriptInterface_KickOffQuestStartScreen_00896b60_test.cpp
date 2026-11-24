#include <stdio.h>
typedef const char* CCharString;
enum ETutorialCategory { TUTORIAL_CATEGORY_QUEST_CARD=28 };
enum ETCInterfaceType { TCI_HERO=41 };
namespace NGlobalConsole { bool GEnableRegionLockingSaveSystem; }

struct CASuspendableProcess {
    bool stop;
    static CASuspendableProcess* GetPActiveProcess(){static CASuspendableProcess p;return &p;}
    bool IsSuspendableProcessStopping() const {return stop;}
};
struct CTCHero {
    int tutorials;
    void GiveTutorialForCategory(ETutorialCategory category,bool forced) {
        tutorials+=(category==TUTORIAL_CATEGORY_QUEST_CARD&&!forced)?1:100;
    }
};
struct CTCQuestCompletionUI {
    enum CompletionType {QUEST_START=0};
    bool active,quitting,fade;
    int saved,activations;
    const char* name;
    void SaveInitialValues(const CCharString& s){++saved;name=s;}
    void Activate(CompletionType t,const CCharString& s,bool first,const wchar_t* summary,bool last){
        activations+=(t==QUEST_START && first && !*summary && last)?1:100;name=s;
    }
    bool IsActive()const{return active;}
    bool IsQuitting()const{return quitting;}
    bool IsQuittingFadeOut()const{return fade;}
};
static CTCQuestCompletionUI ui;
struct CThingPlayerCreature {
    bool dead;CTCHero* hero;
    bool IsDead()const{return dead;}
    bool GetTC(CTCQuestCompletionUI** out){*out=&ui;return true;}
    bool GetTC(ETCInterfaceType id,CTCHero** out){*out=(id==TCI_HERO)?hero:0;return *out!=0;}
};
static bool IS_THING_ALIVE(CThingPlayerCreature* p){return p&&!p->IsDead();}
struct CPlayer {CThingPlayerCreature* player;CThingPlayerCreature* GetPControlledCreature(){return player;}};
struct CPlayerManager {
    CPlayer first,second;unsigned calls;
    CPlayer& GetMainPlayer(){return calls++?second:first;}
};
struct CQuestManager {
    int starts; const char* name;
    static CQuestManager& Get(){static CQuestManager q;return q;}
    void OnQuestStartScreenDisplayed(const CCharString& s){++starts;name=s;}
};
struct CWorld {int saves;void AutoSaveQuestStart(){++saves;}};
struct CGameScriptInterface {
    CPlayerManager* PlayerManager;CWorld* World;bool controlled,stop_on_frame;mutable unsigned frames;
    bool IsHeroControlledByPlayer()const{return controlled;}
    void NewScriptFrame()const {
        ++frames;ui.active=false;
        if(stop_on_frame)CASuspendableProcess::GetPActiveProcess()->stop=true;
    }
    void KickOffQuestStartScreen(const CCharString&,bool,bool)const;
};
void CGameScriptInterface::KickOffQuestStartScreen(const CCharString& script_name, bool display_screen,
    bool pretend_screen_has_been_displayed_for_save_load_purposes) const
{
    if (!display_screen) {
        CTCQuestCompletionUI* ptc_ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_ui);
        ptc_ui->SaveInitialValues(script_name);
    }
    if (display_screen) {
        CTCQuestCompletionUI* ptc_ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_ui);
        ptc_ui->Activate(CTCQuestCompletionUI::QUEST_START, script_name, true, L"", true);
        if (IsHeroControlledByPlayer()) {
            while (ptc_ui->IsActive() && !ptc_ui->IsQuitting() && !ptc_ui->IsQuittingFadeOut() &&
                   !CASuspendableProcess::GetPActiveProcess()->IsSuspendableProcessStopping())
                NewScriptFrame();
            if (CASuspendableProcess::GetPActiveProcess()->IsSuspendableProcessStopping())
                return;
        }
        CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            CTCHero* ptc_hero = 0;
            phero->GetTC(TCI_HERO, &ptc_hero);
            if (ptc_hero)
                ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_QUEST_CARD, false);
        }
    } else if (pretend_screen_has_been_displayed_for_save_load_purposes) {
        if (NGlobalConsole::GEnableRegionLockingSaveSystem) {
            CQuestManager::Get().OnQuestStartScreenDisplayed(script_name);
            World->AutoSaveQuestStart();
        }
    }
}

int main() {
    const CCharString script="quest";
    for (unsigned bits=0;bits<2048;++bits) {
        const bool display=(bits&1)!=0, pretend=(bits&2)!=0, enabled=(bits&4)!=0;
        const bool controlled=(bits&8)!=0, active=(bits&16)!=0, quitting=(bits&32)!=0, fade=(bits&64)!=0;
        const bool stop=(bits&128)!=0, stop_on_frame=(bits&256)!=0;
        const unsigned who=(bits>>9)&3;
        CTCHero hero={0};
        CThingPlayerCreature player={who==2,who==3?0:&hero};
        CPlayerManager pm={{&player},{who==1?0:&player},0};
        CWorld world={0};
        CGameScriptInterface game={&pm,&world,controlled,stop_on_frame,0};
        CTCQuestCompletionUI fresh={active,quitting,fade,0,0,0};
        ui=fresh;CQuestManager::Get().starts=0;CQuestManager::Get().name=0;
        NGlobalConsole::GEnableRegionLockingSaveSystem=enabled;
        CASuspendableProcess::GetPActiveProcess()->stop=stop;
        game.KickOffQuestStartScreen(script,display,pretend);
        const bool yielded=display&&controlled&&active&&!quitting&&!fade&&!stop;
        const bool terminated=display&&controlled&&(stop||(yielded&&stop_on_frame));
        const bool save=!display&&pretend&&enabled;
        if(ui.saved!=int(!display)||ui.activations!=int(display)||ui.name!=script||
           game.frames!=unsigned(yielded)||pm.calls!=1+unsigned(display&&!terminated)||
           hero.tutorials!=int(display&&!terminated&&who==0)||world.saves!=int(save)||
           CQuestManager::Get().starts!=int(save)||(save&&CQuestManager::Get().name!=script))return 1;
    }
    puts("00896b60_TEST PASS");
}