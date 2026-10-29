// CDataInputStream::ReadWithChunking(CDataOutputStream&, unsigned long) -- copy at most 64 KiB at a time through a zero-initialised byte vector
// Retail 0x009964E0, 295 bytes. Read overflow 0x00993CA0; malloc 0x00BFEA0E, free 0x00BFEA14.
// Production input layout supplies all state; output polymorphic slots follow CDataStream and 00994070 audit.
// Leaf: ReadWithChunking
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream
#include "fable_string.h"
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(memcpy, memset)
class CBaseClass { public: virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
struct CDataStream : CBaseClassNonCopyable {
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
};
class CDataOutputStream : public CDataStream { public:
    virtual ~CDataOutputStream();
    CCharString NewLine;
    virtual void Write(const void*, long)=0;
};
template<class T> inline T Min(T a,T b) { return a<b?a:b; }
inline unsigned char* FillBytes(unsigned char* p,unsigned long n) { memset(p,0,n); return p+n; }
struct ByteAllocator {
    ByteAllocator() {}
    unsigned char* allocate(unsigned long n) { return n ? (unsigned char*)malloc(n) : 0; }
};
struct ByteAllocProxy : ByteAllocator {
    unsigned char* Data;
    ByteAllocProxy(const ByteAllocator& a,unsigned char* p):ByteAllocator(a),Data(p) {}
};
struct ByteVectorBase {
    unsigned char *Start,*Finish;
    ByteAllocProxy End;
    ByteVectorBase(unsigned long n,const ByteAllocator& a):Start(0),Finish(0),End(a,0) {
        Start=End.allocate(n); Finish=Start; End.Data=Start+n;
    }
    ~ByteVectorBase() { if(Start) free(Start); }
};
struct ByteVector : ByteVectorBase {
    ByteVector(unsigned long n):ByteVectorBase(n,ByteAllocator()) {
        Finish=FillBytes(Start,n);
    }
    unsigned long size() const { return (unsigned long)(Finish-Start); }
    unsigned char* data() { return size()>0 ? Start : (unsigned char*)this; }
};
struct CDataInputStreamReader : CDataInputStream {
    static __forceinline bool IsValidPosition(__int64 p) { return p>=0 && p<=0x7fffffff; }
    __forceinline void Read(void* dest,long n) {
        if(n>0) {
            __int64 newPosition=(__int64)(StreamPos+n);
            if(!IsValidPosition(newPosition)) return;
            if(n<=SourceChunkBytesRemaining) {
                memcpy(dest,CurrentSourcePtr,n);
                CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+n;
                SourceChunkBytesRemaining-=n;
                StreamPos+=n;
            } else ReadWithSrcChunkOverflow((unsigned char*)dest,n);
        }
    }
    void ReadWithChunking(CDataOutputStream& output,unsigned long n);
};
void CDataInputStreamReader::ReadWithChunking(CDataOutputStream& output,unsigned long n) {
    ByteVector bytes(Min(n,65536UL));
    while(n) {
        unsigned long count=n<bytes.size() ? n : bytes.size();
        Read(bytes.data(),count);
        output.Write(bytes.data(),count);
        n-=count;
    }
}