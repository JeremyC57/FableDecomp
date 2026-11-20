// CHeroSoulsCreatureDef::Copy(const CDefClassBase* other) -- slot 19: *this = the other def (tail call to operator=)
// Leaf: ?Copy@CHeroSoulsCreatureDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27180, 5 bytes: jmp 0x00F27190.
// Identity: slot 19 of vtable 0x012FB500 (its only reference), the vtable whose slot 0 is ??_GCHeroSoulsCreatureDef 0x00F270B0 and
//   slot 20 the landed CHeroSoulsCreatureDef::GetSizeofClass 0x00F270A0 (siblings CArenaWaveDef 0x012FB2F0 / CFeatDef 0x012FB778 have
//   Copy at slot 19 too); Ego_r.pdb + FableWin.pdb ?Copy@CHeroSoulsCreatureDef@@UAEXPBVCDefClassBase@@@Z; the jump target 0x00F27190
//   is the landed CHeroSoulsCreatureDef::operator= (??4CHeroSoulsCreatureDef@@QAEAAV0@ABV0@@Z).
//   Was: _Forward (extern "C" gapscan forwarder stand-in, no Leaf line).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_OPERATOR_ASSIGN
#include "engine/CHeroSoulsCreatureDef.h"

void CHeroSoulsCreatureDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CHeroSoulsCreatureDef*>(other);
}
