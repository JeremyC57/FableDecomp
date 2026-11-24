#include <stdio.h>
#include <string.h>
#include "engine/TRetVoid.h"
static unsigned mask, cursor, logEntries[12], held, adds, drops;
static const char* added;
static bool invalid;
struct CCharString {
    const char* value;
    CCharString(const char* v,long size=-1):value(v){if(size!=-1)invalid=true;++held;}
    ~CCharString(){if(held!=1)invalid=true;--held;++drops;}
};
struct SongList {
    void push_back(const CCharString& v){if(held!=1)invalid=true;added=v.value;++adds;}
};
struct GameInterface {
    bool IsHeroNaked(){logEntries[cursor++]=0;return (mask&1)!=0;}
};
namespace NScript {
class CV_TalentlessBardScript {
public:
    GameInterface Game;
    SongList SongItems;
    bool condition(unsigned n){logEntries[cursor++]=n;return (mask&(1u<<n))!=0;}
    bool IsHeroDressedAsAWoman(){return condition(1);}
    bool IsHeroDressedInWillRobe(){return condition(2);}
    bool IsHeroDressedInArmour(){return condition(3);}
    bool IsHeroDressedAsApprentice(){return condition(4);}
    bool IsActiveThreadTerminating(){return condition(5);}
    void ProcessSongsClothes();
};
}
namespace NScript {
void CV_TalentlessBardScript::ProcessSongsClothes()
{
    TRetVoid _Fail_Val;
    if (Game.IsHeroNaked()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        true ? SongItems.push_back("TEXT_QST_163_WEAR_NOTHING") : _Fail_Val.Return();
    } else if (IsHeroDressedAsAWoman()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_DRESS");
    } else if (IsHeroDressedInWillRobe()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_WILL");
    } else if (IsHeroDressedInArmour()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_ARMOUR");
    } else if (IsHeroDressedAsApprentice()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_APPRENTICE");
    }
}
}

int main(){
    const char* tokens[]={"TEXT_QST_163_WEAR_NOTHING","TEXT_QST_163_WEAR_DRESS","TEXT_QST_163_WEAR_WILL",
                           "TEXT_QST_163_WEAR_ARMOUR","TEXT_QST_163_WEAR_APPRENTICE"};
    NScript::CV_TalentlessBardScript bard;
    for(mask=0;mask<64;++mask){
        cursor=held=adds=drops=0;invalid=false;added=0;
        bard.ProcessSongsClothes();
        unsigned i=0;
        for(;i<5;++i){
            if(logEntries[i]!=i)return 1;
            if(mask&(1u<<i))break;
        }
        if(i==5){if(cursor!=5||adds||drops)return 2;}
        else {
            if(cursor!=i+2||logEntries[i+1]!=5)return 3;
            unsigned expected=(mask&32)?0:1;
            if(adds!=expected||drops!=expected||expected&&(!added||strcmp(added,tokens[i])))return 4;
        }
        if(invalid||held)return 5;
    }
    puts("00ed9bb0_TEST PASS");
    return 0;
}