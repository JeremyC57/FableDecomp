// Original packed map-header version replacement 2 to 3.
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader3@@VCOldFileFormatMapHeader2@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader3@@@@PAX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Flags:
void __cdecl operator delete(void*);
#include "engine/CCountedPointer.h"
#pragma pack(push,1)
template<class T,int Version>class CBaseSerialise {public:unsigned long ByteLength;unsigned char SerialiseVersionNumber;static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void*);};
template<class TNew,class TOld>class CReplaceSerialise {public:unsigned long ByteLength;unsigned char SerialiseVersionNumber;static CCountedPointer<TNew> __fastcall ConvertPreviousUnknownVersionUp(void*);};
class COldFileFormatMapHeader1:public CBaseSerialise<COldFileFormatMapHeader1,1>{};
class COldFileFormatMapHeader2:public CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>{public:void ConvertFromOld(const COldFileFormatMapHeader1*){}};
class COldFileFormatMapHeader3:public CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>{public:void ConvertFromOld(const COldFileFormatMapHeader2*){}};
#pragma pack(pop)
template<>CCountedPointer<COldFileFormatMapHeader3> __fastcall CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(void*data){
 CCountedPointer<COldFileFormatMapHeader3> result(new COldFileFormatMapHeader3);
 CCountedPointer<COldFileFormatMapHeader2> held(static_cast<COldFileFormatMapHeader2*>(0));
 COldFileFormatMapHeader2*old=static_cast<COldFileFormatMapHeader2*>(data);
 if(static_cast<COldFileFormatMapHeader2*>(data)->SerialiseVersionNumber!=2){held=CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(data);old=held.Data;}
 result.Data->ConvertFromOld(old);return result;
}