// std::list<CMaintainedEvent>::push_back() -- STLport 4.6 _list.h `void push_back() { insert(end()); }`: append a default-constructed event through insert(iterator)
// Leaf: ?push_back@?$list@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4AA0, 20 bytes: push ecx ; the end() iterator (the head word at +0) copied into
//   its argument slot ; the hidden result slot's address pushed ; call 0x009E4A20 ; pop ecx ; ret.
// Identity (agentNF19, from landing ci): the landed CMouse::ProcessMaintainedEventsFromEvent 0x00A66FD0 (RELOCATION_MATCH) calls
//   0x009E4AA0 as ?push_back@?$list@VCMaintainedEvent@@...@@QAEXXZ and 0x009E4A20 as list<CMaintainedEvent>::insert(iterator); the
//   body is that STLport inline. Was the placeholder CKeyedOwner_009e4aa0::Register.
// Callee: std::list<CMaintainedEvent>::insert(iterator) 0x009E4A20 (out of line; declared by FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
#define FABLE_RETAIL_CINPUTEVENT_COPY
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CInputEvent.h"
#include "engine/CMaintainedEvent.h"

template void std::list<CMaintainedEvent, std::allocator<CMaintainedEvent> >::push_back();
