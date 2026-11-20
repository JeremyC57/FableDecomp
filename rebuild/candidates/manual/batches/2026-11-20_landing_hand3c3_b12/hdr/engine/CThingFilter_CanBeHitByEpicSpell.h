#ifndef FABLE_ENGINE_CTHINGFILTER_CANBEHITBYEPICSPELL_H
#define FABLE_ENGINE_CTHINGFILTER_CANBEHITBYEPICSPELL_H
// CThingFilter_CanBeHitByEpicSpell (new, hand3c3 row 0x0078D250 CTCBerserk::FrameUpdate): Ego_r.pdb TPI class of size 1, no members
// (ghidra_out/struct_layouts_egor.tsv); Ego_r.pdb / Ego_d.pdb ??RCThingFilter_CanBeHitByEpicSpell@@QBE_NABVCThing@@@Z, out of line
// in retail at 0x006A8D30 (landed). The spell rows bind it as the second filter of a CFilter_Bind2 / CFilter_Bind3 (e.g.
// ??$GetAllThingsInRoughArea@VCThingGameObject@@..V?$CFilter_Bind2@VCThingFilter_In2DRange@@VCThingFilter_CanBeHitByEpicSpell@@@@..
// 0x0078DB20); a value-initialised temporary of it is one zeroed byte (`mov byte ptr [esp+1Ch],0`, 0x0078D404).
class CThing;
class CThingFilter_CanBeHitByEpicSpell {
public:
    bool operator()(const CThing& test_thing) const;                    // 0x006A8D30
};
#endif
