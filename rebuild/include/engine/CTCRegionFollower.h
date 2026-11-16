#ifndef FABLE_ENGINE_CTCREGIONFOLLOWER_H
#define FABLE_ENGINE_CTCREGIONFOLLOWER_H
// New header (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable). Retail opt-in only: FABLE_RETAIL_CTCREGIONFOLLOWER.
// Class: Fable.exe RTTI .?AVCTCRegionFollower@@ : CTCBase (class key V, pdb_class_keys.tsv); interface TCI_REGION_FOLLOWER (0x7A:
// `test dword [thing+2Ch],4000000h` and LowerBound key 0x7A at 0x00960D22..0x00960D47). Method: FableWin.pdb
// ?IsRegionFollower@CTCRegionFollower@@QBE_NPBVCThing@@@Z, out of line in retail at 0x006AEB60 (`push creature ; call`, 0x00960D58),
// and the inline ?StatGetInterfaceType@CTCRegionFollower@@SI?AW4ETCInterfaceType@@XZ. Members are not modelled (no sizeof assertion).
#ifdef FABLE_RETAIL_CTCREGIONFOLLOWER
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CThing;
class CTCRegionFollower : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_REGION_FOLLOWER; }
    bool IsRegionFollower(const CThing* thing) const;                    // 0x006AEB60
#ifdef FABLE_RETAIL_CTCREGIONFOLLOWER_REGIONLOAD
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?IsCreatureFollowingHeroAcrossRegions@CTCRegionFollower@@QBE_NABVCThingAICreature@@@Z
    // (0x006AEEA0), ?IsCreatureAllowedInRegionAt@CTCRegionFollower@@QBE_NABVCThingAICreature@@ABVC3DVector@@@Z (0x006AEBF0),
    // ?RemoveFollower@CTCRegionFollower@@QAEXAAVCThing@@@Z (0x006AEBA0), all called; GetFollowers 0x006AEE40 (landed row's name; in no PDB name set:
    // signature as CTCFollowed::GetFollowers, FableWin ?GetFollowers@CTCFollowed@@QAEXAAV?$vector@PAVCThing@@...; descriptive). Declarations only.
    void GetFollowers(std::vector<CThing*>&);                            // 0x006AEE40
    bool IsCreatureFollowingHeroAcrossRegions(const class CThingAICreature&) const;   // 0x006AEEA0
    bool IsCreatureAllowedInRegionAt(const class CThingAICreature&, const C3DVector&) const;   // 0x006AEBF0
    void RemoveFollower(CThing&);                                        // 0x006AEBA0
#endif // FABLE_RETAIL_CTCREGIONFOLLOWER_REGIONLOAD
};
#endif
#endif // FABLE_ENGINE_CTCREGIONFOLLOWER_H
