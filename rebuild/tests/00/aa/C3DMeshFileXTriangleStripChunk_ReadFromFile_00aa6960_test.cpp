#include <stdio.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#define __int64 long long
// Host storage adapters only. VC7.1 compiles the real shared headers included below.
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
class CDataInputStream { public:
 void* __vftable; unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr;
 unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;
};
#define FABLE_ENGINE_C3DMESHFILEXTRIANGLESTRIPCHUNK_H
class C3DMeshFileXTriangleStripChunk { public:
 void* __vftable; unsigned char prefix[12];
 union {void* align; unsigned char StripVertices[3*sizeof(void*)];};
};
#endif
// C3DMeshFileXTriangleStripChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read a counted ushort triangle-strip index buffer
// Retail 0x00AA6960, 246 bytes; overflow reader 0x00993CA0, vector<ushort>::resize 0x00517900.
// Shared target header has StripVertices +0x10; use retail three-pointer vector prefix only.
// Writer 0x00AA5930 and resize callee confirm two-byte indices and array-as-empty-list sentinel.
// Leaf: ReadFromFile@C3DMeshFileXTriangleStripChunkMethods
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileXTriangleStripChunk.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CInputStreamVirtuals { virtual void v0(); virtual void v1(); virtual unsigned long GetPosition(); };
struct CInputStreamAccess : CDataInputStream {
 unsigned long GetPosition() { return ((CInputStreamVirtuals*)this)->GetPosition(); }
 void ReadWithSrcChunkOverflow(unsigned char*, long); // 0x00993CA0
 static __forceinline bool IsValidPosition(__int64 pos) {return pos>=0 && pos<=0x7fffffff;}
 __forceinline void Read(void* buffer,long size) {
  if(size<=0) return;
  __int64 newPosition=(__int64)(StreamPos+size);
  if(!IsValidPosition(newPosition)) return;
  if(size<=SourceChunkBytesRemaining) {
   memcpy(buffer,CurrentSourcePtr,size);
   CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
   SourceChunkBytesRemaining-=size;
   StreamPos+=size;
  } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
 }
 __forceinline unsigned int ReadULong() {unsigned int value;Read(&value,4);return value;}
};


template<class T> class CArray { public:
 T* Begin;T* End;T* Capacity;
 unsigned long Size()const{return (unsigned long)(End-Begin);}
 T* GetData(){return Size()>0?Begin:(T*)this;}
 void Resize(unsigned long,const T&); // 0x00517900
};
class CChunkedFileLoader;
struct C3DMeshFileXTriangleStripChunkMethods : C3DMeshFileXTriangleStripChunk {
 CArray<unsigned short>& Indices(){return *(CArray<unsigned short>*)StripVertices;}
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFileXTriangleStripChunkMethods::ReadFromFile(CDataInputStream& input,unsigned long,CChunkedFileLoader&) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 unsigned long count=in.ReadULong();
 Indices().Resize(count,0);
 in.Read(Indices().GetData(),Indices().Size()*sizeof(unsigned short));
}


class CChunkedFileLoader { public:};
static int errors,slowCalls,resizeCalls;
static CInputStreamAccess* expectedStream;
static CArray<unsigned short>* expectedArray;
static void Check(bool b){if(!b)++errors;}
void CInputStreamAccess::ReadWithSrcChunkOverflow(unsigned char* out,long size) {
 Check(this==expectedStream);Check(size>0);++slowCalls;
 memcpy(out,CurrentSourcePtr,(unsigned int)size);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
 StreamPos+=size;SourceChunkBytesRemaining=0;
}
template<class T> void CArray<T>::Resize(unsigned long count,const T& fill) {
 Check(this==expectedArray);Check(fill==0);++resizeCalls;
 delete[] Begin;Begin=new T[count];End=Capacity=Begin+count;
 for(unsigned long i=0;i<count;++i)Begin[i]=fill;
}
static void Run(unsigned int count,long available,int slowExpected) {
 unsigned short values[]={0,65535,17,40000,3};unsigned char bytes[14];
 memcpy(bytes,&count,4);memcpy(bytes+4,values,count*2);
 C3DMeshFileXTriangleStripChunkMethods chunk;CInputStreamAccess stream;CChunkedFileLoader loader;
 CArray<unsigned short>& indices=chunk.Indices();indices.Begin=new unsigned short[2];indices.End=indices.Capacity=indices.Begin+2;
 stream.StreamPos=40;stream.StreamSize=900;stream.CurrentSourcePtr=bytes;stream.SourceChunkBytesRemaining=available;
 expectedStream=&stream;expectedArray=&indices;slowCalls=resizeCalls=0;
 chunk.ReadFromFile(stream,4+count*2,loader);
 Check(indices.Size()==count && resizeCalls==1 && slowCalls==slowExpected);
 Check(stream.StreamPos==44+count*2 && stream.CurrentSourcePtr==bytes+4+count*2);
 Check(stream.SourceChunkBytesRemaining==(slowExpected?0:available-4-count*2));
 for(unsigned int i=0;i<count;++i)Check(indices.Begin[i]==values[i]);
 delete[] indices.Begin;
}
int main(){Run(5,14,0);Run(3,4,1);Run(2,0,2);Run(0,4,0);Run(0,0,1);printf("00aa6960_TEST %s\n",errors?"FAIL":"PASS");return errors?1:0;}