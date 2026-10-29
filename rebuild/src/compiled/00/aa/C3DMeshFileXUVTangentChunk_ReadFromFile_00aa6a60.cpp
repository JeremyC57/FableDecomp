// C3DMeshFileXUVTangentChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the U and V tangent arrays
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA6A60, length 528 bytes (virtual, chunk reader slot).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined stream Read's slow path),
//          vector<C3DVector>::resize(n, const C3DVector&) @ 0x0075C040 (label propagated as vector<CBrainBehaviour>).
// Shape: an unused version dword, then the tangent count; both arrays are resized to the count with an
// uninitialised C3DVector fill value (the 12-byte frame), then each array's data is read in one bulk
// Read of Size()*12 bytes into GetData() (the array itself when empty, as in the landed TriangleStrip reader).
// Layout: UTangents +0x10 and VTangents +0x1C: retail STLport vectors are 12 bytes, the generated header's
// 0x10 CArray is too large (retail adds 0x1C for the second array), so the arrays are viewed through
// a three-pointer CArray model over the header's storage.
// Leaf: ReadFromFile@C3DMeshFileXUVTangentChunkMethods
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CInputStreamAccess   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileXUVTangentChunk.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CInputStreamAccess : CDataInputStream {
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
class C3DVector { public: float X, Y, Z; C3DVector() {} };
template<class T> class CArray { public:
 T* Begin;T* End;T* Capacity;
 unsigned long Size()const{return (unsigned long)(End-Begin);}
 T* GetData(){return Size()>0?Begin:(T*)this;}
 void Resize(unsigned long,const T&); // 0x0075C040
};
class CChunkedFileLoader;
struct C3DMeshFileXUVTangentChunkMethods : C3DMeshFileXUVTangentChunk {
 CArray<C3DVector>& U(){return *(CArray<C3DVector>*)UTangents;}
 CArray<C3DVector>& V(){return *(CArray<C3DVector>*)(UTangents+12);}
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void C3DMeshFileXUVTangentChunkMethods::ReadFromFile(CDataInputStream& input,unsigned long,CChunkedFileLoader&) {
 CInputStreamAccess& in=(CInputStreamAccess&)input;
 in.ReadULong();
 unsigned long count=in.ReadULong();
 U().Resize(count,C3DVector());
 V().Resize(count,C3DVector());
 in.Read(U().GetData(),U().Size()*sizeof(C3DVector));
 in.Read(V().GetData(),V().Size()*sizeof(C3DVector));
}