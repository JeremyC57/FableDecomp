// 00a91390 ConvertVertexStream_StaticToFormat
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ?Convert@ExternalRetry_00a91390@@YIXHHHHHH@Z
#include <stdlib.h>
namespace ExternalRetry_00a91390 {
struct __true_type{__true_type(){}};
struct CBaseClass {CBaseClass();__declspec(noinline) virtual ~CBaseClass();};
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
struct CVertexShaderInputStatic:CVertexShaderInput {CVertexShaderInputStatic(int p){Initialise(p,1);}void Initialise(int,int);};
struct CVertexStreamConverter {CVertexStreamConverter();void ConvertStream(int,int,CVertexStreamFormat*,int,int,CVertexStreamFormat*,int,int);};
void __fastcall Convert(int out,int input,int a,int b,int c,int d){CVertexShaderInputStatic first(input);CVertexStreamFormat* f=&first.DataStreams.first->StreamFormat;int n=f->GetVertexSize();CVertexShaderInputStatic second(b);CVertexStreamFormat* s=&second.DataStreams.first->StreamFormat;int m=s->GetVertexSize();CVertexStreamConverter cv;cv.ConvertStream(out,n,f,a,m,s,c,d);}
}

#include <assert.h>
#include <stdio.h>
#include <new>
namespace ExternalRetry_00a91390 {
typedef CVertexShaderInput::CStreamInfo CStreamInfo;typedef CVertexStreamFormat::CDataElement CDataElement;
static int deaths,binds,runs;static CVertexStreamFormat* passed[2];
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){++deaths;}
void CVertexShaderInputStatic::Initialise(int value,int mode){assert(mode==1&&DataStreams.first==0&&DataStreams.last==0&&DataStreams.end==0&&D3DDeclaration.first==0&&D3DDeclaration.last==0&&D3DDeclaration.end==0);int slot=binds++;assert(value==(slot%2?3:1));DataStreams.first=(CStreamInfo*)malloc(2*sizeof(CStreamInfo));for(int i=0;i<2;++i){new(DataStreams.first+i)CStreamInfo;DataStreams.first[i].StreamIndex=value+i;CVertexStreamFormat& p=DataStreams.first[i].StreamFormat;p.DataElements.first=(CDataElement*)malloc(2*sizeof(CDataElement));p.DataElements.first[0].Register=5;p.DataElements.first[0].DataType=slot%2?2:0;p.DataElements.first[1].Register=7;p.DataElements.first[1].DataType=8;p.DataElements.last=p.DataElements.first+2;p.DataElements.end=p.DataElements.last;}DataStreams.last=DataStreams.first+2;DataStreams.end=DataStreams.last;passed[slot%2]=&DataStreams.first->StreamFormat;D3DDeclaration.first=(_D3DVERTEXELEMENT9*)malloc(2*sizeof(_D3DVERTEXELEMENT9));D3DDeclaration.last=D3DDeclaration.first+2;D3DDeclaration.end=D3DDeclaration.last;}
int CVertexStreamFormat::GetVertexSize(){assert(this==passed[(binds-1)%2]);static const int sizes[]={4,8,12,16,4,4,8,4,2,4,6,8,4,4};int n=0;for(CDataElement*p=DataElements.first;p!=DataElements.last;++p)if(p->DataType<14)n+=sizes[p->DataType];return n;}
CVertexStreamConverter::CVertexStreamConverter(){}void CVertexStreamConverter::ConvertStream(int out,int n,CVertexStreamFormat*f,int a,int m,CVertexStreamFormat*s,int c,int d){assert(out==7&&n==6&&m==14&&a==2&&c==4&&d==5&&f==passed[0]&&s==passed[1]&&f!=s);++runs;}
}
int main(){using namespace ExternalRetry_00a91390;if(sizeof(CVertexStreamConverter)!=1||sizeof(CBaseClass)!=4||sizeof(CDataElement)!=8||sizeof(_D3DVERTEXELEMENT9)!=8||sizeof(CVertexStreamFormat)!=16||sizeof(CStreamInfo)!=20||sizeof(CVertexShaderInput)!=28||sizeof(CVertexShaderInputStatic)!=28)return 1;Convert(7,1,2,3,4,5);assert(runs==1&&binds==2&&deaths==6);Convert(7,1,2,3,4,5);assert(runs==2&&binds==4&&deaths==12);puts("00a91390_TEST PASS");return 0;}