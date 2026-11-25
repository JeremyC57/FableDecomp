// CCreatureAction_ThrowGameObject::Clone() const -- `return new CCreatureAction_ThrowGameObject(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_ThrowGameObject@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062F290, 34 bytes: operator new(0xB4) 0x00BFEA1A, null test, then the copy constructor 0x0062F2C0 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_ThrowGameObject@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_ThrowGameObject@@QAE@ABV0@@Z: FableWin name set; retail 0x0062F2C0 stores
//   CCreatureAction_ThrowGameObject's vftable 0x01259044 first; the landed row at 0x0062F2C0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_ThrowGameObject@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_ThrowGameObject; a local class over its RTTI direct base CCreatureActionBase
//   engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_ThrowGameObject : public CCreatureActionBase
{
public:
    CCreatureAction_ThrowGameObject(const CCreatureAction_ThrowGameObject& other);   // 0x0062F2C0
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_ThrowGameObject@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0xB4 - sizeof(CCreatureActionBase)];   // CCreatureAction_ThrowGameObject's own members (not modelled: the copy constructor is out of line); sizeof 0xB4
};

CCreatureActionBase* CCreatureAction_ThrowGameObject::Clone() const
{
    return new CCreatureAction_ThrowGameObject(*this);
}
