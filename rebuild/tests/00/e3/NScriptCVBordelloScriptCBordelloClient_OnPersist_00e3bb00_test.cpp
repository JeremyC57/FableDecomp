#include <stdio.h>
#include <string.h>
class CPersistContext { public: template<class T> void Transfer(const char*,T&,const T&); };
class CGameScriptInterfaceBase; class CScriptThing;
class CEntityScriptBase { public: virtual void OnPersist(CPersistContext&); CGameScriptInterfaceBase* Game; unsigned char Me[8]; unsigned char _base10[4]; };
namespace NScript { class CQ_SunnyvaleMasterData; class CV_BordelloScript { public: class CBordelloClient : public CEntityScriptBase { private: virtual void OnPersist(CPersistContext&); public: CV_BordelloScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData; long BrainState,ClientID,SexChance,NextLine; void InvokePersist(CPersistContext& c) { OnPersist(c); } }; }; }
void NScript::CV_BordelloScript::CBordelloClient::OnPersist(CPersistContext& context) { context.Transfer("BrainState",BrainState,0L); context.Transfer("ClientID",ClientID,0L); context.Transfer("SexChance",SexChance,0L); }

static int transferIndex=0,bad=0; static void* expectedFields[3]; static const char* expectedNames[3]; static const char* expectedKinds; static CPersistContext* expectedContext;
template<class T> struct Kind { static char value() { return 'L'; } }; template<> struct Kind<float> { static char value() { return 'F'; } };
template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& fallback) { if(transferIndex>=3) { bad=1; return; } if(this!=expectedContext || &value!=expectedFields[transferIndex] || strcmp(name,expectedNames[transferIndex]) || fallback!=T(0) || Kind<T>::value()!=expectedKinds[transferIndex]) bad=1; value=T(100+transferIndex); ++transferIndex; }
void CEntityScriptBase::OnPersist(CPersistContext&) {}
int main() { NScript::CV_BordelloScript::CBordelloClient object; CPersistContext context; expectedContext=&context; expectedKinds="LLL";
object.BrainState=0; expectedFields[0]=&object.BrainState; expectedNames[0]="BrainState";
object.ClientID=0; expectedFields[1]=&object.ClientID; expectedNames[1]="ClientID";
object.SexChance=0; expectedFields[2]=&object.SexChance; expectedNames[2]="SexChance";
object.NextLine=771; object.InvokePersist(context);
if(bad || transferIndex!=3 || object.BrainState!=100 || object.ClientID!=101 || object.SexChance!=102 || object.NextLine!=771) { puts("00e3bb00_TEST FAIL"); return 1; } puts("00e3bb00_TEST PASS"); return 0; }