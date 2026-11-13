#ifndef FABLE_ENGINE_CAISTATEGROUP_GOTOPUB_H
#define FABLE_ENGINE_CAISTATEGROUP_GOTOPUB_H
// New header (chunk 2020, row 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable). Retail class: RTTI .?AVCAIStateGroup_GoToPub@@ :
// CAIStateGroupBase : CBaseClass, vtable 0x0128BEC4 (the landed implicit destructor 0x008FDCCA destroys the five pointer members
// +0x34 .. +0x14 and tail-jumps to ~CAIStateGroupBase). Layout: Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0xA8; no containers, so
// the retail offsets are the PDB's) -- the same members the landed CAIStateGroup_GameMaster::GetSeat 0x0094CB40 and FallAsleep
// 0x0094A0E0 read (PPub +0x14, PTable +0x24, PTankard +0x34, SeatIndex +0x3C, Pos +0x40, RHSet +0x4C, ActionPointIndex +0x64) and
// IsRunnable reads (NextFramePub +0x68, UnderScriptControl +0xA1). CAIStateGroup_GameMaster derives from it, so the members are
// protected. Methods: the Ego_r.pdb / FableWin.pdb spellings (protected `IAE` helpers, protected virtual `MAE` overrides) at their
// retail addresses (Ego_r donor order GetObjectWithScriptName .. IsStaffed .. GetSeat .. FreeSeat .. GetBar = retail 0x00947EB0,
// 0x00947FB0, 0x009482A0, 0x009486D0, 0x009490D0; each body checked: GetSeat `ret 8`, FreeSeat frees PTable's seat through
// CTCTavernTable at ActionPointIndex). GetSeat's two parameters default to NULL: IsRunnable 0x00949D7C..0x0094A0B7 expands exactly
// GetSeat's no-table branch (0x009483A9..0x009486CD) with the table / seat tests folded away, while the scripted call
// 0x00949C89 passes both. Parameter names are descriptive.
#ifdef FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CThing; class CThingObject; class CThingBuilding; class CThingAICreature;
class CAIStateGroup_GoToPub : public CAIStateGroupBase {
public:
    CAIStateGroup_GoToPub();                                                // ??0CAIStateGroup_GoToPub@@QAE@XZ
protected:
    virtual bool IsRunnable();                                              // 0x00949AB0 ?IsRunnable@CAIStateGroup_GoToPub@@MAE_NXZ
    virtual void Update();                                                  // ?Update@CAIStateGroup_GoToPub@@MAEXXZ
    CThingObject* GetObjectWithScriptName(const CCharString& script_name);  // 0x00947EB0
    bool IsStaffed();                                                       // 0x00947FB0
    bool GetSeat(CThing* table = 0, const CCharString* seat_name = 0);      // 0x009482A0
    void FreeSeat();                                                        // 0x009486D0
    bool GetBar();                                                          // 0x009490D0
    CIntelligentPointer<CThingBuilding>   PPub;                             // +0x14
    CIntelligentPointer<CThingObject>     PBar;                             // +0x1C
    CIntelligentPointer<CThingObject>     PTable;                           // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;                          // +0x2C
    CIntelligentPointer<CThing>           PTankard;                         // +0x34
    long            SeatIndex;                                              // +0x3C
    C3DVector       Pos;                                                    // +0x40
    CRightHandedSet RHSet;                                                  // +0x4C
    long            ActionPointIndex;                                       // +0x64
    long            NextFramePub;                                           // +0x68
    long            FrameLeavePub;                                          // +0x6C
    bool            Sitting;                                                // +0x70
    long            BarActionPointIndex;                                    // +0x74
    C3DVector       BarPos;                                                 // +0x78
    CRightHandedSet BarRHSet;                                               // +0x84
    long            BarIndex;                                               // +0x9C
    bool            HandsOffMode;                                           // +0xA0
    bool            UnderScriptControl;                                     // +0xA1
    long            TimeBeforeAskingForBeer;                                // +0xA4
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_GoToPub, PPub) == 0x14);
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_GoToPub, SeatIndex) == 0x3c);
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_GoToPub, ActionPointIndex) == 0x64);
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_GoToPub, NextFramePub) == 0x68);
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_GoToPub, UnderScriptControl) == 0xa1);
    }
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_GoToPub) == 0xa8);
#endif // FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#endif // FABLE_ENGINE_CAISTATEGROUP_GOTOPUB_H
