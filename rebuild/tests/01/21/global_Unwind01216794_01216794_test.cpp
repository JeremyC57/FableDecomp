// Standalone production source plus real shared-object decoder fixture.
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#ifdef _MSC_VER
// Exclude only the CRT header declaration: production retains its real DLL import.
#define rand DecoderFixtureUnusedCRTRand
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <assert.h>
#ifdef _MSC_VER
#undef rand
#endif
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
#define TEST_UNWIND
#endif
// 00c73af0 CreateDecoderStream
// Leaf: ?CreateDecoderStream@@YIPAVIAudioStream@@PAXPBGABUPKCBufferedReaderParams@@@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23
#include "libpk/IAudioStream.h"
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) int __cdecl rand(void);
#else
#include <stdlib.h>
#endif
extern bool g_ForceTestTone;
long __fastcall ClassifyAudioExtension(const CString& name)
{
 if(g_ForceTestTone)return 2;
 if(CString::SafeStrlen(name.m_pchData)<4)return 3;
 CString tail=name.Right(4);
 if(tail.CompareNoCase(".ogg")==0)return 1;
 if(tail.CompareNoCase(".wma")==0)return 0;
 return 3;
}
#define PK_REPORT_ERROR(line,text) { { CString s; s << ".\\CDecompressorInterface.cpp" << "(" << (long)(line) << ") : " << text << "\n"; if(g_pfnAssertHook)g_pfnAssertHook(s.m_pchData); } CDebugBreak(); }
IAudioStream* __fastcall CreateDecoderStream(void* source,const unsigned short* name,const PKCBufferedReaderParams& params)
{
 char path[0x100];
 WideToMultiByte(name,path,0x100);
 long type;
 {CString classified(path);type=ClassifyAudioExtension(classified);}
 IAudioStream* stream=0;
 if(type==0){
  stream=new CNullAudioStream(source,params);
  if(!stream)PK_REPORT_ERROR(40,"EMEM");
 }
 else if(type==1){
  stream=new COggVorbisStream(source,params);
  if(!stream)PK_REPORT_ERROR(45,"EMEM");
 }
 else if(type==2)stream=new CTestToneAudioStream(((float)rand()*(1.0f/32767.0f)+1.0f)*440.0f);
 return stream;
}


struct NilStorage {CStringData Header;char Text[1];};
static NilStorage Nil={{-1,0,0},{0}};
char* _afxPchNil=Nil.Text;
PFNASSERTHOOK g_pfnAssertHook=0;
bool g_ForceTestTone=false;
static bool Active,Disposing,InsideRight,FailAllocation,OggValid;
static unsigned Codec,HookEnabled,NameIndex,RandomIndex,ConfigIndex;
static int Events[64],Used;
static unsigned BoundaryCalls,FaultAt,ExpectedFaultCases,NormalCases,ThrowCases;
static unsigned Allocs,Frees,Constructed,ReaderConstructed,ReaderDestroyed,StreamDestroyed,RandomCalls;
static unsigned SuffixConstructed,PathDestroyed,ErrorDestroyed,BreakCalls,HookCalls;
static unsigned long RandomValue;
static float Frequency;
static char ExpectedPath[256],Message[128];
static unsigned char SourceToken;
static const PKCBufferedReaderParams* Params;
static void* Object;static unsigned char* Raw;static size_t ObjectSize;
static const CString* Strings[8];static int Roles[8];static unsigned StringCount;
static void Record(int n){assert(Used<64);Events[Used++]=n;}
static void Boundary(int n){Record(n);++BoundaryCalls;
#ifdef TEST_UNWIND
 if(FaultAt==BoundaryCalls)throw std::bad_alloc();
#else
 assert(!FaultAt);
#endif
}
static void CheckGuards(){assert(Raw&&Object==(void*)(Raw+16));for(unsigned i=0;i<16;++i)assert(Raw[i]==0xa5&&Raw[16+ObjectSize+i]==0xa5);}
void* __cdecl operator new(size_t n) throw(std::bad_alloc){
 if(!Active){void*p=malloc(n);assert(p);return p;}
 assert(!Object&&Codec<3);size_t expected=Codec==0?sizeof(CNullAudioStream):Codec==1?sizeof(COggVorbisStream):sizeof(CTestToneAudioStream);assert(n==expected);
 Boundary(8);++Allocs;if(FailAllocation)return 0;
 Raw=(unsigned char*)malloc(n+32);assert(Raw);ObjectSize=n;memset(Raw,0xa5,n+32);Object=Raw+16;return Object;
}
void __cdecl operator delete(void* p) throw(){
 if(p==Object&&Object){CheckGuards();if(Active){Record(50);++Frees;}else assert(Disposing);free(Raw);Raw=0;Object=0;return;}
 assert(!Active);free(p);
}
static int Role(const CString* p){for(unsigned i=0;i<StringCount;++i)if(Strings[i]==p)return Roles[i];return -1;}
static void RegisterString(const CString* p,int role){assert(StringCount<8&&Role(p)==-1);Strings[StringCount]=p;Roles[StringCount++]=role;}
static char* StringData(const char* text){size_t n=strlen(text);CStringData*h=(CStringData*)malloc(sizeof(CStringData)+n+1);assert(h);h->nRefs=1;h->nDataLength=h->nAllocLength=(long)n;char*p=h->data();memcpy(p,text,n+1);return p;}
CString::CString(const char* text){
 const int role=InsideRight?2:1;Boundary(role==1?2:5);if(role==1)assert(strcmp(text,ExpectedPath)==0);else++SuffixConstructed;
 m_pchData=StringData(text);RegisterString(this,role);
}
CString::CString(const CString& other):m_pchData(other.m_pchData){
 int role=Role(&other);assert(role>=0);if(GetData()->nRefs>=0)++GetData()->nRefs;
 // Treat optional C++03 return-value transport copies as the same logical
 // suffix value; the actual shared CStringData ownership remains reference-counted.
 for(unsigned i=0;i<StringCount;++i)if(Strings[i]==&other)Roles[i]=0;
 RegisterString(this,role);
}
CString::~CString(){
 int role=Role(this);assert(role>=0);
 if(role==1){Record(42);++PathDestroyed;}else if(role==2)Record(43);else if(role==3){Record(44);++ErrorDestroyed;}
 unsigned i=0;while(Strings[i]!=this)++i;for(unsigned k=i+1;k<StringCount;++k){Strings[k-1]=Strings[k];Roles[k-1]=Roles[k];}--StringCount;
 CStringData*h=GetData();if(h->nRefs>=0&&--h->nRefs==0)free(h);
}
int __stdcall CString::SafeStrlen(const char* text){Boundary(3);assert(text);return (int)strlen(text);}
CString CString::Right(int count)const{
 Boundary(4);size_t n=strlen(m_pchData);assert(count==4&&n>=4);InsideRight=true;CString tail(m_pchData+n-4);InsideRight=false;return tail;
}
static int Lower(int c){return c>='A'&&c<='Z'?c+('a'-'A'):c;}
int CString::CompareNoCase(const char* text)const{
 Boundary(!strcmp(text,".ogg")?6:7);const unsigned char*a=(const unsigned char*)m_pchData;const unsigned char*b=(const unsigned char*)text;
 while(*a&&Lower(*a)==Lower(*b)){++a;++b;}return Lower(*a)-Lower(*b);
}
static void Append(CString* s,const char* text){
 if(Role(s)==-1){assert(s->m_pchData==_afxPchNil);RegisterString(s,3);}
 char buffer[256];strcpy(buffer,s->m_pchData);strcat(buffer,text);CStringData*old=s->GetData();s->m_pchData=StringData(buffer);if(old->nRefs>=0&&--old->nRefs==0)free(old);
}
CString& __fastcall CString::operator<<(const char* text){if(Role(this)==-1)RegisterString(this,3);Boundary(20);Append(this,text);return *this;}
CString& __fastcall CString::operator<<(long n){if(Role(this)==-1)RegisterString(this,3);Boundary(21);assert(n==(Codec==0?40:45));char text[32];sprintf(text,"%ld",n);Append(this,text);return *this;}
void __fastcall WideToMultiByte(const unsigned short* name,char* dest,int size){
 Boundary(1);assert(size==256&&name);unsigned i=0;for(;name[i]&&i<255;++i)dest[i]=name[i]<=127?(char)name[i]:'?';dest[i]=0;assert(strcmp(dest,ExpectedPath)==0);
}
static int __cdecl TestRand(){Record(12);++RandomCalls;assert(Object&&Codec==2&&!FailAllocation);return (int)RandomValue;}
#ifdef _MSC_VER
// Bind the test's actual imported rand pointer in C++, preserving the pasted
// production DLL-import declaration and exact call ABI without assembly.
extern "C" { int (__cdecl* _imp__rand)(void)=TestRand; }
#else
extern "C" int rand(void) throw(){return TestRand();}
#endif
static void __fastcall Report(const char* text){Boundary(22);++HookCalls;assert(ErrorDestroyed==0&&Role((const CString*)0)==-1);assert(strcmp(text,Message)==0);}
CDebugBreak::CDebugBreak(){Boundary(23);++BreakCalls;assert(ErrorDestroyed==1&&StringCount==0);}
CNullAudioStream::CNullAudioStream(void* source,const PKCBufferedReaderParams& p){assert(source==&SourceToken&&&p==Params&&this==Object);Boundary(9);Constructed=1;}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}
PKCCriticalSection::~PKCCriticalSection(){}
void PKCCriticalSection::Enter(){}void PKCCriticalSection::Leave(){}
PKCSemaphore::PKCSemaphore(long,long):m_hSemaphore(0){}
PKCSemaphore::~PKCSemaphore(){}void PKCSemaphore::Wait(){}void PKCSemaphore::Release(){}
PKCMailbox::PKCMailbox(void*q,long n):m_pQueue(q),m_semaphore(0,n){}
PKCBufferedReader::PKCBufferedReader(void* source,const PKCBufferedReaderParams& p)
 :m_mailbox(0,(long)(p.ExtraBuffers+p.BaseBuffers)),m_outstanding(0),m_completedReadOffset(0){
 assert(source==&SourceToken&&&p==Params);Boundary(10);++ReaderConstructed;memset(_unknown00,0,sizeof(_unknown00));
}
PKCBufferedReader::~PKCBufferedReader(){++ReaderDestroyed;}
COggVorbisStream::COggVorbisStream(void* source,const PKCBufferedReaderParams& p):m_reader(source,p),m_valid(OggValid?1:0),m_opened(1){assert(this==Object&&&p==Params);Boundary(11);Constructed=1;memset(&m_vf,0,sizeof(m_vf));}
COggVorbisStream::~COggVorbisStream(){++StreamDestroyed;}
CTestToneAudioStream::CTestToneAudioStream(float frequency):m_position(0),m_sampleRate(44100){assert(this==Object&&RandomCalls==1);Boundary(13);Frequency=frequency;Constructed=1;unsigned bits=0x3d80634a;memcpy(&m_scale,&bits,4);}
#define STREAM_UNUSED(K) \
 long K::GetLength(){assert(false);return 0;} \
 long K::Seek(unsigned long,bool){assert(false);return 0;} \
 long K::Read(SAudioBuffer*,unsigned long){assert(false);return 0;} \
 long K::GetRate(){assert(false);return 0;} \
 long K::GetState(){assert(false);return 0;}
STREAM_UNUSED(CNullAudioStream)
STREAM_UNUSED(COggVorbisStream)
STREAM_UNUSED(CTestToneAudioStream)
struct Plan {int Event[64],Size;unsigned Boundaries;bool Error,Returns;};
static void Add(Plan&p,int n){assert(p.Size<64);p.Event[p.Size++]=n;if(n!=12&&n<40)++p.Boundaries;}
static unsigned ExpectedCodec(const char* name){
 if(g_ForceTestTone)return 2;size_t n=strlen(name);if(n<4)return 3;
 const char* ext=name+n-4;if(Lower(ext[0])=='.'&&Lower(ext[1])=='o'&&Lower(ext[2])=='g'&&Lower(ext[3])=='g')return 1;
 if(Lower(ext[0])=='.'&&Lower(ext[1])=='w'&&Lower(ext[2])=='m'&&Lower(ext[3])=='a')return 0;
 return 3;
}
static Plan Oracle(){
 Plan p;memset(&p,0,sizeof(p));Add(p,1);Add(p,2);
 if(!g_ForceTestTone){Add(p,3);if(strlen(ExpectedPath)>=4){Add(p,4);Add(p,5);Add(p,6);if(Codec!=1)Add(p,7);Add(p,43);}}
 Add(p,42);
 if(Codec>=3)return p;
 Add(p,8);
 if(!FailAllocation){if(Codec==0)Add(p,9);else if(Codec==1){Add(p,10);Add(p,11);}else{Add(p,12);Add(p,13);}p.Returns=true;return p;}
 if(Codec==2)return p;
 p.Error=true;Add(p,20);Add(p,20);Add(p,21);Add(p,20);Add(p,20);Add(p,20);if(HookEnabled)Add(p,22);Add(p,44);Add(p,23);return p;
}
static void Reset(){
 assert(!Object&&!StringCount);Active=true;Disposing=InsideRight=false;Used=0;BoundaryCalls=0;Allocs=Frees=Constructed=ReaderConstructed=ReaderDestroyed=StreamDestroyed=RandomCalls=SuffixConstructed=PathDestroyed=ErrorDestroyed=BreakCalls=HookCalls=0;Frequency=0;Raw=0;ObjectSize=0;
}
static void Run(const Plan&p,unsigned fault,const unsigned short* wide){
 Reset();FaultAt=fault;bool caught=false;IAudioStream* stream=0;
#ifdef TEST_UNWIND
 try{stream=CreateDecoderStream(&SourceToken,wide,*Params);}catch(const std::bad_alloc&){caught=true;}
#else
 stream=CreateDecoderStream(&SourceToken,wide,*Params);
#endif
 assert(!StringCount);
 if(fault){assert(caught&&BoundaryCalls==fault&&!Object);assert(Frees==(Allocs&&!FailAllocation?1U:0U));assert(ReaderDestroyed==ReaderConstructed);++ThrowCases;}
 else{
  assert(!caught&&BoundaryCalls==p.Boundaries&&Used==p.Size);for(int i=0;i<p.Size;++i)assert(Events[i]==p.Event[i]);
  assert((stream!=0)==p.Returns&&Constructed==(p.Returns?1U:0U));assert(Allocs==(Codec<3?1U:0U)&&Frees==0);
  assert(BreakCalls==(p.Error?1U:0U)&&HookCalls==(p.Error&&HookEnabled?1U:0U));assert(ErrorDestroyed==(p.Error?1U:0U));assert(PathDestroyed==1);
  assert(RandomCalls==(p.Returns&&Codec==2?1U:0U));
  if(stream){
   assert((void*)stream==Object);CheckGuards();
   if(Codec==1){COggVorbisStream*o=(COggVorbisStream*)stream;assert(o->m_valid==(OggValid?1:0)&&o->m_opened==1&&ReaderConstructed==1&&ReaderDestroyed==0);}
   if(Codec==2){CTestToneAudioStream*t=(CTestToneAudioStream*)stream;assert(Frequency==(RandomValue?880.0f:440.0f)&&t->m_position==0&&t->m_sampleRate==44100);unsigned bits;memcpy(&bits,&t->m_scale,4);assert(bits==0x3d80634a);}
   Active=false;Disposing=true;delete stream;Disposing=false;assert(!Object&&ReaderDestroyed==ReaderConstructed&&StreamDestroyed==(Codec==1?1U:0U));
  }
  ++NormalCases;
 }
 Active=false;
}
int main(){
 const char* names[]={"","x",".ogg","TRACK.OGG","clip.wma","MIX.WMA","opaque.bin"};
 PKCBufferedReaderParams configs[]={PKCBufferedReaderParams(1,0,2),PKCBufferedReaderParams(2048,1,3),PKCBufferedReaderParams(8192,2,5)};
 const unsigned long randoms[]={0,32767};
 for(NameIndex=0;NameIndex<7;++NameIndex)for(unsigned tone=0;tone<2;++tone)for(unsigned fail=0;fail<2;++fail)for(HookEnabled=0;HookEnabled<2;++HookEnabled)for(RandomIndex=0;RandomIndex<2;++RandomIndex)for(ConfigIndex=0;ConfigIndex<3;++ConfigIndex)for(unsigned valid=0;valid<2;++valid){
  g_ForceTestTone=tone!=0;FailAllocation=fail!=0;OggValid=valid!=0;Params=&configs[ConfigIndex];RandomValue=randoms[RandomIndex];
  unsigned short wide[256];size_t n=strlen(names[NameIndex]);for(size_t j=0;j<=n;++j)wide[j]=(unsigned char)names[NameIndex][j];if(NameIndex==6)wide[0]=0x1234;
  for(size_t k=0;k<=n;++k)ExpectedPath[k]=wide[k]<=127?(char)wide[k]:'?';
  Codec=ExpectedCodec(ExpectedPath);sprintf(Message,".\\CDecompressorInterface.cpp(%ld) : EMEM\n",Codec==0?40L:45L);g_pfnAssertHook=HookEnabled?Report:0;
  Plan p=Oracle();Run(p,0,wide);
#ifdef TEST_UNWIND
  ExpectedFaultCases+=p.Boundaries;for(unsigned fault=1;fault<=p.Boundaries;++fault)Run(p,fault,wide);
#endif
 }
 assert(NormalCases==672);
#ifdef TEST_UNWIND
 assert(ThrowCases==ExpectedFaultCases&&ThrowCases==4104);
#endif
 puts("01216794_TEST PASS");return 0;
}