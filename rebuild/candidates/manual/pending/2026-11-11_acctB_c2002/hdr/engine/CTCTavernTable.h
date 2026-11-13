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
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
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
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
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
};
#ifdef FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, Tankards) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, SeatsCurrentlyUsed) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, NumPeopleSeated) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTable, ActionPointTankardStrlen) == 0x54);
FABLE_STATIC_ASSERT(sizeof(CTCTavernTable) == 0x60);
#endif
#endif
