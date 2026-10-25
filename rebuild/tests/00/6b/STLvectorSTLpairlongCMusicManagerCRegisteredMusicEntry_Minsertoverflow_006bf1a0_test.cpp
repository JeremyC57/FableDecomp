// 006bf1a0 std::vector<std::pair<long,CMusicManager::CRegisteredMusicEntry>>::_M_insert_overflow
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
// Layout: retail CWideStringData uses text/start+0,finish+4,storage+8 and signed refs+12,total16; copy0099b758 and release0099b4d9 prove refs+12.
#pragma pack(push,1)
struct CWideStringData{unsigned short*PData,*Finish,*Storage;int RefCount;};
#pragma pack(pop)
struct CWideString{CWideStringData*PData;CWideString();CWideString(const CWideString&);~CWideString();};
// Layout: PDB CRegisteredMusicEntry has SrcFileName+0,DstFileName+4,Gain+8,Offset+12,CacheToHD+16, total20; pair stride24.
struct CMusicManager{struct CRegisteredMusicEntry{CWideString SrcFileName,DstFileName;float Gain,Offset;bool CacheToHD;};};
namespace std{
struct __false_type{__false_type(){}};
template<class A,class B>struct pair{A first;B second;pair(){}__forceinline pair(const pair&x):first(x.first),second(x.second){}};
typedef pair<long,CMusicManager::CRegisteredMusicEntry> MusicEntry;
template<class Tag>__declspec(noinline)MusicEntry*__fastcall uninitialized_copy(MusicEntry*a,MusicEntry*b,MusicEntry*c,const Tag&){while(a!=b){if(c)new(c)MusicEntry(*a);++a;++c;}return c;}
template<class Tag>__declspec(noinline)MusicEntry*__fastcall uninitialized_fill_n(MusicEntry*c,unsigned n,const MusicEntry&v,const Tag&){while(n--){if(c)new(c)MusicEntry(v);++c;}return c;}
template<class T>const T&max(const T&a,const T&b){return a<b?b:a;}
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);};
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&v,const __false_type&,unsigned n,bool atEnd){
 unsigned old=_M_finish-_M_start;unsigned capacity=old+max(old,n);
 T*buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T*out=uninitialized_copy(_M_start,pos,buffer,__false_type());
 if(n==1){if(out)new(out)T(v);++out;}
 else out=uninitialized_fill_n(out,n,v,__false_type());
 if(!atEnd)out=uninitialized_copy(pos,_M_finish,out,__false_type());
 _M_clear();_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
}
template struct vector<MusicEntry>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
static int allocations;
CWideString::CWideString():PData(0){}
CWideString::CWideString(const CWideString&x):PData(x.PData){if(PData)++PData->RefCount;}
CWideString::~CWideString(){if(PData&&--PData->RefCount<=0){free(PData->PData);delete PData;--allocations;}}
static void text(CWideString&s,unsigned short ch){s.PData=new CWideStringData;s.PData->RefCount=1;s.PData->PData=(unsigned short*)malloc(2*sizeof(unsigned short));s.PData->Finish=s.PData->PData+1;s.PData->Storage=s.PData->PData+2;s.PData->PData[0]=ch;s.PData->PData[1]=0;++allocations;}
namespace std{


template<>void vector<MusicEntry>::_M_clear(){for(MusicEntry*p=_M_start;p!=_M_finish;++p)p->~MusicEntry();free(_M_start);}
}
int main(){using namespace std;assert(sizeof(MusicEntry)==24&&sizeof(CWideStringData)==16);{MusicEntry seed;seed.first=42;seed.second.Gain=.75f;seed.second.Offset=3;seed.second.CacheToHD=true;text(seed.second.SrcFileName,65);text(seed.second.DstFileName,90);vector<MusicEntry>v;memset(&v,0,sizeof v);__false_type tag;v._M_insert_overflow(v._M_start,seed,tag,1,false);assert(v._M_finish-v._M_start==1&&seed.second.SrcFileName.PData->RefCount==2);v._M_insert_overflow(v._M_start,seed,tag,2,false);assert(v._M_finish-v._M_start==3&&v._M_start[2].second.Offset==3&&seed.second.DstFileName.PData->RefCount==4);v._M_insert_overflow(v._M_finish,seed,tag,0,true);assert(v._M_finish-v._M_start==3&&v._M_end_of_storage-v._M_start==6);v._M_clear();assert(seed.second.SrcFileName.PData->RefCount==1);}assert(!allocations);puts("006bf1a0_TEST PASS");}