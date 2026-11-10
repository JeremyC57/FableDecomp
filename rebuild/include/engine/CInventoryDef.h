#ifndef FABLE_ENGINE_CINVENTORYDEF_H
#define FABLE_ENGINE_CINVENTORYDEF_H
// Retail CInventoryDef (agentSND, 2026-11-10, row 0x005A94BD CTCInventoryTrade::OpenInventory): the def NInventory::CTCInventoryBase
// holds in PDef (Ego_r.pdb CDefPointer<const CInventoryDef>, GetPDef). Members: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv,
// sizeof 2656) in order and spelling, typed at the retail STLport offsets: the parent CParentDefClassBase is 0x39 bytes in retail
// (0x3D in the PDB), so the first float sits at +0x3C (PDB +0x40) and each 16-byte PDB vector is 12 bytes. Evidence for the
// offsets: 0x005A95E1 / 0x005A95F2 read the two C2DVector corners at +0xA4 / +0xAC (PDB MeshAreaTLPos +184 / MeshAreaBRPos +192)
// into SetInventoryAnimRenderBox's C2DBoxF. PARTIAL: only the members up to MeshAreaBRPos are modelled (no sizeof; the class is
// only reached through a def pointer).
#include <stddef.h>
#include "engine/CParentDefClassBase.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"

class CInventoryDef : public CParentDefClassBase {
public:
    float InitialCategoryMovementRepeatDelay;                   // +0x3C
    float ContinuedCategoryMovementRepeatDelay;                 // +0x40
    float InitialItemMovementRepeatDelay;                       // +0x44
    float ContinuedItemMovementRepeatDelay;                     // +0x48
    long MovementSound;                                         // +0x4C
    long NoMovementSound;                                       // +0x50
    long InventoryOpenSound;                                    // +0x54
    bool TrackSelectedItems;                                    // +0x58
    std::vector<long> InitialInventoryClothingItems;            // +0x5C
    std::vector<long> InitialInventoryWeaponsItems;             // +0x68
    std::vector<long> InitialInventoryStatsItems;               // +0x74
    std::vector<long> InitialInventoryStandardItems;            // +0x80
    C2DVector ClothingShadedCircleTLPos;                        // +0x8C
    C2DVector ClothingShadedCircleExtents;                      // +0x94
    C2DVector CategoriesTLPos;                                  // +0x9C
    C2DVector MeshAreaTLPos;                                    // +0xA4
    C2DVector MeshAreaBRPos;                                    // +0xAC
};
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialCategoryMovementRepeatDelay) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialInventoryClothingItems) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaTLPos) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaBRPos) == 0xAC);

#endif // FABLE_ENGINE_CINVENTORYDEF_H
