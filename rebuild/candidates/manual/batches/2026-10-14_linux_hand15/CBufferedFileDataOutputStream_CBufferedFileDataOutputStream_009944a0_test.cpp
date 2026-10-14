#include <stdio.h>
#include <stddef.h>
#include <string.h>
static int g_baseCalls; static const char* g_name; static long g_nameLen; static int g_lengthCalls;
struct CCharString { void* m_pStringData; CCharString(const char* text, long length) { g_name = text; g_nameLen = length; m_pStringData = (void*)text; } };
struct CBase { virtual void v00() {} CBase() { ++g_baseCalls; } };
struct CDataOutputStream : CBase { CCharString m_name; virtual void v00() {} CDataOutputStream() : CBase(), m_name("CBufferedFileDataOutputStream", -1) { } };
struct CAFile {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual long GetLength() { ++g_lengthCalls; return 4096; }
};
struct CBufferedFileDataOutputStream : CDataOutputStream {
    CAFile* File; unsigned char* Buffer; long BufferSize; long BufferDataSize; long DataStartFilePtr; long FileSize; long FilePos;
    virtual void v00() {}
    CBufferedFileDataOutputStream(CAFile* file, long bufferSize);
};
CBufferedFileDataOutputStream::CBufferedFileDataOutputStream(CAFile* file, long bufferSize) : CDataOutputStream()
{
    File = file; BufferSize = bufferSize; Buffer = new unsigned char[bufferSize]; BufferDataSize = 0; DataStartFilePtr = 0; FileSize = file->GetLength(); FilePos = FileSize;
}
int main()
{
    CAFile f; CBufferedFileDataOutputStream s(&f, 256);
    bool ok = g_baseCalls == 1 && strcmp(g_name, "CBufferedFileDataOutputStream") == 0 && g_nameLen == -1 && s.File == &f && s.Buffer != 0 && s.BufferSize == 256 && s.BufferDataSize == 0 && s.DataStartFilePtr == 0
        && g_lengthCalls == 1 && s.FileSize == 4096 && s.FilePos == 4096 && sizeof(CBufferedFileDataOutputStream) == 0x24 && offsetof(CBufferedFileDataOutputStream, File) == 8;
    s.Buffer[255] = 1; delete[] s.Buffer;
    if (!ok) { printf("009944a0_TEST FAIL\n"); return 1; }
    printf("009944a0_TEST PASS\n"); return 0;
}
