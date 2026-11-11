#ifndef FABLE_ENGINE_CTARGETINGDEF_H
#define FABLE_ENGINE_CTARGETINGDEF_H
// CTargetingDef (new shared header, chunk 1025, rows 0x008028D0 / 0x00802B60, the player-targeting TU; the generated flat model is
// engine/_quarantine/CTargetingDef.h). Retail class: RTTI .?AVCTargetingDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv).
// Members: Ego_r.pdb names and types (struct_layouts_egor.tsv, sizeof 0x94) at the retail offsets: everything up to the map holds
// (Type +0x28 .. ThirdPersonTargetingEnemyPreferenceFactor +0x70, TargetingRanges +0x74); the STLport 4.6 map is 12 bytes in
// retail (Ego_r's PDB view is 16), so the four weights after it sit 4 lower (+0x80..+0x8C, sizeof 0x90). Retail evidence: the
// landed GetTargetingRange 0x00802890 / CTCTargetingPlayer::OnInitialActivate 0x008026A0 read the map header at +0x74 and the node
// value at +0x14; the map's find is out of line in retail (0x007F9830, `ret 8`, hidden const_iterator).
// Inline accessor: FableWin.pdb ?GetTargetingRange@CTargetingDef@@QBEMW4ETargetingType@@@Z (0x024CB750 in FableWin; no Ego_r.pdb or
// Fable.exe copy: the file-static GetTargetingRange 0x00802890 is its expansion).
// ETargetingType: the enumerator names are not on disk (PDB extracts, strings, FSE, EgoCore); the rows cast the values they use.
// Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_map.h"
enum ETargetingType;                                                     // MS forward enum (as engine/CTCTargeted.h)
class CTargetingDef : public CSubDefClassBase {
public:
    // FableWin.pdb ?StatGetTypeName@CTargetingDef@@SI?AVCCharString@@XZ is not in the name sets; the class-name key GetSubDef
    // looks up is the RTTI name (the landed 0x00802D10 / 0x007EA960 spelling).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTargetingDef", -1); }
    float GetTargetingRange(ETargetingType type) const
    {
        std::map<ETargetingType, float>::const_iterator it = TargetingRanges.find(type);
        if (it != TargetingRanges.end())
            return (*it).second;
        return 0.0f;
    }
    unsigned long Type;                                                  // +0x28
    bool          UseGlowTargeting;                                      // +0x2c
    float         TargetArc;                                             // +0x30
    float         TargetArcPCThirdPersonAimingMode;                      // +0x34
    bool          TargetingBasePosFromCamera;                            // +0x38
    bool          TargetingFacingDirFromCamera;                          // +0x39
    bool          TargetingPCFacingDirFromCamera;                        // +0x3a
    bool          RejectTargetsBehindIfRunning;                          // +0x3b
    bool          AcceptOnlyEnemiesBehind;                               // +0x3c
    float         EnemyBehindTargetingRange;                             // +0x40
    float         FirstPersonTargetingAngleFalloff;                      // +0x44
    float         FirstPersonTargetingDistanceFalloff;                   // +0x48
    float         ThirdPersonTargetingAngleFalloff;                      // +0x4c
    float         ThirdPersonTargetingDistanceFalloff;                   // +0x50
    float         ZLockTargetSelectAngleFalloff;                         // +0x54
    float         ZLockTargetSelectDistanceFalloff;                      // +0x58
    float         FirstPersonTargetingOverrideShortRangeFactor;          // +0x5c
    float         FirstPersonTargetingOverrideShortRangeFalloff;         // +0x60
    float         ThirdPersonTargetingOverrideShortRangeFactor;          // +0x64
    float         ThirdPersonTargetingOverrideShortRangeFalloff;         // +0x68
    float         FirstPersonTargetingEnemyPreferenceFactor;             // +0x6c
    float         ThirdPersonTargetingEnemyPreferenceFactor;             // +0x70
    std::map<ETargetingType, float> TargetingRanges;                     // +0x74 (12 bytes, STLport 4.6)
    float         PreferPlayerWeight;                                    // +0x80 (Ego_r +0x84)
    float         PreferNonCombatantsWeight;                             // +0x84
    float         PreferLastAttackerWeight;                              // +0x88
    float         PreferCurrentBestEnemyWeight;                          // +0x8c
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTargetingDef, Type) == 0x28);
        FABLE_STATIC_ASSERT(offsetof(CTargetingDef, EnemyBehindTargetingRange) == 0x40);
        FABLE_STATIC_ASSERT(offsetof(CTargetingDef, TargetingRanges) == 0x74);
        FABLE_STATIC_ASSERT(offsetof(CTargetingDef, PreferPlayerWeight) == 0x80);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTargetingDef) == 0x90);
#endif // FABLE_ENGINE_CTARGETINGDEF_H
