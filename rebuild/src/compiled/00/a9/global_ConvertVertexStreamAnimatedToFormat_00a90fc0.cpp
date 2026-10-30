// 00a90fc0 ConvertVertexStream_AnimatedToFormat
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ?Convert@ExternalRetry_00a90fc0@@YIXHHHHHH@Z
#include <stdlib.h>
namespace ExternalRetry_00a90fc0 {
struct __true_type{__true_type(){}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// Layout: CArray retains a three-pointer retail vector base; donor allocator bytes do not exist in retail.
template<class T>struct CArray {T *first,*last,*end;__forceinline CArray():first(0),last(0),end(0){}__forceinline ~CArray();};
struct _D3DVERTEXELEMENT9 {unsigned short Stream,Offset;unsigned char Type,Method,Usage,UsageIndex;};
struct CVertexStreamFormat:CBaseClass {struct CDataElement {unsigned Register,DataType;};CArray<CDataElement> DataElements;__forceinline virtual ~CVertexStreamFormat(){}int GetVertexSize();};
// Layout: retail CStreamInfo20 contains StreamIndex0/StreamFormat4; format16 contains its DataElements vector at4.
// Layout: retail CVertexShaderInput28 has DataStreams4 and D3DDeclaration10. Both enclosing input destructors are compiler-generated.
struct CVertexShaderInput:CBaseClass {struct CStreamInfo {unsigned StreamIndex;CVertexStreamFormat StreamFormat;};CArray<CStreamInfo> DataStreams;CArray<_D3DVERTEXELEMENT9> D3DDeclaration;__declspec(noinline) CVertexShaderInput();};
// Original helper name is unavailable in labels/catalog; retain the target's default manifest label.
template<class Tag>__declspec(noinline) void __fastcall FUN_00a5e900(CVertexShaderInput::CStreamInfo* first,CVertexShaderInput::CStreamInfo* last,const Tag&){for(;first!=last;++first)first->~CStreamInfo();}
template<class T>__forceinline CArray<T>::~CArray(){if(first)free(first);}
template<>__forceinline CArray<CVertexShaderInput::CStreamInfo>::~CArray(){FUN_00a5e900(first,last,__true_type());if(first)free(first);}
CVertexShaderInput::CVertexShaderInput():DataStreams(),D3DDeclaration(){}
struct CVertexShaderInputAnimated:CVertexShaderInput {CVertexShaderInputAnimated(int p){Initialise(p,1);}void Initialise(int,int);};
struct CVertexStreamConverter {CVertexStreamConverter();void ConvertStream(int,int,CVertexStreamFormat*,int,int,CVertexStreamFormat*,int,int);};
void __fastcall Convert(int out,int input,int a,int b,int c,int d){CVertexShaderInputAnimated first(input);CVertexStreamFormat* f=&first.DataStreams.first->StreamFormat;int n=f->GetVertexSize();CVertexShaderInputAnimated second(b);CVertexStreamFormat* s=&second.DataStreams.first->StreamFormat;int m=s->GetVertexSize();CVertexStreamConverter cv;cv.ConvertStream(out,n,f,a,m,s,c,d);}
}