// CSpecialEffectsDef::Copy(const CDefClassBase* other) -- slot 19 of the special-effects def: the CSubDefClassBase part (CDefClassBase::operator=
//   0x00431F10), then the SpecialEffectsMap +0x28 (CSpecialEffectsStringMap: one map<unsigned long,long>) through its out-of-line STLport
//   _Rb_tree::operator= 0x0045C79E (the implicit wrapper and map assignments are inline)
// Leaf: ?Copy@CSpecialEffectsDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C77F, 31 bytes, thiscall `ret 4`, no return value.
// Identity: Ego_r.pdb 0x00506AA3 / FableWin.pdb 0x019353E4 ?Copy@CSpecialEffectsDef@@UAEXPBVCDefClassBase@@@Z; engine/CSpecialEffectsDef.h
//   (SpecialEffectsMap +0x28 on the 0x25-byte CSubDefClassBase; the header's _pad_0x25 is alignment, not a member, so the assignment is
//   written member by member). Was a local stand-in model (SubEffectDef::Copy for the tree assignment, CDefClassBase with a pad).
// NF70 (round 7, land1117fu q10766): moved onto the shared header (overlay opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_COPY declares the override).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_COPY
#include "engine/CSpecialEffectsDef.h"

void CSpecialEffectsDef::Copy(const CDefClassBase* other)
{
    const CSpecialEffectsDef& source = *static_cast<const CSpecialEffectsDef*>(other);
    CSubDefClassBase::operator=(source);
    SpecialEffectsMap = source.SpecialEffectsMap;
}
