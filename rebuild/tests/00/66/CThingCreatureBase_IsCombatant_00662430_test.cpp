#include "rebuild_abi.h"
#include <cstdio>
static int g_added=0;
struct CThingCreatureBase;
struct Sub { void Add(CThingCreatureBase *self); };
void Sub::Add(CThingCreatureBase *self){ g_added=1; }
struct Obj { char pad[0x4c]; Sub *m_sub; };
static Sub g_sub; static Obj g_obj;
extern "C" Obj *FABLE_FASTCALL FableGet_662430(CThingCreatureBase *self){ g_obj.m_sub=&g_sub; return &g_obj; }
struct CThingCreatureBase { char dummy; void IsCombatant(); };
void CThingCreatureBase::IsCombatant(){ Sub *s=FableGet_662430(this)->m_sub; s->Add(this); }
int main(){ CThingCreatureBase o; o.IsCombatant();
 if(!g_added){std::printf("B1\n");return 1;}
 std::printf("AC_662430_OK\n"); return 0; }