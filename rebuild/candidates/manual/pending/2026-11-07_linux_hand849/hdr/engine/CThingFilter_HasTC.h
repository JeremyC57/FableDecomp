#ifndef FABLE_ENGINE_CTHINGFILTER_HASTC_H
#define FABLE_ENGINE_CTHINGFILTER_HASTC_H
#include "engine/CThing.h"
// Original TPI23ff9: unary_function<CThing,bool> base, private Type enum.
class CThingFilter_HasTC : public std::unary_function<CThing,bool> {
private: ETCInterfaceType Type;
public:
 CThingFilter_HasTC(ETCInterfaceType type):Type(type){}
#ifdef FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
 // Per-TU opt-in (chunk 849, row 0x00501450; Codex pass 13 FABLE_RETAIL_CTHINGFILTER_HASTC_EXTERNAL): the call operator declared only,
 // as retail keeps it out of line (??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z, 0x006A80A0, landed). With the inline body visible in
 // the same-TU GetAllThings body, VC7.1 sees the filter temporary is only read and hoists its `mov [esp+..],64h` above the region
 // loop; retail stores it inside the loop, after LoadRegion (0x005014FD).
 bool operator()(const CThing& thing)const;
#else
 bool operator()(const CThing& thing)const{return thing.HasTC(Type);}
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_HasTC)==4);
#endif
