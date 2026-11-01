// 00c3bcb0 AudioCriteriaLoader_00c3bcb0::LoadCriteria (owner uncertain)
// Flags: /O2 /Oy /EHsc
// Layout: reader pointer this+0; callback slot0 receives context,text,count,driver array; disasm00c3bcb0.
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 returns the entry's CString,
//   IsOk 0x00C307C0) and the text goes through CString::operator<<(const char*) 0x00C0DEC0 (__fastcall); the segment name is a CString
//   temporary (0x00C0BFE0 / ~CString 0x00C0BA40). Was CriteriaText / CriteriaLog / CriteriaContext / AppendErrorText stand-ins.
class CString{public:char*m_pchData;CString(const char*);~CString();CString& __fastcall operator<<(const char*);};
class CLoadLog{public:CString& AddError();bool IsOk() const;};
struct CriteriaReader{int FindSegment(const CString&);int Read(void*,int);};
struct CriteriaCallback{virtual void Entry(CLoadLog*,const char*,unsigned,const unsigned*)=0;};
struct AudioCriteriaLoader_00c3bcb0{CriteriaReader*reader;void LoadCriteria(CLoadLog*,CriteriaCallback*);};
void AudioCriteriaLoader_00c3bcb0::LoadCriteria(CLoadLog*context,CriteriaCallback*callback){if(reader->FindSegment(CString("LHAudioBankCriteiaInfo"))<0){context->AddError()<<"Could not find the criteria info segment";return;}unsigned count;if(reader->Read(&count,4)<4){context->AddError()<<"Could not read the number of criteria entries!";return;}for(unsigned i=0;i<count;++i){unsigned length;if(reader->Read(&length,4)<4){context->AddError()<<"Could not read the text length";return;}char*text=new char[length+1];text[length]=0;if(reader->Read(text,length)<(int)length){context->AddError()<<"Could not read text";operator delete(text);return;}unsigned number;if(reader->Read(&number,4)<4){context->AddError()<<"Could not read the number of variations";operator delete(text);return;}unsigned*drivers=0;if(number){drivers=new unsigned[number];if(reader->Read(drivers,number*4)<(int)(number*4)){context->AddError()<<"Could not read the driver list";operator delete(text);delete[]drivers;return;}}callback->Entry(context,text,number,drivers);operator delete(text);if(drivers)delete[]drivers;if(!context->IsOk())return;}}