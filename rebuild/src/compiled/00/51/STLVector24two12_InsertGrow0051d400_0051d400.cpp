// 0051d400 STL::Vector24_two12::InsertGrow_0051d400
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride24 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_0051d400 {Tag_0051d400(){}};
// Layout: retail two 12-byte subobject ctors at00514eb0 and0051b4c0; stride24.
struct Data16{int words[4];};
struct A_0051d400{Data16* start;Data16* finish;Data16* end;A_0051d400():start(0),finish(0),end(0){}A_0051d400(const A_0051d400&);~A_0051d400();};
struct B_0051d400{Data16* start;Data16* finish;Data16* end;B_0051d400():start(0),finish(0),end(0){}B_0051d400(const B_0051d400&);~B_0051d400();};struct Element_0051d400{A_0051d400 a;B_0051d400 b;Element_0051d400(){}__forceinline Element_0051d400(const Element_0051d400& o):a(o.a),b(o.b){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_0051d400 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_0051d400&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_0051d400* __fastcall UninitializedCopy(const Element_0051d400* first,const Element_0051d400* last,Element_0051d400* out,const Tag_0051d400&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_0051d400(*first);
 return out;
}
__declspec(noinline) Element_0051d400* __fastcall UninitializedFill(Element_0051d400* out,unsigned int count,const Element_0051d400& value,const Tag_0051d400&) {
 for(;count;--count,++out)if(out)new(out)Element_0051d400(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_0051d400* first,Element_0051d400* last,const Tag_0051d400&) {
 for(;first!=last;++first)first->~Element_0051d400();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_0051d400<T>::InsertGrow(T* position,const T& value,const Tag_0051d400&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_0051d400());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_0051d400());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_0051d400());
 DestroyRange(start,finish,Tag_0051d400());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_0051d400<Element_0051d400>::InsertGrow(Element_0051d400*,const Element_0051d400&,const Tag_0051d400&,unsigned int,bool);