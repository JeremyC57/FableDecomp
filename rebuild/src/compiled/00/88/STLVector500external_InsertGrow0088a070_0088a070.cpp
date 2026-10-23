// 0088a070 STL::Vector500_external::InsertGrow_0088a070
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride500 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_0088a070 {Tag_0088a070(){}};
struct Element_0088a070{int data[125];Element_0088a070(){}Element_0088a070(const Element_0088a070&);~Element_0088a070();};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_0088a070 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_0088a070&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_0088a070* __fastcall UninitializedCopy(const Element_0088a070* first,const Element_0088a070* last,Element_0088a070* out,const Tag_0088a070&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_0088a070(*first);
 return out;
}
#pragma auto_inline(on)
__forceinline Element_0088a070* __fastcall UninitializedFill(Element_0088a070* out,unsigned int count,const Element_0088a070& value,const Tag_0088a070&) {
 for(;count>0;--count,++out)if(out)new(out)Element_0088a070(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_0088a070* first,Element_0088a070* last,const Tag_0088a070&) {
 for(;first!=last;++first)first->~Element_0088a070();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_0088a070<T>::InsertGrow(T* position,const T& value,const Tag_0088a070&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_0088a070());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_0088a070());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_0088a070());
 DestroyRange(start,finish,Tag_0088a070());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_0088a070<Element_0088a070>::InsertGrow(Element_0088a070*,const Element_0088a070&,const Tag_0088a070&,unsigned int,bool);