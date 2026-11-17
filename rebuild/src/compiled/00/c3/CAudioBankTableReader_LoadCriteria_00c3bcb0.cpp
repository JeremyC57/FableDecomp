// 00c3bcb0 CAudioBankTableReader::LoadCriteria(CLoadLog*, CriteriaCallback*)
// Leaf: ?LoadCriteria@CAudioBankTableReader@@QAEXPAVCLoadLog@@PAUCriteriaCallback@@@Z
// Flags: /O2 /Oy /EHsc
// Layout: reader pointer this+0; callback slot0 receives context,text,count,driver array; disasm00c3bcb0.
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 returns the entry's CString,
//   IsOk 0x00C307C0) and the text goes through CString::operator<<(const char*) 0x00C0DEC0 (__fastcall); the segment name is a CString
//   temporary (0x00C0BFE0 / ~CString 0x00C0BA40). Was CriteriaText / CriteriaLog / CriteriaContext / AppendErrorText stand-ins.
// NF59 (round 7, queue q10546): the owner is the descriptive CAudioBankTableReader (NF57 q10487: ctor 0x00C3BB40, Load 0x00C3BB50;
//   ReadBankData 0x00C30F60 binds ?LoadCriteria@CAudioBankTableReader@@QAEXPAVCLoadLog@@PAUCriteriaCallback@@@Z); no libpk header declares
//   it, so the one-member declaration below, as 0x00C3BB40 / 0x00C30F60 declare it. The segment reader is libpk/CSegmentFile.h's
//   CSegmentFile (OpenSegment 0x00C2D9C0 / Read 0x00C2DA00 under FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23), the string engine/CString.h and
//   the log libpk/CScriptHost.h's CLoadLog (the include block of 0x00C30F60); were the address-named class and CriteriaReader / CString /
//   CLoadLog stand-ins.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"
struct CriteriaCallback{virtual void Entry(CLoadLog*,const char*,unsigned,const unsigned*)=0;};
struct CAudioBankTableReader{CSegmentFile*reader;void LoadCriteria(CLoadLog*,CriteriaCallback*);};
void CAudioBankTableReader::LoadCriteria(CLoadLog*context,CriteriaCallback*callback){if(reader->OpenSegment(CString("LHAudioBankCriteiaInfo"))<0){context->AddError()<<"Could not find the criteria info segment";return;}unsigned count;if(reader->Read(&count,4)<4){context->AddError()<<"Could not read the number of criteria entries!";return;}for(unsigned i=0;i<count;++i){unsigned length;if(reader->Read(&length,4)<4){context->AddError()<<"Could not read the text length";return;}char*text=new char[length+1];text[length]=0;if(reader->Read(text,length)<(int)length){context->AddError()<<"Could not read text";operator delete(text);return;}unsigned number;if(reader->Read(&number,4)<4){context->AddError()<<"Could not read the number of variations";operator delete(text);return;}unsigned*drivers=0;if(number){drivers=new unsigned[number];if(reader->Read(drivers,number*4)<(int)(number*4)){context->AddError()<<"Could not read the driver list";operator delete(text);delete[]drivers;return;}}callback->Entry(context,text,number,drivers);operator delete(text);if(drivers)delete[]drivers;if(!context->IsOk())return;}}