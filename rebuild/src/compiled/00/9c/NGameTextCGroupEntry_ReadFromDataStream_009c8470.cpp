// NGameText::CGroupEntry::ReadFromDataStream -- PDB00B15EF0
// Leaf: ReadFromDataStream
#include "engine/CDataInputStream.h"
#include "engine/CResource.h"
#include <string.h>
#ifdef _MSC_VER
typedef long NativeLong;
#else
typedef fable_i32 NativeLong;
#endif
// Native serialized long32 is explicit on host; all pointers retain full host width.
struct InputReader:CDataInputStream {InputReader(); // genuine CDataInputStream default PDB00AFBBC0
 private:void ReadWithSrcChunkOverflow(unsigned char*,long); //00993CA0, PDB00AFBD30
 public:static __forceinline bool IsValidPosition(__int64 p){return p>=0&&p<=0x7fffffff;}
 __forceinline void Read(void*dest,long n){if(n>0){__int64 next=(__int64)(StreamPos+n);if(!IsValidPosition(next))return;if(n<=SourceChunkBytesRemaining){memcpy(dest,CurrentSourcePtr,n);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+n;SourceChunkBytesRemaining-=n;StreamPos+=n;}else ReadWithSrcChunkOverflow((unsigned char*)dest,n);}}
 __forceinline NativeLong ReadLong(){NativeLong v;Read(&v,4);return v;}
};
struct ResourceMethods:CResource {ResourceMethods();void SetResourceSize(unsigned long);}; //009FBF20/PDB00B6BDD0, true nonconst void(ulong)
struct BankFileAsync;
struct BankFileData {BankFileData(void*,unsigned long,BankFileAsync&);unsigned long Sizeof()const;}; //actual ctor PDB00B29D50; opaque constructor boundary //009D5620/PDB00B29690, no repository header, opaque pointee
struct CountedFile {CountedFile();~CountedFile();BankFileData*Data;void*Info;unsigned long Sizeof()const{return Data?Data->Sizeof()+sizeof(CountedFile):sizeof(CountedFile);}}; // genuine counted-pointer default PDB02CD4830
struct LongArray {LongArray();~LongArray();NativeLong*First,*Last,*End;unsigned long capacity()const{return End-First;}void Resize(unsigned long,const NativeLong&);}; //004428F0 native vector resize, not an opaque CArray method
struct CDataBankGroupEntry;
struct CGroupEntry:ResourceMethods {CGroupEntry(CountedFile&,const CDataBankGroupEntry*); //actual PDB00B15D40
 LongArray Entries;CountedFile PFileData;const CDataBankGroupEntry*Owner;
 void ReadFromDataStream(InputReader&);
}; //native52 vs donor56, Entries vector12 vs donor16
void CGroupEntry::ReadFromDataStream(InputReader& stream){
 NativeLong count=stream.ReadLong();Entries.Resize(count,0);
 for(long i=0;i<count;++i)Entries.First[i]=stream.ReadLong();
 unsigned long size=sizeof(CGroupEntry)+Entries.capacity()*sizeof(NativeLong);
 size+=PFileData.Sizeof()-sizeof(CountedFile);SetResourceSize(size);
}