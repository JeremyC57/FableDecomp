// NGameText::CGroupEntry::ReadFromDataStream -- PDB00B15EF0
// Leaf: ReadFromDataStream
#include "engine/CDataInputStream.h"
#include "engine/CResource.h"
#include <string.h>
struct InputReader:CDataInputStream {InputReader(); // genuine CDataInputStream default PDB00AFBBC0
 void ReadWithSrcChunkOverflow(unsigned char*,long); //00993CA0, PDB00AFBD30
 static __forceinline bool IsValidPosition(__int64 p){return p>=0&&p<=0x7fffffff;}
 __forceinline void Read(void*dest,long n){if(n>0){__int64 next=(__int64)(StreamPos+n);if(!IsValidPosition(next))return;if(n<=SourceChunkBytesRemaining){memcpy(dest,CurrentSourcePtr,n);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+n;SourceChunkBytesRemaining-=n;StreamPos+=n;}else ReadWithSrcChunkOverflow((unsigned char*)dest,n);}}
};
struct ResourceMethods:CResource {ResourceMethods();void SetResourceSize(unsigned long);}; //009FBF20/PDB00B6BDD0, true nonconst void(ulong)
struct BankFileData {unsigned long Sizeof()const;}; //009D5620/PDB00B29690, no repository header, opaque pointee
struct CountedFile {CountedFile();BankFileData*Data;void*Info;unsigned long Sizeof()const{return Data?Data->Sizeof()+sizeof(CountedFile):sizeof(CountedFile);}}; // genuine counted-pointer default PDB02CD4830
struct LongArray {LongArray();long*First,*Last,*End;void Resize(unsigned long,const long&);}; //004428F0 native vector resize, not an opaque CArray method
struct CDataBankGroupEntry;
struct CGroupEntry:ResourceMethods {CGroupEntry(CountedFile&,const CDataBankGroupEntry*); //actual PDB00B15D40
 LongArray Entries;CountedFile PFileData;const CDataBankGroupEntry*Owner;
 void ReadFromDataStream(InputReader&);
}; //native52 vs donor56, Entries vector12 vs donor16
void CGroupEntry::ReadFromDataStream(InputReader& stream){
 long count;stream.Read(&count,4);long entry=0;Entries.Resize(count,entry);
 for(long i=0;i<count;++i){stream.Read(&entry,4);Entries.First[i]=entry;}
 SetResourceSize(sizeof(CGroupEntry)+(Entries.End-Entries.First)*sizeof(long)+PFileData.Sizeof()-sizeof(CountedFile));
}
