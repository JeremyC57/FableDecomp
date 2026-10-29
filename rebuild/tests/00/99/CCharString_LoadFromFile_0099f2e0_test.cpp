#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define PASS9_MS_STRUCT __attribute__((ms_struct))
#else
#define PASS9_MS_STRUCT
#endif
// 0099f2e0 CCharString::LoadFromFile
// Flags:
// Leaf: ?LoadFromFile@CCharString@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z
#include <new>
#define CCharString Pass9CharStringStorage
#define CCharStringData Pass9CharStringDataStorage
#define CAFile Pass9FileStaticDeclaration
#include "fable_filesystem.h"
#undef CAFile
#undef CCharStringData
#undef CCharString
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#endif
class CASuspendableProcess;
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
enum FileAccessType{};
// Layout: actual RTTI CAFile primary CBaseClassNonCopyable4. Original virtual
// order from CDiskFileWin32 vtable0122d06c, slots1..9 match donor overrides.
class CAFile:public CBaseClassNonCopyable{public:
 virtual~CAFile();virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
 virtual void Close()=0;virtual void Read(void*,long,CASuspendableProcess*)=0;
 virtual void Write(const void*,long,CASuspendableProcess*)=0;
 virtual void SetPosition(long)=0;virtual void MovePosition(long)=0;
 virtual long GetPosition()const=0;virtual void SetLength(long)=0;virtual long GetLength()const=0;
};
// Layout: original CBasicString<char> is13 packed bytes and NoRefs is13.
#pragma pack(push,1)
template<class T>class PASS9_MS_STRUCT CBasicString{public:
 T*PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;
 __forceinline CBasicString(unsigned long n,T c):PData(0),StringLength(0),DataLength(0),UseFastExtend(true){SetString(n,c);}
protected:void SetString(unsigned long,T);
};
class CCharStringData{public:CBasicString<char>Data;long NoRefs;
 __forceinline CCharStringData(long n,char c):Data(n,c),NoRefs(1){}
};
#pragma pack(pop)
class CCharString{public:CCharStringData*PStringData;
 void LoadFromFile(CAFile&,CASuspendableProcess*,long);
protected:void UnassignString();
};
void CCharString::LoadFromFile(CAFile&file,CASuspendableProcess*process,long length){
 if(PStringData)UnassignString();
 if(length==-1)length=file.GetLength()-file.GetPosition();
 PStringData=new CCharStringData(length,' ');
 if(PStringData&&length>0)file.Read(PStringData->Data.PData,length,process);
}

static bool active,allocationFailure,bad;
static unsigned int allocCalls,unassignCalls,fillCalls,readCalls,getLengthCalls,getPositionCalls,events[16],eventCount;
static unsigned int expectedPattern;static long expectedLength;static CAFile*expectedFile;static CASuspendableProcess*expectedProcess;static CCharString*expectedOwner;
static void record(unsigned int event){if(eventCount>=16){bad=true;return;}events[eventCount++]=event;}
void*__cdecl operator new(size_t n)throw(std::bad_alloc){
 if(active){++allocCalls;record(4);if(n!=17)bad=true;if(allocationFailure)return 0;}
 void*p=malloc(n);if(p)memset(p,expectedPattern,n);return p;
}
void __cdecl operator delete(void*p)throw(){free(p);}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CAFile::~CAFile(){}
template<>void CBasicString<char>::SetString(unsigned long n,char c){
 if(active){++fillCalls;record(5);if(n!=(unsigned long)expectedLength||c!=' ')bad=true;}
 if(PData)free(PData);
 if(n>64){PData=0;StringLength=n;DataLength=0;return;}
 PData=(char*)malloc(n+1);if(!PData){bad=true;return;}memset(PData,c,n);PData[n]=0;StringLength=n;DataLength=n+1;
}
void CCharString::UnassignString(){
 if(active){++unassignCalls;record(1);if(this!=expectedOwner||!PStringData)bad=true;}
 if(PStringData&&--PStringData->NoRefs<=0){free(PStringData->Data.PData);free(PStringData);}PStringData=0;
}
class FixtureFile:public CAFile{public:long length,position;unsigned char contents[64];
 virtual void Open(const CWideString&,FileAccessType,unsigned long){bad=true;}virtual void Close(){bad=true;}
 virtual void Read(void*buffer,long n,CASuspendableProcess*process){++readCalls;record(6);if(this!=expectedFile||process!=expectedProcess||n!=expectedLength||!buffer||n<=0||n>64||!expectedOwner->PStringData||buffer!=expectedOwner->PStringData->Data.PData)bad=true;else memcpy(buffer,contents,n);}
 virtual void Write(const void*,long,CASuspendableProcess*){bad=true;}virtual void SetPosition(long){bad=true;}virtual void MovePosition(long){bad=true;}
 virtual long GetPosition()const{++getPositionCalls;record(3);if(this!=expectedFile)bad=true;return position;}
 virtual void SetLength(long){bad=true;}virtual long GetLength()const{++getLengthCalls;record(2);if(this!=expectedFile)bad=true;return length;}
};
int main(){
 if(sizeof(CBasicString<char>)!=13||sizeof(CCharStringData)!=17||sizeof(CCharString)!=4||sizeof(CAFile)!=4)return 1;
 unsigned int cases=0;const long lengths[7]={-3,-2,-1,0,1,4,17};unsigned char processStorage[32];
 for(unsigned int pattern=0;pattern<16;++pattern)for(unsigned int old=0;old<3;++old)for(unsigned int count=0;count<7;++count)for(unsigned int fileCase=0;fileCase<4;++fileCase)for(unsigned int fail=0;fail<2;++fail)for(unsigned int process=0;process<2;++process){
  expectedPattern=pattern*17;active=false;allocationFailure=fail!=0;bad=false;
  union Storage{double align;unsigned char bytes[36];}storage;memset(storage.bytes,expectedPattern,sizeof(storage.bytes));
  CCharString*owner=new(storage.bytes+16)CCharString;owner->PStringData=0;
  CCharStringData*previous=0;
  if(old){previous=(CCharStringData*)malloc(17);memset(previous,expectedPattern,17);new(previous)CCharStringData(3,' ');previous->NoRefs=old;owner->PStringData=previous;}
  FixtureFile file;file.length=5+fileCase*4;file.position=1+fileCase;
  for(unsigned int i=0;i<64;++i)file.contents[i]=(unsigned char)(i+pattern*3);
  unsigned char beforeContents[64];memcpy(beforeContents,file.contents,64);
  expectedLength=lengths[count]==-1?file.length-file.position:lengths[count];expectedFile=&file;expectedProcess=process?reinterpret_cast<CASuspendableProcess*>(processStorage):0;expectedOwner=owner;
  allocCalls=unassignCalls=fillCalls=readCalls=getLengthCalls=getPositionCalls=eventCount=0;active=true;
  owner->LoadFromFile(file,expectedProcess,lengths[count]);active=false;
  if(bad||allocCalls!=1||unassignCalls!=(old?1U:0U)||fillCalls!=(fail?0U:1U)||readCalls!=(!fail&&expectedLength>0?1U:0U)||getLengthCalls!=(lengths[count]==-1?1U:0U)||getPositionCalls!=(lengths[count]==-1?1U:0U))return 2;
  unsigned int e=0;if(old&&events[e++]!=1)return 3;if(lengths[count]==-1&&(events[e++]!=2||events[e++]!=3))return 4;if(events[e++]!=4)return 5;if(!fail&&events[e++]!=5)return 6;if(!fail&&expectedLength>0&&events[e++]!=6)return 7;if(e!=eventCount)return 8;
  if(fail){if(owner->PStringData)return 9;}else{
   CCharStringData*data=owner->PStringData;if(!data||data->NoRefs!=1||data->Data.StringLength!=(unsigned long)expectedLength||!data->Data.UseFastExtend)return 10;
   if(expectedLength>=0){if(!data->Data.PData||data->Data.PData[expectedLength]!=0||memcmp(data->Data.PData,file.contents,expectedLength))return 11;}
   else if(data->Data.PData)return 12;
   free(data->Data.PData);free(data);
  }
  if(old==2){if(previous->NoRefs!=1||previous->Data.StringLength!=3||memcmp(previous->Data.PData,"   ",4))return 13;free(previous->Data.PData);free(previous);}
  if(memcmp(beforeContents,file.contents,64)||file.length!=5+(long)fileCase*4||file.position!=1+(long)fileCase)return 14;
  for(unsigned int i=0;i<16;++i)if(storage.bytes[i]!=expectedPattern||storage.bytes[20+i]!=expectedPattern)return 15;
  ++cases;
 }
 if(cases!=5376)return 16;puts("0099f2e0_TEST PASS");return 0;
}