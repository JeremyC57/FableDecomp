// CCreatureAction_AimMissileWeapon::Clone() const -- `return new CCreatureAction_AimMissileWeapon(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_AimMissileWeapon@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00719D20, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x00719D50 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_AimMissileWeapon@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_AimMissileWeapon@@QAE@ABV0@@Z: FableWin name set; retail 0x00719D50 stores
//   CCreatureAction_AimMissileWeapon's vftable 0x0126556C first; the landed row at 0x00719D50 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_AimMissileWeapon@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_AimMissileWeapon.h does not give the class at sizeof 0xB0 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_AimMissileWeapon : public CCreatureActionBase
{
public:
    CCreatureAction_AimMissileWeapon(const CCreatureAction_AimMissileWeapon& other);   // 0x00719D50
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_AimMissileWeapon@@UAEXXZ (FableWin name set; the base's is pure and retail instantiates the class)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_AimMissileWeapon's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_AimMissileWeapon::Clone() const
{
    return new CCreatureAction_AimMissileWeapon(*this);
}
