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
#ifdef FABLE_RETAIL_CINVENTORYDEF_DOLLS
#include "engine/CDefString.h"
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CEngineGraphic.h"
#endif

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
#ifdef FABLE_RETAIL_CINVENTORYDEF_DOLLS
    // Opt-in (agentNF12, 2026-11-11, rows 0x005B37F7 CTCInventoryClothing::InitialiseMannequin / 0x005CD767
    // CTCInventoryStats::InitialiseMannequin): the Ego_r.pdb members after MeshAreaBRPos, in order and spelling, through
    // StatsPersonalityDoll, at the retail STLport offsets (PDB - 0x14 up to StatLevelsTexts, PDB - 0x1C after the two 12-byte
    // vectors). Evidence: InitialiseMannequin reads the doll def index at +0xD4 (clothing, PDB ClothingDoll +232;
    // `cmp dword ptr [ecx+0D4h],0`, 0x005B380B) and +0x2E0 (stats, PDB StatsPersonalityDoll +764; `cmp dword ptr [ecx+2E0h],0`,
    // 0x005CD77B). CEngineGraphic is the 14-byte packed class; EInventoryIdentifier (no definition on disk) is kept opaque.
    // Members only: no change before +0xB4.
    C2DVector NextCategoryOffset;                           // +0xB4 (PDB +200)
    C2DVector FirstItemOffsetFromCategory;                  // +0xBC (PDB +208)
    C2DVector NextItemOffset;                               // +0xC4 (PDB +216)
    CDefString GenericFont;                                 // +0xCC (PDB +224)
    CRGBColour BackGroundColour;                            // +0xD0 (PDB +228)
    long ClothingDoll;                                      // +0xD4 (PDB +232)
    C3DVector DollCentreOffset;                             // +0xD8 (PDB +236)
    float DollBoundingSphereRadius;                         // +0xE4 (PDB +248)
    C2DVector TotalExperienceTextPos;                       // +0xE8 (PDB +252)
    unsigned long TotalExperienceText;                      // +0xF0 (PDB +260)
    C2DVector SpendingExperienceTextPos;                    // +0xF4 (PDB +264)
    unsigned long SpendingExperienceText;                   // +0xFC (PDB +272)
    C2DVector StrengthLevelTextPos;                         // +0x100 (PDB +276)
    CRGBColour StrengthLevelColour;                         // +0x108 (PDB +284)
    C2DVector SkillLevelTextPos;                            // +0x10C (PDB +288)
    CRGBColour SkillLevelColour;                            // +0x114 (PDB +296)
    C2DVector WillLevelTextPos;                             // +0x118 (PDB +300)
    CRGBColour WillLevelColour;                             // +0x120 (PDB +308)
    C2DVector RenownLevelTextPos;                           // +0x124 (PDB +312)
    C2DVector HeroLevelTextPos;                             // +0x12C (PDB +320)
    C2DVector StatLevelsFirstLineTextPos;                   // +0x134 (PDB +328)
    C2DVector StatLevelsNextLineTextOffset;                 // +0x13C (PDB +336)
    C2DVector ButtonAssignmentTextPos;                      // +0x144 (PDB +344)
    C2DVector ButtonAssignmentTextOffset;                   // +0x14C (PDB +352)
    std::vector<unsigned long> StatLevelsTexts;             // +0x154 (PDB +360)
    std::vector<CRGBColour> StatLevelsColours;              // +0x160 (PDB +376)
    long WeaponsTextSeparator;                              // +0x16C (PDB +392)
    long WeaponsTextNumberOfSlots;                          // +0x170 (PDB +396)
    long WeaponsTextSlot;                                   // +0x174 (PDB +400)
    long WeaponsTextSuccessfulAugmentation;                 // +0x178 (PDB +404)
    long WeaponsTextNoFreeSlots;                            // +0x17C (PDB +408)
    long WeaponsTextReadyToAugment;                         // +0x180 (PDB +412)
    long WeaponsTextNotReadyToAugment;                      // +0x184 (PDB +416)
    C2DVector WeaponsDescriptionBoxTLPos;                   // +0x188 (PDB +420)
    C2DVector WeaponsDescriptionBoxExtents;                 // +0x190 (PDB +428)
    C2DVector WeaponsActiveMeleeIconPos;                    // +0x198 (PDB +436)
    C2DVector WeaponsActiveRangeIconPos;                    // +0x1A0 (PDB +444)
    C2DVector WeaponAugmentationTextStartPos;               // +0x1A8 (PDB +452)
    C2DVector WeaponAugmentationTextNewLineOffset;          // +0x1B0 (PDB +460)
    C2DVector AugmentationStatusTextAreaTLPos;              // +0x1B8 (PDB +468)
    C2DVector AugmentationStatusTextAreaBRPos;              // +0x1C0 (PDB +476)
    float TimeToDisplayWarningMessageInSeconds;             // +0x1C8 (PDB +484)
    C2DVector WeaponsShadedCircleTLPos;                     // +0x1CC (PDB +488)
    C2DVector WeaponsShadedCircleExtents;                   // +0x1D4 (PDB +496)
    float NumberOfSecondsForObjectRotation;                 // +0x1DC (PDB +504)
    long MaxNumberOfItemsToDisplayPerCategory;              // +0x1E0 (PDB +508)
    long MaxNumberOfCategoriesToDisplay;                    // +0x1E4 (PDB +512)
    long MaxNumberOfScreenNamesToDisplay;                   // +0x1E8 (PDB +516)
    unsigned long InventoryName;                            // +0x1EC (PDB +520)
    unsigned char InventoryIdentifier[0x4];                 // +0x1F0 EInventoryIdentifier (PDB +524)
    C2DVector MapScreenMapDisplayBoxTLPos;                  // +0x1F4 (PDB +528)
    C2DVector MapScreenMapDisplayBoxBRPos;                  // +0x1FC (PDB +536)
    C2DVector MapScreenRegionNamePos;                       // +0x204 (PDB +544)
    long MapScreenWorldMapGraphic;                          // +0x20C (PDB +552)
    C2DVector MapScreenWorldMapPos;                         // +0x210 (PDB +556)
    long MapScreenRegionUnknownGraphic;                     // +0x218 (PDB +564)
    long MapScreenRegionUnknownHighlightGraphic;            // +0x21C (PDB +568)
    long MapScreenRegionHeroGraphic;                        // +0x220 (PDB +572)
    long MapScreenRegionQuestGraphic;                       // +0x224 (PDB +576)
    long MapScreenRegionTeleportGraphic;                    // +0x228 (PDB +580)
    long MapScreenRegionHouseGraphic;                       // +0x22C (PDB +584)
    C2DVector StatsTitleTLPos;                              // +0x230 (PDB +588)
    C2DVector StatsShadedCircleTLPos;                       // +0x238 (PDB +596)
    C2DVector StatsShadedCircleExtents;                     // +0x240 (PDB +604)
    CEngineGraphic StatsProgBarL;                           // +0x248 (PDB +612)
    CEngineGraphic StatsProgBarC;                           // +0x256 (PDB +626)
    CEngineGraphic StatsProgBarR;                           // +0x264 (PDB +640)
    CEngineGraphic StatsProgBarValue;                       // +0x272 (PDB +654)
    C2DVector StatsProgBarValueOffset;                      // +0x280 (PDB +668)
    C2DVector StatsPersonalityHeadingTLPos;                 // +0x288 (PDB +676)
    C2DVector StatsPersonalityHeadingBRPos;                 // +0x290 (PDB +684)
    unsigned long StatsPersonalityRenownLowerLabel;         // +0x298 (PDB +692)
    unsigned long StatsPersonalityRenownUpperLabel;         // +0x29C (PDB +696)
    unsigned long StatsPersonalityGoodnessLowerLabel;       // +0x2A0 (PDB +700)
    unsigned long StatsPersonalityGoodnessUpperLabel;       // +0x2A4 (PDB +704)
    unsigned long StatsPersonalityAttractivenessLowerLabel; // +0x2A8 (PDB +708)
    unsigned long StatsPersonalityAttractivenessUpperLabel; // +0x2AC (PDB +712)
    unsigned long StatsPersonalityScarinessLowerLabel;      // +0x2B0 (PDB +716)
    unsigned long StatsPersonalityScarinessUpperLabel;      // +0x2B4 (PDB +720)
    unsigned long StatsPersonalityNicenessLowerLabel;       // +0x2B8 (PDB +724)
    unsigned long StatsPersonalityNicenessUpperLabel;       // +0x2BC (PDB +728)
    C2DVector StatsPersonalityLabelTextOffset;              // +0x2C0 (PDB +732)
    C2DVector StatsPersonalityLowerLabelTLPos;              // +0x2C8 (PDB +740)
    float StatsPersonalityLowerLabelWidth;                  // +0x2D0 (PDB +748)
    float StatsPersonalityLowerLabelHeight;                 // +0x2D4 (PDB +752)
    float StatsPersonalitySliderWidth;                      // +0x2D8 (PDB +756)
    float StatsPersonalityUpperLabelWidth;                  // +0x2DC (PDB +760)
    long StatsPersonalityDoll;                              // +0x2E0 (PDB +764)
#endif
};
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialCategoryMovementRepeatDelay) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialInventoryClothingItems) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaTLPos) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaBRPos) == 0xAC);
#ifdef FABLE_RETAIL_CINVENTORYDEF_DOLLS
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, ClothingDoll) == 0xD4);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, StatLevelsTexts) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, StatsProgBarL) == 0x248);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, StatsPersonalityDoll) == 0x2E0);
#endif

#endif // FABLE_ENGINE_CINVENTORYDEF_H
