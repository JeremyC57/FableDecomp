// NUISystem::CUIIconsDef::Copy(CDefClassBase const*) -- copies another icons def into this one through the assignment operator (tail jump to operator= 0x00432C70)
// Leaf: ?Copy@CUIIconsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00432C60, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 19 of NUISystem::CUIIconsDef's vtable 0x01230F54 (the slot holds this body in 1 vtable(s), owner NUISystem::CUIIconsDef); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts Copy(CDefClassBase const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Copy@CUIIconsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z; no `ret` to check (NORET).
// Was: NUISystem::CUIIconsDef::vfunc_19 (rebuild/src/compiled/00/43/gapscan_sub00432c60_00432c60.cpp).
// No shared header for NUISystem::CUIIconsDef (RTTI .?AVCUIIconsDef@NUISystem@@ : CParentDefClassBase): minimal local declaration
// of the real class, with the out-of-line assignment declared (the original is compiler-generated; Ego_r ??4CUIIconsDef@NUISystem@@
// QAEAAV01@ABV01@@Z is the catalog name of 0x00432C70).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"
namespace NUISystem {
class CUIIconsDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase*);
    CUIIconsDef& operator=(const CUIIconsDef&);                     // 0x00432C70
};
void CUIIconsDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIIconsDef*>(def);
}
}
