#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <new>
static void check(bool b){if(!b)std::abort();}
class CCharString{ public:std::string value;CCharString(){}CCharString(const char* s):value(s){}const CCharString& operator=(const char* s){value=s;return *this;}bool operator<(const CCharString& x)const{return value<x.value;}};
struct _RTL_CRITICAL_SECTION{int state;};
class CFileInstaller;
static CFileInstaller* current;static int eventCount,threadCount,failureMask,seed;static std::string trace;static int handles[4];
void InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
void* CreateEventW(void*,int,int,const unsigned short*);
unsigned int _beginthread(void (*)(void*),unsigned int,void*);
class CFileInstaller{ public:
 class CBufferChunk{ public:};class CFileInfo{ public:};class CActiveFile{ public:};class CActiveRead{ public:};
 _RTL_CRITICAL_SECTION CriticalSection;std::vector<CBufferChunk> BufferChunks;
 unsigned char* PReadBuffer;unsigned char* PWriteBuffer;long AvailableBuffers,AllocatedInputBuffers;
 std::list<CFileInfo*>FilesToCopy,FilesToFinish;std::list<CActiveFile>ActiveFiles;std::set<CCharString>CompletedFiles;
 CActiveFile*PCurrentReadFile;CActiveFile*PCurrentWriteFile;long NoActiveReads,NoActiveWrites;bool WriteMode;
 unsigned long BufferChunkSize,NoBufferChunks;long NoTotalBytesWritten,TotalSizeOfAllFiles;
 CActiveRead* PCompletedRead;bool ThreadFinished,DecompressionThreadFinished,ExitThread,Running;unsigned long Paused;bool Completed,IgnoreTimeStampWhenCopying;
 void*Event;void*DecompressionEvent;void*PauseStartEvent;void*PauseFinishEvent;std::map<CCharString,CCharString>CompressedFileTypes;
 CFileInstaller();static void BeginThread(void*){}static void BeginDecompressionThread(void*){}
};
void InitializeCriticalSection(_RTL_CRITICAL_SECTION* p){check(trace.empty()&&p==&current->CriticalSection);check(current->BufferChunks.empty()&&current->FilesToCopy.empty()&&current->FilesToFinish.empty()&&current->ActiveFiles.empty()&&current->CompletedFiles.empty()&&current->CompressedFileTypes.empty());p->state=seed+1;trace+='I';}
void* CreateEventW(void* a,int manual,int initial,const unsigned short* name){check(!a&&!manual&&!initial&&!name&&eventCount<4);check(current->CriticalSection.state==seed+1&&!current->ThreadFinished&&!current->DecompressionThreadFinished&&!current->ExitThread&&!current->Running&&current->Completed&&current->Paused==0);trace+='E';int i=eventCount++;handles[i]=seed*4+i;return (failureMask&(1<<i))?0:&handles[i];}
static void checkFields(){
 check(!current->PCompletedRead&&!current->PCurrentReadFile&&!current->PCurrentWriteFile&&!current->PReadBuffer&&!current->PWriteBuffer);
 check(!current->BufferChunkSize&&!current->NoBufferChunks&&!current->AvailableBuffers&&!current->AllocatedInputBuffers&&!current->NoTotalBytesWritten&&!current->TotalSizeOfAllFiles&&!current->NoActiveReads&&!current->NoActiveWrites);
 check(!current->ThreadFinished&&!current->DecompressionThreadFinished&&!current->ExitThread&&!current->Running&&current->Completed&&!current->Paused&&!current->WriteMode&&!current->IgnoreTimeStampWhenCopying);
 void* out[]={current->Event,current->DecompressionEvent,current->PauseStartEvent,current->PauseFinishEvent};
 for(int i=0;i<4;++i)check(out[i]==((failureMask&(1<<i))?0:&handles[i]));
 check(current->CompressedFileTypes.size()==1&&current->CompressedFileTypes.find(CCharString(".biz"))->second.value==".big");
}
unsigned int _beginthread(void (*f)(void*),unsigned int stack,void* arg){check(eventCount==4&&threadCount<2&&stack==0x3000&&arg==current);checkFields();check(f==(threadCount==0?CFileInstaller::BeginThread:CFileInstaller::BeginDecompressionThread));trace+=threadCount==0?'R':'D';++threadCount;return (seed&1)?0xffffffffU:(unsigned int)(seed+threadCount);}
CFileInstaller::CFileInstaller()
{
    InitializeCriticalSection(&CriticalSection);
    DecompressionThreadFinished = false;
    ThreadFinished = false;
    ExitThread = false;
    Running = false;
    Completed = true;
    Paused = 0;
    Event = CreateEventW(0, 0, 0, 0);
    DecompressionEvent = CreateEventW(0, 0, 0, 0);
    PauseStartEvent = CreateEventW(0, 0, 0, 0);
    PauseFinishEvent = CreateEventW(0, 0, 0, 0);
    PCompletedRead = 0;
    ThreadFinished = false;
    PCurrentReadFile = 0;
    PCurrentWriteFile = 0;
    PReadBuffer = 0;
    PWriteBuffer = 0;
    BufferChunkSize = 0;
    NoBufferChunks = 0;
    AvailableBuffers = 0;
    AllocatedInputBuffers = 0;
    NoTotalBytesWritten = 0;
    TotalSizeOfAllFiles = 0;
    ExitThread = false;
    NoActiveReads = 0;
    NoActiveWrites = 0;
    WriteMode = false;
    IgnoreTimeStampWhenCopying = false;
    {
        CCharString extension(".biz");
        CompressedFileTypes[extension] = ".big";
    }
    _beginthread(BeginThread, 0x3000, this);
    _beginthread(BeginDecompressionThread, 0x3000, this);
}

int main(){int cases=0;for(seed=0;seed<8;++seed)for(failureMask=0;failureMask<16;++failureMask){
 void* storage=std::malloc(sizeof(CFileInstaller));check(storage!=0);std::memset(storage,0xa5,sizeof(CFileInstaller));current=static_cast<CFileInstaller*>(storage);eventCount=threadCount=0;trace="";
 CFileInstaller* made=new(storage)CFileInstaller;check(made==current&&eventCount==4&&threadCount==2&&trace=="IEEEERD");checkFields();
 check(current->BufferChunks.empty()&&current->FilesToCopy.empty()&&current->FilesToFinish.empty()&&current->ActiveFiles.empty()&&current->CompletedFiles.empty());
 made->~CFileInstaller();std::free(storage);++cases;
 }check(cases==128);std::puts("009c5890_TEST PASS");return 0;}