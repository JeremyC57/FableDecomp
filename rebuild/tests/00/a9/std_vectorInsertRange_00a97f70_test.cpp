// 00a97f70 N00a97f70::vector<Element>::_M_fill_insert (STLport; catalog: vector_InsertRange)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: counted pointer data/control at0/4 and 8-byte stride; a95890 uses ecx/edx plus one stack input and ret4. Tagged copy helpers remain distinct.
#include <new>
namespace N00a97f70 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline void Release() { RefBlock* block=Block;if(block) {if(--block->Refs==0) {block->Destroy(block->Data); ::operator delete(block);}} }
 __forceinline Counted& operator=(const Counted& o) {T* data=o.Data; RefBlock* block=o.Block; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){T* data=o.Data;RefBlock* block=o.Block;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
struct String {unsigned int Key; String();String(const String&);~String();String& operator=(const String&);bool FastCompare(const String&) const;};
struct WideString {unsigned int Key; WideString();WideString(const WideString&);~WideString();WideString& operator=(const WideString&);};
struct Element {Counted<void> P;};
__forceinline void _Construct(Element* p,const Element& x){if(p)new(p)Element(x);}
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) _Construct(first,x); return first; }
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
    __uninitialized_fill_n(_M_finish,n-after,x_copy);
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
#include <cstdlib>
namespace N00a97f70 {
void Base::v00(){} Base::~Base(){}
static int destroyed,liveControls,overflowCalls;
void __fastcall Dispose(void* p){delete (int*)p;++destroyed;--liveControls;}
void SetPointer(Element& e,int id){e.P.Data=new int(id);e.P.Block=new RefBlock;e.P.Block->Refs=1;e.P.Block->Destroy=Dispose;e.P.Block->Data=e.P.Data;++liveControls;}

template<class T>void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool){
 ++overflowCalls;T copy(x);unsigned int oldSize=(unsigned int)(_M_finish-_M_start),offset=(unsigned int)(pos-_M_start);unsigned int extra=oldSize>n?oldSize:n;
 T* fresh=(T*)std::malloc((oldSize+extra)*sizeof(T));T* end=fresh;T* p;
 for(p=_M_start;p!=pos;++p,++end)new(end)T(*p);
 for(unsigned int i=0;i<n;++i,++end)new(end)T(copy);
 for(p=pos;p!=_M_finish;++p,++end)new(end)T(*p);
 for(p=_M_start;p!=_M_finish;++p)p->~T();std::free(_M_start);_M_start=fresh;_M_finish=end;_M_end_of_storage=fresh+oldSize+extra;
 if((unsigned int)(_M_finish-_M_start)!=oldSize+n||offset>oldSize)++overflowCalls;
}
struct Owner {vector<Element> V;Owner(){V._M_start=(Element*)std::malloc(7*sizeof(Element));V._M_finish=V._M_start;V._M_end_of_storage=V._M_start+7;}~Owner(){for(Element* p=V._M_start;p!=V._M_finish;++p)p->~Element();std::free(V._M_start);}};
int ID(const Element& e){return *(int*)e.P.Data;}
}
int main(){using namespace N00a97f70;
 {Counted<void> first,second;first.Data=new int(41);first.Block=new RefBlock;first.Block->Refs=1;first.Block->Destroy=Dispose;first.Block->Data=first.Data;++liveControls;
 second.Data=new int(42);second.Block=new RefBlock;second.Block->Refs=1;second.Block->Destroy=Dispose;second.Block->Data=second.Data;++liveControls;
 first=second;if(destroyed!=1||second.Block->Refs!=2)return 1;first=first;second=first;if(second.Block->Refs!=2)return 2;}
 if(destroyed!=2||liveControls!=0)return 3;
 {Element probe;SetPointer(probe,77);_Construct(0,probe);if(probe.P.Block->Refs!=1||liveControls!=1||destroyed!=2)return 11;Element* copy=(Element*)::operator new(sizeof(Element));_Construct(copy,probe);if(copy->P.Data!=probe.P.Data||copy->P.Block!=probe.P.Block||probe.P.Block->Refs!=2)return 12;copy->~Element();::operator delete(copy);if(probe.P.Block->Refs!=1||destroyed!=2)return 13;}
 if(destroyed!=3||liveControls)return 14;
 {Owner owner;vector<Element>& v=owner.V;for(int i=0;i<3;++i){new(v._M_finish)Element;SetPointer(*v._M_finish,i+1);++v._M_finish;}RefBlock* shared=v._M_start[1].P.Block;
 v._M_fill_insert(v._M_start,1,v._M_start[1]);if(v._M_finish-v._M_start!=4||shared->Refs!=2||ID(v._M_start[0])!=2||ID(v._M_start[1])!=1||ID(v._M_start[2])!=2||ID(v._M_start[3])!=3)return 4;
 v._M_fill_insert(v._M_start+3,3,v._M_start[0]);if(v._M_finish-v._M_start!=7||shared->Refs!=5||ID(v._M_start[6])!=3)return 5;
 for(int i=2;i<6;++i)if(ID(v._M_start[i])!=2)return 6;
 v._M_fill_insert(v._M_start,0,v._M_start[0]);if(shared->Refs!=5||overflowCalls!=0)return 7;
 v._M_fill_insert(v._M_start+2,2,v._M_start[0]);if(v._M_finish-v._M_start!=9||shared->Refs!=7||overflowCalls!=1||ID(v._M_start[1])!=1||ID(v._M_start[8])!=3)return 8;
 for(int i=2;i<8;++i)if(ID(v._M_start[i])!=2)return 9;
 }
 if(destroyed!=6||liveControls!=0)return 10;
 std::printf("00a97f70_TEST PASS\n");return 0;}