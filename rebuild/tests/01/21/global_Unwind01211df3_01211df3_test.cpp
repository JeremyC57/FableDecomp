// Self-contained shared-object bank-loader behavior and unwind test.
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
#define TEST_UNWIND
#endif
// 00c16f00 ReadDeferredBank
// Leaf: ?ReadDeferredBank@@YGXPAVCLoadLog@@PAXPAVCScriptHost@@PBUSBankRequest@@@Z
// Flags: /EHsc
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#include "libpk/PKCAutoFile.h"
// Complete genuine four-logical-argument deferred loader. Under its native
// EHsc policy this selected857Bbody is exact; genuine callers below supply
// optimizer visibility, with their own code-generation residues unclaimed.
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"
#if defined(_MSC_VER)
// RTM selects scalar allocation for POD arrays unless array operators are declared.
// Native PE imports prove array new[]/delete[]; GNU uses its standard declarations.
void* __cdecl operator new[](size_t size);
void __cdecl operator delete[](void* pointer) throw();
#endif
static long HasBankSegments(CSegmentFile* file);
static void __stdcall ReadDeferredBank(CLoadLog* log, void* file, CScriptHost* host, const SBankRequest* request)
{
    CSegmentFile reader(file, request->Chunk);
    if (reader.ErrorCount) {
        log->AddError() << "There were problems decoding the file segment structure";
        return;
    }
    long version = ReadBankInfoCount(&reader);
    if (version < 0) {
        log->AddError() << "Could not extract the version number";
        return;
    }
    if (!HasBankSegments(&reader)) {
        log->AddError() << "This bank is not a normal LUG!";
        return;
    }
    if (version != 4) {
        log->AddError() << "Expecting deferred bank version number " << 4L << "... got " << version;
        return;
    }
    if (reader.OpenSegment(CString("LHAudioBankCompData")) < 0) {
        log->AddError() << "Could not open the composite data segment";
        return;
    }
    fable_u32 compData = reader.GetPosition();
    if (reader.OpenSegment(CString("LHAudioBankLookupTable")) < 0) {
        log->AddError() << "Could not open the lookup table";
        return;
    }
    fable_i32 highestPriority;
    if (reader.Read(&highestPriority, 4) < 4) {
        log->AddError() << "Could not read the highest priority";
        return;
    }
    fable_u32 count;
    if (reader.Read(&count, 4) < 4) {
        log->AddError() << "Could not read the number of samples";
        return;
    }
    if (count == 0)
        return;
    SLookupEntry* table = new SLookupEntry[count];
    if (reader.Read(table, count * sizeof(SLookupEntry)) < (int)(count * sizeof(SLookupEntry))) {
        delete[] table;
        log->AddError() << "Could not read in the lookup table";
        return;
    }
    for (unsigned long i = 0; i < count; ++i)
        host->QueueEvent(table[i].Key, table[i].Offset + compData, table[i].Length);
    host->HighestPriority = highestPriority;
    host->Deferred = 1;
    delete[] table;
}

// Genuine complete file-local helper body recovered from both native203B and
// historicalhand433/766. Ordinary static visibility is tried before any policy.
// Native private convention consumesESI; audit must verify actual emitted ABI.
static long HasBankSegments(CSegmentFile* file)
{
    bool missing = file->OpenSegment(CString("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString("LHAudioBankCompData")) < 0;
    return !missing;
}

void __fastcall WideToMultiByte(const unsigned short*,char*,int);
void __fastcall ReadBankData(void*,CBankData*,unsigned long,CLoadLog*);
CBankData* __fastcall ReadMetadataData(void*);
static void __stdcall RunBankProcessor(CScriptHost* bank,CBankData* data,CLoadLog* log,const SBankRequest* settings)
{
 CBankProcessor processor(data,bank,log);
 processor.m_pCriteriaTriggers=settings->CriteriaTriggers;
 processor.m_pNumberTriggers=settings->NumberTriggers;
 processor.m_bSampleNameTriggers=settings->SampleNameTriggers;
 processor.m_bNamedTriggers=settings->NamedTriggers;
 processor.m_option76=settings->Option76;
 processor.m_option74=settings->Option74;
 processor.m_option75=settings->Option75;
 if(settings->EnableBothExtra){processor.m_option77=true;processor.m_option78=true;}
 processor.Load();
}
static void __stdcall ProcessBankEntry(const SBankRequest* settings,void* file,CLoadLog* log,CScriptHost* bank)
{
 CBankData data;
 ReadBankData(file,&data,settings->Chunk,log);
 if(log->IsOk())RunBankProcessor(bank,&data,log,settings);
}
// Genuine complete nativeC17400metadata path. No made-up ABI forwarding body.
CBankDataOwner::~CBankDataOwner(){if(Pointer){delete Pointer;Pointer=0;}}
static void __stdcall ReadMetadataBank(CLoadLog* log,void* file,CScriptHost* bank,const SBankRequest* settings)
{
 CBankDataOwner data(ReadMetadataData(file));
 if(!data.Pointer){log->AddError()<<"Could not load from file";return;}
 RunBankProcessor(bank,data.GetChecked(),log,settings);
}
static void __stdcall LoadBankByFormat(PKCFileManager* owner,CLoadLog* log,CScriptHost* bank,const unsigned short* path,const SBankRequest* settings)
{
 PKCAutoFile file(owner->Open(path));
 if(!file.m_pFile){log->AddError()<<"Could not open "<<path;return;}
 char name[256];WideToMultiByte(path,name,256);
 CString filename(name);CString extension=filename.Right(4);
 if(extension.CompareNoCase(".lug")==0)ProcessBankEntry(settings,file.GetChecked(),log,bank);
 else if(extension.CompareNoCase(".lut")==0)ReadDeferredBank(log,file.GetChecked(),bank,settings);
 else if(extension.CompareNoCase(".met")==0)ReadMetadataBank(log,file.GetChecked(),bank,settings);
 else log->AddError()<<"This bank is not a recognised format (.lug, .lut, .met)";
}
bool __fastcall OpenBankFile(PKCFileManager* owner,const unsigned short* path,CScriptHost* bank,const SBankRequest* settings)
{
 CLoadLog log;
 LoadBankByFormat(owner,&log,bank,path,settings);
 char name[256];WideToMultiByte(path,name,256);
 log.SetSourceName(name);
 if(!log.IsOk())return false;
 return true;
}


static bool WActive,WInsideRight,WLogAlive,WFileOpen,WCanOpen,WParseFail,WProcessFail,WMetadataNull,WAllocating;
static unsigned WDataAlive,WProcessorAlive,WFileDtors,WConversions,WLoads,WNormal,WThrows;
static const unsigned short* WPath;static char WName[256];
static CAudioFile* WFileAddress;static const SBankRequest* WSettings;
static void* WHeap;
static const CString* WStrings[16];static int WRoles[16];static unsigned WStringCount;
static void WMakeString(CString*,const char*);static bool WDestroyString(const CString*);
static char Empty[1]={0};char* _afxPchNil=Empty;
PFNASSERTHOOK g_pfnAssertHook=0;CDebugBreak::CDebugBreak(){assert(false);}
static CString ErrorText;
static char ErrorBuffer[256];
static unsigned char SourceToken;
static CLoadLog* Log;static CScriptHost* Host;
static unsigned Mode,Count,OpenCalls,ReadCalls,Queued,ReaderDtors,LiveTemps;
static unsigned FaultAt,FaultCalls,ExpectedFaults,NormalCases,ExceptionCases;
static long Version,Priority;
static fable_u32 Composite;
static unsigned long ExpectedChunk;
static int Trace[256],TraceSize;
static bool Active;
static void* Array;
static unsigned Allocations,Deletions;
static SLookupEntry Input[5];
static const CString* TempObjects[4];static int TempIds[4];
static void Record(int n){assert(TraceSize<256);Trace[TraceSize++]=n;}
static void Boundary(int n){Record(n);++FaultCalls;
#ifdef TEST_UNWIND
 if(FaultAt==FaultCalls)throw std::bad_alloc();
#else
 assert(!FaultAt);
#endif
}
void* __cdecl operator new[](size_t n) throw(std::bad_alloc){
 assert(Active&&Count&&n==Count*12);Boundary(14);assert(!Array);
 Array=malloc(n);assert(Array);++Allocations;return Array;
}
void __cdecl operator delete[](void* p) throw(){
 assert(Active&&p==Array&&Array);Record(17);++Deletions;free(p);Array=0;
}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}
PKCCriticalSection::~PKCCriticalSection(){}
void PKCCriticalSection::Enter(){}void PKCCriticalSection::Leave(){}
PKCSemaphore::PKCSemaphore(long,long):m_hSemaphore(0){}
PKCSemaphore::~PKCSemaphore(){}void PKCSemaphore::Wait(){}void PKCSemaphore::Release(){}
PKCMailbox::PKCMailbox(void*q,long n):m_pQueue(q),m_semaphore(0,n){}
PKCBufferedReader::PKCBufferedReader(void* source,const PKCBufferedReaderParams&p)
 :m_mailbox(0,(long)(p.ExtraBuffers+p.BaseBuffers)),m_outstanding(0),m_completedReadOffset(0){
 assert(source==(WActive?(void*)WFileAddress:(void*)&SourceToken)&&p.Granularity==ExpectedChunk&&p.ExtraBuffers==0&&p.BaseBuffers==2);
 memset(_unknown00,0,sizeof(_unknown00));
}
CBufferedRangeReader::CBufferedRangeReader(void* source,unsigned long chunk)
 :Reader(source,PKCBufferedReaderParams(chunk,0,2)),RangeStart(0),RangeLength(0),ReadOffset(0){}
CBufferedRangeReader::~CBufferedRangeReader(){}
long CBufferedRangeReader::SetRange(unsigned long,unsigned long){assert(false);return 0;}
long CBufferedRangeReader::Read(void*,long){assert(false);return 0;}
unsigned long CBufferedRangeReader::GetCount()const{assert(false);return 0;}
CSegmentFile::CSegmentFile(void* source,unsigned long chunk):Reader(source,chunk),ErrorCount(Mode==1?1:0){Boundary(1);}
CSegmentFile::~CSegmentFile(){Record(40);++ReaderDtors;assert(!LiveTemps);}
long __fastcall ReadBankInfoCount(CSegmentFile*){Boundary(2);return Version;}
CString::CString(const char* text){
 if(WActive&&strncmp(text,"LHAudioBank",11)){WMakeString(this,text);return;}
 int id=0;
 if(!strcmp(text,"LHAudioBankLookupTable"))id=OpenCalls==0?3:10;
 else{assert(!strcmp(text,"LHAudioBankCompData"));id=OpenCalls==1?5:7;}
 Boundary(id);assert(LiveTemps<4);TempObjects[LiveTemps]=this;TempIds[LiveTemps]=id;++LiveTemps;m_pchData=(char*)text;
}
CString::~CString(){
 if(WActive&&WDestroyString(this))return;
 if(!Active||m_pchData==Empty||m_pchData==ErrorBuffer)return;
 assert(LiveTemps&&TempObjects[LiveTemps-1]==this);Record(50+TempIds[LiveTemps-1]);--LiveTemps;
}
long CSegmentFile::OpenSegment(const CString& name){
 const int ids[]={4,6,8,11};assert(OpenCalls<4&&LiveTemps);
 unsigned call=OpenCalls++;Boundary(ids[call]);
 assert(!strcmp(name.m_pchData,(call==0||call==3)?"LHAudioBankLookupTable":"LHAudioBankCompData"));
 return (Mode==2&&call==0)||(Mode==3&&call==1)||(Mode==4&&call==2)||(Mode==5&&call==3)?-1:0;
}
unsigned long CSegmentFile::GetPosition(){Boundary(9);return Composite;}
int CSegmentFile::Read(void* output,int bytes){
 unsigned call=ReadCalls++;
 if(call==0){Boundary(12);assert(bytes==4);fable_i32 p=(fable_i32)Priority;memcpy(output,&p,4);return Mode==6?3:4;}
 if(call==1){Boundary(13);assert(bytes==4);fable_u32 c=Count;memcpy(output,&c,4);return Mode==7?-1:4;}
 assert(call==2&&Array==output&&bytes==(int)Count*12);Boundary(15);memcpy(output,Input,bytes);return Mode==8?bytes-1:bytes;
}
bool CLoadLog::IsOk()const{if(WActive)Record(97);return Valid;}
CString& CLoadLog::AddError(){assert(this==Log);Boundary(18);Valid=false;return ErrorText;}
CString& __fastcall CString::operator<<(const char* t){assert(this==&ErrorText);Boundary(19);strcat(ErrorBuffer,t);return *this;}
CString& __fastcall CString::operator<<(long n){assert(this==&ErrorText);Boundary(20);char b[32];sprintf(b,"%ld",n);strcat(ErrorBuffer,b);return *this;}
CScriptHost::CScriptHost(void* p):Payload(0),Owner(p),Deferred(0),HighestPriority(1){memset(Unrecovered0c,0xa5,sizeof(Unrecovered0c));memset(Unrecovered50,0xa5,sizeof(Unrecovered50));}
CScriptHost::~CScriptHost(){}
void CScriptHost::QueueEvent(long key,long offset,long length){
 assert(this==Host&&Queued<Count&&Deferred==55&&HighestPriority==99);
 Boundary(16);const SLookupEntry& entry=Input[Queued++];
 assert((fable_u32)key==(fable_u32)entry.Key&&(fable_u32)length==(fable_u32)entry.Length);
 assert((fable_u32)offset==(fable_u32)((fable_u32)entry.Offset+Composite));
}
void CScriptHost::Post(){assert(false);}long CScriptHost::GetEventCount(){assert(false);return 0;}
void* CScriptHost::GetEventByIndex(unsigned long){assert(false);return 0;}
void CScriptHost::ClearEvents(){assert(false);}
long CScriptHost::GetQueuedEvent(long,long*,long*){assert(false);return 0;}
bool CScriptHost::TryActivate(void*){assert(false);return false;}
CLoadLog::CLoadLog():PKCTRList<CLoadMessage>(&Tree),Tree(0),Valid(true){if(WActive){Boundary(80);assert(!WLogAlive);WLogAlive=true;Log=this;}}
CLoadLog::~CLoadLog(){if(WActive){Record(198);assert(WLogAlive);WLogAlive=false;}}
#define COMPARATORS(K) \
 int K::IsEqual(const void*,const void*)const{assert(false);return 0;} \
 int K::IsLess(const void*,const void*)const{assert(false);return 0;} \
 int K::IsKeyEqual(const void*,const void*)const{assert(false);return 0;} \
 int K::IsKeyLess(const void*,const void*)const{assert(false);return 0;}
COMPARATORS(CLoadLog)
COMPARATORS(CSegmentList)
void CLoadLog::TransferAllFrom(PKCTRList<CLoadMessage>*){assert(false);}
CLoadMessage* CLoadLog::FindByKey(const unsigned long&){assert(false);return 0;}
void CSegmentList::TransferAllFrom(PKCTRList<CSegment>*){assert(false);}
CSegment* CSegmentList::FindByKey(const CString&){assert(false);return 0;}
template<class T> void PKCTRList<T>::Add(T*){assert(false);}
template<class T> void PKCTRList<T>::Remove(T*){assert(false);}
template<class T> void PKCTRList<T>::MorphToArray(){assert(false);}
template<class T> void PKCTRList<T>::MorphToTree(){assert(false);}
template<class T> T* PKCTRList<T>::Find(const T*){assert(false);return 0;}
template<class T> void PKCTRList<T>::ForEach(void*){assert(false);}
template<class T> T* PKCTRList<T>::GetFirst(){assert(false);return 0;}
template<class T> T* PKCTRList<T>::GetLast(){assert(false);return 0;}
template<class T> T* PKCTRList<T>::GetItemByIndex(long){assert(false);return 0;}
template<class T> void PKCTRList<T>::Reserve(long){assert(false);}
template<class T> long PKCTRList<T>::GetCount(){assert(false);return 0;}
#define CONTAINER(K) \
 int K::IsArray(){assert(false);return 0;} int K::IsTree(){assert(false);return 0;} \
 void K::MorphToArray(void*){assert(false);}void K::MorphToTree(void*){assert(false);} \
 void K::Reserve(long){assert(false);}void K::Add(void*,void*){assert(false);}void K::Remove(void*,void*){assert(false);} \
 void* K::Find(void*,const void*){assert(false);return 0;}void* K::FindByKey(void*,const void*){assert(false);return 0;} \
 void* K::GetItemByIndex(long){assert(false);return 0;}long K::GetCount(){assert(false);return 0;} \
 void* K::GetFirst(){assert(false);return 0;}void* K::GetLast(){assert(false);return 0;}void K::ForEach(void*){assert(false);}
CONTAINER(PKCContainer_00c17b70)
CONTAINER(PKCTRRedblackTree)
PKCContainer_00c17b70::~PKCContainer_00c17b70(){}
PKCTRRedblackTree::PKCTRRedblackTree(PKCTRAllocator*p):PAllocator(p),POwnedAllocator(0),Root(&Sentinel),Count(0){assert(!p);Sentinel.Left=Sentinel.Right=&Sentinel;Sentinel.Parent=0;Sentinel.Colour=0;Sentinel.Key=0;}
PKCTRRedblackTree::~PKCTRRedblackTree(){}
struct Plan {int Events[64],Size;unsigned Boundaries;char Error[256];bool Success;};
static void Add(Plan& p,int n){assert(p.Size<64);p.Events[p.Size++]=n;if(n<30&&n!=17)++p.Boundaries;}
static void Error(Plan& p,const char* text){Add(p,18);Add(p,19);strcpy(p.Error,text);}
static Plan Oracle(){
 Plan p;memset(&p,0,sizeof(p));Add(p,1);
 if(Mode==1){Error(p,"There were problems decoding the file segment structure");Add(p,40);return p;}
 Add(p,2);if(Version<0){Error(p,"Could not extract the version number");Add(p,40);return p;}
 Add(p,3);Add(p,4);
 if(Mode==2){Add(p,53);Error(p,"This bank is not a normal LUG!");Add(p,40);return p;}
 Add(p,5);Add(p,6);Add(p,55);Add(p,53);
 if(Mode==3){Error(p,"This bank is not a normal LUG!");Add(p,40);return p;}
 if(Version!=4){Add(p,18);Add(p,19);Add(p,20);Add(p,19);Add(p,20);sprintf(p.Error,"Expecting deferred bank version number 4... got %ld",Version);Add(p,40);return p;}
 Add(p,7);Add(p,8);Add(p,57);
 if(Mode==4){Error(p,"Could not open the composite data segment");Add(p,40);return p;}
 Add(p,9);Add(p,10);Add(p,11);Add(p,60);
 if(Mode==5){Error(p,"Could not open the lookup table");Add(p,40);return p;}
 Add(p,12);if(Mode==6){Error(p,"Could not read the highest priority");Add(p,40);return p;}
 Add(p,13);if(Mode==7){Error(p,"Could not read the number of samples");Add(p,40);return p;}
 if(!Count){Add(p,40);return p;}
 Add(p,14);Add(p,15);
 if(Mode==8){Add(p,17);Error(p,"Could not read in the lookup table");Add(p,40);return p;}
 for(unsigned n=0;n<Count;++n)Add(p,16);
 Add(p,17);Add(p,40);p.Success=true;return p;
}
static void Reset(){OpenCalls=ReadCalls=Queued=ReaderDtors=LiveTemps=FaultCalls=Allocations=Deletions=0;TraceSize=0;Array=0;ErrorBuffer[0]=0;ErrorText.m_pchData=ErrorBuffer;Host->Deferred=55;Host->HighestPriority=99;Log->Valid=true;}
static void Run(const Plan& p,unsigned fault){
 Reset();FaultAt=fault;SBankRequest request={0,0,(unsigned long)ExpectedChunk};bool caught=false;Active=true;
#ifdef TEST_UNWIND
 try{ReadDeferredBank(Log,&SourceToken,Host,&request);}catch(const std::bad_alloc&){caught=true;}
#else
 ReadDeferredBank(Log,&SourceToken,Host,&request);
#endif
 assert(!LiveTemps&&ReaderDtors==(fault==1?0U:1U));
 if(!fault){
  assert(!caught&&TraceSize==p.Size);for(int n=0;n<p.Size;++n)assert(Trace[n]==p.Events[n]);
  assert(FaultCalls==p.Boundaries&&strcmp(ErrorBuffer,p.Error)==0);
  assert(Log->IsOk()==(p.Error[0]==0));
  assert(Queued==(p.Success?Count:0));
  assert(Host->Deferred==(p.Success?1:55)&&Host->HighestPriority==(p.Success?Priority:99));
  assert(!Array&&Allocations==Deletions);++NormalCases;
 }else{
  assert(caught&&FaultCalls==fault);
  // A raw lookup allocation has no native owning EH stage. A callback throw
  // after allocation leaks it; observe that behavior and reclaim fixture memory.
  if(Array){assert(Allocations==1&&Deletions==0);free(Array);Array=0;}
  ++ExceptionCases;
 }
 for(unsigned n=0;n<sizeof(Host->Unrecovered0c);++n)assert(Host->Unrecovered0c[n]==0xa5);
 for(unsigned n=0;n<sizeof(Host->Unrecovered50);++n)assert(Host->Unrecovered50[n]==0xa5);
 Active=false;
}
static int TestDeferredBody(){
 CLoadLog log;CScriptHost host(0);Log=&log;Host=&host;
 const long versions[]={-1,3,4,5},priorities[]={-7,11};const unsigned counts[]={0,1,3};
 const fable_u32 positions[]={0,64,0xfffffff0U};const unsigned long chunks[]={1,2048,8192};
 for(Mode=0;Mode<9;++Mode)for(unsigned v=0;v<4;++v)for(unsigned c=0;c<3;++c)for(unsigned z=0;z<2;++z)for(unsigned b=0;b<3;++b){
  Version=versions[v];Count=counts[c];Priority=priorities[z];Composite=positions[b];ExpectedChunk=chunks[(v+c+b)%3];
  for(unsigned i=0;i<5;++i){Input[i].Key=(fable_i32)i-2;Input[i].Length=10+(fable_i32)i;Input[i].Offset=(fable_i32)i*16;}
  Plan p=Oracle();Run(p,0);
#ifdef TEST_UNWIND
  ExpectedFaults+=p.Boundaries;for(unsigned fault=1;fault<=p.Boundaries;++fault)Run(p,fault);
#endif
 }
 assert(NormalCases==648);
#ifdef TEST_UNWIND
 assert(ExceptionCases==ExpectedFaults&&ExpectedFaults==5424);
#endif
 return 0;
}


static void WMakeString(CString* p,const char* text){
 const int role=WInsideRight?186:184;Boundary(WInsideRight?86:84);assert(WStringCount<16);
 assert(WInsideRight||!strcmp(text,WName));p->m_pchData=(char*)text;WStrings[WStringCount]=p;WRoles[WStringCount++]=role;
}
static bool WDestroyString(const CString* p){
 for(unsigned i=0;i<WStringCount;++i)if(WStrings[i]==p){if(WRoles[i])Record(WRoles[i]);for(unsigned k=i+1;k<WStringCount;++k){WStrings[k-1]=WStrings[k];WRoles[k-1]=WRoles[k];}--WStringCount;return true;}return false;
}
CString::CString(const CString& other):m_pchData(other.m_pchData){
 assert(WActive);unsigned i=0;while(i<WStringCount&&WStrings[i]!=&other)++i;assert(i<WStringCount&&WStringCount<16);
 WStrings[WStringCount]=this;WRoles[WStringCount++]=WRoles[i];WRoles[i]=0;
}
CString CString::Right(int n)const{Boundary(85);assert(n==4);size_t size=strlen(m_pchData);WInsideRight=true;CString result(m_pchData+(size>4?size-4:0));WInsideRight=false;return result;}
static int WLower(int n){return n>='A'&&n<='Z'?n+'a'-'A':n;}
int CString::CompareNoCase(const char* text)const{
 Boundary(!strcmp(text,".lug")?87:!strcmp(text,".lut")?88:89);const unsigned char*a=(const unsigned char*)m_pchData,*b=(const unsigned char*)text;
 while(*a&&WLower(*a)==WLower(*b)){++a;++b;}return WLower(*a)-WLower(*b);
}
CString& __fastcall CString::operator<<(const unsigned short* text){assert(this==&ErrorText&&text==WPath);Boundary(99);strcat(ErrorBuffer,WName);return *this;}
void __fastcall WideToMultiByte(const unsigned short* path,char* out,int size){
 assert(WActive&&path==WPath&&size==256);Boundary(82);++WConversions;unsigned i=0;for(;path[i]&&i<255;++i)out[i]=path[i]<128?(char)path[i]:'?';out[i]=0;assert(!strcmp(out,WName));
}
void CLoadLog::SetSourceName(const char* name){assert(WActive&&this==Log&&!strcmp(name,WName));Boundary(96);}
#ifdef FABLE_RETAIL_CLOADLOG_MESSAGE_PASS23
CString& CLoadLog::AddMessage(){assert(false);return ErrorText;}
#endif
CAudioFile::~CAudioFile(){}PKCFileManager::~PKCFileManager(){}
class FixtureFile:public CAudioFile {public:
 virtual bool GetSize(unsigned long*){assert(false);return false;}
 virtual bool Read(unsigned long,long,void*,void*,void*,void*){assert(false);return false;}
 virtual unsigned long GetUnitSize(){assert(false);return 0;}
 virtual void Close(){assert(WActive&&WFileOpen);Record(190);WFileOpen=false;}
};
class FixtureFileManager:public PKCFileManager {public:
 virtual CAudioFile* Open(const unsigned short* path){assert(WActive&&path==WPath);Boundary(81);assert(!WFileOpen);if(WCanOpen){WFileOpen=true;return WFileAddress;}return 0;}
};
PKCAutoFile::PKCAutoFile(CAudioFile* p):m_pFile(p){}
PKCAutoFile::~PKCAutoFile(){assert(WActive);Record(194);++WFileDtors;if(m_pFile)m_pFile->Close();}
CAudioFile* PKCAutoFile::GetChecked(){assert(WActive&&m_pFile==WFileAddress&&WFileOpen);return m_pFile;}
CBankData* CBankDataOwner::GetChecked(){assert(WActive&&Pointer&&WDataAlive);return Pointer;}
void* __cdecl operator new(size_t n)throw(std::bad_alloc){void*p=malloc(n);assert(p);if(WActive&&WAllocating){assert(!WHeap&&n==sizeof(CBankData));WHeap=p;}return p;}
void __cdecl operator delete(void* p)throw(){if(p&&p==WHeap){Record(195);WHeap=0;}free(p);}
CONTAINER(PKCTRMorphable)
PKCTRMorphable::PKCTRMorphable(long n):Implementation(0){assert(n==1);}
PKCTRMorphable::~PKCTRMorphable(){}
template<class T> CBankRecordList<T>::CBankRecordList():PKCTRList<T>(&Container),Container(1){}
template<class T> int CBankRecordList<T>::IsEqual(const void*,const void*)const{assert(false);return 0;}
template<class T> int CBankRecordList<T>::IsLess(const void*,const void*)const{assert(false);return 0;}
template<class T> int CBankRecordList<T>::IsKeyEqual(const void*,const void*)const{assert(false);return 0;}
template<class T> int CBankRecordList<T>::IsKeyLess(const void*,const void*)const{assert(false);return 0;}
template<class T> void CBankRecordList<T>::TransferAllFrom(PKCTRList<T>*){assert(false);}
template<class T> T* CBankRecordList<T>::FindByKey(const unsigned long&){assert(false);return 0;}
CBankData::CBankData(){assert(WActive);Boundary(90);++WDataAlive;}
CBankData::~CBankData(){assert(WActive&&WDataAlive);Record(192);--WDataAlive;}
CIndexMap_00c3abd0::CIndexMap_00c3abd0():PKCTRList<SIndexNode_00c3abd0>(&Tree),Tree(0){}
CSampleIndexMap::CSampleIndexMap():PKCTRList<SSampleIndexNode>(&Tree),Tree(0){}
CSampleIndexMap::~CSampleIndexMap(){}
COMPARATORS(CIndexMap_00c3abd0)
COMPARATORS(CSampleIndexMap)
void CIndexMap_00c3abd0::TransferAllFrom(PKCTRList<SIndexNode_00c3abd0>*){assert(false);}
SIndexNode_00c3abd0* CIndexMap_00c3abd0::FindByKey(const unsigned long&){assert(false);return 0;}
void* CIndexMap_00c3abd0::Get(unsigned long){assert(false);return 0;}
void CSampleIndexMap::TransferAllFrom(PKCTRList<SSampleIndexNode>*){assert(false);}
SSampleIndexNode* CSampleIndexMap::FindByKey(const unsigned long&){assert(false);return 0;}
CBankProcessor::CBankProcessor(CBankData*data,void*manager,CLoadLog*log):m_pData(data),m_pManager(manager),m_pLog(log),m_pCriteriaTriggers(0),m_pNumberTriggers(0),m_option74(false),m_option75(false),m_option76(false),m_option77(false),m_option78(false),m_bSampleNameTriggers(false),m_bNamedTriggers(false){
 assert(WActive&&data&&WDataAlive&&manager==Host&&log==Log);Boundary(92);++WProcessorAlive;
}
CBankProcessor::~CBankProcessor(){assert(WActive&&WProcessorAlive);Record(193);--WProcessorAlive;}
void CBankProcessor::Load(){
 assert(WActive&&WProcessorAlive&&m_pData&&m_pManager==Host&&m_pLog==Log);
 assert(m_pCriteriaTriggers==WSettings->CriteriaTriggers&&m_pNumberTriggers==WSettings->NumberTriggers);
 assert(m_bSampleNameTriggers==WSettings->SampleNameTriggers&&m_bNamedTriggers==WSettings->NamedTriggers);
 assert(m_option74==WSettings->Option74&&m_option75==WSettings->Option75&&m_option76==WSettings->Option76);
 assert(m_option77==WSettings->EnableBothExtra&&m_option78==WSettings->EnableBothExtra);
 Boundary(93);++WLoads;if(WProcessFail)Log->Valid=false;
}
void __fastcall ReadBankData(void*file,CBankData*data,unsigned long chunk,CLoadLog*log){assert(WActive&&file==WFileAddress&&data&&WDataAlive&&chunk==ExpectedChunk&&log==Log);Boundary(91);if(WParseFail)Log->Valid=false;}
CBankData* __fastcall ReadMetadataData(void*file){assert(WActive&&file==WFileAddress);Boundary(94);if(WMetadataNull)return 0;WAllocating=true;CBankData*p=new CBankData;WAllocating=false;return p;}
CNamedIndex::CNamedIndex():PKCTRList<CItem>(0){}
COMPARATORS(CNamedIndex)
void CNamedIndex::TransferAllFrom(PKCTRList<CItem>*){assert(false);}
CItem* CNamedIndex::FindByKey(const unsigned long&){assert(false);return 0;}
struct WPlan {int Events[256],Size;char Error[256];bool Result;};
static void WP(WPlan&p,int n){assert(p.Size<256);p.Events[p.Size++]=n;}
static void WE(WPlan&p,const char*text){WP(p,18);WP(p,19);strcpy(p.Error,text);p.Result=false;}
static WPlan WOracle(unsigned codec){
 WPlan p;memset(&p,0,sizeof(p));p.Result=true;WP(p,80);WP(p,81);
 if(!WCanOpen){WE(p,"Could not open ");WP(p,99);strcat(p.Error,WName);}
 else{
  WP(p,82);WP(p,84);WP(p,85);WP(p,86);WP(p,87);
  if(codec!=0)WP(p,88);if(codec>1)WP(p,89);
  if(codec==0){
   WP(p,90);WP(p,91);WP(p,97);
   if(WParseFail)p.Result=false;
   else{WP(p,92);WP(p,93);WP(p,193);if(WProcessFail)p.Result=false;}
   WP(p,192);
  }else if(codec==1){Plan d=Oracle();for(int i=0;i<d.Size;++i)WP(p,d.Events[i]);strcpy(p.Error,d.Error);p.Result=d.Error[0]==0;}
  else if(codec==2){WP(p,94);if(WMetadataNull)WE(p,"Could not load from file");else{WP(p,90);WP(p,92);WP(p,93);WP(p,193);WP(p,192);WP(p,195);if(WProcessFail)p.Result=false;}}
  else WE(p,"This bank is not a recognised format (.lug, .lut, .met)");
  WP(p,186);WP(p,184);
 }
 WP(p,194);if(WCanOpen)WP(p,190);WP(p,82);WP(p,96);WP(p,97);WP(p,198);return p;
}
static unsigned WRun(FixtureFileManager& manager,const WPlan& plan,unsigned fault){
 WLogAlive=WFileOpen=WInsideRight=WAllocating=false;WDataAlive=WProcessorAlive=WFileDtors=WConversions=WLoads=WStringCount=0;WHeap=0;
 OpenCalls=ReadCalls=Queued=ReaderDtors=LiveTemps=FaultCalls=Allocations=Deletions=0;TraceSize=0;Array=0;ErrorBuffer[0]=0;ErrorText.m_pchData=ErrorBuffer;Host->Deferred=55;Host->HighestPriority=99;FaultAt=fault;
 bool caught=false,result=false;WActive=Active=true;
#ifdef TEST_UNWIND
 try{result=OpenBankFile(&manager,WPath,Host,WSettings);}catch(const std::bad_alloc&){caught=true;}
#else
 result=OpenBankFile(&manager,WPath,Host,WSettings);
#endif
 assert(!WLogAlive&&!WFileOpen&&!WDataAlive&&!WProcessorAlive&&!WHeap&&!WStringCount&&!LiveTemps);
 if(!fault){
  assert(!caught&&result==plan.Result&&TraceSize==plan.Size);for(int n=0;n<plan.Size;++n)assert(Trace[n]==plan.Events[n]);
  assert(!strcmp(ErrorBuffer,plan.Error)&&WFileDtors==1&&WConversions==(WCanOpen?2U:1U)&&!Array&&Allocations==Deletions);++WNormal;
 }else{assert(caught&&FaultCalls==fault);if(Array){assert(Allocations==1&&!Deletions);free(Array);Array=0;}++WThrows;}
 WActive=Active=false;return FaultCalls;
}
int main(){
 assert(TestDeferredBody()==0);
 FixtureFile file;FixtureFileManager manager;WFileAddress=&file;CScriptHost host(0);Host=&host;CNamedSet criteria;unsigned long numbers=1;
 const char*names[]={"a.lug","UPPER.LUG","b.lut","UPPER.LUT","c.met","UPPER.MET","x.ogg","","x","wide?.bad"};
 const unsigned codecs[]={0,0,1,1,2,2,3,3,3,3};const unsigned long chunks[]={1,2048,8192};
 for(unsigned name=0;name<10;++name)for(unsigned open=0;open<2;++open)for(unsigned mode=0;mode<4;++mode)for(unsigned cfg=0;cfg<3;++cfg)for(unsigned bits=0;bits<64;++bits){
  strcpy(WName,names[name]);unsigned short path[256];unsigned i=0;for(;WName[i];++i)path[i]=(unsigned char)WName[i];path[i]=0;if(name==9)path[4]=0x1234;WPath=path;
  WCanOpen=open!=0;WParseFail=mode==1;WProcessFail=mode==2;WMetadataNull=mode==3;ExpectedChunk=chunks[cfg];
  SBankRequest settings={bits&1?&criteria:0,bits&2?(void*)&numbers:0,chunks[cfg],(bits&1)!=0,(bits&2)!=0,(bits&4)!=0,(bits&8)!=0,(bits&16)!=0,(bits&32)!=0};WSettings=&settings;
  Mode=mode==3?2:0;Version=mode==1?-1:4;Count=mode==2?0:2;Priority=-7;Composite=64;
  for(i=0;i<5;++i){Input[i].Key=(fable_i32)i-2;Input[i].Length=10+(fable_i32)i;Input[i].Offset=(fable_i32)i*16;}
  WPlan p=WOracle(codecs[name]);unsigned boundaries=WRun(manager,p,0);
#ifdef TEST_UNWIND
  if(bits==0||bits==21||bits==63)for(unsigned fault=1;fault<=boundaries;++fault)WRun(manager,p,fault);
#endif
 }
 assert(WNormal==15360);
#ifdef TEST_UNWIND
 assert(WThrows==7794);
#endif
 puts("01211df3_TEST PASS");return 0;
}