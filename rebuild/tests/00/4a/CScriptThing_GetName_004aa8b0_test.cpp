#include <cstdio>
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING
#include "engine/CScriptThing.h"
class CCharString { public: int m_id; };
CCharString CScriptThing::NullString;   // 0x0143E8EC
struct CScriptThingVirt {
  virtual void* d0(){return 0;}
  virtual void* GetName();
};
struct CScriptThing_Methods : CScriptThing {
  void* GetName();
};
void* CScriptThingVirt::GetName(){ return (void*)0x1234; }
void* CScriptThing_Methods::GetName(){
  CScriptThingVirt* p=(CScriptThingVirt*)this->PImp_Data;
  if(!p) return &NullString;
  return p->GetName();
}
int main(){ CScriptThingVirt pp; CScriptThing_Methods o; o.PImp_Data=(CScriptThing*)&pp;
 if(o.GetName()!=(void*)0x1234){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 CScriptThing_Methods o2; o2.PImp_Data=0; if(o2.GetName()!=(void*)&CScriptThing::NullString){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 std::printf("004aa8b0_TEST PASS\n"); return 0;}
