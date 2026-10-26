#include "rebuild_abi.h"
#include <cstdio>
struct CScriptThingInner {
  virtual bool m0(int name){ return true; }
  virtual bool m1(int name){ return true; }
  virtual bool m2(int name){ return true; }
  virtual bool m3(int name){ return true; }
  virtual bool m4(int name){ return true; }
  virtual bool m5(int name){ return true; }
  virtual bool m6(int name){ return true; }
  virtual bool m7(int name){ return true; }
  virtual bool m8(int name){ return true; }
  virtual bool m9(int name){ return true; }
  virtual bool m10(int name){ return true; }
  virtual bool m11(int name){ return true; }
  virtual bool m12(int name){ return true; }
  virtual bool m13(int name){ return true; }
  virtual bool m14(int name){ return true; }
  virtual bool m15(int name){ return true; }
  virtual bool m16(int name){ return true; }
  virtual bool m17(int name){ return true; }
  virtual bool m18(int name){ return true; }
  virtual bool m19(int name){ return true; }
  virtual bool m20(int name){ return true; }
  virtual bool m21(int name){ return true; }
  virtual bool m22(int name){ return true; }
  virtual bool m23(int name){ return true; }
  virtual bool m24(int name){ return true; }
  virtual bool m25(int name){ return true; }
  virtual bool m26(int name){ return true; }
  virtual bool m27(int name){ return true; }
  virtual bool m28(int name){ return true; }
  virtual bool m29(int name){ return true; }
  virtual bool m30(int name){ return true; }
  virtual bool m31(int name){ return true; }
  virtual bool m32(int name){ return true; }
  virtual bool m33(int name){ return true; }
  virtual bool m34(int name){ return true; }
  virtual bool m35(int name){ return true; }
  virtual bool m36(int name){ return true; }
  virtual bool m37(int name){ return true; }
  virtual bool m38(int name){ return true; }
  virtual bool m39(int name){ return true; }
  virtual bool m40(int name){ return true; }
  virtual bool m41(int name){ return true; }
  virtual bool m42(int name){ return true; }
};
class CScriptThing { public: void *pad0; CScriptThingInner *m_impl; bool MsgIsHitByAnySpecialAbility(int name); };
bool CScriptThing::MsgIsHitByAnySpecialAbility(int name){ CScriptThingInner *p=m_impl; if(!p) return false; return p->m42(name); }
int main(){
    CScriptThing t; t.m_impl=0;
    if(t.MsgIsHitByAnySpecialAbility(0)){std::printf("BADNULL\n");return 1;}
    CScriptThingInner impl; CScriptThingInner *pi = &impl; t.m_impl=pi;
    if(!t.MsgIsHitByAnySpecialAbility(1)){std::printf("BADFWD\n");return 1;}
    std::printf("MSG_4aad40_OK\n"); return 0;
}