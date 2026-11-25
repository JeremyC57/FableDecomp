// CCreatureAction_ReloadMissileWeapon::Clone() const -- `return new CCreatureAction_ReloadMissileWeapon(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_ReloadMissileWeapon@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00719DA0, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x00719DD0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_ReloadMissileWeapon@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_ReloadMissileWeapon@@QAE@ABV0@@Z: FableWin name set; retail 0x00719DD0 stores
//   CCreatureAction_ReloadMissileWeapon's vftable 0x012656A4 first; the landed row at 0x00719DD0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_ReloadMissileWeapon@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_ReloadMissileWeapon; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_ReloadMissileWeapon : public CCreatureActionBase
{
public:
    CCreatureAction_ReloadMissileWeapon(const CCreatureAction_ReloadMissileWeapon& other);   // 0x00719DD0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_ReloadMissileWeapon@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_ReloadMissileWeapon's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_ReloadMissileWeapon::Clone() const
{
    return new CCreatureAction_ReloadMissileWeapon(*this);
}
