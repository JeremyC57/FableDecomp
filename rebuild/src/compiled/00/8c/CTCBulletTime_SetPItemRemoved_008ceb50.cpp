// CTCBulletTime::SetPItemRemoved(CIntelligentPointer<CThing> item) -- store the thing taken out of the world during bullet time (PItemRemoved = item)
// Leaf: ?SetPItemRemoved@CTCBulletTime@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CEB50, 34 bytes (+ int3 padding), thiscall, `ret 8` (the 8-byte
//   CIntelligentPointer by value).
// Identity (probable): CTCBulletTime::SetPItemRemoved(CIntelligentPointer<CThing>); evidence: the body sets the intelligent
//   pointer at this+0x3C from a by-value CIntelligentPointer; FableWin.pdb declares ?SetPItemRemoved@<class>@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z
//   for fourteen spell classes and Ego_r.pdb puts PItemRemoved at +0x3C only in CTCBulletTime (no STL member before it, so
//   retail = Ego_r); the row sits in the time-spell creature-action code (CCreatureAction_CastTimeSpell 0x008CEB90 follows) and
//   the same-shape setter before it, 0x008CEAB0, writes +0x44 = CTCBattleCharge::PItemRemoved. Retail never references it
//   (no call or pointer), hence probable.
// Label: the manifest's "CTCInventoryBase::GetItemDescription" is propagated (family J106).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90
//   (Ego_r.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z), CBaseIntelligentPointer::~CBaseIntelligentPointer()
//   0x00A01C10 (the argument's destructor).
// Shape: `PItemRemoved = item;` -- CIntelligentPointer<CThing>::operator=(const CIntelligentPointer&) (FableWin.pdb
//   ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z) inline as SetPItem(other.GetPItem()).
// Layout: engine/CTCBulletTime.h under a layout name (PItemRemoved +0x3C is an opaque 8-byte member there).
#define CTCBulletTime CTCBulletTimePdb
#include "engine/CTCBulletTime.h"
#undef CTCBulletTime

class CBaseObject;
class CBaseObjectPointer;
class CThing;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
#pragma pack(pop)

class CTCBulletTime : public CTCBulletTimePdb {
public:
    void SetPItemRemoved(CIntelligentPointer<CThing> item);
};

void CTCBulletTime::SetPItemRemoved(CIntelligentPointer<CThing> item)
{
    reinterpret_cast<CIntelligentPointer<CThing>&>(PItemRemoved) = item;
}