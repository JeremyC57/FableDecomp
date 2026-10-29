// Original packed map-header version replacement 2 to 3.
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader3@@VCOldFileFormatMapHeader2@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader3@@@@PAX@Z
// Flags:
void __cdecl operator delete(void*);
class CCPPointerInfo {public: unsigned long RefCount;void (__fastcall*DeleteFunc)(void*);void*Data;CCPPointerInfo(unsigned long count,void(__fastcall*deleter)(void*),void*p):RefCount(count),DeleteFunc(deleter),Data(p){}};
template<class T>class CCountedPointer {public:T*Data;CCPPointerInfo*Info;static void __fastcall DeleteData(void*);
 CCountedPointer(T*p):Data(p),Info(p?new CCPPointerInfo(1,DeleteData,p):0){}
 CCountedPointer(const CCountedPointer&x):Data(x.Data),Info(x.Info){if(Info)++Info->RefCount;}
 void ShareData(T*p,CCPPointerInfo*info){if(Info!=info){if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(Info);}Data=p;Info=info;if(Info)++Info->RefCount;}}
 CCountedPointer&operator=(const CCountedPointer&x){ShareData(x.Data,x.Info);return *this;}
 ~CCountedPointer(){if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(Info);}Info=0;Data=0;}
};
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