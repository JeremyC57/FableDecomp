#include <string>
#include <set>
#include <list>
#include <vector>
#include <stdio.h>
typedef std::string CCharString;
template<class T> struct CCountedPointer {
    T* p;
    CCountedPointer(T* data):p(data){}
    T& operator*() const{return *p;}
};
struct CTCDayOrNightOnlySupport {
    static const CCharString DAY_ONLY_SUFFIX,NIGHT_ONLY_SUFFIX;
};
const CCharString CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX="DayOnly";
const CCharString CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX="NightOnly";
class CQuestManager {
public:
    struct CActiveQuest { CCharString quest;
        CActiveQuest(const char* s):quest(s){}
        const CCharString& GetSerialisationSectionName() const{return quest;}
    };
    std::list<CCountedPointer<CActiveQuest> > ActiveQuests;
    bool DayActive,NightActive;
    void GetActiveQuestSectionNames(std::set<CCharString>&)const;
};
void CQuestManager::GetActiveQuestSectionNames(std::set<CCharString>& names) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator quest = ActiveQuests.begin(); quest != ActiveQuests.end(); ++quest)
    {
        CActiveQuest& active_quest=**quest;
        names.insert(active_quest.GetSerialisationSectionName());
    }
    std::vector<CCharString> day_names;
    std::vector<CCharString> night_names;
    if (DayActive) {
        day_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            day_names.push_back(*it + "%" + CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX);
        day_names.push_back("NULL%" + CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX);
    }
    if (NightActive) {
        night_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            night_names.push_back(*it + "%" + CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX);
        night_names.push_back("NULL%" + CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX);
    }
    names.insert(day_names.begin(), day_names.end());
    names.insert(night_names.begin(), night_names.end());
}
static bool check(bool day,bool night,bool active,bool preexisting) {
    CQuestManager q;
    q.DayActive=day;q.NightActive=night;
    CQuestManager::CActiveQuest a("Oak"),b("Oak"),c("Ash");
    if(active) {q.ActiveQuests.push_back(&a);q.ActiveQuests.push_back(&b);q.ActiveQuests.push_back(&c);}
    std::set<CCharString> got,expected;
    if(preexisting) {got.insert("Old");expected.insert("Old");}
    if(active) {expected.insert("Oak");expected.insert("Ash");}
    if(day) {
        expected.insert("NULL%DayOnly");
        if(active) {expected.insert("Oak%DayOnly");expected.insert("Ash%DayOnly");}
        if(preexisting) expected.insert("Old%DayOnly");
    }
    if(night) {
        expected.insert("NULL%NightOnly");
        if(active) {expected.insert("Oak%NightOnly");expected.insert("Ash%NightOnly");}
        if(preexisting) expected.insert("Old%NightOnly");
    }
    q.GetActiveQuestSectionNames(got);
    return got==expected;
}
int main() {
    for(unsigned mask=0;mask<16;++mask) if(!check((mask&1)!=0,(mask&2)!=0,(mask&4)!=0,(mask&8)!=0)) return 1;
    puts("004b2ca0_TEST PASS");return 0;
}