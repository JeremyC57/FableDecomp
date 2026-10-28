// SContext_00c62500::ClearChannels() -- Remove each ambient record, release its live handle, and delete it.
// Leaf: ?ClearChannels@SContext_00c62500@@QAEXXZ
// Identity: own destructor caller00c47418, real PKCTRList virtual Head11/Remove8, item release00c62180 and destructor00c62140. Descriptive ambient emitter/source identities; helper spellings reused where available.
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
void SContext_00c62500::ClearChannels(){while(CHandleOwner_00c62180* item=Channels.GetFirst()){Channels.RemoveItem(item);item->ReleaseHandle(this);delete item;}}