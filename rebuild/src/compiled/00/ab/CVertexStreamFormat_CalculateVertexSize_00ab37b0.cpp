// 00ab37b0 CVertexStreamFormat::CalculateVertexSize
// Flags:
// Leaf: ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
enum EVertexStreamDataType{};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CArray.h"
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