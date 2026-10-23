// 00a99840 C3DAnimFileSaver::SaveAnimFileCompressed
// Flags: /O2 /Oy /W3
// Leaf: SaveAnimFileCompressed
#define FABLE_RETAIL_MEMORY_OUTPUT_STREAM
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CDataOutputStream.h"
#include <stdlib.h>
#include <string.h>
namespace X_00a99840{
// Layout: active retail opt-in header owns all24 bytes. Method-only adapters add no fields. Actual base storage is8 bytes; virtual slot2 position, slot4 write.
struct CDataOutputStreamSlots{virtual void Slot0()=0;virtual void Slot1()=0;virtual unsigned long GetPosition()const=0;virtual void Slot3()=0;virtual void Write(const void*,long)=0;};
struct CDataOutputStreamMethods: ::CDataOutputStream{void WriteULONG(unsigned long);__forceinline unsigned long GetPosition()const{return ((const CDataOutputStreamSlots*)this)->GetPosition();}__forceinline void Write(const void*p,long n){((CDataOutputStreamSlots*)this)->Write(p,n);}};
struct CMemoryDataOutputStreamMethods: ::CMemoryDataOutputStream{CMemoryDataOutputStreamMethods();~CMemoryDataOutputStreamMethods();unsigned long GetLength()const;const void*PeekData()const;};
// Layout: retail CArray<char> has three pointers, copies bytes and releases storage through free.
template<class T>__forceinline T*uninitialized_fill_n(T*first,unsigned n,const T&v){memset(first,v,n*sizeof(T));return first+n;}
template<class T>struct CArray{T*Start,*Finish,*Storage;__forceinline CArray(unsigned n):Start(0),Finish(0),Storage(0){Start=n?(T*)malloc(n*sizeof(T)):0;Storage=Start+n;Finish=uninitialized_fill_n(Start,n,T());}__forceinline ~CArray(){if(Start)free(Start);}__forceinline T*GetData(){return Finish-Start?Start:(T*)this;}};
struct CChunkedFile{};struct C3DAnimFile:CChunkedFile{};
struct CChunkedFileSaver{unsigned long SaveChunkedFile(CChunkedFile&,::CDataOutputStream&);};
struct C3DAnimFileSaver:CChunkedFileSaver{unsigned long SaveAnimFileCompressed(C3DAnimFile&,::CDataOutputStream&);};
extern "C" int __cdecl lzo1x_999_compress(const void*,unsigned long,void*,unsigned long*,void*);
unsigned long C3DAnimFileSaver::SaveAnimFileCompressed(C3DAnimFile&file,::CDataOutputStream&stream){
 CMemoryDataOutputStreamMethods mem;SaveChunkedFile(file,(::CDataOutputStream&)mem);
 unsigned long length=mem.GetLength();unsigned long compressedSize=(length*65>>6)+128;
 CArray<char>compressed(compressedSize);CArray<char>work(0x70000);
 lzo1x_999_compress(mem.PeekData(),length,compressed.GetData(),&compressedSize,work.GetData());
 CDataOutputStreamMethods&out=(CDataOutputStreamMethods&)stream;
 unsigned long first=out.GetPosition();out.WriteULONG(length);out.Write(compressed.GetData(),compressedSize);return out.GetPosition()-first;
}
}