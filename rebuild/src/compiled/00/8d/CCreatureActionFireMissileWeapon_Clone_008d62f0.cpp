// CCreatureAction_FireMissileWeapon::Clone() const -- `return new CCreatureAction_FireMissileWeapon(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_FireMissileWeapon@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D62F0, 34 bytes: operator new(0xC8) 0x00BFEA1A, null test, then the copy constructor 0x008D6320 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_FireMissileWeapon@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_FireMissileWeapon@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x008D6320 stores
//   CCreatureAction_FireMissileWeapon's vftable 0x01285424 first; the landed row at 0x008D6320 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_FireMissileWeapon@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_FireMissileWeapon.h does not give the class at sizeof 0xC8 with Clone declared in the
//   prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase
{
public:
    CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon& other);   // 0x008D6320
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_FireMissileWeapon@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xC8 - sizeof(CCreatureActionBase)];   // CCreatureAction_FireMissileWeapon's own members (not modelled: the copy constructor is out of line); sizeof 0xC8
};

CCreatureActionBase* CCreatureAction_FireMissileWeapon::Clone() const
{
    return new CCreatureAction_FireMissileWeapon(*this);
}
