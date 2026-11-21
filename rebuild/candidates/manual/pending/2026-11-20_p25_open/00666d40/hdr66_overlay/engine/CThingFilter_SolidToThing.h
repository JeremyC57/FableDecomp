#ifndef FABLE_ENGINE_CTHINGFILTER_SOLIDTOTHING_H
#define FABLE_ENGINE_CTHINGFILTER_SOLIDTOTHING_H
// New header (agentC24, row 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision). Ego_r TPI CThingFilter_SolidToThing, 8 bytes:
// SourceThing (CThing*, +0x00) and PTCSolidToThing (CTCSolidToThing*, +0x04) (struct_layouts_egor.tsv). Ego_r.pdb / FableWin.pdb
// ??0CThingFilter_SolidToThing@@QAE@ABVCThing@@@Z (retail 0x00661620 out of line: SourceThing = &thing, PTCSolidToThing = 0, `ret 4`)
// and ??RCThingFilter_SolidToThing@@QBE_NABVCThing@@@Z. Declarations only.
#include "stlport/fable_containers.h"
class CThing;
class CTCSolidToThing;
class CThingFilter_SolidToThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_SolidToThing(const CThing& thing);                  // 0x00661620
    bool operator()(const CThing& thing) const;
private:
    const CThing* SourceThing;                                        // +0x00
    CTCSolidToThing* PTCSolidToThing;                                 // +0x04
};
#ifdef FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_BODY
// Opt-in (hand4c 2026-11-19, row 0x0066A1E0): the operator() body, visible to the includer (FableWin.pdb ??RCThingFilter_SolidToThing@@QBE_NABVCThing@@@Z,
// debug 0x01DAEBB0, 29 bytes: `cmp ecx,[eax] ; jne` -> false on the source thing, true otherwise; Ego_d parameter test_thing). Retail
// expands it in the ForEachInRoughArea instance 0x0066A1E0 (0x0066A34C `cmp edi,[ecx] ; je`) and in the out-of-line Bind3 copy 0x00669040.
inline bool CThingFilter_SolidToThing::operator()(const CThing& test_thing) const
{
    if (&test_thing == SourceThing)
        return false;
    return true;
}
#endif
#ifdef FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_CTOR_BODY
// Opt-in (p25R 2026-11-21, row 0x00666D40 CThingCreatureBase::GetHeightZBelowAt): the constructor body, visible to the includer. Retail keeps an
// out-of-line copy (0x00661620) and expands it in GetHeightZBelowAt (0x00666E74..0x00666E7C: SourceThing = edi, PTCSolidToThing = 0). Text:
// Ego_d.pdb ??0CThingFilter_SolidToThing@@QAE@ABVCThing@@@Z line table (60: 15 B = prolog + SourceThing, 61: 10 B = PTCSolidToThing(NULL),
// 62: 9 B = `{}`) and its parameter source_thing.
inline CThingFilter_SolidToThing::CThingFilter_SolidToThing(const CThing& source_thing)
    : SourceThing(&source_thing)
    , PTCSolidToThing(0)
{
}
#endif
#endif
