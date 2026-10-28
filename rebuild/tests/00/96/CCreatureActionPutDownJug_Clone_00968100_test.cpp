#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
void* __cdecl operator new(unsigned int);
#else
#include <stddef.h>
#endif
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char State[0xa8-8];CCreatureActionBase();CCreatureActionBase(const CCreatureActionBase&);virtual CCreatureActionBase* Clone()const=0;};class CCombatActionBase:public CCreatureActionBase{public:unsigned char CombatState[0x114-0xa8];CCombatActionBase();CCombatActionBase(const CCombatActionBase&);};
enum EUnsheatheWeaponType{UNSHEATHE_DEFAULT=0,UNSHEATHE_TEST=17};namespace NCreatureMode{enum EMode{MODE_DEFAULT=0,MODE_TEST=39};}
class CCreatureAction_PutDownThingAtTable:public CCreatureActionBase{public:NCreatureMode::EMode Mode;};
class CCreatureAction_PutDownJug:public CCreatureAction_PutDownThingAtTable{public:virtual CCreatureActionBase* Clone()const;};
CCreatureActionBase* CCreatureAction_PutDownJug::Clone()const{return new CCreatureAction_PutDownJug(*this);}
static int copied;static const void* seen;static void* allocated;static unsigned int allocationSize;
#ifdef _MSC_VER
void* __cdecl operator new(unsigned int n){allocationSize=n;allocated=malloc(n);return allocated;}void __cdecl operator delete(void* p){free(p);}
#else
void* operator new(size_t n){allocationSize=(unsigned int)n;allocated=malloc(n);return allocated;}void operator delete(void* p) throw(){free(p);}
#endif
CCreatureActionBase::CCreatureActionBase(){}CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase& p){++copied;seen=&p;memcpy(State,p.State,sizeof(State));}CCombatActionBase::CCombatActionBase(){}CCombatActionBase::CCombatActionBase(const CCombatActionBase& p){++copied;seen=&p;memcpy(State,p.State,sizeof(State));memcpy(CombatState,p.CombatState,sizeof(CombatState));}
int main(){CCreatureAction_PutDownJug original;memset(original.State,0x35,sizeof(original.State));original.Mode=NCreatureMode::MODE_TEST;CCreatureActionBase* raw=original.Clone();CCreatureAction_PutDownJug* copy=(CCreatureAction_PutDownJug*)raw;if(copied!=1 || seen!=&original || raw!=allocated || raw==&original || allocationSize!=sizeof(CCreatureAction_PutDownJug) || copy->Mode!=NCreatureMode::MODE_TEST || memcmp(copy->State,original.State,sizeof(original.State)))goto fail;delete copy;puts("00968100_TEST PASS");return 0;fail:puts("00968100_TEST FAIL");return 1;}