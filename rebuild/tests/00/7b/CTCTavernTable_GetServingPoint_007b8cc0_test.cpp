#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
static void check(bool v){if(!v)exit(3);}
struct CCharString: std::string {
    CCharString(const char* s=""):std::string(s){}
    CCharString(const std::string& s):std::string(s){}
    long GetLength()const{return size();}
    CCharString GetSubString(long start,long n)const{return substr(start,n);}
};
static CCharString operator+(const char* a,const CCharString& b){return std::string(a)+b.c_str();}
static long GFCharStringToInt(const CCharString& x){return atol(x.c_str());}
struct C3DVector {};struct CRightHandedSet {};struct CThing {};
struct CTCTavernTankard{float Fullness;};
struct CThingObject{CTCTavernTankard* tc;void GetTC(CTCTavernTankard** out){if(tc)*out=tc;}};
struct Ref {CThingObject* p;CThingObject* Get()const{return p;}CThingObject* operator->()const{return p;}};
struct CTCNamedUserActionPoints{
    std::vector<CCharString> names;
    CCharString maid;
    bool maidOk;int released;
    bool scan(CCharString prefix,long& index,long start){for(long i=start;i<(long)names.size();++i)if(names[i].find(prefix)==0){index=i;return true;}return false;}
    bool GetFullNamedActionPoint(CThing&,CCharString prefix,long& index,long start){return scan(prefix,index,start);}
    bool GetFreeNamedActionPoint(CThing&,C3DVector&,CRightHandedSet&,CCharString prefix,long& index,long start,bool reserve){
        check(reserve);if(prefix.find("ACTION_POINT_MAID_")==0){maid=prefix;if(maidOk){index=70;return true;}return false;}return scan(prefix,index,start);
    }
    void SetActionPointAsFree(long){++released;}
    void GetName(long i,CCharString& s){s=names[i];}
};
struct Parent{CTCNamedUserActionPoints points;void GetTC(CTCNamedUserActionPoints** p){*p=&points;}};
struct CTCTavernTable {
    enum ServingMode {SERVE_DRINK,SERVE_FOOD,REMOVE_FOOD};
    Parent parent;bool AvailableForUse;std::vector<Ref> Tankards,Food;std::vector<bool> SeatsCurrentlyUsed;
    long ActionPointSitStrlen,ActionPointGameMasterStrlen;
    Parent& GetParentThing(){return parent;}
    bool GetSeatState(long index){return SeatsCurrentlyUsed.size()>(unsigned long)index?SeatsCurrentlyUsed[index]:false;}
    bool GetServingPoint(CThing&,C3DVector&,CRightHandedSet&,long&,long&,CThingObject**,ServingMode,bool);
};
bool CTCTavernTable::GetServingPoint(CThing& thing, C3DVector& pos, CRightHandedSet& rhset, long& action_point_index, long& seat_index,
                                     CThingObject** p_tankard, ServingMode serving_mode, bool occupied)
{
    if (!AvailableForUse)
        return false;
    long starting_index = 0;
    for (;;) {
        CTCNamedUserActionPoints* named_user_action_points;
        GetParentThing().GetTC(&named_user_action_points);
        long str_len;
        if (occupied) {
            if (!named_user_action_points->GetFullNamedActionPoint(thing, "ACTION_POINT_SIT_", action_point_index, starting_index)) {
                if (!named_user_action_points->GetFullNamedActionPoint(thing, "ACTION_POINT_GAME_MASTER_", action_point_index, starting_index)) {
                    action_point_index = -1;
                    return false;
                }
                str_len = ActionPointGameMasterStrlen;
            } else
                str_len = ActionPointSitStrlen;
        } else {
            if (!named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, "ACTION_POINT_SIT_", action_point_index, starting_index, true)) {
                action_point_index = -1;
                return false;
            }
            str_len = ActionPointSitStrlen;
            named_user_action_points->SetActionPointAsFree(action_point_index);
        }
        CCharString name;
        named_user_action_points->GetName(action_point_index, name);
        CCharString number = name.GetSubString(str_len, name.GetLength());
        long val = GFCharStringToInt(number);
        bool needs_attending;
        switch (serving_mode) {
        case SERVE_DRINK: {
            if (val <= 0 || (unsigned long)val > Tankards.size())
                return false;
            if (!Tankards[val - 1].Get())
                return false;
            CTCTavernTankard* tc_tavern_tankard = 0;
            Tankards[val - 1]->GetTC(&tc_tavern_tankard);
            if (!tc_tavern_tankard)
                return false;
            needs_attending = tc_tavern_tankard->Fullness == 0.0f && GetSeatState(val - 1) == occupied;
            break;
        }
        case SERVE_FOOD:
            if (val <= 0 || (unsigned long)val > Food.size())
                return false;
            needs_attending = Food[val - 1].Get() == 0;
            break;
        case REMOVE_FOOD:
            if (val <= 0 || (unsigned long)val > Food.size())
                return false;
            needs_attending = Food[val - 1].Get() != 0;
            break;
        default:
            needs_attending = false;
            break;
        }
        if (needs_attending) {
            if (serving_mode == SERVE_DRINK)
                *p_tankard = Tankards[val - 1].Get();
            CCharString stand_point("ACTION_POINT_MAID_");
            stand_point += number.GetLength() > 1 ? number : "0" + number;
            if (named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, stand_point, action_point_index, 0, true)) {
                seat_index = val - 1;
                return true;
            }
        }
        starting_index = action_point_index + 1;
    }
}
int main(){
    for(int i=0;i<20;++i){
        CTCTavernTable t;CThing thing;C3DVector pos;CRightHandedSet rh;
        t.ActionPointSitStrlen=17;t.ActionPointGameMasterStrlen=25;t.AvailableForUse=i!=0;
        CTCNamedUserActionPoints& p=t.parent.points;p.released=0;p.maidOk=i!=11;
        bool occupied=i!=8 && i!=9 && i!=19;
        if(i!=1)p.names.push_back(i==2?"ACTION_POINT_GAME_MASTER_1":i==3?"ACTION_POINT_SIT_0":i==4?"ACTION_POINT_SIT_18":i==16?"ACTION_POINT_SIT_12":"ACTION_POINT_SIT_1");
        if(i==17)p.names.push_back("ACTION_POINT_SIT_2");
        CTCTavernTankard tk;tk.Fullness=i==7?1.0f:0.0f;CThingObject obj;obj.tc=i==6?0:&tk;
        Ref empty={0},full={&obj};t.Food.resize(12,empty);t.Tankards.resize(12,i==5?empty:full);
        if(i!=19)t.SeatsCurrentlyUsed.resize(12,i==9?true:occupied);
        if(i==13 || i==14 || i==17)t.Food[0]=full;
        CTCTavernTable::ServingMode mode=(i==12 || i==13 || i==17)?CTCTavernTable::SERVE_FOOD:(i==14 || i==15)?CTCTavernTable::REMOVE_FOOD:i==18?(CTCTavernTable::ServingMode)3:CTCTavernTable::SERVE_DRINK;
        long index=-7,seat=-8;CThingObject* got=0;
        bool result=t.GetServingPoint(thing,pos,rh,index,seat,&got,mode,occupied);
        bool ok=i==2 || i==8 || i==10 || i==12 || i==14 || i==16 || i==17 || i==19;
        check(result==ok);
        if(ok){check(index==70 && seat==(i==16?11:i==17?1:0));check(p.maid==(i==16?"ACTION_POINT_MAID_12":i==17?"ACTION_POINT_MAID_02":"ACTION_POINT_MAID_01"));}
        else check(seat==-8);
        check(got==((ok && mode==CTCTavernTable::SERVE_DRINK)||i==11?&obj:0));
        check(p.released==(!occupied?1:0));
    }
    puts("007b8cc0_TEST PASS");return 0;
}