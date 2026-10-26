// 00433530 vector<CCharString>::_M_insert_overflow (catalog: std::vector::Reallocate)
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct __false_type { __false_type() {} };
class CCharStringData;class CCharString { public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
__forceinline void CopyConstruct(CCharString* p,const CCharString& x) { new(p) CCharString(x); }
void* __cdecl Place(unsigned int,void*);
__forceinline void Construct(CCharString* p,const CCharString& x) { if(p) CopyConstruct(p,x); }
__forceinline void Destroy(CCharString* p) {p->~CCharString();}

__forceinline CCharString* __fastcall UninitCopy(CCharString* first,CCharString* last,CCharString* out,const __false_type&) {for(;first!=last;++first,++out) Construct(out,*first);return out;}
__forceinline CCharString* __fastcall UninitFill(CCharString* out,unsigned int n,const CCharString& x,const __false_type&) {for(;n>0;--n,++out) Construct(out,x);return out;}

template<class T> __forceinline void DestroyStorage(T* first,T* last){for(;first!=last;++first)Destroy(first);}
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool); };
template<class T> __forceinline const T& Max(const T& a,const T& b) {return a<b?b:a;}
template<class T> void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend) {
 const unsigned int old_size=_M_finish-_M_start;
 const unsigned int len=old_size+Max(old_size,n);
 T* new_start=len ? (T*)malloc(len*sizeof(T)) : 0;
 T* new_finish=UninitCopy(_M_start,pos,new_start,__false_type());
 if(n==1) { Construct(new_finish,x); ++new_finish; }
 else new_finish=UninitFill(new_finish,n,x,__false_type());
 if(!atend) new_finish=UninitCopy(pos,_M_finish,new_finish,__false_type());
 DestroyStorage(_M_start,_M_finish);
 if(_M_start) free(_M_start);
 _M_finish=new_finish; _M_start=new_start; _M_end_of_storage=new_start+len;
}
template void vector<CCharString>::_M_insert_overflow(CCharString*,const CCharString&,const __false_type&,unsigned int,bool);


#include <cstdio>
#include <cstring>
#include <cstdlib>
extern "C++" {

#pragma pack(push,1)
struct CCharStringData {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& o):PStringData(o.PStringData){if(PStringData)++PStringData->Refs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}}
CCharString& CCharString::operator=(const CCharString& o){if(PStringData!=o.PStringData){if(o.PStringData)++o.PStringData->Refs;if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}PStringData=o.PStringData;}return *this;}
void SetElement(CCharString& s,unsigned int value){CCharString t;t.PStringData=new CCharStringData;std::memset(t.PStringData,0,sizeof *t.PStringData);t.PStringData->Value=value;t.PStringData->Refs=1;++liveStringBlocks;s=t;}

}


extern "C++" {
bool Cross255(){
 if(sizeof(CCharStringData)!=17)return false;
 CCharStringData probe;if((unsigned int)((char*)&probe.Refs-(char*)&probe)!=13)return false;
 int initialLive=liveStringBlocks,initialDestroyed=destroyedStringBlocks;
 { CCharString root;SetElement(root,123);CCharString* copies=(CCharString*)::operator new(300*sizeof(CCharString));
 for(int i=0;i<300;++i)new(copies+i)CCharString(root);
 if(root.PStringData->Refs!=301||liveStringBlocks!=initialLive+1)return false;
 copies[0]=root;if(root.PStringData->Refs!=301)return false;
 for(int j=0;j<45;++j)copies[j].~CCharString();
 if(root.PStringData->Refs!=256||liveStringBlocks!=initialLive+1)return false;
 copies[45].~CCharString();if(root.PStringData->Refs!=255||liveStringBlocks!=initialLive+1)return false;
 for(int k=46;k<300;++k)copies[k].~CCharString();
 if(root.PStringData->Refs!=1||liveStringBlocks!=initialLive+1)return false;
 ::operator delete(copies);
 }
 return liveStringBlocks==initialLive&&destroyedStringBlocks==initialDestroyed+1;
}
}

int main(){if(!Cross255())return 100;{CCharString* a=(CCharString*)std::malloc(5*sizeof(CCharString));for(int i=0;i<4;++i){new(a+i)CCharString();SetElement(a[i],i+1);}CCharString x;SetElement(x,99);vector<CCharString> v={a,a+4,a+5};v._M_insert_overflow(a+2,x,__false_type(),2,false);unsigned int want[]={1,2,99,99,3,4};if(v._M_finish-v._M_start!=6||v._M_end_of_storage-v._M_start!=8)return 1;for(int j=0;j<6;++j)if(v._M_start[j].PStringData->Value!=want[j])return 2;if(x.PStringData->Refs!=3)return 3;v._M_insert_overflow(v._M_finish,x,__false_type(),1,true);if(v._M_finish-v._M_start!=7||v._M_start[6].PStringData!=x.PStringData||x.PStringData->Refs!=4)return 4;for(CCharString* p=v._M_start;p!=v._M_finish;++p)p->~CCharString();std::free(v._M_start);}if(liveStringBlocks!=0)return 5;std::printf("00433530_TEST PASS\n");return 0;}