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