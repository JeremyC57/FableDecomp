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