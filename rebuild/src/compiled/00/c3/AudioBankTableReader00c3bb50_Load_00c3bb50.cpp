// 00c3bb50 AudioBankTableReader_00c3bb50::Load
// Flags: /O2 /Oy /EHsc /GS
// Leaf: Load
// Layout: retail receiver+0 reader, resource stack block28c with enabled104. String LHAudioBankSampleTable identifies table format; owning class uncertain.
// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 / IsOk 0x00C307C0, as the landed
//   LoadBank 0x00C17260 and 0x00C172F0) written through the CString stream operators 0x00C0DEC0 (const char*) and 0x00C0DF10 (unsigned
//   long, bound ??6CString@@QAIAAV0@K@Z by 0x00C4C080); the segment name is a CString temporary (0x00C0BFE0 / ~CString 0x00C0BA40).
//   Was String / Log::Text / Log::Number / Context stand-ins.
class CString{public:char* m_pchData;CString(const char*);~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(unsigned long);};
struct Reader{int Find(const CString&);int Read(void*,unsigned);};
class CLoadLog{public:CString& AddError();bool IsOk() const;};
struct Callback{virtual void Resource(CLoadLog*,const void*)=0;};
struct AudioBankTableReader_00c3bb50{Reader* reader;void Load(CLoadLog*,Callback*);};
void AudioBankTableReader_00c3bb50::Load(CLoadLog* context,Callback* callback){
 if(reader->Find(CString("LHAudioBankSampleTable"))<0){context->AddError()<<"Could not find the bank sample table segment";return;}
 unsigned count;if(reader->Read(&count,4)<4){context->AddError()<<"Could not read the number of samples!";return;}
 count&=0xffff;char resource[0x28c];for(unsigned index=0;index<count;++index){
  if((unsigned)reader->Read(resource,sizeof(resource))<sizeof(resource)){context->AddError()<<"Could not read in resource "<<(unsigned long)index;return;}
  if(*(int*)(resource+0x104)){callback->Resource(context,resource);if(!context->IsOk())return;}
 }
}