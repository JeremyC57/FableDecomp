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
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#include "stlport/fable_containers.h"
class CThingAICreature;
#endif
class CTCTavernTable : public CTCBase {
public:
    enum ServingMode { SERVING_MODE_0 = 0, SERVING_MODE_1 = 1, SERVING_MODE_2 = 2 };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_TABLE; }
    bool GetServingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index,
                         CThingObject** tankard, ServingMode mode, bool reserve);                  // 0x007B8CC0
    void FreeServingPoint(long action_point_index);                                               // 0x007B9090
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
#endif
};
#endif
