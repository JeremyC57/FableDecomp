// CCreatureAction_SheatheItemToInventory::Clone() const -- `return new CCreatureAction_SheatheItemToInventory(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_SheatheItemToInventory@@EBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0C20, 34 bytes: operator new(0xB0) 0x00BFEA1A, null test, then the copy constructor 0x006A0C50 on `*this`;
//   a private const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_SheatheItemToInventory@@EBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_SheatheItemToInventory@@QAE@ABV0@@Z: FableWin name set; retail 0x006A0C50 stores
//   CCreatureAction_SheatheItemToInventory's vftable 0x0125C83C first; the landed row at 0x006A0C50 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_SheatheItemToInventory@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureAction_SheatheItemToInventory.h does not give the class at sizeof 0xB0 with Clone declared in
//   the prelude tried (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase
{
public:
    CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other);   // 0x006A0C50
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_SheatheItemToInventory@@UAEXXZ (both PDB name sets)
private:
    virtual CCreatureActionBase* Clone() const;
public:
    unsigned char _body[0xB0 - sizeof(CCreatureActionBase)];   // CCreatureAction_SheatheItemToInventory's own members (not modelled: the copy constructor is out of line); sizeof 0xB0
};

CCreatureActionBase* CCreatureAction_SheatheItemToInventory::Clone() const
{
    return new CCreatureAction_SheatheItemToInventory(*this);
}
