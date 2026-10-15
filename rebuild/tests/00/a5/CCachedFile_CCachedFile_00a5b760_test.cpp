#include <cstdio>

static int g_baseBuilt = 0, g_stringBuilt = 0, g_copied = 0;

struct CBase { CBase() { ++g_baseBuilt; } virtual ~CBase() { } };
struct CAFile : CBase { };
struct CByteArrayRange { unsigned char* m_pFirst; unsigned char* m_pLast; CByteArrayRange() : m_pFirst(0), m_pLast(0) { } };
struct CByteArrayEnd { unsigned char* m_pEnd; CByteArrayEnd() : m_pEnd(0) { } };
struct CByteArray : CByteArrayRange { CByteArrayEnd m_end; };
struct CWideString { unsigned short* m_pString; CWideString() : m_pString(0) { ++g_stringBuilt; } };
struct CCountedPointer { void* m_pObject; void* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { } };

struct CCachedFile : CAFile {
    long Position; long BufferStartPosition; CByteArray DataBuffer; CWideString Pathname; CCountedPointer PDiskFile;
    bool Opened; bool BufferValid;
    CCachedFile() : Position(3), BufferStartPosition(4), Opened(true), BufferValid(true) { }
    CCachedFile(const CCachedFile& other) : CAFile(), DataBuffer(), Pathname(), PDiskFile() { Copy(other); }
    void Copy(const CCachedFile& other)   // stand-in for 0x00A5B650: copies the position state, records the call
    {
        ++g_copied; Position = other.Position; BufferStartPosition = other.BufferStartPosition;
        Opened = other.Opened; BufferValid = false;
    }
};

int main()
{
    CCachedFile a; a.DataBuffer.m_pFirst = (unsigned char*)&a; a.PDiskFile.m_pObject = &a;
    CCachedFile b(a);
    if (g_baseBuilt != 2 || g_stringBuilt != 2 || g_copied != 1 || b.DataBuffer.m_pFirst || b.DataBuffer.m_pLast
        || b.DataBuffer.m_end.m_pEnd || b.Pathname.m_pString || b.PDiskFile.m_pObject || b.PDiskFile.m_pCount
        || b.Position != 3 || b.BufferStartPosition != 4 || !b.Opened)
    { std::printf("00a5b760_TEST FAIL\n"); return 1; }
    std::printf("00a5b760_TEST PASS\n");
    return 0;
}