#ifndef FABLE_ENGINE_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED_H
#define FABLE_ENGINE_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED_H
// New header (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable). Retail opt-in only:
// FABLE_RETAIL_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED (needs FABLE_RETAIL_CINTELLIGENTPOINTER).
// Class key V (pdb_class_keys.tsv). Members (Ego_r.pdb TPI, 24 bytes): PCreature CIntelligentPointer<CThingAICreature> +0x00 (vtable
// 0x0123E7E0 .?AV?$CIntelligentPointer@VCThingAICreature@@@@), LastVDeedID +0x08, LastSourceUID unsigned __int64 +0x10.
// Methods: FableWin.pdb ??0CThingFilter_IsOpinionSourceWithFreshDeed@@QAE@PAVCThingAICreature@@J_K@Z (inline in retail: the base
// pointer ctor 0x00A01B10, the vptr, SetPItem 0x00A01B90, then the two members, 0x009609F5..0x00960A1B), Ego_r.pdb / FableWin.pdb
// ??RCThingFilter_IsOpinionSourceWithFreshDeed@@QBE_NABVCThing@@@Z (out of line at 0x00960FC0, the IsRunnable TU) and the implicit
// destructor (FableWin ??1..QAE@XZ: only ~CBaseIntelligentPointer 0x00A01C10 on PCreature in retail, 0x00960ED9).
#ifdef FABLE_RETAIL_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED
#include "rebuild_abi.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingAICreature;
class CThingFilter_IsOpinionSourceWithFreshDeed {
public:
    CThingFilter_IsOpinionSourceWithFreshDeed(CThingAICreature* creature, long last_vdeed_id, unsigned __int64 last_source_uid)
        : PCreature(creature), LastVDeedID(last_vdeed_id), LastSourceUID(last_source_uid) { }
    bool operator()(const CThing& thing) const;                          // 0x00960FC0
private:
    CIntelligentPointer<CThingAICreature> PCreature;                     // +0x00
    long LastVDeedID;                                                    // +0x08
    unsigned __int64 LastSourceUID;                                      // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsOpinionSourceWithFreshDeed) == 0x18);
#endif
#endif // FABLE_ENGINE_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED_H
