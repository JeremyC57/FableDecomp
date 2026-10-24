// Behaviour test for 0x0051A240 CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile: the real body over an
// in-memory CAFile; a current-version record is read straight into the object, an older record goes through the scratch
// block, ConvertToThisVersion and the copy.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
class CASuspendableProcess;
void* __cdecl operator new[](unsigned int size);
void __cdecl operator delete[](void* block);

class CAFile {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);
    virtual void v04() {}
    virtual void SetPosition(long position);
    virtual void v06() {}
    virtual long GetPosition();
    void ReadVar(int* value, CASuspendableProcess* process);
    void ReadVar(unsigned char* value, CASuspendableProcess* process);
    const unsigned char* Data; long Position;
    CAFile(const unsigned char* data) : Data(data), Position(0) {}
};
void CAFile::Read(void* buffer, long size, CASuspendableProcess*) { memcpy(buffer, Data + Position, size); Position += size; }
void CAFile::SetPosition(long position) { Position = position; }
long CAFile::GetPosition() { return Position; }
void CAFile::ReadVar(int* value, CASuspendableProcess* p) { Read(value, 4, p); }
void CAFile::ReadVar(unsigned char* value, CASuspendableProcess* p) { Read(value, 1, p); }

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static long __fastcall GetMaxSize(long version, long size)
    {
        long maxSize = (version == GetClassVersionNumber()) ? size : TOld::GetMaxSize(version, size);
        return MAX(maxSize, sizeof(T));
    }
    static void __fastcall ConvertToThisVersion(void* data);
    void ReadFromFile(CAFile& file, CASuspendableProcess* process);
};
#pragma pack(pop)
template <class T, class TOld> void CReplaceSerialise<T, TOld>::ReadFromFile(CAFile& file, CASuspendableProcess* process)
{
    int length;
    file.ReadVar(&length, process);
    long position = file.GetPosition();
    unsigned char version;
    file.ReadVar(&version, process);
    file.SetPosition(position);
    if (version == GetClassVersionNumber())
    {
        ByteLength = length;
        file.Read(&SerialiseVersionNumber, length - 4, process);
        return;
    }
    unsigned char* buffer = new unsigned char[GetMaxSize(version, 0)];
    file.Read(buffer + 4, length - 4, process);
    *(unsigned long*)buffer = length;
    ConvertToThisVersion(buffer);
    *static_cast<T*>(this) = *(T*)buffer;
    delete[] buffer;
}
#pragma pack(push, 1)
template <class T, long Version> class CBaseSerialise {
public:
    unsigned long ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short __fastcall GetClassVersionNumber() { return Version; }
    static long __fastcall GetMaxSize(long version, long size) { return MAX(size, sizeof(T)); }
};
class COldFileFormatLevel1 : public CBaseSerialise<COldFileFormatLevel1, 1> { public: unsigned char _p[13 - 5]; };
class COldFileFormatLevel2 : public CReplaceSerialise<COldFileFormatLevel2, COldFileFormatLevel1> { public: enum { VERSION = 2 }; unsigned char _p[17 - 5]; };
class COldFileFormatLevel3 : public CReplaceSerialise<COldFileFormatLevel3, COldFileFormatLevel2> { public: enum { VERSION = 3 }; unsigned char _p[21 - 5]; };
class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3> { public: enum { VERSION = 4 }; unsigned char _p[25 - 5]; };
#pragma pack(pop)
template void CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ReadFromFile(CAFile& file, CASuspendableProcess* process);

static int g_converted = 0;
static long g_convertedLength = 0;
// Upgrade stand-in: an older record is widened to the current layout (version 4, payload shifted up by four bytes).
template <> void __fastcall CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertToThisVersion(void* data)
{
    unsigned char* block = (unsigned char*)data;
    ++g_converted;
    g_convertedLength = *(long*)block;
    unsigned char payload[21];
    memcpy(payload, block + 4, 21);
    block[4] = 4;
    memset(block + 5, 0, 20);
    memcpy(block + 9, payload + 1, 16);
    *(long*)block = 25;
}

static int CheckCurrent()
{
    unsigned char record[25];
    *(long*)record = 25; record[4] = 4;
    for (int i = 5; i < 25; ++i) record[i] = (unsigned char)(0x40 + i);
    CAFile file(record);
    CFileFormatLevel level; memset(&level, 0xEE, sizeof(level));
    level.ReadFromFile(file, 0);
    if (level.ByteLength != 25) return 1;
    if (level.SerialiseVersionNumber != 4) return 2;
    if (memcmp(level._p, record + 5, 20) != 0) return 3;
    if (file.Position != 25) return 4;
    if (g_converted != 0) return 5;
    return 0;
}
static int CheckOld()
{
    unsigned char record[21];
    *(long*)record = 21; record[4] = 3;
    for (int i = 5; i < 21; ++i) record[i] = (unsigned char)(0x60 + i);
    CAFile file(record);
    CFileFormatLevel level; memset(&level, 0xEE, sizeof(level));
    level.ReadFromFile(file, 0);
    if (g_converted != 1) return 11;
    if (g_convertedLength != 21) return 12;
    if (level.ByteLength != 25) return 13;
    if (level.SerialiseVersionNumber != 4) return 14;
    if (memcmp(level._p + 4, record + 5, 16) != 0) return 15;
    if (file.Position != 21) return 16;
    return 0;
}
int main()
{
    int r = CheckCurrent();
    if (r == 0) r = CheckOld();
    if (r != 0) { printf("0051a240_TEST FAIL %d\n", r); return 1; }
    printf("0051a240_TEST PASS\n");
    return 0;
}