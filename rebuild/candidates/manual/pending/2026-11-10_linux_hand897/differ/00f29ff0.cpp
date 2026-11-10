// CCutsceneDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other cutscene def to this one (a tail jump into CCutsceneDef::operator=)
// Grade: DIFFER(5v219) -- EXTENT_WRONG: the row's 219 bytes are this 5-byte `jmp 0x00F2A000`, 11 int3 bytes of padding, and the
//   separate 204-byte function 0x00F2A000 (CCutsceneDef::operator=). The 5 bytes this source builds equal retail's first 5
//   (relocation-masked; `e9 rel32` to operator=). Split the row: 0x00F29FF0 length 5, new row 0x00F2A000 length 204.
// Leaf: ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29FF0, 5 bytes: `jmp 0x00F2A000` (`this` stays in ecx and `other` in the caller's argument
//   slot, so VC7.1 turns the operator= call into a tail jump), then int3 padding to 0x00F2A000.
// Identity: slot 19 (Copy) of the no-RTTI CCutsceneDef vtable 0x012FB6E0 (abs .rdata reference 0x012FB72C; slot 18 Transfer
//   0x00F2A1D0 and slot 20 GetSizeofClass 0x00F29D90 landed), Ego_r.pdb ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z next to
//   ??4CCutsceneDef@@QAEAAV0@ABV0@@Z; the jump target 0x00F2A000 is only reached from here and is the implicit memberwise
//   operator= (CDefClassBase / CParentDefClassBase copy 0x00431F60, PDefManager +0x1c, GlobalIndex +0x20, the three flag bits of
//   +0x24, InstantiationName +0x25, the SubDefInfoMap vector copy 0x006313A0 at +0x29, then the eight vector<CCharString>
//   operator= 0x00432EE9 calls for +0x3c..+0x90). The same shape as the landed CLightningDef::Copy 0x00461ED3.
// Model: the shared engine/CDefClassBase.h / CParentDefClassBase.h def chain (FABLE_RETAIL_DEF_CLASS_BASE); the cutscene's own
//   members are not needed for this body (operator= stays out of line).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CCutsceneDef : public CParentDefClassBase {
public:
    CCutsceneDef& operator=(const CCutsceneDef& other);       // 0x00F2A000 (??4CCutsceneDef@@QAEAAV0@ABV0@@Z)
    virtual void Copy(const CDefClassBase* other);            // slot 19
};

void CCutsceneDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CCutsceneDef*>(other);
}
