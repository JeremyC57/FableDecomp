// 01213c1a bank sample-trigger full-body real-object behavior test
// Flags: none
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
// 01213c1a CBankProcessor::AddSampleNameTriggers parent00c2e610 and generated CString cleanup
// Leaf: ?AddSampleNameTriggers@CBankProcessor@@QAEXXZ
// Flags: /EHsc
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#include "libpk/CBankProcessor.h"

void CBankProcessor::AddSampleNameTriggers()
{
    if (!m_pCriteriaTriggers || !m_bSampleNameTriggers)
        return;
    for (unsigned long i = 0; i < static_cast<unsigned long>(m_pData->m_drivers.GetCount()); ++i) {
        SDriverRecord* record = m_pData->m_drivers.GetItemByIndex(i);
        void* driver = m_drivers.Get(record->m_number);
        if (!driver) {
            m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
        }
        else if (record->m_number == record->m_sample) {
            SSampleHeader* sample = m_pData->m_samples.FindByKey(record->m_sample);
            CString name = sample->m_fileName.GetFileNameWithoutExtension();
            name.MakeUpper();
            if (m_pCriteriaTriggers->Find(name, 0)) {
                m_pLog->AddMessage() << "Duplicate event trigger for string '" << name << "'. Hash code = " << name.GetHash();
            }
            else {
                CEventTriggerRef trigger(m_pCriteriaTriggers, (const char*)name, m_bNamedTriggers);
                trigger.InsertDriver(trigger.GetDriverCount(), driver);
            }
        }
    }
}

int  PKCContainer_00c17b70::IsArray(){abort();}
int  PKCContainer_00c17b70::IsTree(){abort();}
void  PKCContainer_00c17b70::MorphToArray(void* comparator){abort();}
void  PKCContainer_00c17b70::MorphToTree(void* comparator){abort();}
void  PKCContainer_00c17b70::Reserve(long count){abort();}
void  PKCContainer_00c17b70::Add(void* owner, void* item){abort();}
void  PKCContainer_00c17b70::Remove(void* owner, void* item){abort();}
void*  PKCContainer_00c17b70::Find(void* comparator, const void* item){abort();}
void*  PKCContainer_00c17b70::FindByKey(void* comparator, const void* key){abort();}
void*  PKCContainer_00c17b70::GetItemByIndex(long index){abort();}
long  PKCContainer_00c17b70::GetCount(){abort();}
void*  PKCContainer_00c17b70::GetFirst(){abort();}
void*  PKCContainer_00c17b70::GetLast(){abort();}
void  PKCContainer_00c17b70::ForEach(void* visitor){abort();}
int  PKCTRRedblackTree::IsArray(){abort();}
int  PKCTRRedblackTree::IsTree(){abort();}
void  PKCTRRedblackTree::MorphToArray(void*){abort();}
void  PKCTRRedblackTree::MorphToTree(void*){abort();}
void  PKCTRRedblackTree::Reserve(long){abort();}
void  PKCTRRedblackTree::Add(void*,void*){abort();}
void  PKCTRRedblackTree::Remove(void*,void*){abort();}
void*  PKCTRRedblackTree::Find(void*,const void*){abort();}
void*  PKCTRRedblackTree::FindByKey(void*,const void*){abort();}
void*  PKCTRRedblackTree::GetItemByIndex(long){abort();}
long  PKCTRRedblackTree::GetCount(){abort();}
void*  PKCTRRedblackTree::GetFirst(){abort();}
void*  PKCTRRedblackTree::GetLast(){abort();}
void  PKCTRRedblackTree::ForEach(void*){abort();}
int  CIndexMap_00c3abd0::IsEqual(const void*,const void*)const{abort();}
int  CIndexMap_00c3abd0::IsLess(const void*,const void*)const{abort();}
int  CIndexMap_00c3abd0::IsKeyEqual(const void*,const void*)const{abort();}
int  CIndexMap_00c3abd0::IsKeyLess(const void*,const void*)const{abort();}
void  CIndexMap_00c3abd0::TransferAllFrom(PKCTRList<SIndexNode_00c3abd0>*){abort();}
SIndexNode_00c3abd0* CIndexMap_00c3abd0::FindByKey(const unsigned long&){abort();}
int  CNamedIndex::IsEqual(const void*,const void*)const{abort();}
int  CNamedIndex::IsLess(const void*,const void*)const{abort();}
int  CNamedIndex::IsKeyEqual(const void*,const void*)const{abort();}
int  CNamedIndex::IsKeyLess(const void*,const void*)const{abort();}
void  CNamedIndex::TransferAllFrom(PKCTRList<CItem>*){abort();}
CItem* CNamedIndex::FindByKey(const unsigned long&){abort();}
int  CLoadLog::IsEqual(const void*,const void*)const{abort();}
int  CLoadLog::IsLess(const void*,const void*)const{abort();}
int  CLoadLog::IsKeyEqual(const void*,const void*)const{abort();}
int  CLoadLog::IsKeyLess(const void*,const void*)const{abort();}
void  CLoadLog::TransferAllFrom(PKCTRList<CLoadMessage>*){abort();}
CLoadMessage*  CLoadLog::FindByKey(const unsigned long&){abort();}

static CBankProcessor*Processor;static CBankData*Data;static CLoadLog*Log;static CNamedSet*Registry;
static SDriverRecord*DriverRecords;static SSampleHeader*Samples;static CLoadMessage*Messages;
static unsigned long Count,Current,GlobalHash;static int Pattern,PathVariant,ThrowAt,Phase;
static int CountCalls,ItemCalls,MapCalls,SampleCalls,FileCalls,UpperCalls,FindCalls,HashCalls,LogCalls,TriggerCalls,DriverCountCalls,InsertCalls,ParamCtor;
static int MessageSteps[8],MessageKinds[8],Outstanding;
static CString*CurrentName;static bool NameAlive;static CEventTriggerRef*LastTrigger;
static int DriverTokens[8],EntryTokens[8],TriggerTokens[8];
static const char*Paths[]={"voice.wav","dir\\mix.ogg","\\dir\\sound..wav","plain",".hidden","\\\\net\\voice.v1.wav","x/y.sound.wav"};
static const char*Bases[]={"voice","mix","sound","plain","","voice.v1","x/y.sound"};
static const char*Uppers[]={"VOICE","MIX","SOUND","PLAIN","","VOICE.V1","X/Y.SOUND"};
static unsigned long CaseOf(){return (Current+(unsigned long)PathVariant)%7;}
static int Kind(){return (Pattern+(int)Current)%4;}
static void ThrowIf(int n){
#ifndef _MSC_VER
 if(ThrowAt==n)throw n;
#else
 assert(!ThrowAt);(void)n;
#endif
}
struct EmptyBlock{CStringData Header;char Text[1];};static EmptyBlock Empty={{-1,0,0},{0}};
char*_afxPchNil=Empty.Text;PFNASSERTHOOK g_pfnAssertHook=0;CDebugBreak::CDebugBreak(){abort();}
static CStringData*Header(char*p){return reinterpret_cast<CStringData*>(p)-1;}
static void Drop(char*p){if(p!=_afxPchNil){CStringData*h=Header(p);assert(h->nRefs>0);if(--h->nRefs==0){free(h);--Outstanding;}}}
static void Put(CString&s,const char*text){size_t n=strlen(text);char*next=_afxPchNil;if(n){CStringData*h=(CStringData*)malloc(sizeof(CStringData)+n+1);assert(h);h->nRefs=1;h->nDataLength=(long)n;h->nAllocLength=(long)n;next=(char*)(h+1);memcpy(next,text,n+1);++Outstanding;}Drop(s.m_pchData);s.m_pchData=next;}
CString::CString(const char*s):m_pchData(_afxPchNil){if(Phase==3){assert(CurrentName&&s==CurrentName->m_pchData);++ParamCtor;}Put(*this,s);}
CString::CString(const CString&s):m_pchData(s.m_pchData){if(m_pchData!=_afxPchNil)++Header(m_pchData)->nRefs;}
CString::~CString(){if(this==CurrentName){NameAlive=false;CurrentName=0;}Drop(m_pchData);m_pchData=_afxPchNil;}
void CString::CopyBeforeWrite(){if(m_pchData!=_afxPchNil&&Header(m_pchData)->nRefs>1){char buf[256];assert(strlen(m_pchData)<sizeof(buf));strcpy(buf,m_pchData);Put(*this,buf);}}
CString CString::GetFileNameWithoutExtension()const{assert(this==&Samples[Current].m_fileName&&Phase==1);assert(strcmp(m_pchData,Paths[CaseOf()])==0);++FileCalls;return CString(Bases[CaseOf()]);}
void CString::MakeUpper(){assert(Phase==1&&strcmp(m_pchData,Bases[CaseOf()])==0);CurrentName=this;NameAlive=true;CopyBeforeWrite();for(char*p=m_pchData;*p;++p)if(*p>='a'&&*p<='z')*p=(char)(*p-'a'+'A');assert(strcmp(m_pchData,Uppers[CaseOf()])==0);++UpperCalls;ThrowIf(1);}
unsigned long CString::GetHash()const{assert(this==CurrentName&&NameAlive&&Phase==2&&Kind()==2);assert(strcmp(m_pchData,Uppers[CaseOf()])==0);++HashCalls;GlobalHash=0x8f000000UL+(unsigned long)PathVariant*256+Current;return GlobalHash;}
static CString*MessageText(){assert(LogCalls>0);return &Messages[LogCalls-1].Text;}
CString& __fastcall CString::operator<<(const char*s){assert(this==MessageText());int m=LogCalls-1;if(MessageSteps[m]==0){assert(strcmp(s,MessageKinds[m]==0?"Could not find driver number ":"Duplicate event trigger for string '")==0);}else{assert(MessageKinds[m]==2&&MessageSteps[m]==2&&strcmp(s,"'. Hash code = ")==0);}++MessageSteps[m];return *this;}
CString& __fastcall CString::operator<<(const CString&s){assert(this==MessageText()&&MessageKinds[LogCalls-1]==2&&MessageSteps[LogCalls-1]==1);assert(&s==CurrentName&&NameAlive&&strcmp(s.m_pchData,Uppers[CaseOf()])==0);++MessageSteps[LogCalls-1];return *this;}
CString& __fastcall CString::operator<<(unsigned long n){assert(this==MessageText());int m=LogCalls-1;if(MessageKinds[m]==0){assert(MessageSteps[m]==1&&n==DriverRecords[Current].m_number);}else{assert(MessageSteps[m]==3&&n==GlobalHash);}++MessageSteps[m];return *this;}
CString&CLoadLog::AddMessage(){assert(this==Log&&Valid);assert(Kind()==0||Kind()==2);ThrowIf(3);if(LogCalls)assert(MessageSteps[LogCalls-1]==(MessageKinds[LogCalls-1]==0?2:4));if(Kind()==2)assert(Phase==2);else assert(!NameAlive&&Phase==0);int i=LogCalls++;assert(i<8);MessageKinds[i]=Kind();MessageSteps[i]=0;Messages[i].Error=false;Messages[i].Sequence=(unsigned long)(100+i);return Messages[i].Text;}
CItem*CNamedSet::Find(const CString&name,long*collisions){assert(this==Registry&&collisions==0&&&name==CurrentName&&NameAlive);assert(strcmp(name.m_pchData,Uppers[CaseOf()])==0);++FindCalls;if(Kind()==2){Phase=2;return reinterpret_cast<CItem*>(&EntryTokens[Current]);}assert(Kind()==3);Phase=3;return 0;}
CEventTriggerRef::CEventTriggerRef(CNamedSet*registry,CString name,bool named):m_pRegistry(registry),m_pTrigger(reinterpret_cast<CItem*>(&TriggerTokens[Current])){assert(registry==Registry&&Phase==3&&named==Processor->m_bNamedTriggers&&NameAlive);assert(strcmp(name.m_pchData,Uppers[CaseOf()])==0&&ParamCtor==TriggerCalls+1);LastTrigger=this;++TriggerCalls;ThrowIf(2);Phase=4;}
unsigned long CEventTriggerRef::GetDriverCount(){assert(this==LastTrigger&&m_pRegistry==Registry&&m_pTrigger==reinterpret_cast<CItem*>(&TriggerTokens[Current])&&Phase==4);++DriverCountCalls;Phase=5;return 3+Current;}
long CEventTriggerRef::InsertDriver(unsigned long position,void*driver){assert(this==LastTrigger&&Phase==5&&position==3+Current&&driver==&DriverTokens[Current]);++InsertCalls;Phase=6;return 0;}
void*CIndexMap_00c3abd0::Get(unsigned long n){assert(this==&Processor->m_drivers&&n==DriverRecords[Current].m_number&&Phase==0);++MapCalls;return Kind()==0?0:&DriverTokens[Current];}
CBankProcessor::CBankProcessor(CBankData*d,void*manager,CLoadLog*log):m_pData(d),m_pManager(manager),m_pLog(log),m_pCriteriaTriggers(0),m_pNumberTriggers(0),m_bSampleNameTriggers(false),m_bNamedTriggers(false){}
CBankProcessor::~CBankProcessor(){}
CLoadLog::CLoadLog():PKCTRList<CLoadMessage>(&Tree),Tree(0),Valid(true){}CLoadLog::~CLoadLog(){}
CIndexMap_00c3abd0::CIndexMap_00c3abd0():PKCTRList<SIndexNode_00c3abd0>(&Tree),Tree(0){}
CNamedIndex::CNamedIndex():PKCTRList<CItem>(0){}
PKCContainer_00c17b70::~PKCContainer_00c17b70(){}
PKCTRRedblackTree::PKCTRRedblackTree(PKCTRAllocator*p):PAllocator(p),POwnedAllocator(0),Root(&Sentinel),Count(0){Sentinel.Left=Sentinel.Right=&Sentinel;Sentinel.Parent=0;Sentinel.Key=0;Sentinel.Colour=0;}
PKCTRRedblackTree::~PKCTRRedblackTree(){}
template<class T>CBankRecordList<T>::CBankRecordList():PKCTRList<T>(0){}
template<class T>int CBankRecordList<T>::IsEqual(const void*,const void*)const{abort();return 0;}
template<class T>int CBankRecordList<T>::IsLess(const void*,const void*)const{abort();return 0;}
template<class T>int CBankRecordList<T>::IsKeyEqual(const void*,const void*)const{abort();return 0;}
template<class T>int CBankRecordList<T>::IsKeyLess(const void*,const void*)const{abort();return 0;}
template<class T>void CBankRecordList<T>::TransferAllFrom(PKCTRList<T>*){abort();}
template<class T>T*CBankRecordList<T>::FindByKey(const unsigned long&key){assert((void*)this==(void*)&Data->m_samples&&&key==&DriverRecords[Current].m_sample&&key==Samples[Current].m_number);assert(Kind()>=2&&Phase==0);++SampleCalls;Phase=1;return reinterpret_cast<T*>(&Samples[Current]);}
template<class T>void PKCTRList<T>::MorphToArray(){abort();}
template<class T>void PKCTRList<T>::MorphToTree(){abort();}
template<class T>T*PKCTRList<T>::Find(const T*){abort();return 0;}
template<class T>void PKCTRList<T>::Remove(T*){abort();}
template<class T>void PKCTRList<T>::Add(T*){abort();}
template<class T>void PKCTRList<T>::ForEach(void*){abort();}
template<class T>T*PKCTRList<T>::GetFirst(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetLast(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetItemByIndex(long i){assert((void*)this==(void*)&Data->m_drivers&&i>=0&&(unsigned long)i<Count);assert(i==ItemCalls);assert(!NameAlive);Current=(unsigned long)i;Phase=0;++ItemCalls;return reinterpret_cast<T*>(&DriverRecords[i]);}
template<class T>void PKCTRList<T>::Reserve(long){abort();}
template<class T>long PKCTRList<T>::GetCount(){assert((void*)this==(void*)&Data->m_drivers);assert(!NameAlive);++CountCalls;return(long)Count;}
static void Run(bool registry,bool enabled,bool named,unsigned long count,int pattern,int path,unsigned long first,int throwat){
 assert(Outstanding==0);Phase=-1;CurrentName=0;NameAlive=false;
 {CBankData data;CLoadLog log;CBankProcessor processor(&data,0,&log);CNamedSet reg;SDriverRecord records[8];SSampleHeader samples[8];CLoadMessage messages[8];
  Processor=&processor;Data=&data;Log=&log;Registry=&reg;DriverRecords=records;Samples=samples;Messages=messages;Count=count;Pattern=pattern;PathVariant=path;ThrowAt=throwat;
  processor.m_pCriteriaTriggers=registry?&reg:0;processor.m_bSampleNameTriggers=enabled;processor.m_bNamedTriggers=named;
  CountCalls=ItemCalls=MapCalls=SampleCalls=FileCalls=UpperCalls=FindCalls=HashCalls=LogCalls=TriggerCalls=DriverCountCalls=InsertCalls=ParamCtor=0;GlobalHash=0;LastTrigger=0;
  for(int i=0;i<8;++i){records[i].m_number=first+(unsigned long)i*3;records[i].m_sample=((pattern+i)%4==1)?records[i].m_number+1000:records[i].m_number;samples[i].m_number=records[i].m_sample;Put(samples[i].m_fileName,Paths[(i+path)%7]);messages[i].Error=true;messages[i].Sequence=0;MessageSteps[i]=MessageKinds[i]=0;}
#ifdef _MSC_VER
  assert(sizeof(CBankRecordList<SSampleHeader>)==16&&sizeof(CIndexMap_00c3abd0)==48&&sizeof(CLoadLog)==52);assert(sizeof(CEventTriggerRef)==8&&sizeof(CBankProcessor)==0x7c);assert((char*)&processor.m_drivers-(char*)&processor==0xc&&(char*)&processor.m_pCriteriaTriggers-(char*)&processor==0x6c&&(char*)&processor.m_bSampleNameTriggers-(char*)&processor==0x79);assert((char*)&samples[0].m_fileName-(char*)&samples[0]==4&&(char*)&records[0].m_sample-(char*)&records[0]==4);assert((char*)&log.Valid-(char*)&log==0x30);
#endif
#ifndef _MSC_VER
  try{processor.AddSampleNameTriggers();}catch(int n){assert(n==throwat&&!NameAlive&&Outstanding==8&&log.Valid);Phase=-1;CurrentName=0;return;}
#else
  processor.AddSampleNameTriggers();
#endif
  assert(!throwat&&!NameAlive&&log.Valid&&Outstanding==8);assert(processor.m_pData==&data&&processor.m_pLog==&log&&processor.m_pCriteriaTriggers==(registry?&reg:0)&&processor.m_bSampleNameTriggers==enabled&&processor.m_bNamedTriggers==named);
  int expectedLogs=0,expectedNames=0,expectedCreates=0,expectedDuplicates=0;
  if(registry&&enabled){for(unsigned long i=0;i<count;++i){int k=(pattern+(int)i)%4;if(k==0)++expectedLogs;if(k>=2)++expectedNames;if(k==2){++expectedLogs;++expectedDuplicates;}if(k==3)++expectedCreates;}assert(CountCalls==(int)count+1&&ItemCalls==(int)count&&MapCalls==(int)count);}
  else assert(!CountCalls&&!ItemCalls&&!MapCalls);
  assert(SampleCalls==expectedNames&&FileCalls==expectedNames&&UpperCalls==expectedNames&&FindCalls==expectedNames);assert(HashCalls==expectedDuplicates&&LogCalls==expectedLogs&&TriggerCalls==expectedCreates&&DriverCountCalls==expectedCreates&&InsertCalls==expectedCreates&&ParamCtor==expectedCreates);
  for(int i=0;i<LogCalls;++i){assert(MessageSteps[i]==(MessageKinds[i]==0?2:4));assert(!messages[i].Error&&messages[i].Sequence==(unsigned long)(100+i));}
  for(int i=0;i<8;++i){assert(strcmp(samples[i].m_fileName.m_pchData,Paths[(i+path)%7])==0);assert(records[i].m_number==first+(unsigned long)i*3);}
  Phase=-1;CurrentName=0;
 }
 assert(Outstanding==0);
}
int main(){unsigned long counts[]={0,1,2,4,7},firsts[]={0,1,1000,40000};long cases=0;for(int reg=0;reg<2;++reg)for(int e=0;e<2;++e)for(int n=0;n<2;++n)for(int c=0;c<5;++c)for(int pattern=0;pattern<8;++pattern)for(int path=0;path<7;++path)for(int first=0;first<4;++first){Run(reg!=0,e!=0,n!=0,counts[c],pattern,path,firsts[first],0);++cases;}assert(cases==8960);
#ifndef _MSC_VER
 Run(true,true,true,1,3,0,1,1);assert(Outstanding==0);Run(true,true,true,1,3,0,1,2);assert(Outstanding==0);Run(true,true,true,1,0,0,1,3);assert(Outstanding==0);
#endif
 puts("01213c1a_TEST PASS");return 0;}