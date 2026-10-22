#include "rebuild_abi.h"
#include "engine/CScriptThing.h"
// CScriptThing::MsgPerformedSpecialAbility @ 0x004aaca0
// mov ecx,[ecx+4]; test; jne L; xor al,al; ret 4; L: mov eax,[ecx]; jmp [eax+0x94]
// Proxy: forward to the wrapped implementation object at this->f4; false when absent.
struct CScriptThingInner {
  virtual bool m0(int abilityEnum);
  virtual bool m1(int abilityEnum);
  virtual bool m2(int abilityEnum);
  virtual bool m3(int abilityEnum);
  virtual bool m4(int abilityEnum);
  virtual bool m5(int abilityEnum);
  virtual bool m6(int abilityEnum);
  virtual bool m7(int abilityEnum);
  virtual bool m8(int abilityEnum);
  virtual bool m9(int abilityEnum);
  virtual bool m10(int abilityEnum);
  virtual bool m11(int abilityEnum);
  virtual bool m12(int abilityEnum);
  virtual bool m13(int abilityEnum);
  virtual bool m14(int abilityEnum);
  virtual bool m15(int abilityEnum);
  virtual bool m16(int abilityEnum);
  virtual bool m17(int abilityEnum);
  virtual bool m18(int abilityEnum);
  virtual bool m19(int abilityEnum);
  virtual bool m20(int abilityEnum);
  virtual bool m21(int abilityEnum);
  virtual bool m22(int abilityEnum);
  virtual bool m23(int abilityEnum);
  virtual bool m24(int abilityEnum);
  virtual bool m25(int abilityEnum);
  virtual bool m26(int abilityEnum);
  virtual bool m27(int abilityEnum);
  virtual bool m28(int abilityEnum);
  virtual bool m29(int abilityEnum);
  virtual bool m30(int abilityEnum);
  virtual bool m31(int abilityEnum);
  virtual bool m32(int abilityEnum);
  virtual bool m33(int abilityEnum);
  virtual bool m34(int abilityEnum);
  virtual bool m35(int abilityEnum);
  virtual bool m36(int abilityEnum);
  virtual bool m37(int abilityEnum);
};
struct CScriptThing_Methods : CScriptThing { bool MsgPerformedSpecialAbility(int abilityEnum); };
bool CScriptThing_Methods::MsgPerformedSpecialAbility(int abilityEnum)
{
    CScriptThingInner *p = (CScriptThingInner*)PImp_Data;
    if (!p) return false;
    return p->m37(abilityEnum);
}
