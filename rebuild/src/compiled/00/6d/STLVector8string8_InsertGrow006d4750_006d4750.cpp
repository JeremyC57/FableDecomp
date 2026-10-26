// 006d4750 STL::Vector8_string8::InsertGrow_006d4750
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride8 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_006d4750 {Tag_006d4750(){}};
// Layout: CCharString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
class CCharString { public:void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element_006d4750{CCharString name;int value;Element_006d4750(){}__forceinline Element_006d4750(const Element_006d4750& o):name(o.name),value(o.value){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_006d4750 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_006d4750&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_006d4750* __fastcall UninitializedCopy(const Element_006d4750* first,const Element_006d4750* last,Element_006d4750* out,const Tag_006d4750&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_006d4750(*first);
 return out;
}
__declspec(noinline) Element_006d4750* __fastcall UninitializedFill(Element_006d4750* out,unsigned int count,const Element_006d4750& value,const Tag_006d4750&) {
 for(;count;--count,++out)if(out)new(out)Element_006d4750(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_006d4750* first,Element_006d4750* last,const Tag_006d4750&) {
 for(;first!=last;++first)first->~Element_006d4750();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_006d4750<T>::InsertGrow(T* position,const T& value,const Tag_006d4750&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_006d4750());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_006d4750());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_006d4750());
 T* oldEnd=finish;for(T* i=start;i!=oldEnd;++i)i->name.~CCharString();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_006d4750<Element_006d4750>::InsertGrow(Element_006d4750*,const Element_006d4750&,const Tag_006d4750&,unsigned int,bool);