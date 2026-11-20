#ifndef FABLE_ENGINE_CINITBASECLASS_H
#define FABLE_ENGINE_CINITBASECLASS_H
#include "engine/CBaseClass.h"
// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
#ifdef FABLE_RETAIL_CINITBASECLASS
// Retail opt-in (chunk 734, row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb ?IsValid@CInitBaseClass@@QBE_NXZ, out of line in
// retail at 0x0099A330 (`mov al,[ecx+4] ; ret`: Valid); UpdateFrame calls it on each CPlayer (RTTI CPlayer : CInitBaseClass).
bool IsValid() const;                                                    // 0x0099A330
#endif
#ifdef FABLE_RETAIL_CINITBASECLASS_VALIDATE
// Retail opt-in (NF132, q4055): Ego_r.pdb + FableWin.pdb ?Validate@CInitBaseClass@@UBEXXZ, the class's first own virtual: vtable
// 0x0129A7CC slot 1 = 0x0099A340 (`ret`), after the inherited CBaseClass destructor slot 0.
virtual void Validate() const;                                           // slot 1, 0x0099A340
#endif
#ifdef FABLE_RETAIL_CINITBASECLASS_UNINITIALISE
// Retail opt-in (NF132, q4019): Ego_r.pdb + FableWin.pdb ?Uninitialise@CInitBaseClass@@IAEXXZ (protected, non-virtual), out of
// line at 0x0099A360 (`cmp byte [ecx+4],1 ; jne ; dec [0x013BCA14] ; mov byte [ecx+4],0 ; ret`: clears Valid).
protected:
void Uninitialise();                                                     // 0x0099A360
public:
#endif
};
#endif
