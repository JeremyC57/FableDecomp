// CAIStateGroup_GoToPub::FreeSeat() -- give the creature's seat back to the tavern table, put the tankard it was drinking from
// back on the table, and clear the table / tankard pointers and the two seat indices.
// Leaf: ?FreeSeat@CAIStateGroup_GoToPub@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009486D0, length 432 bytes (plain `ret`, __fastcall, no arguments).
// Name: Ego_r.pdb (0x0096C400) and FableWin.pdb (0x0180FCB4) both give ?FreeSeat@CAIStateGroup_GoToPub@@IAEXXZ (protected).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CVectorMap::LowerBound 0x0040F020 (the inlined CThing::GetTC lookups: TCI_TAVERN_TABLE 0xB8 = bit 24 of +0x34,
//   TCI_CARRYING 0x46 = bit 6 of +0x28, TCI_TAVERN_TANKARD 0xBA = bit 26 of +0x34),
//   CTCTavernTable::FreeSeat(long, long) 0x007B8AA0, CTCCarrying::GetThingInPrimarySlot 0x006A1440 /
//   RemoveThingFromPrimarySlot(bool) 0x006A1E00, CTCTavernTankard::ReplaceTankard 0x00947C30.
// Shape: guarded by "has a table and an action point". The component out-pointers are uninitialised locals filled by the inlined
// GetTC (hit path: lookup result; miss path: the never-written slot, one 8-byte frame slot shared with the key temporaries and
// the two pointer temporaries). The carried thing is read into a local BEFORE the tankard pointer is read (retail calls
// 0x006A1440 first, keeps it in esi, then GetPItem: `cmp esi,eax`). The pointer members are cleared by assigning 0, which builds
// a CIntelligentPointer<T>(0) temporary (vtables 0x01238C6C <CThing>, 0x0125A858 <CThingObject>) and runs the inline operator=
// (GetPItem on the temporary, SetPItem on the member); the -1 of both index stores lives in esi from the join after the tankard
// block (`or esi,-1`), the ActionPointIndex test itself is an immediate compare.
// IS_THING_ALIVE is the retail template from engine/ThingAlive.h (FableWin instantiation 0x01CCA150) with the retail
// CIntelligentPointer<T> != int comparison (FableWin 0x01CD9DD0): GetPItem, test, then the inlined IsDead.
// No engine header exists for CAIStateGroup_GoToPub or CTCTavernTankard: local declarations (Ego_r.pdb member names and offsets:
// PPub +0x14, PBar +0x1c, PTable +0x24, PBarman +0x2c, PTankard +0x34, SeatIndex +0x3c, Pos +0x40, RHSet +0x4c,
// ActionPointIndex +0x64).
#define FABLE_RETAIL_CTCTAVERNTABLE_FREESEAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
#include "engine/CThingObject.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTavernTable.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CThingBuilding;

class CTCTavernTankard { public: void ReplaceTankard(); };                    // 0x00947C30

class CAIStateGroup_GoToPub : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingBuilding> PPub;       // +0x14
    CIntelligentPointer<CThingObject> PBar;         // +0x1c
    CIntelligentPointer<CThingObject> PTable;       // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;  // +0x2c
    CIntelligentPointer<CThing> PTankard;           // +0x34
    long SeatIndex;                                 // +0x3c
    C3DVector Pos;                                  // +0x40
    CRightHandedSet RHSet;                          // +0x4c
    long ActionPointIndex;                          // +0x64 (-1 = none)
protected:
    void FreeSeat();
};
void CAIStateGroup_GoToPub::FreeSeat()
{
    if (PTable.Get() && ActionPointIndex != -1) {
        CTCTavernTable* table;
        PTable.Get()->GetTC(TCI_TAVERN_TABLE, &table);
        table->FreeSeat(SeatIndex, ActionPointIndex);
        CTCCarrying* carrying;
        GetCreature().GetTC(TCI_CARRYING, &carrying);
        if (IS_THING_ALIVE(PTankard)) {
            CThing* held = carrying->GetThingInPrimarySlot();
            if (held == PTankard.Get())
                carrying->RemoveThingFromPrimarySlot(false);
            CTCTavernTankard* tankard;
            PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tankard);
            tankard->ReplaceTankard();
        }
        ActionPointIndex = -1;
        PTankard = 0;
        PTable = 0;
        SeatIndex = -1;
    }
}