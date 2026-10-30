// CDataInputStream::ReadLine() -- read until CR/LF/NUL; CR consumes following LF or seeks back one character
// Retail00994B50,289 bytes. Calls default narrow string99E4B0, appendchar99F180, copy99EC30, destructor99EAE0, overflow993CA0.
// Actual fable_string.h lacks copy/append API. CStringReadResult adds only these genuine native operations, NO storage.
// Native hidden result at ret4 is one pointer-sized string; copy constructs it from local before actual CCharString cleanup.
// The field-free adapter preserves actual CCharString ownership/storage; narrow no-elision portability is not claimed.
// Leaf: ReadLine
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#include "fable_string.h"
#include <string.h>
#pragma intrinsic(memcpy)
struct CStringReadResult : CCharString {
    CStringReadResult():CCharString() {}
    CStringReadResult(const CStringReadResult&); //0099EC30 native CCharString copy constructor
    CCharString& AppendChar(char); //0099F180 native operator+=(char); return unused
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
struct CDataStream : CBaseClassNonCopyable {
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
};
class CDataInputStream : public CDataStream { public:
    unsigned long StreamPos;
    long StreamSize;
    void* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
    static __forceinline bool IsValidPosition(__int64 p) { return p>=0 && p<=0x7fffffff; }
    __forceinline void Read(void* dest,long n) {
        __int64 newPosition=(__int64)(StreamPos+n);
        if(!IsValidPosition(newPosition)) return;
        if(SourceChunkBytesRemaining>=n) {
            memcpy(dest,CurrentSourcePtr,n);
            CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+n;
            SourceChunkBytesRemaining-=n;
            StreamPos+=n;
        } else ReadWithSrcChunkOverflow((unsigned char*)dest,n);
    }
    __forceinline char ReadChar() { char value; Read(&value,1); return value; }
    CStringReadResult ReadLine();
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
CStringReadResult CDataInputStream::ReadLine() {
    CStringReadResult result;
    while(StreamPos!=(unsigned long)StreamSize) {
        char c=ReadChar();
        if(c=='\r') {
            if(StreamPos!=(unsigned long)StreamSize) {
                char next=ReadChar();
                if(next!='\n') SetPosition(GetPosition()-1);
            }
            break;
        }
        if(c=='\n'||c==0) break;
        result.AppendChar(c);
    }
    return result;
}