// 00568fc0 STL::Vector16_quest16::InsertGrow_00568fc0
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride16 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_00568fc0 {Tag_00568fc0(){}};
// Layout: CCharString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
class CCharString { public:void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element_00568fc0{CCharString a,b,c;char x,y;Element_00568fc0(){}__forceinline Element_00568fc0(const Element_00568fc0& o):a(o.a),b(o.b),c(o.c),x(o.x),y(o.y){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_00568fc0 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_00568fc0&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_00568fc0* __fastcall UninitializedCopy(const Element_00568fc0* first,const Element_00568fc0* last,Element_00568fc0* out,const Tag_00568fc0&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_00568fc0(*first);
 return out;
}
__declspec(noinline) Element_00568fc0* __fastcall UninitializedFill(Element_00568fc0* out,unsigned int count,const Element_00568fc0& value,const Tag_00568fc0&) {
 for(;count;--count,++out)if(out)new(out)Element_00568fc0(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_00568fc0* first,Element_00568fc0* last,const Tag_00568fc0&) {
 for(;first!=last;++first)first->~Element_00568fc0();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_00568fc0<T>::InsertGrow(T* position,const T& value,const Tag_00568fc0&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_00568fc0());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_00568fc0());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_00568fc0());
 DestroyRange(start,finish,Tag_00568fc0());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_00568fc0<Element_00568fc0>::InsertGrow(Element_00568fc0*,const Element_00568fc0&,const Tag_00568fc0&,unsigned int,bool);