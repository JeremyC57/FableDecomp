#ifndef FABLE_ENGINE_CMESSAGEEVENTFILTERS_H
#define FABLE_ENGINE_CMESSAGEEVENTFILTERS_H
// New (chunk 1016, row 0x00934E50): the AI message-event filters, Ego_r.pdb layouts:
//   CMessageEventFilter_IsOfType { EMessageEventType Type; } (4 bytes), ??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z out of
//   line at 0x006AF1C0 (`[event+8] == Type`);
//   CMessageEventFilter_ThingSearchFilter<T> { T* Filter; } (4 bytes; ctor ??0?$CMessageEventFilter_ThingSearchFilter@V..@@@@QAE@ABV..@@@Z),
//   ??R?$CMessageEventFilter_ThingSearchFilter@V..@@@@QBE_NABVCMessageEvent@@@Z inline in retail: the originator as an AI creature
//   (CMessageEvent::GetPOriginator<CThingAICreature>), then the thing filter on it (0x00934E82..0x00934EB1);
//   CThingFilter_In2DRange { C3DVector Pos; float Range; } (16 bytes), ??RCThingFilter_In2DRange@@QBE_NABVCThing@@@Z out of line at
//   0x006A76B0 (the physics position's XY distance squared against Range squared).
// Opt-in: FABLE_RETAIL_CMESSAGEEVENTFILTERS (needs FABLE_RETAIL_CMESSAGEEVENT and a complete CThingAICreature with
// FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS for the ThingSearchFilter body).
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS
#include "engine/CMessageEvent.h"
#include "engine/C3DVector.h"
class CThing; class CThingAICreature;
class CMessageEventFilter_IsOfType {
public:
    bool operator()(const CMessageEvent& event) const;                   // 0x006AF1C0
private:
    EMessageEventType Type;                                              // +0x00
};
template<class T> class CMessageEventFilter_ThingSearchFilter {
public:
    CMessageEventFilter_ThingSearchFilter(const T& filter) : Filter(&filter) { }
    bool operator()(const CMessageEvent& event) const {
        CThingAICreature* thing = event.GetPOriginator<CThingAICreature>();
        return thing && (*Filter)(*thing);
    }
private:
    const T* Filter;                                                     // +0x00
};
class CThingFilter_In2DRange {
public:
    bool operator()(const CThing& thing) const;                          // 0x006A76B0
private:
    C3DVector Pos;                                                       // +0x00
    float Range;                                                         // +0x0c
};
#endif
#endif // FABLE_ENGINE_CMESSAGEEVENTFILTERS_H
