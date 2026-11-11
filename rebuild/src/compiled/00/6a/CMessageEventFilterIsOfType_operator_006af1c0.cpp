// CMessageEventFilter_IsOfType::operator()(const CMessageEvent& event) const -- true when the event's type is the filter's type
// Leaf: ??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF1C0, 15 bytes: `mov eax,[esp+4] ; mov eax,[eax+8] ; sub eax,[ecx] ; neg eax ;
//   sbb eax,eax ; inc eax ; ret 4`.
// Name fix (naming agent NF21, 2026-11-11): the body compares CMessageEvent +0x08 (Ego_r.pdb CMessageEvent::Type, EMessageEventType)
//   with the filter's first dword (Ego_r.pdb CMessageEventFilter_IsOfType { EMessageEventType Type; }); HasExtraData would read
//   ExtraData +0x3C. Ego_r.pdb and FableWin.pdb spell ??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z, and the landed
//   FindMostRecentMessage / FindAllMessages rows call this address with an IsOfType filter. The catalog row was
//   "CMessageEventFilter_HasExtraData::operator()" over the generated stand-ins E / S::op.
// Layout: the shared engine/CMessageEvent.h (Type +0x08) and engine/CMessageEventFilters.h (Type +0x00, out-of-line operator()).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"

bool CMessageEventFilter_IsOfType::operator()(const CMessageEvent& event) const
{
    return event.Type == Type;
}
