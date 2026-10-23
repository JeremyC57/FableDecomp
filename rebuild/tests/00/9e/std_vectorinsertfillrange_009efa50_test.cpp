// 009efa50 N009efa50::vector<Element>::_M_insert_overflow (STLport; catalog: vector_insert_fill_range)
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>
namespace N009efa50 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct __false_type { __false_type() {} };
struct CCharStringData;struct CCharString {CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element {Element(){}virtual void dtor(unsigned int);unsigned int W0;CCharString S;unsigned int A,B;unsigned char Flag;unsigned int C;__forceinline Element(const Element& o):W0(o.W0),S(o.S),A(o.A),B(o.B),Flag(o.Flag),C(o.C){}};
void* __cdecl Place(unsigned int,void*);
__forceinline void _Construct(Element* p,const Element& x) {if(p) new(p) Element(x);}
__forceinline void _Destroy(Element* p) {p->dtor(0);}
#pragma auto_inline(off)

__declspec(noinline) Element* __fastcall __uninitialized_copy(Element* first,Element* last,Element* out,const __false_type&) {for(;first!=last;++first,++out) _Construct(out,*first);return out;}
__declspec(noinline) Element* __fastcall __uninitialized_fill_n(Element* out,unsigned int n,const Element& x,const __false_type&) {for(;n>0;--n,++out) _Construct(out,x);return out;}

#pragma auto_inline(on)
template<class T> __forceinline void DestroyStorage(T* first,T* last){for(;first!=last;++first)_Destroy(first);}
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool); };
template<class T> __forceinline const T& Max(const T& a,const T& b) {return a<b?b:a;}
template<class T> void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend) {
 const unsigned int old_size=_M_finish-_M_start;
 const unsigned int len=old_size+Max(old_size,n);
 T* new_start=len ? (T*)malloc(len*sizeof(T)) : 0;
 T* new_finish=__uninitialized_copy(_M_start,pos,new_start,__false_type());
 if(n==1) { _Construct(new_finish,x); ++new_finish; }
 else new_finish=__uninitialized_fill_n(new_finish,n,x,__false_type());
 if(!atend) new_finish=__uninitialized_copy(pos,_M_finish,new_finish,__false_type());
 DestroyStorage(_M_start,_M_finish);
 if(_M_start) free(_M_start);
 _M_finish=new_finish; _M_start=new_start; _M_end_of_storage=new_start+len;
}
template void vector<Element>::_M_insert_overflow(Element*,const Element&,const __false_type&,unsigned int,bool);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace N009efa50 {

#pragma pack(push,1)
struct CCharStringData {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& o):PStringData(o.PStringData){if(PStringData)++PStringData->Refs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}}
CCharString& CCharString::operator=(const CCharString& o){if(PStringData!=o.PStringData){if(o.PStringData)++o.PStringData->Refs;if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}PStringData=o.PStringData;}return *this;}
void SetString(CCharString& s,unsigned int value){CCharString t;t.PStringData=new CCharStringData;std::memset(t.PStringData,0,sizeof *t.PStringData);t.PStringData->Value=value;t.PStringData->Refs=1;++liveStringBlocks;s=t;}

void Element::dtor(unsigned int flags){if(flags&1)delete this;else this->~Element();}
void Init(Element& e,unsigned int n){e.W0=n;e.A=n+100;e.B=n+200;e.C=n+300;e.Flag=(unsigned char)(n&1);SetString(e.S,n);}
}

namespace N009efa50 {
bool Cross255(){
 if(sizeof(CCharStringData)!=17)return false;
 CCharStringData probe;if((unsigned int)((char*)&probe.Refs-(char*)&probe)!=13)return false;
 int initialLive=liveStringBlocks,initialDestroyed=destroyedStringBlocks;
 { CCharString root;SetString(root,123);CCharString* copies=(CCharString*)::operator new(300*sizeof(CCharString));
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

int main(){using namespace N009efa50;if(!Cross255())return 100;{Element* a=(Element*)std::malloc(5*sizeof(Element));for(int i=0;i<4;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);vector<Element> v={a,a+4,a+5};v._M_insert_overflow(a+2,x,__false_type(),2,false);unsigned int want[]={1,2,99,99,3,4};if(v._M_finish-v._M_start!=6||v._M_end_of_storage-v._M_start!=8)return 1;for(int j=0;j<6;++j)if(v._M_start[j].W0!=want[j]||v._M_start[j].A!=want[j]+100||v._M_start[j].S.PStringData->Value!=want[j])return 2;if(x.S.PStringData->Refs!=3)return 3;v._M_insert_overflow(v._M_finish,x,__false_type(),1,true);if(v._M_finish-v._M_start!=7||v._M_start[6].S.PStringData!=x.S.PStringData||x.S.PStringData->Refs!=4)return 4;for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();std::free(v._M_start);}if(liveStringBlocks!=0)return 5;std::printf("009efa50_TEST PASS\n");return 0;}