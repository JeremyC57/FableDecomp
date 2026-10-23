// 00c3bcb0 AudioCriteriaLoader_00c3bcb0::LoadCriteria (owner uncertain)
// Flags: /O2 /Oy /EHsc
// Layout: reader pointer this+0; callback slot0 receives context,text,count,driver array; disasm00c3bcb0.
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
struct CriteriaText{char*data;CriteriaText(const char*);~CriteriaText();};
// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
struct CriteriaLog{char*data;};
struct CriteriaContext{CriteriaLog*CreateErrorText();bool Continue();};
CriteriaLog* __fastcall AppendErrorText(CriteriaLog*,const char*);
struct CriteriaReader{int FindSegment(const CriteriaText&);int Read(void*,int);};
struct CriteriaCallback{virtual void Entry(CriteriaContext*,const char*,unsigned,const unsigned*)=0;};
struct AudioCriteriaLoader_00c3bcb0{CriteriaReader*reader;void LoadCriteria(CriteriaContext*,CriteriaCallback*);};
void AudioCriteriaLoader_00c3bcb0::LoadCriteria(CriteriaContext*context,CriteriaCallback*callback){if(reader->FindSegment(CriteriaText("LHAudioBankCriteiaInfo"))<0){AppendErrorText(context->CreateErrorText(),"Could not find the criteria info segment");return;}unsigned count;if(reader->Read(&count,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the number of criteria entries!");return;}for(unsigned i=0;i<count;++i){unsigned length;if(reader->Read(&length,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the text length");return;}char*text=new char[length+1];text[length]=0;if(reader->Read(text,length)<(int)length){AppendErrorText(context->CreateErrorText(),"Could not read text");operator delete(text);return;}unsigned number;if(reader->Read(&number,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the number of variations");operator delete(text);return;}unsigned*drivers=0;if(number){drivers=new unsigned[number];if(reader->Read(drivers,number*4)<(int)(number*4)){AppendErrorText(context->CreateErrorText(),"Could not read the driver list");operator delete(text);delete[]drivers;return;}}callback->Entry(context,text,number,drivers);operator delete(text);if(drivers)delete[]drivers;if(!context->Continue())return;}}