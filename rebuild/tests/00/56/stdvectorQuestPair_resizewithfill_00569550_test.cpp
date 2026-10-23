// 00569550 N00569550::vector<Element>::_M_fill_insert (STLport; catalog: vector_QuestPair_::std::vector_QuestPair_::resize_with_fill)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: oracle element stride 16 bytes and local copy/destruction call offsets; uncertain element identity.
namespace N00569550 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* _M_start; unsigned int* _M_finish; unsigned int* _M_end_of_storage;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(_M_start) free(_M_start); }
};
struct CCharStringData;
struct CCharString {CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element { CCharString A,B,C; unsigned char Flag,Flag2; };
void _Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x,const __false_type&) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> struct vector {
 T* _M_start; T* _M_finish; T* _M_end_of_storage;
 void _M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend=false);
 void _M_fill_insert(T* pos,unsigned int n,const T& x);
};
template<class T> void vector<T>::_M_fill_insert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(_M_end_of_storage-_M_finish)>=n) {
   T x_copy=x;
   const unsigned int after=_M_finish-pos;
   T* old_finish=_M_finish;
   if(after>n) {
    __uninitialized_copy(_M_finish-n,_M_finish,_M_finish,__false_type());
    _M_finish+=n;
    __copy_backward_ptrs(pos,old_finish-n,old_finish,__false_type());
    fill(pos,pos+n,x_copy);
   } else {
    __uninitialized_fill_n(_M_finish,n-after,x_copy,__false_type());
    _M_finish+=n-after;
    __uninitialized_copy(pos,old_finish,_M_finish,__false_type());
    _M_finish+=after;
    fill(pos,old_finish,x_copy);
   }
  } else _M_insert_overflow(pos,x,__false_type(),n);
 }
}
template void vector<Element>::_M_fill_insert(Element*,unsigned int,const Element&);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
namespace N00569550 {

#pragma pack(push,1)
struct CCharStringData {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& o):PStringData(o.PStringData){if(PStringData)++PStringData->Refs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}}
CCharString& CCharString::operator=(const CCharString& o){if(PStringData!=o.PStringData){if(o.PStringData)++o.PStringData->Refs;if(PStringData&&--PStringData->Refs==0){delete PStringData;--liveStringBlocks;++destroyedStringBlocks;}PStringData=o.PStringData;}return *this;}
void SetString(CCharString& s,unsigned int value){CCharString t;t.PStringData=new CCharStringData;std::memset(t.PStringData,0,sizeof *t.PStringData);t.PStringData->Value=value;t.PStringData->Refs=1;++liveStringBlocks;s=t;}

static int overflowCalls;
void _Construct(Element* p,const Element& o){new(p)Element(o);}
template<class T>void vector<T>::_M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool){++overflowCalls;}
void Init(Element& e,unsigned int n){SetString(e.A,n);SetString(e.B,n+100);SetString(e.C,n+200);e.Flag=(unsigned char)(n&1);e.Flag2=(unsigned char)((n+1)&1);}
unsigned int Key(const Element& e){return e.A.PStringData->Value;}
bool Payload(const Element& e,unsigned int n){return e.A.PStringData->Value==n&&e.B.PStringData->Value==n+100&&e.C.PStringData->Value==n+200&&e.Flag==(n&1)&&e.Flag2==((n+1)&1);}
}


namespace N00569550 {
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

int main(){using namespace N00569550;if(!Cross255())return 100;{Element* a=(Element*)std::malloc(12*sizeof(Element));for(int i=0;i<5;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);vector<Element> v={a,a+5,a+12};v._M_fill_insert(a+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(!Payload(a[j],want[j]))return 1;if(a[1].A.PStringData!=x.A.PStringData||x.A.PStringData->Refs!=3)return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();Init(a[q],q+1);}v._M_finish=a+3;v._M_fill_insert(a+2,4,x);if(v._M_finish-a!=7||!Payload(a[6],3)||x.A.PStringData->Refs!=5)return 3;v._M_fill_insert(a+2,1,a[0]);if(v._M_finish-a!=8||!Payload(a[2],1)||a[0].A.PStringData->Refs!=2)return 4;v._M_fill_insert(a,0,x);v._M_end_of_storage=v._M_finish;v._M_fill_insert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);}if(liveStringBlocks!=0)return 6;std::printf("00569550_TEST PASS\n");return 0;}