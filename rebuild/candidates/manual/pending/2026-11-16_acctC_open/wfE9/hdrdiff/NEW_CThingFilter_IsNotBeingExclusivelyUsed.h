#ifndef FABLE_ENGINE_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_H
#define FABLE_ENGINE_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_H
// New header (wfE9, row 0x00931390 CAIStateGroup_MinionReactToCriminal::Update). Ego_r.pdb (0x005A4AE0) / FableWin.pdb (0x01F95CB0)
// ??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z (retail 0x006A8AF0, landed, out of line). An empty filter
// (struct_layouts_egor.tsv: sizeof 1, no members).
class CThing;
class CThingFilter_IsNotBeingExclusivelyUsed {
public:
    bool operator()(const CThing& thing) const;   // 0x006A8AF0
};
#endif
