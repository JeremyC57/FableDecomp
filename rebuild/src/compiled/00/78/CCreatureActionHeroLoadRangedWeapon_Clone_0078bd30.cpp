// CCreatureAction_HeroLoadRangedWeapon::Clone() const -- `return new CCreatureAction_HeroLoadRangedWeapon(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_HeroLoadRangedWeapon@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078BD30, 34 bytes: operator new(0xB8) 0x00BFEA1A, null test, then the copy constructor 0x0078BD60 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_HeroLoadRangedWeapon@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_HeroLoadRangedWeapon@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0078BD60 stores
//   CCreatureAction_HeroLoadRangedWeapon's vftable 0x0126B9D4 first; the landed row at 0x0078BD60 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_HeroLoadRangedWeapon@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_HeroLoadRangedWeapon.h does not give the class at sizeof 0xB8 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase
{
public:
    CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& other);   // 0x0078BD60
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_HeroLoadRangedWeapon@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB8 - sizeof(CCreatureActionBase)];   // CCreatureAction_HeroLoadRangedWeapon's own members (not modelled: the copy constructor is out of line); sizeof 0xB8
};

CCreatureActionBase* CCreatureAction_HeroLoadRangedWeapon::Clone() const
{
    return new CCreatureAction_HeroLoadRangedWeapon(*this);
}
