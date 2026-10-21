// behaviour test for 0x00994780
#include <stdio.h>
static int g_log[8]; static int g_n = 0;
class CAFile;
class CBaseClass {
public:
    virtual ~CBaseClass() { g_log[g_n++] = 3; }
};
class CDataStream : public CBaseClass {
public:
    virtual ~CDataStream() {}
    virtual void _slot01() {}
    virtual void _slot02() {}
    virtual void Close() = 0;                            // slot 3
};
class CDataInputStream : public CDataStream {
public:
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0c
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
};
class CFileDataInputStream : public CDataInputStream {
public:
    CAFile*        File;                                 // +0x18
    unsigned char* Buffer;                               // +0x1c
    long           BufferSize;                           // +0x20
    virtual ~CFileDataInputStream();
    virtual void Close() { g_log[g_n++] = 1; }
};
CFileDataInputStream::~CFileDataInputStream()
{
    Close();
}

int main()
{
    CBaseClass* p = new CFileDataInputStream; delete p;
    int ok = g_n == 2 && g_log[0] == 1 && g_log[1] == 3 && sizeof(CFileDataInputStream) == 0x24;
    printf(ok ? "00994780_TEST PASS\n" : "00994780_TEST FAIL\n");
    return ok ? 0 : 1;
}