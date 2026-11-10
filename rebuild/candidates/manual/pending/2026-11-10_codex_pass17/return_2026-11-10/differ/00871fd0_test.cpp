// Behaviour fixture: selected target and original rolling-average template, with a controlled tools getter.
// Actual shared layouts and exact target name are checked in the compiler grade.
#include <cstdio>
#include <cstdlib>
namespace NCameraModes {
class CTools {public:
    template<class T> T GetRollingAverage(const T& current_val,const T& new_val,float weight)const
    {return (current_val*weight+new_val)/(weight+1.0f);}
};
class CStringAndCageBase {public:
    float CurrentStringLength,DesiredStringLength,DesiredCageRadius,CurrentCageRadius;
    long TimerForExpansion;
    mutable unsigned Calls;
    mutable float StringAtCall[2],CageAtCall[2];
    mutable long TimerAtCall[2];
    CTools Tools;
    const CTools& GetCamTools()const {
        if(Calls>=2)std::abort();StringAtCall[Calls]=CurrentStringLength;CageAtCall[Calls]=CurrentCageRadius;
        TimerAtCall[Calls]=TimerForExpansion;++Calls;return Tools;
    }
    void UpdateXYVars();
};
}
void NCameraModes::CStringAndCageBase::UpdateXYVars()
{
    CurrentStringLength = GetCamTools().GetRollingAverage(CurrentStringLength,DesiredStringLength,3.0f);
    float new_cage_radius = DesiredCageRadius;
    if (TimerForExpansion > 0) {
        new_cage_radius = DesiredStringLength;
        --TimerForExpansion;
    }
    CurrentCageRadius = GetCamTools().GetRollingAverage(CurrentCageRadius,new_cage_radius,3.0f);
}

static void Check(bool value){if(!value)std::abort();}
int main(){
    const float values[]={-8,0,4,16};const long timers[]={-2,0,1,2,4,100};unsigned cases=0;
    for(unsigned a=0;a<4;++a)for(unsigned b=0;b<4;++b)for(unsigned c=0;c<4;++c)
    for(unsigned d=0;d<4;++d)for(unsigned t=0;t<6;++t){
        NCameraModes::CStringAndCageBase mode;
        mode.CurrentStringLength=values[a];mode.DesiredStringLength=values[b];
        mode.DesiredCageRadius=values[c];mode.CurrentCageRadius=values[d];mode.TimerForExpansion=timers[t];
        for(unsigned frame=0;frame<4;++frame){
            float old_string=mode.CurrentStringLength,old_cage=mode.CurrentCageRadius;long old_timer=mode.TimerForExpansion;
            float expected_string=(3.0f*old_string+values[b])/4.0f;
            float desired=old_timer>0?values[b]:values[c];float expected_cage=(3.0f*old_cage+desired)/4.0f;
            long expected_timer=old_timer>0?old_timer-1:old_timer;
            mode.Calls=0;mode.UpdateXYVars();
            Check(mode.Calls==2);Check(mode.CurrentStringLength==expected_string&&mode.CurrentCageRadius==expected_cage);
            Check(mode.TimerForExpansion==expected_timer);Check(mode.DesiredStringLength==values[b]&&mode.DesiredCageRadius==values[c]);
            Check(mode.StringAtCall[0]==old_string&&mode.CageAtCall[0]==old_cage&&mode.TimerAtCall[0]==old_timer);
            Check(mode.StringAtCall[1]==expected_string&&mode.CageAtCall[1]==old_cage&&mode.TimerAtCall[1]==expected_timer);
            ++cases;
        }
    }
    Check(cases==6144);std::puts("00871fd0_TEST PASS");return 0;
}
