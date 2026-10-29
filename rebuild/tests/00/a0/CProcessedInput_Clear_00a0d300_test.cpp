#include <stdio.h>
#include <string.h>
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

struct FixtureGuard{unsigned long before;CProcessedInput input;unsigned long after;};
int main(){
 unsigned int cases=0;
 for(unsigned int seed=0;seed<256;++seed)for(unsigned int mask=0;mask<16;++mask){
  FixtureGuard guard;guard.before=0x12345678;guard.after=0x87654321;
  memset(&guard.input,seed,sizeof(guard.input));
  for(int i=0;i<4;++i){guard.input.GameEvents[i].Valid=((mask>>i)&1)!=0;guard.input.GameEvents[i].Replacement=((seed>>i)&1)!=0;}
  CProcessedInput expected;memcpy(&expected,&guard.input,sizeof(expected));
  expected.Player=-1;expected.Type=static_cast<EProcessedEventType>(0);expected.Priority=static_cast<EGameEventPriority>(2);expected.NoGameEvents=0;
  for(int i=0;i<4;++i)expected.GameEvents[i].Valid=false;
  guard.input.Clear();
  if(memcmp(&expected,&guard.input,sizeof(expected))||guard.before!=0x12345678||guard.after!=0x87654321)return 1;
  guard.input.Clear();
  if(memcmp(&expected,&guard.input,sizeof(expected))||guard.before!=0x12345678||guard.after!=0x87654321)return 2;
  cases+=2;
 }
 if(cases!=8192)return 3;puts("00a0d300_TEST PASS");return 0;
}