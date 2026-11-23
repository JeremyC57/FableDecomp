// NGameText::CGroupEntry::ReadFromDataStream -- PDB00B15EF0
// Leaf: ReadFromDataStream
// NF64 (round 7, q10639): the local LongArray stand-in replaced by the shared engine/CArray.h (CArray<T> : std::vector<T>); the inherited
//   resize is kept out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE) and binds ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0 (was LongArray::Resize).
#define FABLE_RETAIL_CRESOURCE_FLAT_SETRESOURCESIZE   // NF302: ?SetResourceSize@CResource@@QAEXK@Z (both PDBs) from the header
#include "fable_compiler.h"
#define FABLE_CDATAINPUTSTREAM_FRIEND struct InputReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
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
 private: //00993CA0, PDB00AFBD30
 public:static FABLE_FORCEINLINE bool IsValidPosition(__int64 p){return p>=0&&p<=0x7fffffff;}
 FABLE_FORCEINLINE void Read(void*dest,long n){if(n>0){__int64 next=(__int64)(StreamPos+n);if(!IsValidPosition(next))return;if(n<=SourceChunkBytesRemaining){memcpy(dest,CurrentSourcePtr,n);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+n;SourceChunkBytesRemaining-=n;StreamPos+=n;}else ReadWithSrcChunkOverflow((unsigned char*)dest,n);}}
 FABLE_FORCEINLINE NativeLong ReadLong(){NativeLong v;Read(&v,4);return v;}
};
struct ResourceMethods:CResource {ResourceMethods();}; //SetResourceSize is CResource::SetResourceSize 009FBF20/PDB00B6BDD0, true nonconst void(ulong) (header opt-in above; NF302)
struct BankFileAsync;
struct BankFileData {BankFileData(void*,unsigned long,BankFileAsync&);unsigned long Sizeof()const;}; //actual ctor PDB00B29D50; opaque constructor boundary //009D5620/PDB00B29690, no repository header, opaque pointee
struct CountedFile {CountedFile();~CountedFile();BankFileData*Data;void*Info;unsigned long Sizeof()const{return Data?Data->Sizeof()+sizeof(CountedFile):sizeof(CountedFile);}}; // genuine counted-pointer default PDB02CD4830
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_C   // STLport 4.6 _vector.h includes _vector.c; the repo header declares capacity() in that branch
#include "engine/CArray.h"   // Ego_r.pdb CGroupEntry::Entries is CArray<long> (NF64: was a local LongArray stand-in)
struct CDataBankGroupEntry;
struct CGroupEntry:ResourceMethods {CGroupEntry(CountedFile&,const CDataBankGroupEntry*); //actual PDB00B15D40
 CArray<long> Entries;CountedFile PFileData;const CDataBankGroupEntry*Owner;
 void ReadFromDataStream(InputReader&);
}; //native52 vs donor56, Entries vector12 vs donor16
void CGroupEntry::ReadFromDataStream(InputReader& stream){
 NativeLong count=stream.ReadLong();Entries.resize(count,0);
 for(long i=0;i<count;++i)Entries[i]=stream.ReadLong();
 unsigned long size=sizeof(CGroupEntry)+Entries.capacity()*sizeof(NativeLong);
 size+=PFileData.Sizeof()-sizeof(CountedFile);SetResourceSize(size);
}