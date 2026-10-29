#include <stdio.h>
#include <string.h>
// 00ab3810 CVertexStreamFormat::AddDataElement
// Flags: /Gr
// Leaf: ?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z
#include <new>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
enum EVertexStreamDataType{};
class CBaseClass{public:virtual~CBaseClass();};
namespace std{
 struct __false_type{};
 template<class T>class allocator{};
 template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
 template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
 template<class T1,class T2>__forceinline void _Construct(T1*p,const T2&x){new(p)T1(x);}
 template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
 protected:
  __declspec(noinline) void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);
 public:
  unsigned int size()const{return this->_M_finish-this->_M_start;}
  __forceinline void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);}
 };
 template<class T,class A>__declspec(noinline)void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __false_type&,unsigned int fill,bool atEnd){
  unsigned int old_size=size();unsigned int capacity=old_size+(old_size<fill?fill:old_size);
  T*new_start=(T*)malloc(capacity*sizeof(T));if(!new_start)throw bad_alloc();
  T*new_finish=new_start;
  for(T*p=this->_M_start;p!=position;++p,++new_finish)new(new_finish)T(*p);
  for(unsigned int i=0;i<fill;++i,++new_finish)new(new_finish)T(x);
  if(!atEnd)for(T*p=position;p!=this->_M_finish;++p,++new_finish)new(new_finish)T(*p);
  for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();
  if(this->_M_start)free(this->_M_start);
  this->_M_start=new_start;this->_M_finish=new_finish;this->_M_end_of_storage._M_data=new_start+capacity;
 }
}
template<class T>class CArray:public std::vector<T>{};
// Layout: original CBaseClass4 and STLport12-byte CArray DataElements4; real
// nested CDataElement Register0/DataType4, size8. No matching engine header.
class CVertexStreamFormat:public CBaseClass{public:
 class CDataElement{public:unsigned long Register;EVertexStreamDataType DataType;
  __forceinline CDataElement(unsigned long r,EVertexStreamDataType t):Register(r),DataType(t){}
 };
 CArray<CDataElement>DataElements;
 void AddDataElement(unsigned long,EVertexStreamDataType);
};
void CVertexStreamFormat::AddDataElement(unsigned long r,EVertexStreamDataType t){DataElements.push_back(CDataElement(r,t));}

CBaseClass::~CBaseClass(){}
typedef CVertexStreamFormat::CDataElement Element;
struct FixtureState{Element*first,*finish,*end;};
static unsigned long typeBits(EVertexStreamDataType type){unsigned long result;memcpy(&result,&type,4);return result;}
int main(){
 if(sizeof(CVertexStreamFormat)!=16||sizeof(Element)!=8||sizeof(FixtureState)!=12)return 1;
 unsigned int cases=0;
 for(unsigned int pattern=0;pattern<4;++pattern)for(unsigned int seed=0;seed<32;++seed)for(unsigned int n=0;n<=16;++n)for(unsigned int extra=0;extra<3;++extra){
  unsigned int capacity=n+extra;Element*buffer=(Element*)malloc((capacity+3)*8);if(!buffer)return 2;memset(buffer,pattern*73,(capacity+3)*8);
  for(unsigned int i=0;i<n;++i){unsigned long bits=(i+seed)%16;EVertexStreamDataType type;memcpy(&type,&bits,4);new(buffer+i)Element((seed+i)*0x01010101UL,type);}
  FixtureState before={buffer,buffer+n,buffer+capacity};union Storage{double align;unsigned char bytes[48];}storage;memset(storage.bytes,pattern*73,sizeof(storage.bytes));CVertexStreamFormat*format=new(storage.bytes+16)CVertexStreamFormat;memcpy(&format->DataElements,&before,12);
  unsigned char beforeOwner[16],beforeRecords[21*8];memcpy(beforeOwner,format,16);memcpy(beforeRecords,buffer,(capacity+3)*8);
  unsigned long r=seed%2?0xffffffffUL:(seed*0x01010101UL);unsigned long tBits=seed%16;EVertexStreamDataType type;memcpy(&type,&tBits,4);
  format->AddDataElement(r,type);FixtureState after;memcpy(&after,&format->DataElements,12);
  unsigned int expectedCapacity=n==capacity?n+(n?n:1):capacity;
  if(after.finish-after.first!=(long)(n+1)||after.end-after.first!=(long)expectedCapacity||after.first[n].Register!=r||typeBits(after.first[n].DataType)!=tBits)return 3;
  if(n&&memcmp(after.first,beforeRecords,n*8))return 4;
  if(n<capacity){if(after.first!=buffer||memcmp((unsigned char*)buffer+(n+1)*8,beforeRecords+(n+1)*8,(capacity+3-n-1)*8))return 5;}
  else if(after.first==buffer)return 6;
  if(memcmp(beforeOwner,format,4))return 7;for(unsigned int i=0;i<16;++i)if(storage.bytes[i]!=pattern*73||storage.bytes[32+i]!=pattern*73)return 8;
  free(after.first);format->~CVertexStreamFormat();++cases;
 }
 // Repeated append crosses all growth boundaries and preserves every earlier record.
 for(unsigned int seed=0;seed<8;++seed){CVertexStreamFormat format;Element*buffer=(Element*)malloc(8);FixtureState state={buffer,buffer,buffer};memcpy(&format.DataElements,&state,12);
  for(unsigned int i=0;i<64;++i){unsigned long bits=(seed+i)%16;EVertexStreamDataType type;memcpy(&type,&bits,4);format.AddDataElement(seed*1000+i,type);memcpy(&state,&format.DataElements,12);if(state.finish-state.first!=(long)(i+1))return 9;
   for(unsigned int j=0;j<=i;++j)if(state.first[j].Register!=seed*1000+j||typeBits(state.first[j].DataType)!=(seed+j)%16)return 10;++cases;
  }free(state.first);
 }
 if(cases!=7040)return 11;puts("00ab3810_TEST PASS");return 0;
}