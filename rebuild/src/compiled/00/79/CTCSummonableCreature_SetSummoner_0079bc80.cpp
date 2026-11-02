// CTCSummonableCreature::SetSummoner(CThingCreatureBase* summoner) -- store the summoner in Summoner through a temporary CIntelligentPointer<CThingCreatureBase> (converting constructor, then the pointer assignment)
// Leaf: ?SetSummoner@CTCSummonableCreature@@QAEXPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079BC80, 71 bytes, thiscall, `ret 4`; no caller (an unreferenced COMDAT: retail
//   inlines it at its use site).
// Identity (proven): (1) FableWin.pdb spells ?SetSummoner@CTCSummonableCreature@@QAEXPAVCThingCreatureBase@@@Z (donor 0x026A3850,
//   next to SetHasBeenSummoned); Ego_r.pdb inlines it (no out-of-line copy) and lays CTCSummonableCreature out as CTCBase +
//   PDef (CDefPointer<const CSummonableCreatureDef>) +0x0C, HasBeenSummoned +0x10, Summoner (CIntelligentPointer<CThingCreatureBase>)
//   at the byte-aligned +0x11 -- the only Ego_r class with that pointer type at +0x11; the landed ctor 0x0079C240 builds Summoner
//   at retail +0x11 with the same vtable 0x0123E7D8 (RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@) the temporary here
//   stores; (2) the row sits in CTCSummonSpell's TU, and CTCSummonSpell::CreateMinion 0x0079ACC0 inlines exactly this body at
//   0x0079B095..0x0079B0CA right after the HasBeenSummoned = true store on the minion's summonable component. The catalog label
//   "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90 (twice), CBaseIntelligentPointer::GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: the chunk 700 setter (0x007F5810 / 0x008ABB50 / 0x008D73E0): `Summoner = summoner;` through the PDB's
//   CIntelligentPointer(T*) and operator=(const CIntelligentPointer&) inline; no derived destructor declared, so the temporary
//   dies through the base destructor.
// Layout: engine/CTCBase.h (flattened vptr, `struct` key: header debt, as chunk 700) as the base; `#pragma pack(1)` on the
//   CIntelligentPointer classes (retail packs them: Summoner at +0x11); PDef from engine/CDefPointer.h. No engine header exists
//   for CTCSummonableCreature.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CTCBase.h"                                 // CTCBase (flattened vptr, 0xc bytes)

class CBaseObject;
class CBaseObjectPointer;
class CThingCreatureBase;
class CSummonableCreatureDef;
#pragma pack(push, 1)                                       // the pointer classes are byte-aligned (Summoner sits at +0x11)
class CBaseIntelligentPointer {                             // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* Get() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(pop)

class CTCSummonableCreature : public CTCBase {              // vtable 0x0126C41C, 0x19 bytes (Ego_r 0x1c)
public:
    void SetSummoner(CThingCreatureBase* summoner);
    CDefPointer<const CSummonableCreatureDef> PDef;         // +0x0c
    bool                                      HasBeenSummoned;   // +0x10
    CIntelligentPointer<CThingCreatureBase>   Summoner;     // +0x11
};

void CTCSummonableCreature::SetSummoner(CThingCreatureBase* summoner)
{
    Summoner = summoner;
}