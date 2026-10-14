#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_copies = 0;
struct CBase { virtual void v00() {} CBase() { ++g_baseCtors; } };
struct CAFile : CBase { };
struct CByteArray { unsigned char* m_pFirst; unsigned char* m_pLast; CByteArray() : m_pFirst(0), m_pLast(0) {} };
struct CWideString { unsigned short* m_pString; CWideString() : m_pString(0) {} };
struct CCountedPointer { void* m_pObject; void* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) {} };
struct CCachedFile : CAFile {
    virtual CAFile* Clone() const;
    long Position; long BufferStartPosition; CByteArray DataBuffer; long DataBufferCapacity; CWideString Pathname; CCountedPointer PDiskFile; bool Opened; bool BufferValid;
    CCachedFile() : Position(7), BufferStartPosition(9), DataBufferCapacity(0), Opened(true), BufferValid(true) {}
    CCachedFile(const CCachedFile& other) : CAFile(), DataBuffer(), DataBufferCapacity(0), Pathname(), PDiskFile() { Copy(other); }
    void Copy(const CCachedFile& other) { ++g_copies; Position = other.Position; BufferStartPosition = other.BufferStartPosition; Opened = other.Opened; BufferValid = other.BufferValid; }
};
CAFile* CCachedFile::Clone() const { return new CCachedFile(*this); }

int main()
{
    CCachedFile src;
    CCachedFile* clone = static_cast<CCachedFile*>(src.Clone());
    if (clone == 0 || clone == &src || g_baseCtors != 2 || g_copies != 1) { std::printf("00a5b710_TEST FAIL\n"); return 1; }
    if (clone->Position != 7 || clone->BufferStartPosition != 9 || clone->DataBuffer.m_pFirst != 0 || clone->DataBufferCapacity != 0 || clone->PDiskFile.m_pObject != 0 || !clone->Opened) { std::printf("00a5b710_TEST FAIL\n"); return 1; }
    delete clone;
    std::printf("00a5b710_TEST PASS\n");
    return 0;
}
