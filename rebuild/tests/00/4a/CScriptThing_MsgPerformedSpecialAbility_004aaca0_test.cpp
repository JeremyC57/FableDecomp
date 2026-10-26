#include "rebuild_abi.h"
#include <cstdio>
struct CScriptThingInner {
  virtual bool m0(int abilityEnum){ return true; }
  virtual bool m1(int abilityEnum){ return true; }
  virtual bool m2(int abilityEnum){ return true; }
  virtual bool m3(int abilityEnum){ return true; }
  virtual bool m4(int abilityEnum){ return true; }
  virtual bool m5(int abilityEnum){ return true; }
  virtual bool m6(int abilityEnum){ return true; }
  virtual bool m7(int abilityEnum){ return true; }
  virtual bool m8(int abilityEnum){ return true; }
  virtual bool m9(int abilityEnum){ return true; }
  virtual bool m10(int abilityEnum){ return true; }
  virtual bool m11(int abilityEnum){ return true; }
  virtual bool m12(int abilityEnum){ return true; }
  virtual bool m13(int abilityEnum){ return true; }
  virtual bool m14(int abilityEnum){ return true; }
  virtual bool m15(int abilityEnum){ return true; }
  virtual bool m16(int abilityEnum){ return true; }
  virtual bool m17(int abilityEnum){ return true; }
  virtual bool m18(int abilityEnum){ return true; }
  virtual bool m19(int abilityEnum){ return true; }
  virtual bool m20(int abilityEnum){ return true; }
  virtual bool m21(int abilityEnum){ return true; }
  virtual bool m22(int abilityEnum){ return true; }
  virtual bool m23(int abilityEnum){ return true; }
  virtual bool m24(int abilityEnum){ return true; }
  virtual bool m25(int abilityEnum){ return true; }
  virtual bool m26(int abilityEnum){ return true; }
  virtual bool m27(int abilityEnum){ return true; }
  virtual bool m28(int abilityEnum){ return true; }
  virtual bool m29(int abilityEnum){ return true; }
  virtual bool m30(int abilityEnum){ return true; }
  virtual bool m31(int abilityEnum){ return true; }
  virtual bool m32(int abilityEnum){ return true; }
  virtual bool m33(int abilityEnum){ return true; }
  virtual bool m34(int abilityEnum){ return true; }
  virtual bool m35(int abilityEnum){ return true; }
  virtual bool m36(int abilityEnum){ return true; }
  virtual bool m37(int abilityEnum){ return true; }
};
class CScriptThing { public: void *pad0; CScriptThingInner *m_impl; bool MsgPerformedSpecialAbility(int abilityEnum); };
bool CScriptThing::MsgPerformedSpecialAbility(int abilityEnum){ CScriptThingInner *p=m_impl; if(!p) return false; return p->m37(abilityEnum); }
int main(){
    CScriptThing t; t.m_impl=0;
    if(t.MsgPerformedSpecialAbility(0)){std::printf("BADNULL\n");return 1;}
    CScriptThingInner impl; CScriptThingInner *pi = &impl; t.m_impl=pi;
    if(!t.MsgPerformedSpecialAbility(1)){std::printf("BADFWD\n");return 1;}
    std::printf("MSG_4aaca0_OK\n"); return 0;
}