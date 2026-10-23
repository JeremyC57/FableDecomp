// test for 00993b20 CDataInputStream ctor
#include <stdio.h>
#include <string.h>
#include <new>
namespace N_0099a2f0 { class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2F0 (folded body)
    virtual ~CBaseClass();
}; } using N_0099a2f0::CBaseClass;
class CDataStream : public CBaseClass {
public:
    virtual ~CDataStream() {}
    virtual void _slot01();
    virtual void _slot02();
    virtual void Close() = 0;                            // slot 3
};
class CDataInputStream : public CDataStream {
public:
    CDataInputStream();
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0c
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
};
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}

static int g_base;
CBaseClass::CBaseClass() { ++g_base; }
CBaseClass::~CBaseClass() {}
void CDataStream::_slot01() {}
void CDataStream::_slot02() {}
struct TestStream : CDataInputStream { virtual void Close() {} };
int main()
{
    static unsigned char buf[sizeof(TestStream)]; memset(buf, 0x77, sizeof(buf));
    TestStream* s = new (buf) TestStream;
    int ok = sizeof(CDataInputStream) == 0x18 && g_base == 1 && s->StreamPos == 0 && s->StreamSize == 0
          && s->CurrentSourcePtr == 0 && s->SourceChunkStreamPos == 0 && s->SourceChunkBytesRemaining == 0;
    printf(ok ? "00993b20_TEST PASS\n" : "00993b20_TEST FAIL\n");
    return ok ? 0 : 1;
}