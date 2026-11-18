#ifndef FABLE_ENGINE_CAISTATEGROUP_USESSEAT_H
#define FABLE_ENGINE_CAISTATEGROUP_USESSEAT_H
// Shared header (NF67, round 7; promoted by the coordinator at landing nf67). The retail class
// CAIStateGroup_UsesSeat (Fable.exe RTTI .?AVCAIStateGroup_UsesSeat@@ : CAIStateGroupBase; the base of CAIStateGroup_MinionSitOnSeatBase
// and CAIStateGroup_SitInPub). Layout Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv, sizeof 0x34): PSeat CIntelligentPointer<CThingObject>
// +0x14, GotSeatPos +0x1c, Sitting +0x1d, SeatPosIndex +0x20, SeatPos C3DVector +0x24, SittingOnBench +0x30. Members: the Ego_r.pdb /
// FableWin.pdb set (egor_pdb_names.tsv), declarations only; virtual slot order not verified (no row here calls them virtually).
// (shared header since landing nf67; FABLE_RETAIL_CAISTATEGROUP_USESSEAT, which its first rows define, is no longer needed)
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThingObject;
class CThingBuilding;
enum EMovementType;                                                      // MS forward enum (engine/EMovementType.h)
namespace NCreatureMode { class CModeSitBase; }
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {
public:
    virtual bool IsStillValid() const;                                   // ?IsStillValid@CAIStateGroup_UsesSeat@@UBE_NXZ
    virtual void OnReplace();                                            // ?OnReplace@CAIStateGroup_UsesSeat@@UAEXXZ
protected:
    CAIStateGroup_UsesSeat();                                            // ??0CAIStateGroup_UsesSeat@@IAE@XZ
    virtual NCreatureMode::CModeSitBase* AllocChairSittingMode() const;  // ?AllocChairSittingMode@CAIStateGroup_UsesSeat@@MBEPAVCModeSitBase@NCreatureMode@@XZ
    CThingObject* GetFreeSeatInBuilding(CThingBuilding& building) const;         // IBE
    CThingObject* GetRandomFreeSeatInBuilding(CThingBuilding& building) const;   // IBE, retail 0x00938BF0
    bool GetUpFromSeat();
    bool IsSeatValid();                                                  // retail 0x00938580
    bool MoveToSeat(EMovementType movement);
    bool MoveToSeatAndSitDown(EMovementType movement);
    void SetSeat(CThingObject* seat);                                    // retail 0x009106E0
    bool SitInSeat();
    CIntelligentPointer<CThingObject> PSeat;                             // +0x14
    bool GotSeatPos;                                                     // +0x1c
    bool Sitting;                                                        // +0x1d
    long SeatPosIndex;                                                   // +0x20
    C3DVector SeatPos;                                                   // +0x24
    bool SittingOnBench;                                                 // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_UsesSeat) == 0x34);
// end of class CAIStateGroup_UsesSeat
#endif // FABLE_ENGINE_CAISTATEGROUP_USESSEAT_H
