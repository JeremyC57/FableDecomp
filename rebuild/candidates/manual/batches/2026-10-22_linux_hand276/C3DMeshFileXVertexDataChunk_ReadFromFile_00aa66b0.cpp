// C3DMeshFileXVertexDataChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the vertex format header, compression params and raw vertex bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA66B0, length 592 bytes (virtual, chunk reader slot).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined stream Read's slow path),
//          ReadFromFile(CVertexCompressionParams&, CDataInputStream&) @ 0x00AA5460 (__fastcall free reader),
//          vector<char>::resize(n, const char&) @ 0x004122D0 (label propagated as vector<CEditRegion>).
// Shape: a version dword (kept in the frame), then FormatID/VertexCount/VertexSize/InitFlags through by-value
// ReadULong helpers (the helper local reuses the dead stream-argument slot); version >= 2 adds the
// compression params; the vertex byte array is resized to Count*Size (zero fill char in the same slot)
// and filled with one bulk Read of that byte count into GetData() (the array itself when empty).
// Layout: engine/C3DMeshFileXVertexDataChunk.h; VertexData +0x40 viewed as the retail 12-byte STLport vector.
// Leaf: ReadFromFile@C3DMeshFileXVertexDataChunkMethods
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileXVertexDataChunk.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CInputStreamAccess : CDataInputStream {
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
 __forceinline unsigned long ReadULong() {unsigned long value;Read(&value,4);return value;}
};
template<class T> struct CArray {
 T* Begin;T* End;T* Capacity;
 unsigned long Size()const{return (unsigned long)(End-Begin);}
 T* GetData(){return Size()>0?Begin:(T*)this;}
 void Resize(unsigned long,const T&); // 0x004122D0
};
struct CVertexCompressionParams;
void __fastcall ReadFromFile(CVertexCompressionParams& params, CDataInputStream& in); // 0x00AA5460
struct CChunkedFileLoader;
struct C3DMeshFileXVertexDataChunkMethods : C3DMeshFileXVertexDataChunk {
 CArray<char>& Data(){return *(CArray<char>*)VertexData;}
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFileXVertexDataChunkMethods::ReadFromFile(CDataInputStream& input,unsigned long,CChunkedFileLoader&) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 unsigned long version=in.ReadULong();
 FormatID=in.ReadULong();
 VertexCount=in.ReadULong();
 VertexSize=in.ReadULong();
 InitFlags=in.ReadULong();
 if(version>=2)
  ::ReadFromFile(*(CVertexCompressionParams*)&VBCParams_PosScaleX,input);
 long bytes=VertexCount*VertexSize;
 Data().Resize(bytes,0);
 in.Read(Data().GetData(),bytes);
}
