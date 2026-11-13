#ifndef FABLE_ENGINE_CTCTAVERNTABLE_H
#define FABLE_ENGINE_CTCTAVERNTABLE_H
// New header (chunk 1024, rows 0x00966DA0 CAIStateGroup_TavernServer::GetServingPoint / 0x00967D40 Update): the tavern-table
// component's members those rows call. Interface 0xB8 = TCI_TAVERN_TABLE (ETCInterfaceType.h, Ego_r enum; bit 24 of the
// UsingComponents word at CThing +0x34). Ego_r.pdb / FableWin.pdb spellings:
//   ?GetServingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PAPAVCThingObject@@W4ServingMode@1@_N@Z
//     (retail 0x007B8CC0; Ego_r 0x007B7C80), ?FreeServingPoint@CTCTavernTable@@QAEXJ@Z (retail 0x007B9090; Ego_r 0x007B8090, the
//     next PDB body after GetServingPoint as retail's 0x007B9090 is). The nested enum's enumerator names are not in the name
//     sets (descriptive, as the landed CAIStateGroup_ServeFood rows spell them). Declarations only; the layout is not modelled.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class C3DVector;
class CRightHandedSet;
class CThing;
class CThingObject;
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT
class CCharString;
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#include "stlport/fable_containers.h"
class CThingAICreature;
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_SEAT
class CCharString;
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT_B
// Retail opt-in (w22 agent2002, row 0x007B90F0 CTCTavernTable::OnInitialActivate): the members in Ego_r.pdb order and spelling
// (struct_layouts_egor.tsv) at the retail offsets the landed constructor 0x007B8710 stores (STLport containers 12 bytes; the PDB's
// vector<bool> is the generic byte vector in retail: OnInitialActivate grows it through ?_M_fill_insert@?$vector@_NV?$allocator@_N@std@@@std@@
// 0x004676EF and clears entries with `mov byte ptr [edi+ecx],0`), sizeof 0x60, and the slot-6 override plus GetTankardName 0x007B8680
// (Ego_r.pdb ?GetTankardName@CTCTavernTable@@QAE?AVCCharString@@XZ, landed). Needs stlport/fable_containers.h and the retail
// CIntelligentPointer (FABLE_RETAIL_CINTELLIGENTPOINTER).
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
class CThingAICreature;
#endif
class CTCTavernTable : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT_B
    virtual void OnInitialActivate();                                                             //  6 0x007B90F0
    CCharString GetTankardName();                                                                 // 0x007B8680
    bool BeingServed;                                                                             // +0x0c
    std::vector<CThingAICreature*> Occupants;                                                     // +0x10
    std::vector<CIntelligentPointer<CThingObject> > Tankards;                                     // +0x1c
    std::vector<CIntelligentPointer<CThingObject> > Food;                                         // +0x28
    std::vector<bool> SeatsCurrentlyUsed;                                                         // +0x34
    long NumPeopleSeated;                                                                         // +0x40
    bool SomeoneTalking;                                                                          // +0x44
    bool SomeoneLaughing;                                                                         // +0x45
    long ActionPointSitStrlen;                                                                    // +0x48
    long ActionPointGameMasterStrlen;                                                             // +0x4c
    long ActionPointHeroStrlen;                                                                   // +0x50
    long ActionPointTankardStrlen;                                                                // +0x54
    long ActionPointMaidStrlen;                                                                   // +0x58
    bool AvailableForUse;                                                                         // +0x5c
#endif
    enum ServingMode { SERVING_MODE_0 = 0, SERVING_MODE_1 = 1, SERVING_MODE_2 = 2 };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_TABLE; }
    bool GetServingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index,
                         CThingObject** tankard, ServingMode mode, bool reserve);                  // 0x007B8CC0
    void FreeServingPoint(long action_point_index);                                               // 0x007B9090
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_FREESEAT
    // Retail opt-in (wf7, row 0x009486D0 CAIStateGroup_GoToPub::FreeSeat): Ego_r.pdb / FableWin.pdb
    // ?FreeSeat@CTCTavernTable@@QAEXJJ@Z (Ego_r 0x007B7A20, retail 0x007B8AA0, landed), out of line: the row pushes the group's
    // ActionPointIndex then its SeatIndex (0x0094873C). Declaration only.
    void FreeSeat(long seat_index, long action_point_index);                                      // 0x007B8AA0
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_SEATS
    // Nested opt-in (chunk Q22, row 0x0096CD70 CAIStateGroup_EatAtTable::Update): Ego_r.pdb / FableWin.pdb
    // ?SetSeatState@CTCTavernTable@@QAEXJ_N@Z (Ego_r 0x007B7610, retail 0x007B8530) and ?GetFood@CTCTavernTable@@QAEPAVCThingObject@@J@Z
    // (Ego_r 0x007B7590, retail 0x007B85C0), out of line (the Ego_r order GetFood / SetFood / SetSeatState is retail's 0x007B85C0 /
    // 0x007B85F0 / 0x007B8530 run). Members (Ego_r.pdb, retail offsets: the CTCBase is 0xC, no container before Occupants):
    // BeingServed +0x0C, Occupants (vector<CThingAICreature*>) +0x10 -- the row stores the seated creature through Occupants'
    // start word in place (`mov ecx,[ebp+10h] ; mov [ecx+edx*4],eax`, 0x0096CE63). The members after Occupants are not modelled.
    void SetSeatState(long seat, bool used);                                                      // 0x007B8530
    CThingObject* GetFood(long seat);                                                             // 0x007B85C0
    bool BeingServed;                                                                             // +0x0C
    std::vector<CThingAICreature*> Occupants;                                                     // +0x10
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
    // Nested opt-in (wf4, row 0x007B8B10 CTCTavernTable::GetEatingPoint): the members after Occupants (Ego_r.pdb
    // struct_layouts_egor.tsv names and order; retail offsets with 12-byte STLport vectors, proven by the row: Food start/finish
    // +0x28/+0x2C with 8-byte CIntelligentPointer elements, ActionPointSitStrlen +0x48, AvailableForUse +0x5C; SeatsCurrentlyUsed
    // +0x34 / NumPeopleSeated +0x40 by the landed 0x007B8530 / 0x007B8AA0), and Ego_r.pdb / FableWin.pdb
    // ?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z (Ego_r 0x007B7AD0, retail 0x007B8B10).
    // Define FABLE_RETAIL_CINTELLIGENTPOINTER and include engine/CIntelligentPointer.h before this header.
    std::vector<CIntelligentPointer<CThingObject> > Tankards;                                     // +0x1C
    std::vector<CIntelligentPointer<CThingObject> > Food;                                         // +0x28
    unsigned char SeatsCurrentlyUsed[0xC];                 // +0x34 vector<bool> (retail: one byte per seat, 12-byte vector)
    long NumPeopleSeated;                                                                         // +0x40
    bool SomeoneTalking;                                                                          // +0x44
    bool SomeoneLaughing;                                                                         // +0x45
    long ActionPointSitStrlen;                                                                    // +0x48
    long ActionPointGameMasterStrlen;                                                             // +0x4C
    long ActionPointHeroStrlen;                                                                   // +0x50
    long ActionPointTankardStrlen;                                                                // +0x54
    long ActionPointMaidStrlen;                                                                   // +0x58
    bool AvailableForUse;                                                                         // +0x5C
    bool GetEatingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index);   // 0x007B8B10
#endif
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT
    // Retail opt-in (wf12, row 0x0096CB40 CAIStateGroup_EatAtTable::GetSeat): Ego_r.pdb / FableWin.pdb
    // ?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z (retail 0x007B8840), out of
    // line; the row pushes 0 for the action-point name. Declaration only.
    bool GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index,
                 const CCharString* action_point_name);                                          // 0x007B8840
#endif
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_SEAT
    // Opt-in (chunk 2020, row 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable and its same-TU GetSeat 0x009482A0): Ego_r.pdb / FableWin.pdb
    // ?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z (retail 0x007B8840, `ret 18h`:
    // creature, position, facing, action point index, seat index, seat name) and ?GetTankard@CTCTavernTable@@QAEPAVCThingObject@@J@Z
    // (retail 0x007B8590, landed). Declarations only; parameter names descriptive.
    bool GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index,
                 const CCharString* seat_name);                                                  // 0x007B8840
    CThingObject* GetTankard(long seat_index);                                                    // 0x007B8590
#endif
};
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT_B
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, Tankards) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, SeatsCurrentlyUsed) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, NumPeopleSeated) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, ActionPointTankardStrlen) == 0x54);
FABLE_STATIC_ASSERT(sizeof(CTCTavernTable) == 0x60);
#endif
#endif
