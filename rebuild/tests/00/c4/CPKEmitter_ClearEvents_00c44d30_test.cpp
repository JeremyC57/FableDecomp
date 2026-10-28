#include <stdio.h>
// CPKEmitter::ClearEvents() -- Remove each ambient record, release its live handle, and delete it.
// Leaf: ?ClearEvents@CPKEmitter@@QAEXXZ
// Identity: own destructor caller00c44faf, real PKCTRList virtual Head11/Remove8, item release00c61fb0 and destructor00c61c90. Descriptive ambient emitter/source identities; helper spellings reused where available.
#include <new>
class CPKEmitter;struct SContext_00c62500;
class CPKVoiceHandle{public:unsigned long SoundId,Time;bool Unscaled;long Handle;void Release(CPKEmitter*);~CPKVoiceHandle();};
struct CHandleOwner_00c62500{unsigned long SoundId,Time;long Handle;float Offset[3];void RemoveHandle(SContext_00c62500*);~CHandleOwner_00c62500();};
struct CHandleOwner_00c62180{unsigned long SoundId;long Handle;void ReleaseHandle(SContext_00c62500*);~CHandleOwner_00c62180();};
class CPKList{public:virtual ~CPKList();virtual void Unused1(){}virtual void Unused2(){}virtual void Unused3(){}virtual void Unused4(){}virtual void Unused5(){}virtual void Unused6(){}virtual void Unused7(){}virtual void Remove(void*);virtual void Add(void*){}virtual void Unused10(){}virtual void* GetHead();void* Impl;};
template<class T>class PKCTRList:public CPKList{public:unsigned char UnmodeledImpl[40];T* GetFirst(){return (T*)GetHead();}void RemoveItem(T* p){Remove(p);}};
// Layout: genuine libpk PKCTRList48 (CPKList vptr/implementation8 plus40-byte member implementation). Emitter list32; source triggers8 and channels56. No shared headers. Existing helper owner/record spellings retained for stable symbols, roles identified by own constructors/callers. Empty record destructors16/24/8 remain real out-of-line calls.
class CPKEmitter{public:unsigned char Unmodeled00[32];PKCTRList<CPKVoiceHandle> Events;void ClearEvents();};
struct SContext_00c62500{unsigned char Unmodeled00[8];PKCTRList<CHandleOwner_00c62500> Triggers;PKCTRList<CHandleOwner_00c62180> Channels;void ClearTriggers();void ClearChannels();};
void CPKEmitter::ClearEvents(){while(CPKVoiceHandle* item=Events.GetFirst()){Events.RemoveItem(item);item->Release(this);delete item;}}

struct State{void* items[3];int count,index;};static State* active;static int removed,released,destroyed,errors;static void* expectedOwner;
CPKList::~CPKList(){}void* CPKList::GetHead(){State* state=(State*)Impl;return state->index<state->count?state->items[state->index]:0;}void CPKList::Remove(void* p){State* state=(State*)Impl;if(state!=active||p!=state->items[state->index])++errors;++state->index;++removed;}
CPKVoiceHandle::~CPKVoiceHandle(){if(released!=destroyed+1)++errors;++destroyed;}void CPKVoiceHandle::Release(CPKEmitter* owner){if(owner!=expectedOwner||removed!=released+1||this!=active->items[released])++errors;++released;}
int main(){CPKEmitter owner;expectedOwner=&owner;State state;state.count=0;state.index=0;active=&state;owner.Events.Impl=&state;owner.ClearEvents();bool ok=!removed&&!released&&!destroyed;state.count=2;state.items[0]=new CPKVoiceHandle;state.items[1]=new CPKVoiceHandle;owner.ClearEvents();ok=ok&&removed==2&&released==2&&destroyed==2&&state.index==2&&!errors;owner.ClearEvents();ok=ok&&destroyed==2;puts(ok?"00c44d30_TEST PASS":"00c44d30_TEST FAIL");return ok?0:1;}