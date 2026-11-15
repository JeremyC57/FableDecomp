// Verbatim complete Close body; its two calls and actual self-destruction are observed.
#include <assert.h>
#include <stdio.h>
class PKCDefaultFileManager;class CDefaultAudioFile;
class CAudioFile {public:virtual bool GetSize(unsigned long*)=0;virtual bool Read(unsigned long,long,void*,void*,void*,void*)=0;virtual unsigned long GetUnitSize()=0;virtual void Close()=0;virtual ~CAudioFile();};
class CDefaultAudioFile:public CAudioFile {public:
 CDefaultAudioFile(PKCDefaultFileManager*,unsigned long);
 virtual bool GetSize(unsigned long*){return false;}
 virtual bool Read(unsigned long,long,void*,void*,void*,void*){return false;}
 virtual unsigned long GetUnitSize(){return 1;}
 virtual void Close();virtual ~CDefaultAudioFile();
private:PKCDefaultFileManager* m_pManager;unsigned long m_key;friend struct Fixture;
};
class PKCDefaultFileManager {public:bool ReleaseFile(unsigned long);void ReleaseFileObject(CAudioFile*);};
struct Fixture {static void Reassign(CDefaultAudioFile* file,PKCDefaultFileManager* manager){file->m_pManager=manager;file->m_key=123;}};
static CDefaultAudioFile* ExpectedFile;static PKCDefaultFileManager *FirstManager,*FinalManager;
static unsigned long ExpectedKey;static bool ReleaseResult,ReplaceManager;static int Step,DerivedDeaths,BaseDeaths;
CDefaultAudioFile::CDefaultAudioFile(PKCDefaultFileManager* manager,unsigned long key):m_pManager(manager),m_key(key){}
CDefaultAudioFile::~CDefaultAudioFile(){assert(this==ExpectedFile && Step==2);++DerivedDeaths;}
CAudioFile::~CAudioFile(){assert(Step==2 && DerivedDeaths==1);++BaseDeaths;}
bool PKCDefaultFileManager::ReleaseFile(unsigned long key){assert(Step==0 && this==FirstManager && key==ExpectedKey);Step=1;if(ReplaceManager)Fixture::Reassign(ExpectedFile,FinalManager);return ReleaseResult;}
void PKCDefaultFileManager::ReleaseFileObject(CAudioFile* file){assert(Step==1 && this==FinalManager && file==ExpectedFile);Step=2;delete file;}
void CDefaultAudioFile::Close()
{
    m_pManager->ReleaseFile(m_key);
    m_pManager->ReleaseFileObject(this);
}

int main(){
 PKCDefaultFileManager first,second;const unsigned long keys[]={0,1,0x7fffffffUL,0x80000000UL,0xffffffffUL};
 for(int k=0;k<5;++k)for(int result=0;result<2;++result)for(int change=0;change<2;++change){
  ExpectedKey=keys[k];ReleaseResult=result!=0;ReplaceManager=change!=0;FirstManager=&first;FinalManager=change?&second:&first;
  Step=DerivedDeaths=BaseDeaths=0;ExpectedFile=new CDefaultAudioFile(&first,ExpectedKey);
  CAudioFile* base=ExpectedFile;base->Close();assert(Step==2 && DerivedDeaths==1 && BaseDeaths==1);ExpectedFile=0;
 }
 puts("00c32fa0_TEST PASS");return 0;
}