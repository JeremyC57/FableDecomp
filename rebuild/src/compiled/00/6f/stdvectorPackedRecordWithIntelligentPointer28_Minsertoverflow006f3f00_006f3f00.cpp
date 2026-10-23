// 006f3f00 STL::Vector28_external28::InsertGrow_006f3f00
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride28 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_006f3f00 {Tag_006f3f00(){}};
// Layout:006eff60 copies packed byte+0, words+1/+5/+9, int+16 and intelligent pointer8 at+20; dtor calls00a01c10 on+20.
// Layout:00a01c10 releases the separate control block (object+0, pointer references+4), rather than owning the observed object.
struct IntelligentControl{void* object;int references;};
struct CBaseIntelligentPointer{virtual ~CBaseIntelligentPointer();IntelligentControl* control;CBaseIntelligentPointer():control(0){}};
#pragma pack(push,1)
struct Packed{char tag;int a,b,c;};
#pragma pack(pop)
struct Element_006f3f00{Packed packed;int type;CBaseIntelligentPointer pointer;Element_006f3f00(){}Element_006f3f00(const Element_006f3f00&);};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_006f3f00 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_006f3f00&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_006f3f00* __fastcall UninitializedCopy(const Element_006f3f00* first,const Element_006f3f00* last,Element_006f3f00* out,const Tag_006f3f00&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_006f3f00(*first);
 return out;
}
__declspec(noinline) Element_006f3f00* __fastcall UninitializedFill(Element_006f3f00* out,unsigned int count,const Element_006f3f00& value,const Tag_006f3f00&) {
 for(;count;--count,++out)if(out)new(out)Element_006f3f00(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_006f3f00* first,Element_006f3f00* last,const Tag_006f3f00&) {
 for(;first!=last;++first)first->~Element_006f3f00();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_006f3f00<T>::InsertGrow(T* position,const T& value,const Tag_006f3f00&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_006f3f00());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_006f3f00());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_006f3f00());
 T* oldEnd=finish;for(T* p=start;p!=oldEnd;++p)p->pointer.CBaseIntelligentPointer::~CBaseIntelligentPointer();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_006f3f00<Element_006f3f00>::InsertGrow(Element_006f3f00*,const Element_006f3f00&,const Tag_006f3f00&,unsigned int,bool);