// 00a0d300 CProcessedInput::Clear
// Flags:
// Leaf: ?Clear@CProcessedInput@@QAEXXZ
#define CProcessedInput Pass9ProcessedInputStorage
#include "engine/CProcessedInput.h"
#undef CProcessedInput
#define CGameEvent Pass9GameEventStorage
#include "engine/CGameEvent.h"
#undef CGameEvent
#ifndef _MSC_VER
#define __forceinline inline
#endif
enum EProcessedEventType{};
enum EGameEventPriority{};
class CGameEvent{public:long Type;signed char Player;unsigned char Data[32];unsigned char EndPos;bool Valid,Replacement;__forceinline void Uninitialise(){if(Valid)Valid=false;}};
// Layout: imported176-byte storage; original typed enum/array members restored.
class CProcessedInput{public:long Player;EProcessedEventType Type;CGameEvent GameEvents[4];unsigned char NoGameEvents;EGameEventPriority Priority;void Clear();};
FABLE_STATIC_ASSERT(sizeof(CGameEvent)==40);
FABLE_STATIC_ASSERT(sizeof(CProcessedInput)==176);
void CProcessedInput::Clear(){
 Type=static_cast<EProcessedEventType>(0);
 Player=-1;
 NoGameEvents=0;
 Priority=static_cast<EGameEventPriority>(2);
 CGameEvent*event=GameEvents;
 unsigned int count=4;
 do{event->Uninitialise();++event;}while(--count);
}