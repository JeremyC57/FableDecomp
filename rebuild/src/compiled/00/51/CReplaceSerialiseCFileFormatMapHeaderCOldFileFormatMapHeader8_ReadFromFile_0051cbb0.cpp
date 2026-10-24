// CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ReadFromFile(CAFile& file, CASuspendableProcess* process) -- read the map header record, upgrading an older record version to version 9
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051CBB0, 208 bytes, `__fastcall`, `ret 8`; FableWin.pdb
//   ?ReadFromFile@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z;
//   callers 0x00506C09/0x00506D01 (CWorldMap level dimensions/checksum) and 0x00821B43. GetMaxSize inlines the version-9 and -8
//   levels and calls CReplaceSerialise<COldFileFormatMapHeader7,...>::GetMaxSize 0x00519630; ConvertToThisVersion 0x0051C890
//   (manifest label ResourceData_CopyIfDifferentBuffer is propagated).
// Layout: Ego_r.pdb CFileFormatMapHeader 0x10822 bytes, COldFileFormatMapHeader8 0x1081E (the `cmp 1081Eh` / `cmp 10822h` pair);
//   the copy over *this is `rep movsd` x0x4208 + `movsw`.
// Callees: CAFile::ReadVar<int> 0x00516D90, CAFile::ReadVar<unsigned char> 0x00516E10, CAFile virtual slots 7 (+0x1C, the
//   position), 5 (+0x14, seek back) and 3 (+0x0C, Read(void*, long, CASuspendableProcess*)), operator new[] 0x00BFEB22,
//   operator delete[] 0x00BFEB1C, and the static __fastcall ConvertToThisVersion(void*) named below.
// Body: read the record length, peek the version byte and seek back; the current version is read straight over this record
//   (ByteLength stored, `length - 4` bytes read from +4); an older one is read into a GetMaxSize(version, 0) scratch block,
//   its ByteLength set, upgraded in place by ConvertToThisVersion and copied over *this (sizeof(T) bytes), then freed.
// Shape: GetMaxSize is the static __fastcall template `maxSize = version == GetClassVersionNumber() ? size :
//   TOld::GetMaxSize(version, size); return MAX(maxSize, sizeof(T));` -- the named local keeps ONE call per inlined level
//   (a MAX straight over the recursive call evaluates it twice); the comparisons are unsigned because sizeof is.
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
class COldFileFormatMapHeader7 {
public:
    enum { VERSION = 7 };
    static long __fastcall GetMaxSize(long version, long size);                 // 0x00519630 (CReplaceSerialise<Header7,Header6>)
};
class COldFileFormatMapHeader8 : public CReplaceSerialise<COldFileFormatMapHeader8, COldFileFormatMapHeader7> {
public:
    enum { VERSION = 8 };
    unsigned char _payload[0x1081E - 5];                                        // Ego_r.pdb: 0x1081E bytes
};
class CFileFormatMapHeader : public CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8> {
public:
    enum { VERSION = 9 };
    unsigned char _payload[0x10822 - 5];                                        // Ego_r.pdb: 0x10822 bytes
};
#pragma pack(pop)
template <> void __fastcall CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8>::ConvertToThisVersion(void* data);   // 0x0051C890
template void CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8>::ReadFromFile(CAFile& file, CASuspendableProcess* process);