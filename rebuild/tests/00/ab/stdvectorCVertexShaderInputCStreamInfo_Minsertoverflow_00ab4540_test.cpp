#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 e74559c4863860ae3a60e11eddab15d85b5915c4f1fe99213c57fe9fa6e4a165
// 00ab4540 std::vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride20 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_00ab4540 {Tag_00ab4540(){}};
// Layout: RTTI vtable0129d578=CVertexStreamFormat, slot0 deleting dtor00a5e670; PDB struct_layouts_egor.tsv35420 ff has vptr+0 and DataElements+4. Retail array12 makes format16 and CStreamInfo20. CDataElement is8 (Register,DataType).
class CBaseClass{public:virtual ~CBaseClass();};
template<class T>class CArray{ public:T* start;T* finish;T* end;CArray():start(0),finish(0),end(0){}CArray(const CArray&);~CArray(){if(start)free(start);}};
class CVertexStreamFormat:public CBaseClass{
 public:class CDataElement{ public:unsigned int Register;int DataType;};CArray<CDataElement> DataElements;
 CVertexStreamFormat(){}__forceinline CVertexStreamFormat(const CVertexStreamFormat&o):DataElements(o.DataElements){}virtual ~CVertexStreamFormat(){}
};
class CVertexShaderInput{ public:class CStreamInfo{ public:unsigned int StreamIndex;CVertexStreamFormat StreamFormat;CStreamInfo(){}__forceinline CStreamInfo(const CStreamInfo&o):StreamIndex(o.StreamIndex),StreamFormat(o.StreamFormat){}};};
typedef CVertexShaderInput::CStreamInfo Element_00ab4540;
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_00ab4540 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_00ab4540&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_00ab4540* __fastcall UninitializedCopy(const Element_00ab4540* first,const Element_00ab4540* last,Element_00ab4540* out,const Tag_00ab4540&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_00ab4540(*first);
 return out;
}
__declspec(noinline) Element_00ab4540* __fastcall UninitializedFill(Element_00ab4540* out,unsigned int count,const Element_00ab4540& value,const Tag_00ab4540&) {
 for(;count;--count,++out)if(out)new(out)Element_00ab4540(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_00ab4540* first,Element_00ab4540* last,const Tag_00ab4540&) {
 for(;first!=last;++first)first->~Element_00ab4540();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_00ab4540<T>::InsertGrow(T* position,const T& value,const Tag_00ab4540&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_00ab4540());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_00ab4540());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_00ab4540());
 DestroyRange(start,finish,Tag_00ab4540());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_00ab4540<Element_00ab4540>::InsertGrow(Element_00ab4540*,const Element_00ab4540&,const Tag_00ab4540&,unsigned int,bool);

// END exact source snapshot
static int baseDestructions;
CBaseClass::~CBaseClass(){++baseDestructions;}
template<class T>CArray<T>::CArray(const CArray&o){int n=o.finish-o.start;start=n?(T*)malloc(n*sizeof(T)):0;finish=start+n;end=finish;for(int i=0;i<n;++i)start[i]=o.start[i];}
template<class T> void Vector_00ab4540<T>::Clear(){for(T* p=start;p!=finish;++p)p->~T();if(start)free(start);}

int main(){Vector_00ab4540<Element_00ab4540> v;v.start=(Element_00ab4540*)malloc(sizeof(Element_00ab4540));v.finish=v.start;v.end=v.start+1;Tag_00ab4540 tag;Element_00ab4540 value;value.StreamIndex=7;value.StreamFormat.DataElements.start=(CVertexStreamFormat::CDataElement*)malloc(16);value.StreamFormat.DataElements.finish=value.StreamFormat.DataElements.start+2;value.StreamFormat.DataElements.end=value.StreamFormat.DataElements.finish;for(int j=0;j<2;++j){value.StreamFormat.DataElements.start[j].Register=j;value.StreamFormat.DataElements.start[j].DataType=10+j;}
v.InsertGrow(v.start,value,tag,1,true);assert(v.finish-v.start==1 && v.end-v.start==1);
v.InsertGrow(v.start,value,tag,2,false);assert(v.finish-v.start==3 && v.end-v.start==3);assert(v.start[0].StreamIndex==7 && v.start[2].StreamFormat.DataElements.start[1].DataType==11 && v.start[2].StreamFormat.DataElements.start!=value.StreamFormat.DataElements.start);
v.Clear();assert(baseDestructions==4);CBaseClass* format=new CVertexStreamFormat(value.StreamFormat);delete format;assert(baseDestructions==5);puts("00ab4540_TEST PASS");return 0;}