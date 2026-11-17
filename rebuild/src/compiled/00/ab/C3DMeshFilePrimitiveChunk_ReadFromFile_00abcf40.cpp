// C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read one index or a high-bit-tagged index array, then its subchunks
// Retail 0x00ABCF40, 318 bytes. ReadWithSrcChunkOverflow 0x00993CA0; Resize 0x004428F0; base reader 0x00AA8830.
// engine/C3DMeshFilePrimitiveChunk.h and CChunkedFileSuperChunk.h use donor 16-byte vectors:
// MaterialIndices +0x34 vs retail +0x2c. The retail 12-byte vector/base model below agrees
// with landed constructor 0x00ABD080 and WriteToFile 0x00ABC390. Shared chunk/stream storage headers are reused.
// NF64 (round 7, q10639): the local CArray<T> stand-in replaced by the shared engine/CArray.h (CArray<T> : std::vector<T>; Ego_r.pdb
//   MaterialIndices is CArray<long>); the inherited resize is kept out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE) and binds
//   ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0 (was CArray<int>::Resize).
#include "fable_compiler.h"
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CInputStreamAccess   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
#include "engine/CChunkedFileChunk.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CInputStreamVirtuals { virtual void v0(); virtual void v1(); virtual unsigned long GetPosition(); };
struct CInputStreamAccess : CDataInputStream {
 unsigned long GetPosition() { return ((CInputStreamVirtuals*)this)->GetPosition(); }
 static FABLE_FORCEINLINE bool IsValidPosition(__int64 pos) {return pos>=0 && pos<=0x7fffffff;}
 FABLE_FORCEINLINE void Read(void* buffer,long size) {
  __int64 newPosition=(__int64)(StreamPos+size);
  if(!IsValidPosition(newPosition)) return;
  if(SourceChunkBytesRemaining>=size) {
   memcpy(buffer,CurrentSourcePtr,size);
   CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
   SourceChunkBytesRemaining-=size;
   StreamPos+=size;
  } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
 }
 FABLE_FORCEINLINE int ReadLong() {int value;Read(&value,4);return value;}
};
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CArray.h"
struct CCountedChunk {void* Object;void* Count;};
class CChunkedFileLoader;
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
 CArray<CCountedChunk> SubChunks;
 CArray<CCountedChunk> RegisteredSubChunkTypes;
 unsigned long CurrentChunkIndex;
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&); // 0x00AA8830
};
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
 CArray<long> MaterialIndices;   // Ego_r.pdb CArray<long>
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream& input,unsigned long chunkSize,CChunkedFileLoader& loader) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 unsigned long start=in.GetPosition();
 int index=in.ReadLong();
 if(index!=-1 && (index&0x80000000)!=0) {
  int count=index&0x7fffffff;
  MaterialIndices.resize(count,0);
  for(int i=0;i<count;++i) MaterialIndices[i]=in.ReadLong();
 } else {
  MaterialIndices.resize(1,0);
  MaterialIndices[0]=index;
 }
 unsigned long headerSize=in.GetPosition()-start;
 CChunkedFileSuperChunk::ReadFromFile(input,chunkSize-headerSize,loader);
}