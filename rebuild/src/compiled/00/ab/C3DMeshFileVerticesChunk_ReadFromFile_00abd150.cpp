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