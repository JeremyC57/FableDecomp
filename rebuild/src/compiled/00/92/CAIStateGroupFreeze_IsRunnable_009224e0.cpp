// CAIStateGroup_Freeze::IsRunnable() -- runnable while the global flag byte @ 0x013BB68C is set
// Leaf: ?IsRunnable@CAIStateGroup_Freeze@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009224E0, 6 bytes; vftable 0x0128A95C slot 11 (its only retail reference, 0x0128A988).
// Retail bytes:  mov al, byte ptr [0x13bb68c] ; ret
// NF473 (r467b / r469c respell, unit with ctor 0x008FE593 + ??_G 0x0090194E): the self is the PDB spelling ?IsRunnable@CAIStateGroup_Freeze@@UAE_NXZ
//   (Ego_r.pdb + FableWin.pdb name sets; public virtual bool, slot 11 of the RTTI .?AVCAIStateGroup_Freeze@@ vftable), was the non-virtual
//   ?IsRunnable@CAIStateGroup_Freeze@@QAEEXZ (unsigned char) of a one-method local model. The base comes from the shared header
//   (engine/CAIStateGroupBase.h); no engine header exists for CAIStateGroup_Freeze, so the class is declared as the ctor row 0x008FE593 declares it.
//   The global keeps its landed descriptive name, typed bool: ?g_flag_013bb68c@@3_NA (was ?g_flag_013bb68c@@3EA, unsigned char). Evidence: the
//   PDB return type is bool and retail returns the byte as loaded (`mov al,[g] ; ret`); an unsigned char would be normalised (`cmp ; setne`,
//   14 v 6 bytes). Read only here: its only retail reference is 0x009224E1, so this row is its only binder.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

extern bool g_flag_013bb68c;                  // 0x013BB68C (descriptive name; bool)

class CAIStateGroup_Freeze : public CAIStateGroupBase {
public:
    CAIStateGroup_Freeze();                                                 // 0x008FE593
    virtual bool IsRunnable();                                                     // slot 11 0x009224E0
    virtual void Update();                                                         // slot 12 0x009224F0
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_Freeze) == 0x14);

bool CAIStateGroup_Freeze::IsRunnable()
{
    return g_flag_013bb68c;
}
