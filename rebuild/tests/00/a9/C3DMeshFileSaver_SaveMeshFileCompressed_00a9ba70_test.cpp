// 00a9ba70 C3DMeshFileSaver::SaveMeshFileCompressed
// Flags: /O2 /Oy /W3
// Leaf: SaveMeshFileCompressed
#define FABLE_RETAIL_MEMORY_OUTPUT_STREAM
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CDataOutputStream.h"
#include <stdlib.h>
#include <string.h>
namespace X_00a9ba70{
// Layout: active retail opt-in header owns all24 bytes. Method-only adapters add no fields. Actual base storage is8 bytes; virtual slot2 position, slot4 write.
struct CDataOutputStreamSlots{virtual void Slot0()=0;virtual void Slot1()=0;virtual unsigned long GetPosition()const=0;virtual void Slot3()=0;virtual void Write(const void*,long)=0;};
struct CDataOutputStreamMethods: ::CDataOutputStream{void WriteULONG(unsigned long);__forceinline unsigned long GetPosition()const{return ((const CDataOutputStreamSlots*)this)->GetPosition();}__forceinline void Write(const void*p,long n){((CDataOutputStreamSlots*)this)->Write(p,n);}};
struct CMemoryDataOutputStreamMethods: ::CMemoryDataOutputStream{CMemoryDataOutputStreamMethods();~CMemoryDataOutputStreamMethods();unsigned long GetLength()const;const void*PeekData()const;};
// Layout: retail CArray<char> has three pointers, copies bytes and releases storage through free.
template<class T>__forceinline T*uninitialized_fill_n(T*first,unsigned n,const T&v){memset(first,v,n*sizeof(T));return first+n;}
template<class T>struct CArray{T*Start,*Finish,*Storage;__forceinline CArray(unsigned n):Start(0),Finish(0),Storage(0){Start=n?(T*)malloc(n*sizeof(T)):0;Storage=Start+n;Finish=uninitialized_fill_n(Start,n,T());}__forceinline ~CArray(){if(Start)free(Start);}__forceinline T*GetData(){return Finish-Start?Start:(T*)this;}};
struct CChunkedFile{};struct C3DMeshFile:CChunkedFile{};
struct CChunkedFileSaver{unsigned long SaveChunkedFile(CChunkedFile&,::CDataOutputStream&);};
struct C3DMeshFileSaver:CChunkedFileSaver{unsigned long SaveMeshFileCompressed(C3DMeshFile&,::CDataOutputStream&);};
extern "C" int __cdecl lzo1x_999_compress(const void*,unsigned long,void*,unsigned long*,void*);
unsigned long C3DMeshFileSaver::SaveMeshFileCompressed(C3DMeshFile&file,::CDataOutputStream&stream){
 CMemoryDataOutputStreamMethods mem;SaveChunkedFile(file,(::CDataOutputStream&)mem);
 unsigned long length=mem.GetLength();unsigned long compressedSize=(length*65>>6)+128;
 CArray<char>compressed(compressedSize);CArray<char>work(0x70000);
 lzo1x_999_compress(mem.PeekData(),length,compressed.GetData(),&compressedSize,work.GetData());
 CDataOutputStreamMethods&out=(CDataOutputStreamMethods&)stream;
 unsigned long first=out.GetPosition();out.WriteULONG(length);out.Write(compressed.GetData(),compressedSize);return out.GetPosition()-first;
}
}

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
namespace X_00a9ba70{
static unsigned inputLength;static unsigned compressionCalls,saverCalls,destroyed;static unsigned char input[32];
CMemoryDataOutputStreamMethods::CMemoryDataOutputStreamMethods(){memset(this,0,sizeof(*this));}
CMemoryDataOutputStreamMethods::~CMemoryDataOutputStreamMethods(){char**p=(char**)Data;free(p[0]);++destroyed;}
unsigned long CMemoryDataOutputStreamMethods::GetLength()const{char*const*p=(char*const*)Data;return p[1]-p[0];}
const void*CMemoryDataOutputStreamMethods::PeekData()const{char*const*p=(char*const*)Data;return p[0]!=p[1]?p[0]:(const char*)Data;}
unsigned long CChunkedFileSaver::SaveChunkedFile(CChunkedFile&,::CDataOutputStream&out){++saverCalls;CMemoryDataOutputStreamMethods&mem=(CMemoryDataOutputStreamMethods&)out;char**p=(char**)mem.Data;p[0]=(char*)malloc(inputLength);p[1]=p[0]+inputLength;p[2]=p[1];memcpy(p[0],input,inputLength);mem.CurrentPosition=inputLength;return inputLength;}
int __cdecl lzo1x_999_compress(const void*src,unsigned long n,void*dst,unsigned long*size,void*work){++compressionCalls;assert(n==inputLength&&memcmp(src,input,n)==0&&*size==(n*65>>6)+128);for(unsigned i=0;i<0x70000;++i)assert(((unsigned char*)work)[i]==0);for(unsigned j=0;j<n;++j)((char*)dst)[j]=((const char*)src)[n-j-1];*size=n;return 0;}
void CDataOutputStreamMethods::WriteULONG(unsigned long n){Write(&n,4);}
struct OutputFixture:CDataOutputStreamSlots{unsigned char data[128];unsigned pos;OutputFixture():pos(7){memset(data,0,sizeof(data));}void Slot0(){}void Slot1(){}unsigned long GetPosition()const{return pos;}void Slot3(){}void Write(const void*p,long n){assert(pos+n<=sizeof(data));memcpy(data+pos,p,n);pos+=n;}};
}
int main(){using namespace X_00a9ba70;assert(sizeof(::CMemoryDataOutputStream)==24&&sizeof(CMemoryDataOutputStreamMethods)==24&&offsetof(::CMemoryDataOutputStream,CurrentPosition)==20);for(unsigned k=0;k<2;++k){inputLength=k?13:0;for(unsigned i=0;i<inputLength;++i)input[i]=(unsigned char)(i+1);C3DMeshFileSaver saver;C3DMeshFile file;OutputFixture out;unsigned before=destroyed;assert(saver.SaveMeshFileCompressed(file,(::CDataOutputStream&)out)==4+inputLength&&destroyed==before+1);unsigned saved;memcpy(&saved,out.data+7,4);assert(saved==inputLength);for(unsigned j=0;j<inputLength;++j)assert(out.data[11+j]==input[inputLength-j-1]);}assert(compressionCalls==2&&saverCalls==2);puts("00a9ba70_TEST PASS");return 0;}