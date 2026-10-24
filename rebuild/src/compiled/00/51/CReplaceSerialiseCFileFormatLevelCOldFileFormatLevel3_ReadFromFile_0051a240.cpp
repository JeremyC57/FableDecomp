// CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile(CAFile& file, CASuspendableProcess* process) -- read the level record; an older record version is read into a scratch block, upgraded and copied over this one
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051A240, 175 bytes, `ret 8` -- parked by hand chunk 332 (164v175); this is chunk 333's
//   CReplaceSerialise model (RELOCATION_MATCH through a direct-from-exe grade, not yet through the batch oracle).
// Shape: the dead `movzx eax,al ; mov [esp+14h],eax` frame word is the inlined GetMaxSize(version, 0) chain (CFileFormatLevel 4 ->
//   COldFileFormatLevel3 -> 2 -> CBaseSerialise<COldFileFormatLevel1,1>, Ego_r sizes 25/21/17/13) folding to the constant 0x19
//   while VC7.1 keeps the converted version argument's home store.
class CASuspendableProcess;
void* __cdecl operator new[](unsigned int size);         // 0x00BFEB22 (the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C

class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // slot 3 (+0x0C)
    virtual void v04();
    virtual void SetPosition(long position);                                    // slot 5 (+0x14)
    virtual void v06();
    virtual long GetPosition();                                                 // slot 7 (+0x1C)
    void ReadVar(int* value, CASuspendableProcess* process);                    // 0x00516D90 ReadVar<int>
    void ReadVar(unsigned char* value, CASuspendableProcess* process);          // 0x00516E10 ReadVar<unsigned char>
};

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                                                   // +0x00
    unsigned char SerialiseVersionNumber;                                       // +0x04
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