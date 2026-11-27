#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
static void check(bool v){if(!v)exit(2);}
typedef std::string CWideString;
typedef std::string CCharString;
#define FABLE_RETAIL_ASSERT(c) ((void)0)
enum EHeroExperienceStatCategory{HERO_STAT_STRENGTH,HERO_STAT_SKILL,HERO_STAT_WILL};
enum EHeroTrainableStatType{HERO_STAT_STRENGTH_PHYSIQUE,HERO_STAT_STRENGTH_HEALTH,HERO_STAT_STRENGTH_TOUGHNESS,HERO_STAT_SKILL_SPEED,HERO_STAT_SKILL_ACCURACY,HERO_STAT_SKILL_STEALTH,HERO_STAT_WILL_WEAPON_MAGIC,HERO_STAT_WILL_ABILITY_MAGIC,HERO_STAT_WILL_PURE_MAGIC,HERO_STAT_WILL_MAGIC_POWER,NUMBER_OF_TRAINABLE_HERO_STATS};
struct CTCHeroStats{
    long exp[3];std::vector<long> spent;
    long GetHeroStatExperience(EHeroExperienceStatCategory key)const{return exp[key];}
    std::vector<long>& GetExperienceSpentOn(){return spent;}
};
struct Parent{CTCHeroStats stats;bool GetTC(CTCHeroStats** out){*out=&stats;return true;}};
namespace NGameText{struct CDataBank{mutable std::string last;CWideString GetTextBySymbol(const CCharString& id)const{last=id;return "translated:"+id;}};}
struct CMainGameComponent{NGameText::CDataBank bank;static CMainGameComponent* Get(){static CMainGameComponent c;return &c;}const NGameText::CDataBank& GetTextBank()const{return bank;}};
struct CTCInventoryStats{Parent who;Parent& GetParentThing(){return who;}CWideString BuildHeroClass();};
CWideString CTCInventoryStats::BuildHeroClass()
{
    CWideString res;
    CTCHeroStats* hero_stats;
    GetParentThing().GetTC(&hero_stats);
    CCharString Tag("TEXT_GUI_STATS_HEROCLASS_");
    long stexp = hero_stats->GetHeroStatExperience(HERO_STAT_STRENGTH);
    long skexp = hero_stats->GetHeroStatExperience(HERO_STAT_SKILL);
    long mexp = hero_stats->GetHeroStatExperience(HERO_STAT_WILL);
    CCharString middle;
    if (stexp >= skexp && stexp >= mexp)
        middle = "PHYSICAL_";
    else if (skexp >= stexp && skexp >= mexp)
        middle = "SKILL_";
    else if (mexp >= stexp && mexp >= skexp)
        middle = "MAGIC_";
    FABLE_RETAIL_ASSERT(middle.GetLength() != 0);
    Tag += middle;
    std::vector<long>& expon = hero_stats->GetExperienceSpentOn();
    bool first = true;
    long maxexp = 0;
    EHeroTrainableStatType maxstat = HERO_STAT_STRENGTH_PHYSIQUE;
    for (long i = 0; i != expon.size(); ++i) {
        if (first || expon[i] >= maxexp) {
            first = false;
            maxstat = (EHeroTrainableStatType)i;
            maxexp = expon[i];
        }
    }
    CCharString last;
    if (maxexp == 0)
        last = "NONE";
    else {
        if (maxstat == HERO_STAT_STRENGTH_PHYSIQUE)
            last = "STRENGTH";
        if (maxstat == HERO_STAT_STRENGTH_TOUGHNESS)
            last = "TOUGHNESS";
        if (maxstat == HERO_STAT_STRENGTH_HEALTH)
            last = "HEALTH";
        if (maxstat == HERO_STAT_SKILL_SPEED)
            last = "SPEED";
        if (maxstat == HERO_STAT_SKILL_ACCURACY)
            last = "ACCURACY";
        if (maxstat == HERO_STAT_SKILL_STEALTH)
            last = "GUILE";
        if (maxstat == HERO_STAT_WILL_MAGIC_POWER)
            last = "MAGICPOWER";
        if (maxstat == NUMBER_OF_TRAINABLE_HERO_STATS)
            last = "SPELLS";
    }
    FABLE_RETAIL_ASSERT(last.GetLength() != 0);
    Tag += last;
    const NGameText::CDataBank& TextBank = CMainGameComponent::Get()->GetTextBank();
    return TextBank.GetTextBySymbol(Tag);
}
int main(){
    const char* suffix[]={"STRENGTH","HEALTH","TOUGHNESS","SPEED","ACCURACY","GUILE","","","","MAGICPOWER","SPELLS"};
    const long categories[6][3]={{3,1,2},{1,3,2},{1,2,3},{3,3,3},{1,3,3},{3,1,3}};
    const char* middle[]={"PHYSICAL_","SKILL_","MAGIC_","PHYSICAL_","SKILL_","PHYSICAL_"};
    for(int c=0;c<6;++c)for(int winner=-2;winner<11;++winner){
        CTCInventoryStats tc;CTCHeroStats& st=tc.who.stats;
        for(int k=0;k<3;++k)st.exp[k]=categories[c][k];
        if(winner!=-2)st.spent.resize(11,0);
        if(winner>=0){st.spent[0]=7;st.spent[winner]=7;}
        const std::string wanted=std::string("TEXT_GUI_STATS_HEROCLASS_")+middle[c]+(winner<0?"NONE":suffix[winner]);
        check(tc.BuildHeroClass()=="translated:"+wanted && CMainGameComponent::Get()->bank.last==wanted);
    }
    puts("005cd4c4_TEST PASS");return 0;
}