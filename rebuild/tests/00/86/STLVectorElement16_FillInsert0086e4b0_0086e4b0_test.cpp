#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 919f9e8bd9ff7c4cd73be9ae3710179763ed413f9ca1b76adbe66c0f3827a565
// 0086e4b0 STL::VectorElement16::FillInsert_0086e4b0
// Flags: /O2 /Oy /Gr
// Leaf: FillInsert
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void*p){return p;}
struct Tag_0086e4b0{Tag_0086e4b0(){}};
// Layout: element16 has nested vector12 and byte+13; copy ctor00673e10, destructor frees member+0.
struct Inner_0086e4b0{int* start;int* finish;int* end;Inner_0086e4b0():start(0),finish(0),end(0){}Inner_0086e4b0(const Inner_0086e4b0&);~Inner_0086e4b0(){if(start)free(start);}Inner_0086e4b0& operator=(const Inner_0086e4b0&);};
struct Element_0086e4b0{Inner_0086e4b0 vector;char unused;char flag;Element_0086e4b0(){}__forceinline Element_0086e4b0(const Element_0086e4b0&o):vector(o.vector),flag(o.flag){}__forceinline Element_0086e4b0& operator=(const Element_0086e4b0&o){vector=o.vector;flag=o.flag;return *this;}};
// Layout:12-byte vector; retail fill-insertion receives(position,count,const value&).
template<class T>struct Vector_0086e4b0{T* start;T* finish;T* end;void Grow(T*,const T&,const Tag_0086e4b0&,unsigned int,bool);void FillInsert(T*,unsigned int,const T&);};
Element_0086e4b0* __fastcall UninitCopy(Element_0086e4b0*,Element_0086e4b0*,Element_0086e4b0*,const Tag_0086e4b0&);
Element_0086e4b0* __fastcall UninitFill(Element_0086e4b0*,unsigned int,const Element_0086e4b0&);
Element_0086e4b0* __fastcall CopyBackward(Element_0086e4b0*,Element_0086e4b0*,Element_0086e4b0*,const Tag_0086e4b0&,int*);
void __fastcall Fill(Element_0086e4b0*,Element_0086e4b0*,const Element_0086e4b0&);
#pragma auto_inline(off)
__declspec(noinline) Element_0086e4b0* __fastcall UninitCopy(Element_0086e4b0* first,Element_0086e4b0* last,Element_0086e4b0*out,const Tag_0086e4b0&){for(;first!=last;++first,++out)new(out)Element_0086e4b0(*first);return out;}
__declspec(noinline) Element_0086e4b0* __fastcall UninitFill(Element_0086e4b0*out,unsigned int count,const Element_0086e4b0&value){for(;count;--count,++out)new(out)Element_0086e4b0(value);return out;}
__declspec(noinline) Element_0086e4b0* __fastcall CopyBackward(Element_0086e4b0* first,Element_0086e4b0* last,Element_0086e4b0*out,const Tag_0086e4b0&,int*){while(first!=last)*--out=*--last;return out;}
__declspec(noinline) void __fastcall Fill(Element_0086e4b0*first,Element_0086e4b0*last,const Element_0086e4b0&value){for(;first!=last;++first)*first=value;}
#pragma auto_inline(on)
template<class T>void Vector_0086e4b0<T>::FillInsert(T* position,unsigned int count,const T& value){
 if(count){
  if((unsigned int)(end-finish)>=count){
   T copy=value;unsigned int after=finish-position;T* oldFinish=finish;
   if(after>count){UninitCopy(finish-count,finish,finish,Tag_0086e4b0());finish+=count;CopyBackward(position,oldFinish-count,oldFinish,Tag_0086e4b0(),(int*)0);Fill(position,position+count,copy);}
   else{UninitFill(finish,count-after,copy);finish+=count-after;UninitCopy(position,oldFinish,finish,Tag_0086e4b0());finish+=after;Fill(position,oldFinish,copy);}
  }else Grow(position,value,Tag_0086e4b0(),count,false);
 }
}
template void Vector_0086e4b0<Element_0086e4b0>::FillInsert(Element_0086e4b0*,unsigned int,const Element_0086e4b0&);

// END exact source snapshot
Inner_0086e4b0::Inner_0086e4b0(const Inner_0086e4b0& o){int n=o.finish-o.start;start=n?(int*)malloc(n*4):0;finish=start+n;end=finish;for(int i=0;i<n;++i)start[i]=o.start[i];}
template<class T>void Vector_0086e4b0<T>::Grow(T* position,const T&value,const Tag_0086e4b0&,unsigned int count,bool){unsigned int old=finish-start;unsigned int length=old+(old>count?old:count);T*next=(T*)malloc(length*sizeof(T));T*out=next;out=UninitCopy(start,position,out,Tag_0086e4b0());out=UninitFill(out,count,value);out=UninitCopy(position,finish,out,Tag_0086e4b0());for(T*p=start;p!=finish;++p)p->~T();if(start)free(start);start=next;finish=out;end=next+length;}

Inner_0086e4b0& Inner_0086e4b0::operator=(const Inner_0086e4b0&o){if(this!=&o){int n=o.finish-o.start;int*next=n?(int*)malloc(n*4):0;for(int i=0;i<n;++i)next[i]=o.start[i];if(start)free(start);start=next;finish=start+n;end=finish;}return *this;}
int main(){Vector_0086e4b0<Element_0086e4b0>v;v.start=(Element_0086e4b0*)malloc(12*sizeof(Element_0086e4b0));v.finish=v.start;v.end=v.start+12;Element_0086e4b0 value;value.flag=9;for(int i=0;i<3;++i){new(v.finish)Element_0086e4b0();v.finish->flag=(char)(i+1);++v.finish;}
v.FillInsert(v.start,0,value);assert(v.finish-v.start==3);v.FillInsert(v.start+1,1,value);assert(v.finish-v.start==4);assert(v.start[0].flag==1 && v.start[1].flag==9 && v.start[2].flag==2);
v.FillInsert(v.finish-1,3,value);assert(v.finish-v.start==7);v.FillInsert(v.finish,10,value);assert(v.finish-v.start==17);for(Element_0086e4b0*p=v.start;p!=v.finish;++p)p->~Element_0086e4b0();free(v.start);puts("0086e4b0_TEST PASS");return 0;}