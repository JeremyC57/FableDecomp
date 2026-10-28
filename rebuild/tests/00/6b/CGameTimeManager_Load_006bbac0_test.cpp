#include <stdio.h>
#include <string.h>
class CPersistContext { public: template<class T> void Transfer(const char*,T&,const T&); };
class CWorld; class CMainGameComponent;
class CGameTimeManager { public: CWorld* World; CMainGameComponent* GameComponent; float Time,PrevTime,TimeStep; long DayCount; void Load(CPersistContext&); };
void CGameTimeManager::Load(CPersistContext& context) { context.Transfer("Time",Time,0.0f); context.Transfer("TimeStep",TimeStep,0.0f); context.Transfer("DayCount",DayCount,0L); }

static int transferIndex=0,bad=0; static void* expectedFields[3]; static const char* expectedNames[3]; static const char* expectedKinds; static CPersistContext* expectedContext;
template<class T> struct Kind { static char value() { return 'L'; } }; template<> struct Kind<float> { static char value() { return 'F'; } };
template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& fallback) { if(transferIndex>=3) { bad=1; return; } if(this!=expectedContext || &value!=expectedFields[transferIndex] || strcmp(name,expectedNames[transferIndex]) || fallback!=T(0) || Kind<T>::value()!=expectedKinds[transferIndex]) bad=1; value=T(100+transferIndex); ++transferIndex; }
int main() { CGameTimeManager object; CPersistContext context; expectedContext=&context; expectedKinds="FFL";
object.Time=0; expectedFields[0]=&object.Time; expectedNames[0]="Time";
object.TimeStep=0; expectedFields[1]=&object.TimeStep; expectedNames[1]="TimeStep";
object.DayCount=0; expectedFields[2]=&object.DayCount; expectedNames[2]="DayCount";
object.PrevTime=77.0f; object.Load(context);
if(bad || transferIndex!=3 || object.Time!=100 || object.TimeStep!=101 || object.DayCount!=102 || object.PrevTime!=77.0f) { puts("006bbac0_TEST FAIL"); return 1; } puts("006bbac0_TEST PASS"); return 0; }