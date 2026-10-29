#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
void*operator new(std::size_t);
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

static void*allocated[128];static int allocations,frees,failAt,oldCalls,oldDeletes,newDeletes;static void*expected;
void*operator new(std::size_t n){++allocations;if(allocations==failAt)return 0;void*p=std::malloc(n);if(!p)std::abort();std::memset(p,0xa5,n);allocated[allocations]=p;return p;}
void operator delete(void*p){if(!p)return;int i;for(i=1;i<=allocations;++i)if(allocated[i]==p)break;if(i>allocations)std::abort();allocated[i]=0;++frees;std::free(p);}
template<>void __fastcall CCountedPointer<COldFileFormatMapHeader2>::DeleteData(void*p){++oldDeletes;delete static_cast<COldFileFormatMapHeader2*>(p);}
template<>void __fastcall CCountedPointer<COldFileFormatMapHeader3>::DeleteData(void*p){++newDeletes;delete static_cast<COldFileFormatMapHeader3*>(p);}
template<class TNew,class TOld>CCountedPointer<TNew> __fastcall CReplaceSerialise<TNew,TOld>::ConvertPreviousUnknownVersionUp(void*p){if(p!=expected)std::abort();++oldCalls;return CCountedPointer<TNew>(new TNew);}
static void check(bool b){if(!b)std::abort();}
static void reset(){for(int i=1;i<=allocations;++i)check(!allocated[i]);allocations=frees=failAt=oldCalls=oldDeletes=newDeletes=0;}
int main(){check(sizeof(COldFileFormatMapHeader1)==5&&sizeof(COldFileFormatMapHeader2)==5&&sizeof(COldFileFormatMapHeader3)==5);check(sizeof(CCountedPointer<COldFileFormatMapHeader3>)==8&&sizeof(CCPPointerInfo)==12);COldFileFormatMapHeader2 input;input.ByteLength=0x7f123456;expected=&input;
 const unsigned versions[]={0,1,2,3,255};for(unsigned k=0;k<5;++k){reset();input.SerialiseVersionNumber=static_cast<unsigned char>(versions[k]);{CCountedPointer<COldFileFormatMapHeader3> result=CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(&input);check(result.Data&&result.Info&&result.Info->RefCount==1&&result.Info->Data==result.Data);check(oldCalls==(versions[k]!=2));check(oldDeletes==(versions[k]!=2));check(newDeletes==0);const unsigned char*p=reinterpret_cast<const unsigned char*>(result.Data);for(unsigned j=0;j<5;++j)check(p[j]==0xa5);{CCountedPointer<COldFileFormatMapHeader3>copy=result;check(copy.Info==result.Info&&copy.Data==result.Data&&copy.Info->RefCount==2);copy=result;check(copy.Info->RefCount==2);}check(result.Info->RefCount==1);}check(newDeletes==1&&allocations==frees);}
 reset();input.SerialiseVersionNumber=2;failAt=1;{CCountedPointer<COldFileFormatMapHeader3> r=CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(&input);check(!r.Data&&!r.Info);}check(allocations==1&&frees==0&&oldCalls==0&&newDeletes==0);
 reset();failAt=2;void*orphan=0;{CCountedPointer<COldFileFormatMapHeader3>r=CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(&input);check(r.Data&&!r.Info);orphan=r.Data;}check(allocations==2&&frees==0&&newDeletes==0);operator delete(orphan);check(frees==1);reset();std::puts("00516010_TEST PASS");return 0;}