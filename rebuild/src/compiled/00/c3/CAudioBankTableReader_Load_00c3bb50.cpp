// 00c3bb50 CAudioBankTableReader::Load(CLoadLog*, Callback*)
// Flags: /O2 /Oy /EHsc /GS
// Leaf: ?Load@CAudioBankTableReader@@QAEXPAVCLoadLog@@PAUCallback@@@Z
// Layout: retail receiver+0 reader, resource stack block28c with enabled104. String LHAudioBankSampleTable identifies table format; owning class uncertain.
// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 / IsOk 0x00C307C0, as the landed
//   LoadBank 0x00C17260 and 0x00C172F0) written through the CString stream operators 0x00C0DEC0 (const char*) and 0x00C0DF10 (unsigned
//   long, bound ??6CString@@QAIAAV0@K@Z by 0x00C4C080); the segment name is a CString temporary (0x00C0BFE0 / ~CString 0x00C0BA40).
//   Was String / Log::Text / Log::Number / Context stand-ins.
// NF59 (round 7, queue q10545): the owner is the descriptive CAudioBankTableReader (NF57 q10487: ctor 0x00C3BB40 stores the segment
//   file; ReadBankData 0x00C30F60 binds ?Load@CAudioBankTableReader@@QAEXPAVCLoadLog@@PAUCallback@@@Z); no libpk header declares it, so
//   the one-member declaration below, as 0x00C3BB40 / 0x00C30F60 declare it. The segment reader is libpk/CSegmentFile.h's CSegmentFile
//   (OpenSegment 0x00C2D9C0 / Read 0x00C2DA00 under FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23), the string engine/CString.h and the log
//   libpk/CScriptHost.h's CLoadLog (the include block of 0x00C30F60); were the address-named class and Reader / CString / CLoadLog stand-ins.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"
struct Callback{virtual void Resource(CLoadLog*,const void*)=0;};
struct CAudioBankTableReader{CSegmentFile* reader;void Load(CLoadLog*,Callback*);};
void CAudioBankTableReader::Load(CLoadLog* context,Callback* callback){
 if(reader->OpenSegment(CString("LHAudioBankSampleTable"))<0){context->AddError()<<"Could not find the bank sample table segment";return;}
 unsigned count;if(reader->Read(&count,4)<4){context->AddError()<<"Could not read the number of samples!";return;}
 count&=0xffff;char resource[0x28c];for(unsigned index=0;index<count;++index){
  if((unsigned)reader->Read(resource,sizeof(resource))<sizeof(resource)){context->AddError()<<"Could not read in resource "<<(unsigned long)index;return;}
  if(*(int*)(resource+0x104)){callback->Resource(context,resource);if(!context->IsOk())return;}
 }
}