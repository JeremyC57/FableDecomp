#include <stdio.h>
#include <string.h>
#include <new>
// 00ab37b0 CVertexStreamFormat::CalculateVertexSize
// Flags:
// Leaf: ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
enum EVertexStreamDataType{};
class CBaseClass{public:virtual~CBaseClass();};
namespace std{
 template<class T>class allocator{};
 template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
 template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
 template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
  unsigned int size()const{return this->_M_finish-this->_M_start;}
  const T&operator[](unsigned int n)const{return this->_M_start[n];}
 };
}
template<class T>class CArray:public std::vector<T>{public:const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
// Layout: real CBaseClass4 and STLport12-byte CArray DataElements4. Original
// CDataElement Register0/DataType4, size8; donor20-byteowner shrinks to16.
class CVertexStreamFormat:public CBaseClass{public:
 class CDataElement{public:unsigned long Register;EVertexStreamDataType DataType;};
 CArray<CDataElement>DataElements;
 static __declspec(noinline) unsigned long __fastcall GetDataTypeSize(EVertexStreamDataType);
 unsigned long CalculateVertexSize()const;
};
__declspec(noinline) unsigned long __fastcall CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType type){
 switch(type){case 0:case 4:case 5:case 7:case 9:case 12:case 13:return 4;
 case 1:case 6:case 11:return 8;case 2:return 12;case 3:return 16;case 8:return 2;case 10:return 6;}
 return 0;
}
unsigned long CVertexStreamFormat::CalculateVertexSize()const{
 unsigned long size=0;unsigned int i=0;
 if(i<DataElements.size()){
  do{size+=GetDataTypeSize(DataElements[i].DataType);++i;}while(i<DataElements.size());
 }
 return size;
}

CBaseClass::~CBaseClass(){}
class FixtureArray:public CArray<CVertexStreamFormat::CDataElement>{public:void Bind(CVertexStreamFormat::CDataElement*first,unsigned int n){this->_M_start=first;this->_M_finish=first+n;this->_M_end_of_storage._M_data=first+n;}};
int main(){
 typedef CVertexStreamFormat::CDataElement Element;
 if(sizeof(Element)!=8||sizeof(CVertexStreamFormat)!=16||sizeof(CArray<Element>)!=12||sizeof(EVertexStreamDataType)!=4)return 1;
 const unsigned long sizes[16]={4,8,12,16,4,4,8,4,2,4,6,8,4,4,0,0};unsigned int cases=0;
 for(unsigned int pattern=0;pattern<8;++pattern)for(unsigned int seed=0;seed<64;++seed)for(unsigned int n=0;n<=32;++n)for(unsigned int offset=0;offset<4;++offset){
  union Storage{double align;unsigned char bytes[16+32];}storage;memset(storage.bytes,pattern*31,sizeof(storage.bytes));CVertexStreamFormat*format=new(storage.bytes+16)CVertexStreamFormat;
  Element records[40];for(unsigned int i=0;i<40;++i){records[i].Register=(seed*13+i)*0x01010101UL;unsigned long type=(seed+i*7)%16;memcpy(&records[i].DataType,&type,4);}
  FixtureArray array;array.Bind(records+offset,n);format->DataElements=array;unsigned char beforeObject[16],beforeRecords[sizeof(records)];memcpy(beforeObject,format,16);memcpy(beforeRecords,records,sizeof(records));
  unsigned long expected=0;for(unsigned int i=0;i<n;++i)expected+=sizes[(seed+(i+offset)*7)%16];
  unsigned long actual=format->CalculateVertexSize();if(actual!=expected)return 2;
  if(memcmp(beforeObject,format,16)||memcmp(beforeRecords,records,sizeof(records)))return 3;
  for(unsigned int i=0;i<16;++i)if(storage.bytes[i]!=pattern*31||storage.bytes[32+i]!=pattern*31)return 4;
  format->~CVertexStreamFormat();++cases;
 }
 // Use encoded numeric values because original enumerator names are unproved.
 const unsigned long codes[19]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0x7fffffffUL,0x80000000UL,0xffffffffUL};
 for(unsigned int i=0;i<19;++i){EVertexStreamDataType type;memcpy(&type,codes+i,4);unsigned long expected=codes[i]<16?sizes[codes[i]]:0;if(CVertexStreamFormat::GetDataTypeSize(type)!=expected)return 5;++cases;}
 if(cases!=67603)return 6;puts("00ab37b0_TEST PASS");return 0;
}