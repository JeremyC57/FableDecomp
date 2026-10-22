#include "rebuild_abi.h"
#include "engine/CScriptThing.h"
// CScriptThing::MsgIsHitByAnyNonAggressive @ 0x004aad80
// mov ecx,[ecx+4]; test; jne L; xor al,al; ret 4; L: mov eax,[ecx]; jmp [eax+0xb0]
// Proxy: forward to the wrapped implementation object at this->f4; false when absent.
struct CScriptThingInner {
  virtual bool m0(int name);
  virtual bool m1(int name);
  virtual bool m2(int name);
  virtual bool m3(int name);
  virtual bool m4(int name);
  virtual bool m5(int name);
  virtual bool m6(int name);
  virtual bool m7(int name);
  virtual bool m8(int name);
  virtual bool m9(int name);
  virtual bool m10(int name);
  virtual bool m11(int name);
  virtual bool m12(int name);
  virtual bool m13(int name);
  virtual bool m14(int name);
  virtual bool m15(int name);
  virtual bool m16(int name);
  virtual bool m17(int name);
  virtual bool m18(int name);
  virtual bool m19(int name);
  virtual bool m20(int name);
  virtual bool m21(int name);
  virtual bool m22(int name);
  virtual bool m23(int name);
  virtual bool m24(int name);
  virtual bool m25(int name);
  virtual bool m26(int name);
  virtual bool m27(int name);
  virtual bool m28(int name);
  virtual bool m29(int name);
  virtual bool m30(int name);
  virtual bool m31(int name);
  virtual bool m32(int name);
  virtual bool m33(int name);
  virtual bool m34(int name);
  virtual bool m35(int name);
  virtual bool m36(int name);
  virtual bool m37(int name);
  virtual bool m38(int name);
  virtual bool m39(int name);
  virtual bool m40(int name);
  virtual bool m41(int name);
  virtual bool m42(int name);
  virtual bool m43(int name);
  virtual bool m44(int name);
};
struct CScriptThing_Methods : CScriptThing { bool MsgIsHitByAnyNonAggressive(int name); };
bool CScriptThing_Methods::MsgIsHitByAnyNonAggressive(int name)
{
    CScriptThingInner *p = (CScriptThingInner*)PImp_Data;
    if (!p) return false;
    return p->m44(name);
}
