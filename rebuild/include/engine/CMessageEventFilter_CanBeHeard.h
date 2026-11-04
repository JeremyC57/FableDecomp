#ifndef FABLE_ENGINE_CMESSAGEEVENTFILTER_CANBEHEARD_H
#define FABLE_ENGINE_CMESSAGEEVENTFILTER_CANBEHEARD_H
// CMessageEventFilter_CanBeHeard (new, chunk 765): Ego_r.pdb struct_layouts_egor.tsv `@STRUCT CMessageEventFilter_CanBeHeard 4 1`, the
// single member CThingAICreature* Creature +0x00 (as its siblings CanBeSeen / CanBeSeenOrHeard). Ego_r.pdb
// ??RCMessageEventFilter_CanBeHeard@@QBE_NABVCMessageEvent@@@Z (0x006C9960; donor run CanBeSeenOrHeard 0x006C9910, CanBeSeen 0x006C9950,
// CanBeHeard 0x006C9960, MessageIDGreaterThan 0x006C9970 = retail 0x006AF220, 0x006AF260, 0x006AF270, 0x006AF280). The operator is out
// of line in retail (FindMessage 0x0090ED25 and FindMostRecentMessage<CFilter_Bind3<..>> 0x0090EDF3 call it).
class CThingAICreature;
class CMessageEvent;
class CMessageEventFilter_CanBeHeard {
public:
    bool operator()(const CMessageEvent& event) const;                   // 0x006AF270
private:
    CThingAICreature* Creature;                                          // +0x00
};
#endif
