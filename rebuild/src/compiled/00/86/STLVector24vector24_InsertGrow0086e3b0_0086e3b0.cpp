// 0086e3b0 STL::Vector24_vector24::InsertGrow_0086e3b0
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride24 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_0086e3b0 {Tag_0086e3b0(){}};
// Layout: retail pair has 8-byte key, nested 12-byte vector at+8, byte at+21; stride24.
struct Inner_0086e3b0 {int* start;int* finish;int* end;Inner_0086e3b0():start(0),finish(0),end(0){}Inner_0086e3b0(const Inner_0086e3b0&);~Inner_0086e3b0();};
struct Detail_0086e3b0{Inner_0086e3b0 vector;char unused;char flag;Detail_0086e3b0(){}__forceinline Detail_0086e3b0(const Detail_0086e3b0& o):vector(o.vector),flag(o.flag){}};
struct Element_0086e3b0{unsigned __int64 key;Detail_0086e3b0 second;Element_0086e3b0(){}__forceinline Element_0086e3b0(const Element_0086e3b0& o):key(o.key),second(o.second){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_0086e3b0 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_0086e3b0&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_0086e3b0* __fastcall UninitializedCopy(const Element_0086e3b0* first,const Element_0086e3b0* last,Element_0086e3b0* out,const Tag_0086e3b0&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_0086e3b0(*first);
 return out;
}
__declspec(noinline) Element_0086e3b0* __fastcall UninitializedFill(Element_0086e3b0* out,unsigned int count,const Element_0086e3b0& value,const Tag_0086e3b0&) {
 for(;count;--count,++out)if(out)new(out)Element_0086e3b0(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_0086e3b0* first,Element_0086e3b0* last,const Tag_0086e3b0&) {
 for(;first!=last;++first)first->~Element_0086e3b0();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_0086e3b0<T>::InsertGrow(T* position,const T& value,const Tag_0086e3b0&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_0086e3b0());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_0086e3b0());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_0086e3b0());
 Clear();
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_0086e3b0<Element_0086e3b0>::InsertGrow(Element_0086e3b0*,const Element_0086e3b0&,const Tag_0086e3b0&,unsigned int,bool);