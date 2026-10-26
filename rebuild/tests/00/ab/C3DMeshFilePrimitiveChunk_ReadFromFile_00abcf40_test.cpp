#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#define __int64 long long
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CCHUNKEDFILECHUNK_H
class CDataInputStream { public:void* __vftable;unsigned int StreamPos;int StreamSize;void* CurrentSourcePtr;unsigned int SourceChunkStreamPos;int SourceChunkBytesRemaining;};
class CChunkedFileChunk { public:void* vptr;};
#endif
// C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read one index or a high-bit-tagged index array, then its subchunks
// Retail 0x00ABCF40, 318 bytes. ReadWithSrcChunkOverflow 0x00993CA0; Resize 0x004428F0; base reader 0x00AA8830.
// engine/C3DMeshFilePrimitiveChunk.h and CChunkedFileSuperChunk.h use donor 16-byte vectors:
// MaterialIndices +0x34 vs retail +0x2c. The retail 12-byte vector/base model below agrees
// with landed constructor 0x00ABD080 and WriteToFile 0x00ABC390. Shared chunk/stream storage headers are reused.
#include "engine/CDataInputStream.h"
#include "engine/CChunkedFileChunk.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CInputStreamVirtuals { virtual void v0(); virtual void v1(); virtual unsigned long GetPosition(); };
struct CInputStreamAccess : CDataInputStream {
 unsigned long GetPosition() { return ((CInputStreamVirtuals*)this)->GetPosition(); }
 void ReadWithSrcChunkOverflow(unsigned char*, long); // 0x00993CA0
 static __forceinline bool IsValidPosition(__int64 pos) {return pos>=0 && pos<=0x7fffffff;}
 __forceinline void Read(void* buffer,long size) {
  __int64 newPosition=(__int64)(StreamPos+size);
  if(!IsValidPosition(newPosition)) return;
  if(SourceChunkBytesRemaining>=size) {
   memcpy(buffer,CurrentSourcePtr,size);
   CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
   SourceChunkBytesRemaining-=size;
   StreamPos+=size;
  } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
 }
 __forceinline int ReadLong() {int value;Read(&value,4);return value;}
};
template<class T> class CArray { public:
 T* Begin; T* End; T* EndOfStorage;
 void Resize(unsigned long,const T&); // material-index specialization 0x004428F0
 T& operator[](long i) {return Begin[i];}
};
struct CCountedChunk {void* Object;void* Count;};
class CChunkedFileLoader;
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
 CArray<CCountedChunk> SubChunks;
 CArray<CCountedChunk> RegisteredSubChunkTypes;
 unsigned long CurrentChunkIndex;
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&); // 0x00AA8830
};
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
 CArray<int> MaterialIndices;
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream& input,unsigned long chunkSize,CChunkedFileLoader& loader) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 unsigned long start=in.GetPosition();
 int index=in.ReadLong();
 if(index!=-1 && (index&0x80000000)!=0) {
  int count=index&0x7fffffff;
  MaterialIndices.Resize(count,0);
  for(int i=0;i<count;++i) MaterialIndices[i]=in.ReadLong();
 } else {
  MaterialIndices.Resize(1,0);
  MaterialIndices[0]=index;
 }
 unsigned long headerSize=in.GetPosition()-start;
 CChunkedFileSuperChunk::ReadFromFile(input,chunkSize-headerSize,loader);
}

class CChunkedFileLoader { public:};
static int errors, baseCalls, slowCalls, posCalls, resizeCalls;
static CDataInputStream* expectedInput;
static CChunkedFileLoader* expectedLoader;
static C3DMeshFilePrimitiveChunk* expectedChunk;
static unsigned long expectedRemaining, expectedEnd;
static void Check(bool b) { if (!b) ++errors; }
void CInputStreamVirtuals::v0() {} void CInputStreamVirtuals::v1() {}
unsigned long CInputStreamVirtuals::GetPosition() {
 Check((CDataInputStream*)this == expectedInput); ++posCalls;
 return ((CDataInputStream*)this)->StreamPos;
}
void CInputStreamAccess::ReadWithSrcChunkOverflow(unsigned char* out,long size) {
 Check(this == expectedInput); Check(size == 4); ++slowCalls;
 memcpy(out,CurrentSourcePtr,4);
 CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+4;
 StreamPos+=4;
 SourceChunkBytesRemaining=0;
}
template<class T> void CArray<T>::Resize(unsigned long count,const T& fill) {
 Check(this == &expectedChunk->MaterialIndices); Check(fill == 0); ++resizeCalls;
 delete[] Begin; Begin=new T[count]; End=EndOfStorage=Begin+count;
 for(unsigned long i=0;i<count;++i) Begin[i]=fill;
}
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& in,unsigned long size,CChunkedFileLoader& loader) {
 Check(this==expectedChunk); Check(&in==expectedInput); Check(&loader==expectedLoader);
 Check(size==expectedRemaining); Check(in.StreamPos==expectedEnd); ++baseCalls;
}
static void Run(const int* data,int words,long initialAvailable,const int* values,int count,int slowExpected) {
 C3DMeshFilePrimitiveChunk chunk; CInputStreamAccess stream; CChunkedFileLoader loader;
 CInputStreamVirtuals methods;
 stream.__vftable=*(void**)&methods; stream.StreamPos=100; stream.StreamSize=1000;
 stream.CurrentSourcePtr=(void*)data; stream.SourceChunkStreamPos=100;
 stream.SourceChunkBytesRemaining=initialAvailable;
 chunk.MaterialIndices.Begin=chunk.MaterialIndices.End=chunk.MaterialIndices.EndOfStorage=0;
 expectedChunk=&chunk;expectedInput=&stream;expectedLoader=&loader;expectedRemaining=11;expectedEnd=100+words*4;
 baseCalls=slowCalls=posCalls=resizeCalls=0;
 chunk.ReadFromFile(stream,words*4+11,loader);
 Check(baseCalls==1 && resizeCalls==1 && posCalls==2 && slowCalls==slowExpected);
 Check(chunk.MaterialIndices.End-chunk.MaterialIndices.Begin==count);
 for(int i=0;i<count;++i) Check(chunk.MaterialIndices[i]==values[i]);
 Check(stream.CurrentSourcePtr==(const void*)(data+words));
 Check(stream.SourceChunkBytesRemaining==(slowExpected?0:initialAvailable-words*4));
 delete[] chunk.MaterialIndices.Begin;
}
int main() {
 int one[]={7}; int sentinel[]={-1}; int maximum[]={0x7fffffff}; int empty[]={int(0x80000000U)};
 int multiple[]={int(0x80000003U),-1,29,-200}; int expected[]={-1,29,-200};
 Run(one,1,4,one,1,0);Run(sentinel,1,0,sentinel,1,1);Run(maximum,1,4,maximum,1,0);
 Run(empty,1,4,empty,0,0);Run(multiple,4,16,expected,3,0);Run(multiple,4,4,expected,3,3);Run(multiple,4,0,expected,3,4);
 printf("00abcf40_TEST %s\n",errors?"FAIL":"PASS");return errors?1:0;
}