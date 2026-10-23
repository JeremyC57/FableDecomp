// 00c3bb50 AudioBankTableReader_00c3bb50::Load
// Flags: /O2 /Oy /EHsc /GS
// Leaf: Load
// Layout: retail receiver+0 reader, resource stack block28c with enabled104. String LHAudioBankSampleTable identifies table format; owning class uncertain.
struct String{char* data;String(const char*);~String();};
struct Reader{int Find(const String&);int Read(void*,unsigned);};
struct Log{Log& __fastcall Text(const char*);Log& __fastcall Number(unsigned);};
struct Context{Log& CreateErrorText_00c30840();bool Good();};
struct Callback{virtual void Resource(Context*,const void*)=0;};
struct AudioBankTableReader_00c3bb50{Reader* reader;void Load(Context*,Callback*);};
void AudioBankTableReader_00c3bb50::Load(Context* context,Callback* callback){
 if(reader->Find(String("LHAudioBankSampleTable"))<0){context->CreateErrorText_00c30840().Text("Could not find the bank sample table segment");return;}
 unsigned count;if(reader->Read(&count,4)<4){context->CreateErrorText_00c30840().Text("Could not read the number of samples!");return;}
 count&=0xffff;char resource[0x28c];for(unsigned index=0;index<count;++index){
  if((unsigned)reader->Read(resource,sizeof(resource))<sizeof(resource)){context->CreateErrorText_00c30840().Text("Could not read in resource ").Number(index);return;}
  if(*(int*)(resource+0x104)){callback->Resource(context,resource);if(!context->Good())return;}
 }
}