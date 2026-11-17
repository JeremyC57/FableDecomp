#ifndef FABLE_LIBPK_CSCRIPTHOST_H
#define FABLE_LIBPK_CSCRIPTHOST_H
#ifdef FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#include "libpk/CSegmentFile.h"
#include "rebuild_abi.h"
// Descriptive names: no original libpk PDB/RTTI. Native C16F00 reads
// serialized32-bit lookup records, source+8 and bank flags+48/priority+4c.
struct CLoadMessage {
 bool Error;
 CString Text;
 unsigned long Sequence;
};
class CLoadLog:public PKCTRList<CLoadMessage> {
public:
 CLoadLog();
 virtual ~CLoadLog();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<CLoadMessage>*);
 virtual CLoadMessage* FindByKey(const unsigned long&);
 CString& AddError();
#ifdef FABLE_RETAIL_CLOADLOG_MESSAGE_PASS23
 CString& AddMessage(); // NativeC307E0 non-error message; same actual record/list family.
#endif
 bool IsOk()const;
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 void SetSourceName(const char*); // Native direct C308A0 binding.
#endif
 PKCTRRedblackTree Tree;
 bool Valid;
};
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
class CNamedSet;
struct SBankRequest {
 CNamedSet* CriteriaTriggers;void* NumberTriggers;unsigned long Chunk;
 bool SampleNameTriggers,NamedTriggers,Option76,Option74,Option75,EnableBothExtra;
};
#else
struct SBankRequest { fable_u32 Unknown0,Unknown4,Chunk; };
#endif
struct SLookupEntry { fable_i32 Key,Length,Offset; };
// Existing descriptive CScriptHost spelling is retained consistently. Native
// type012b25a8 is an audio host/bank with seven slots; owned component interiors
// outside this function's used fields remain opaque. No invented inheritance.
class CScriptHost {
public:
 CScriptHost(void* owner);
 virtual void Post();
 virtual long GetEventCount();
 virtual void* GetEventByIndex(unsigned long);
 virtual void ClearEvents();
 virtual long GetQueuedEvent(long,long*,long*);
 virtual bool TryActivate(void*);
 virtual ~CScriptHost();
 void* Payload;
 void* Owner;
 unsigned char Unrecovered0c[0x3c];
 long Deferred,HighestPriority;
 unsigned char Unrecovered50[0x54];
 void QueueEvent(long key,long offset,long length);
#ifdef FABLE_RETAIL_CSCRIPTHOST_CREATESAMPLE
 // NF65 overlay opt-in (row 0x00C2E190 CBankProcessor::AddSample): the landed self row's spelling
 // ?CreateSample@CScriptHost@@QAEPAUCSample@@PBUCAudioFormat@@K@Z (0x00C0E700: `new` of a 0x24-byte sample from the format and the data
 // offset; descriptive, libpk has no PDB). The includer defines CAudioFormat. Declaration only.
 struct CSample* CreateSample(const struct CAudioFormat* format, unsigned long dataOffset);   // 0x00C0E700
#endif
};
long __fastcall ReadBankInfoCount(CSegmentFile* file);
// ECX is not consumed by native C16F00. The conventional source has an
// unused first fastcall argument; this does not assert an original source API.
#ifndef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
void __fastcall ReadDeferredBank(void*,void*,CLoadLog*,CScriptHost*,const SBankRequest*);
#endif
FABLE_STATIC_ASSERT_AT(55, sizeof(CLoadMessage)==12);
FABLE_STATIC_ASSERT_AT(56, sizeof(CLoadLog)==52);
FABLE_STATIC_ASSERT_AT(57, offsetof(CLoadLog,Valid)==0x30);
FABLE_STATIC_ASSERT_AT(58, sizeof(CScriptHost)==0xa4);
FABLE_STATIC_ASSERT_AT(59, sizeof(SLookupEntry)==12);
FABLE_STATIC_ASSERT_AT(60, offsetof(SBankRequest,Chunk)==8);
FABLE_STATIC_ASSERT_AT(61, offsetof(CScriptHost,Deferred)==0x48);
FABLE_STATIC_ASSERT_AT(62, offsetof(CScriptHost,HighestPriority)==0x4c);
#endif
#endif
