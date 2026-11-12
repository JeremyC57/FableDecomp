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
class CMessageEventFilter_IsOfType
#if defined(FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT) || defined(FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD) || defined(FABLE_RETAIL_YELLFORHELP_UPDATE)
 : public std::unary_function<CMessageEvent,bool>
#endif
{
public:
#if defined(FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT) || defined(FABLE_RETAIL_YELLFORHELP_UPDATE)
    CMessageEventFilter_IsOfType(EMessageEventType type):Type(type){}
#endif
#if defined(FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR) && !defined(FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT) && !defined(FABLE_RETAIL_YELLFORHELP_UPDATE)
    // Opt-in (wf13, row 0x00492973 CSubtitleRenderer::SetText): the inline converting constructor alone (the filter built in a
    // frame slot, `mov dword ptr [ebp+14h],11h`), with the out-of-line operator() below.
    CMessageEventFilter_IsOfType(EMessageEventType type):Type(type){}
#endif
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
    bool operator()(const CMessageEvent& event) const{return Type==event.Type;}
#else
    bool operator()(const CMessageEvent& event) const;
#endif                   // 0x006AF1C0
private:
    EMessageEventType Type;                                              // +0x00
};
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
// Both original PDB complete types: unary_function base, private UID word pair.
class CMessageEventFilter_IsTargettedAtThing:public std::unary_function<CMessageEvent,bool> {
public:
#ifdef _MSC_VER
 __declspec(noinline)
#endif
 CMessageEventFilter_IsTargettedAtThing(const CThing& thing):UID(thing.GetUniqueID()){}
 bool operator()(const CMessageEvent& event)const{return UID==event.TargetUID;}
private:
#ifdef _MSC_VER
 unsigned __int64 UID;
#else
 unsigned long long UID;
#endif
};
class CMessageEventFilter_IsOriginatedByThing:public std::unary_function<CMessageEvent,bool> {
public:
#ifdef _MSC_VER
 __declspec(noinline)
#endif
 CMessageEventFilter_IsOriginatedByThing(const CThing& thing):UID(thing.GetUniqueID()){}
 bool operator()(const CMessageEvent& event)const{return UID==event.OriginatorUID;}
private:
#ifdef _MSC_VER
 unsigned __int64 UID;
#else
 unsigned long long UID;
#endif
};
#endif
#ifdef FABLE_RETAIL_YELLFORHELP_UPDATE
class CMessageEventFilter_IsOriginatorAlive:public std::unary_function<CMessageEvent,bool>{
public:
 bool operator()(const CMessageEvent & p0) const;
};
class CMessageEventFilter_CanBeSeenOrHeard:public std::unary_function<CMessageEvent,bool>{
public:
 CMessageEventFilter_CanBeSeenOrHeard(const CThingAICreature & p0):Creature(p0){}
 bool operator()(const CMessageEvent & p0) const;
private:
 const CThingAICreature & Creature;
};
#endif
template<class T> class CMessageEventFilter_ThingSearchFilter
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
 : public std::unary_function<CMessageEvent,bool>
#endif
{
public:
    CMessageEventFilter_ThingSearchFilter(const T& filter) : Filter(&filter) { }
    bool operator()(const CMessageEvent& event) const {
        CThingAICreature* thing = event.GetPOriginator<CThingAICreature>();
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
        // Original02b69e10 returns false on the null originator before applying the bound predicate.
        if(!thing)return false;
        return (*Filter)(*thing);
#else
        return thing && (*Filter)(*thing);
#endif
    }
private:
    const T* Filter;                                                     // +0x00
};
class CThingFilter_In2DRange
#ifdef FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
 : public std::unary_function<CThing,bool>
#endif
{
public:
    bool operator()(const CThing& thing) const;                          // 0x006A76B0
private:
    C3DVector Pos;                                                       // +0x00
    float Range;                                                         // +0x0c
};
#endif
#endif // FABLE_ENGINE_CMESSAGEEVENTFILTERS_H
