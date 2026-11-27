#define _HAS_EXCEPTIONS 0
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
typedef std::string CWideString;
class CCharString : public std::string {
public:
    CCharString(const char* s=""):std::string(s){}
    CCharString(const std::string& s):std::string(s){}
    bool EndsWith(const CCharString& suffix)const {return size()>=suffix.size() && compare(size()-suffix.size(),suffix.size(),suffix)==0;}
    unsigned long GetLength()const{return size();}
    CCharString GetSubString(unsigned long offset,unsigned long len)const{return substr(offset,len);}
    CWideString ToWideString()const{return *this;}
};
enum ETutorialCategory { CATEGORY_A, CATEGORY_B, CATEGORY_C };
namespace NGlobalConsole {bool GEnableTutorials;}
struct CFontBank {};
struct CHeroLogBook {
    struct CHeroLogBookEntry { enum ECategory {CATEGORY_TUTORIAL=2}; };
    int count;
    CWideString title,abbreviated,tag;
    void AddEntry(const CWideString& a,const CWideString& b,const CWideString& c,CHeroLogBookEntry::ECategory cat,bool active){
        assert(int(cat)==2&&active);++count;title=a;abbreviated=b;tag=c;
    }
};
struct CPlayer {CHeroLogBook log;CHeroLogBook& GetPlayerHeroLogBook()const{return const_cast<CHeroLogBook&>(log);} };
struct CPlayerManager {CPlayer player;CPlayer& GetMainPlayer(){return player;}};
struct CGameScriptInterface {bool movie;bool IsInMovieSequence()const{return movie;}};
struct CWorld {CGameScriptInterface game;const CGameScriptInterface& GetGameScriptInterface()const{return game;}};
struct CMainGameComponent {
    static CMainGameComponent& singleton(){static CMainGameComponent c;return c;}
    static CMainGameComponent* Get(){return &singleton();}
    CWorld world;CPlayerManager players;CFontBank font;
    const CWorld& PeekWorld()const{return world;}
    CPlayerManager& GetPlayerManager(){return players;}
    const CFontBank& PeekCutSceneMainFont()const{return font;}
};
struct CPlayerGuiDef {
    CCharString TutorialText[3],TutorialLogBookText[3];long TutorialGuildSealGraphics[3];
};
struct CSubtitleRenderer {
    bool inCutscene,fade;int count;CCharString caption;
    bool IsInCutscene()const{return inCutscene;}
    bool IsAllowedToSetText(bool ignore)const{return ignore||!fade;}
    void SetText(const CFontBank& f,const CCharString& text,const CCharString& a,const CCharString& b,const CCharString& c,const CCharString& d,unsigned long flags,float tm,bool x,bool y){
        assert(&f==&CMainGameComponent::Get()->font&&a.empty()&&b.empty()&&c.empty()&&d.empty()&&flags==10&&tm==0&&x&&y);caption=text;++count;
    }
};
struct CPlayerGui {
    CSubtitleRenderer renderer;CPlayerGuiDef def;int count;long graphic;CCharString seal;
    static CPlayerGui& Get(){static CPlayerGui gui;return gui;}
    const CPlayerGuiDef& PeekGuiDef()const{return def;}
    CSubtitleRenderer& GetSubtitleRenderer(){return renderer;}
    void AddGuildSealMessage(const CCharString& t,long n,bool active){assert(active);seal=t;graphic=n;++count;}
};
class CUserProfileManager {
public:
    bool UseNewProfileValues,TutorialSystemEnabled,TutorialSystemEnabledNewProfile,DisplayTutorialsDuringCutscenes;
    int saves; std::vector<ETutorialCategory> GivenTutorialCategories;std::queue<ETutorialCategory> PendingTutorialCategories;
    bool IsTutorialSystemEnabled()const;bool IsTutorialCategoryGiven(ETutorialCategory)const;
    bool GiveTutorialForCategory(ETutorialCategory,bool);void Save(){++saves;}
};
inline bool CUserProfileManager::IsTutorialSystemEnabled() const
{
    return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled;
}

inline bool CUserProfileManager::IsTutorialCategoryGiven(ETutorialCategory category) const
{
    return std::find(GivenTutorialCategories.begin(), GivenTutorialCategories.end(), category) != GivenTutorialCategories.end();
}

bool CUserProfileManager::GiveTutorialForCategory(ETutorialCategory category, bool ignore_fading)
{
    if (NGlobalConsole::GEnableTutorials && IsTutorialSystemEnabled() && !IsTutorialCategoryGiven(category)) {
        CMainGameComponent* pcomponent = CMainGameComponent::Get();
        const CWorld& world = pcomponent->PeekWorld();
        if ((!world.GetGameScriptInterface().IsInMovieSequence() || DisplayTutorialsDuringCutscenes)
            && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()
            && CPlayerGui::Get().GetSubtitleRenderer().IsAllowedToSetText(ignore_fading)) {
            const CPlayerGuiDef& gui_def = CPlayerGui::Get().PeekGuiDef();
            const CCharString& tutorial_text_tag = gui_def.TutorialText[category];
            if (tutorial_text_tag != "") {
                long guild_seal_graphic = gui_def.TutorialGuildSealGraphics[category];
                if (guild_seal_graphic > 0)
                    CPlayerGui::Get().AddGuildSealMessage(tutorial_text_tag, guild_seal_graphic, true);
                else
                    CPlayerGui::Get().GetSubtitleRenderer().SetText(pcomponent->PeekCutSceneMainFont(),
                        tutorial_text_tag, "", "", "", "", 10, 0.0f, true, true);
            }
            CCharString log_book_tag = gui_def.TutorialLogBookText[category];
            if (log_book_tag != "") {
                CCharString log_book_title_tag = log_book_tag;
                if (log_book_title_tag.EndsWith("_PC"))
                    log_book_title_tag = log_book_title_tag.GetSubString(0, log_book_title_tag.GetLength()-3) + "_TITLE";
                else
                    log_book_title_tag += "_TITLE";
                CPlayer& player = pcomponent->GetPlayerManager().GetMainPlayer();
                CHeroLogBook& log_book = player.GetPlayerHeroLogBook();
                log_book.AddEntry(log_book_title_tag.ToWideString(), log_book_title_tag.ToWideString(),
                    log_book_tag.ToWideString(), CHeroLogBook::CHeroLogBookEntry::CATEGORY_TUTORIAL, true);
            }
            GivenTutorialCategories.push_back(category);
            Save();
            return true;
        }
        PendingTutorialCategories.push(category);
    }
    return false;
}

int main() {
    for(unsigned f=0;f<1024;++f) for(unsigned config=0;config<18;++config) {
        CUserProfileManager p; CMainGameComponent& game=*CMainGameComponent::Get();CPlayerGui& gui=CPlayerGui::Get();
        CHeroLogBook& log=game.players.player.log;
        p.saves=gui.renderer.count=gui.count=log.count=0;
        bool active=(f&1)!=0,old_on=(f&2)!=0,new_on=(f&4)!=0,use_new=(f&8)!=0,prior=(f&16)!=0;
        bool movie=(f&32)!=0,allow_movie=(f&64)!=0,cut=(f&128)!=0,fade=(f&256)!=0,ignore=(f&512)!=0;
        NGlobalConsole::GEnableTutorials=active;p.UseNewProfileValues=use_new;p.TutorialSystemEnabled=old_on;p.TutorialSystemEnabledNewProfile=new_on;
        p.DisplayTutorialsDuringCutscenes=allow_movie;game.world.game.movie=movie;gui.renderer.inCutscene=cut;gui.renderer.fade=fade;
        ETutorialCategory cat=CATEGORY_B;
        p.GivenTutorialCategories.push_back(CATEGORY_A);if(prior)p.GivenTutorialCategories.push_back(cat);
        p.PendingTutorialCategories.push(CATEGORY_C);
        long graphic=(long)(config%3)-1;
        gui.def.TutorialGuildSealGraphics[cat]=graphic;
        const char* message=config%6<3?"HELP":"";
        unsigned kind=config/6;const char* tag=kind==0?"":kind==1?"TUTORIAL_GENERAL":"TUTORIAL_MORAL_PC";
        gui.def.TutorialText[cat]=message;gui.def.TutorialLogBookText[cat]=tag;
        bool eligible=active&&(use_new?new_on:old_on)&&!prior;
        bool shown=eligible&&(!movie||allow_movie)&&!cut&&(!fade||ignore);
        bool queued=eligible&&!shown;
        assert(p.GiveTutorialForCategory(cat,ignore)==shown);
        assert(p.saves==int(shown)&&p.GivenTutorialCategories.size()==unsigned(1+int(prior)+int(shown)));
        assert(p.GivenTutorialCategories.front()==CATEGORY_A);
        if(shown||prior)assert(p.GivenTutorialCategories.back()==cat);
        assert(p.PendingTutorialCategories.front()==CATEGORY_C&&p.PendingTutorialCategories.size()==unsigned(1+int(queued)));
        if(queued)assert(p.PendingTutorialCategories.back()==cat);
        assert(gui.count==int(shown&&*message&&graphic>0));
        if(gui.count)assert(gui.seal==message&&gui.graphic==graphic);
        assert(gui.renderer.count==int(shown&&*message&&graphic<=0));
        if(gui.renderer.count)assert(gui.renderer.caption==message);
        assert(log.count==int(shown&&kind!=0));
        if(log.count){assert(log.tag==tag&&log.title==log.abbreviated);assert(log.title==(kind==1?"TUTORIAL_GENERAL_TITLE":"TUTORIAL_MORAL_TITLE"));}
    }
    puts("0040b790_TEST PASS");
}