#include <stdio.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#define __int64 long long
// Host storage adapters only. VC7.1 compiles the real shared headers included below.
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
struct CDataInputStream {
 void* __vftable; unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr;
 unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;
};
#define FABLE_ENGINE_C3DMESHFILEVERTICESCHUNK_H
struct C3DMeshFileVerticesChunk {
 void* __vftable; unsigned char prefix[12];
 union {void* align; unsigned char VertexList[3*sizeof(void*)];};
};
#endif
// C3DMeshFileVerticesChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- resize and bulk-read the packed vertex array
// Retail 0x00ABD150, 289 bytes; ReadWithSrcChunkOverflow 0x00993CA0, erase 0x00A96A90, insert 0x00A97040.
// Shared target header gives VertexList +0x10. Only its first three pointer slots are used (retail12 vs donor16).
// PDB C3DVertex2 has Position +0, Normal +12, U +24, V +28, total32; default construction leaves fields uninitialized.
// Leaf: ReadFromFile@C3DMeshFileVerticesChunkMethods
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileVerticesChunk.h"
struct C3DVector {float X,Y,Z;}; // no generated C3DVector header exists
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

struct C3DVertex2 {C3DVector Position,Normal;float U,V;C3DVertex2() {}};
template<class T> struct CArray {
 T* Begin;T* End;T* Capacity;
 unsigned long Size()const{return (unsigned long)(End-Begin);}
 T* GetData(){return Size()>0?Begin:(T*)this;}
 void Erase(T*,T*); // 0x00A96A90
 void Insert(T*,unsigned long,const T&); // 0x00A97040
 void Resize(unsigned long count,const T& value=T()) {
  if(count<Size()) Erase(Begin+count,End);
  else Insert(End,count-Size(),value);
 }
};
struct CChunkedFileLoader;
struct C3DMeshFileVerticesChunkMethods : C3DMeshFileVerticesChunk {
 CArray<C3DVertex2>& Vertices(){return *(CArray<C3DVertex2>*)VertexList;}
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFileVerticesChunkMethods::ReadFromFile(CDataInputStream& input,unsigned long,CChunkedFileLoader&) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 unsigned long count=in.ReadULong();
 Vertices().Resize(count);
 in.Read(Vertices().GetData(),Vertices().Size()*sizeof(C3DVertex2));
}


struct CChunkedFileLoader {};
static int errors,slowCalls,eraseCalls,insertCalls;
static CInputStreamAccess* expectedStream;
static CArray<C3DVertex2>* expectedArray;
static void Check(bool b) {if(!b)++errors;}
void CInputStreamAccess::ReadWithSrcChunkOverflow(unsigned char* out,long size) {
 Check(this==expectedStream);Check(size>0);++slowCalls;
 memcpy(out,CurrentSourcePtr,(unsigned int)size);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
 StreamPos+=size;SourceChunkBytesRemaining=0;
}
template<class T> void CArray<T>::Erase(T* first,T* last) {
 Check(this==expectedArray);Check(last==End);Check(first>=Begin && first<=End);++eraseCalls;End=first;
}
template<class T> void CArray<T>::Insert(T* where,unsigned long count,const T&) {
 Check(this==expectedArray);Check(where==End);++insertCalls;
 unsigned long oldSize=Size(); T* data=new T[oldSize+count];
 for(unsigned long i=0;i<oldSize;++i)data[i]=Begin[i];
 delete[] Begin;Begin=data;End=Capacity=data+oldSize+count;
}
static void Fill(C3DVertex2& v,float n) {
 v.Position.X=n;v.Position.Y=n+1;v.Position.Z=n+2;
 v.Normal.X=n+3;v.Normal.Y=n+4;v.Normal.Z=n+5;v.U=n+6;v.V=n+7;
}
static void Run(unsigned int count,int prior,long available,int slowExpected) {
 C3DVertex2 values[3];for(int i=0;i<3;++i)Fill(values[i],float(i*10)-15.5f);
 unsigned char bytes[4+3*sizeof(C3DVertex2)];memcpy(bytes,&count,4);memcpy(bytes+4,values,count*sizeof(C3DVertex2));
 C3DMeshFileVerticesChunkMethods chunk; CInputStreamAccess stream;CChunkedFileLoader loader;
 CArray<C3DVertex2>& vertices=chunk.Vertices();vertices.Begin=new C3DVertex2[prior];vertices.End=vertices.Capacity=vertices.Begin+prior;
 for(int j=0;j<prior;++j)Fill(vertices.Begin[j],900.0f+j);
 stream.StreamPos=30;stream.StreamSize=999;stream.CurrentSourcePtr=bytes;stream.SourceChunkBytesRemaining=available;
 expectedStream=&stream;expectedArray=&vertices;slowCalls=eraseCalls=insertCalls=0;
 chunk.ReadFromFile(stream,4+count*sizeof(C3DVertex2),loader);
 Check(vertices.Size()==count);Check(slowCalls==slowExpected);
 Check(eraseCalls==(count<(unsigned int)prior?1:0));Check(insertCalls==(count>=(unsigned int)prior?1:0));
 Check(stream.StreamPos==30+4+count*sizeof(C3DVertex2));
 Check(stream.CurrentSourcePtr==bytes+4+count*sizeof(C3DVertex2));
 Check(stream.SourceChunkBytesRemaining==(slowExpected?0:available-4-count*sizeof(C3DVertex2)));
 for(unsigned int k=0;k<count;++k) {
  const unsigned char* actual=(const unsigned char*)&vertices.Begin[k];const unsigned char* wanted=(const unsigned char*)&values[k];
  for(unsigned int n=0;n<sizeof(C3DVertex2);++n)Check(actual[n]==wanted[n]);
 }
 delete[] vertices.Begin;
}
int main() {
 Run(3,1,100,0);Run(1,3,36,0);Run(2,2,4,1);Run(2,0,0,2);Run(0,2,4,0);Run(0,0,0,1);
 printf("00abd150_TEST %s\n",errors?"FAIL":"PASS");return errors?1:0;
}