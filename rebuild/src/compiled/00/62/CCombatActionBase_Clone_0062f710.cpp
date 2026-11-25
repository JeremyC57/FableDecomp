// CCombatActionBase::Clone() const -- `return new CCombatActionBase(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCombatActionBase@@MBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062F710, 34 bytes: operator new(0x114) 0x00BFEA1A, null test, then the copy constructor 0x0062F3B0 on `*this`;
//   a protected const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCombatActionBase@@MBEPAVCCreatureActionBase@@XZ: RTTI placement + Ego_r/FableWin; callee
//   ??0CCombatActionBase@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x0062F3B0 stores CCombatActionBase's vftable 0x01259E44 first;
//   the landed row at 0x0062F3B0 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCombatActionBase@@QAEPAVCCreatureActionBase@@XZ (non-virtual, non-const); now the real
//   virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCombatActionBase.h does not give the class at sizeof 0x114 with Clone declared in the prelude tried
//   (header debt); a local class over its RTTI direct base CCreatureActionBase engine/CCreatureActionBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"

class CCombatActionBase : public CCreatureActionBase
{
public:
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
    virtual void PerformAction();                                   // ?PerformAction@CCombatActionBase@@UAEXXZ (FableWin name set; the base's is pure and retail instantiates the class)
protected:
    virtual CCreatureActionBase* Clone() const;
public:
    unsigned char _body[0x114 - sizeof(CCreatureActionBase)];   // CCombatActionBase's own members (not modelled: the copy constructor is out of line); sizeof 0x114
};

CCreatureActionBase* CCombatActionBase::Clone() const
{
    return new CCombatActionBase(*this);
}
