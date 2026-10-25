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