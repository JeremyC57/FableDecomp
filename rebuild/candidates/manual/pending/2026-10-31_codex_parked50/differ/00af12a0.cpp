// 00af12a0 NParticleEngine::CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate
// Codex parked-50 best attempt: DIFFER(152v144); residue: 152/144 bytes; correct original stream hierarchy emits an additional CDataInputStream vtable store before the true CDataStream destructor. Earlier 144-byte model has wrong owners/ABI.
// Leaf: ?BuildEmitterTemplate@CEmitterData@CParticleEmitterInfo@NParticleEngine@@QAEXXZ
// Flags:
#include "rebuild_abi.h"
#define CMemoryDataInputStream CMemoryDataInputStreamFields
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
#define CDataInputStream CDataInputStreamFields
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#define CParticleEmitter CParticleEmitterFields
#include "engine/CParticleEmitter.h"
#undef CParticleEmitter
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
#include "engine/CCountedPointer.h"
// Layout: only original destructor virtualslot0 is declared on these projections;
// unused virtual APIs and unread fields are not asserted as a complete class model.
class CDataStream:public CBaseClassNonCopyable{public:virtual ~CDataStream();};
class CDataInputStream:public CDataStream{unsigned char unreadStreamState[20];public:virtual ~CDataInputStream(){}};
class CMemoryDataInputStream:public CDataInputStream{const unsigned char*StreamSource;public:CMemoryDataInputStream(const void*,unsigned long);virtual ~CMemoryDataInputStream(){}};
// Analysis context: native993b10 resets its own vtable then tail-calls the
// shared noncopyable destructor. Its original body is visible but not inlined.
#pragma auto_inline(off)
CDataStream::~CDataStream(){}
#pragma auto_inline(on)
class CThreadedFileCallback:public CBaseClassNonCopyable{public:virtual ~CThreadedFileCallback();};class CMovableResource{public:virtual ~CMovableResource();};
class CBankFileAsyncData:public CThreadedFileCallback,public CMovableResource{void*Data;void*Buffer;unsigned long Length;unsigned char unreadTail[52];public:void*GetData();unsigned long GetLength()const{return Length;}
#ifdef PASS9_TEST
friend int ::main();
#endif
};
namespace NParticleEngine{
// Layout: actual CBaseObject8 and original unread state356 give native364 allocation.
class CParticleEmitter:public CBaseObject{unsigned char unreadState[356];public:CParticleEmitter();void ReadBinary(CDataInputStream&);};
 }
template<>void CCountedPointer<NParticleEngine::CParticleEmitter>::Reset(NParticleEngine::CParticleEmitter*);
namespace NParticleEngine {
class CParticleEmitterInfo{class CEmitterData:public CResource{public:CCountedPointer<CBankFileAsyncData>FileData;CCountedPointer<CParticleEmitter>EmitterTemplate;private:unsigned char unreadOwnerReference[4];public:void BuildEmitterTemplate();};
#ifdef PASS9_TEST
friend int ::main();
#endif
};
void CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate(){CMemoryDataInputStream stream(FileData->GetData(),FileData->GetLength());EmitterTemplate.Reset(new CParticleEmitter);CParticleEmitter*emitter=EmitterTemplate.Data;emitter->ReadBinary(stream);FileData.ReleaseData();}
}
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream)==sizeof(CMemoryDataInputStreamFields));FABLE_STATIC_ASSERT(sizeof(CDataInputStream)==sizeof(CDataInputStreamFields));FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitter)==sizeof(CParticleEmitterFields));
