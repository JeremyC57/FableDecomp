#include <stdio.h>
// CReplaceSerialise::GetMaxSize(long version,long size) -- Compute the scratch size needed through the version chain.
// Leaf: ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatHeightMapCell3@@VCOldFileFormatHeightMapCell2@@@@SIJJJ@Z
// Identity: proven. Own tail caller008256a0 and exact version3/2 plus record-size constants[9, 10, 15] identify CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2>::GetMaxSize(long,long), original static fastcall signature from both PDB sets. Actual packed original record fields and five-byte serialisation header. VERSION enum names are explicitly compile-time model markers, not recovered original enumerator names. Macro repeats the older GetMaxSize expression, reproducing the redundant tail comparisons.
// Layout: packed records, original class keys/templates and named fields; FileLong/FileULong preserve 32-bit disk widths in the host test. No engine header for these record types.
#ifndef _MSC_VER
#define __fastcall
typedef int FileLong;typedef unsigned int FileULong;
#else
typedef long FileLong;typedef unsigned long FileULong;
#endif
#define MAX_VALUE(a,b) ((a)>(b)?(a):(b))
#pragma pack(push,1)
template<class T,long V>class CBaseSerialise{public:FileULong ByteLength;unsigned char SerialiseVersionNumber;static long __fastcall GetMaxSize(long,long size){return MAX_VALUE(size,sizeof(T));}};
template<class T,class TOld>class CReplaceSerialise{public:FileULong ByteLength;unsigned char SerialiseVersionNumber;static unsigned short __fastcall GetClassVersionNumber(){return T::VERSION;}static long __fastcall GetMaxSize(long version,long size){if(version==GetClassVersionNumber())return MAX_VALUE(size,sizeof(T));return MAX_VALUE(TOld::GetMaxSize(version,size),sizeof(T));}};
class COldFileFormatHeightMapCell1:public CBaseSerialise<COldFileFormatHeightMapCell1,1>{public:enum{VERSION=1};float Height;};
class COldFileFormatHeightMapCell2:public CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>{public:enum{VERSION=2};float Height;unsigned char TextureBlendAlpha;};
class COldFileFormatHeightMapCell3:public CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2>{public:enum{VERSION=3};float Height;unsigned char TextureBlendAlpha;unsigned char EngineTheme[3];unsigned char EngineBlend[2];};
#pragma pack(pop)
template long __fastcall CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2 >::GetMaxSize(long,long);
int main(){bool ok=sizeof(COldFileFormatHeightMapCell1)==9&&sizeof(COldFileFormatHeightMapCell2)==10&&sizeof(COldFileFormatHeightMapCell3)==15;long inputs[]={-1,0,1,8,9,10,12,13,14,15,16,17,20,21,22,99,2147483647L};for(long v=-1;v<=5;++v){for(unsigned int i=0;i<sizeof(inputs)/sizeof(inputs[0]);++i){long n=inputs[i];long expected=(n<0||n>15)?n:15;ok=ok&&COldFileFormatHeightMapCell3::GetMaxSize(v,n)==expected;}}puts(ok?"00824ce0_TEST PASS":"00824ce0_TEST FAIL");return ok?0:1;}