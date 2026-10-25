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
// Shape: GetMaxSize is the static __fastcall template of the landed 0x0051ADF0/0x00519630 rows, `if (version ==
//   GetClassVersionNumber()) return MAX(size, sizeof(T)); return MAX(TOld::GetMaxSize(version, size), sizeof(T));` with MAX
//   the evaluate-twice macro; the version-9 and -8 levels inline here and, with the whole chain's bodies in the TU, the
//   dominated second call to the version-7 level 0x00519630 is reused (one call); the comparisons are unsigned because
//   sizeof is.
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
        if (version == GetClassVersionNumber())
            return MAX(size, sizeof(T));
        return MAX(TOld::GetMaxSize(version, size), sizeof(T));
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
template <class T, long V> class CBaseSerialise {
public:
    unsigned long ByteLength;                                                   // +0x00
    unsigned char SerialiseVersionNumber;                                       // +0x04
    static long __fastcall GetMaxSize(long version, long size) { return MAX(size, sizeof(T)); }
};
#pragma pack(push, 1)
class COldFileFormatMapHeader1 : public CBaseSerialise<COldFileFormatMapHeader1, 1> { public: enum { VERSION = 1 }; };
class COldFileFormatMapHeader2 : public CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1> { public: enum { VERSION = 2 }; };
class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> { public: enum { VERSION = 3 }; };
class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned char _payload[0x840E - 5];
};
class COldFileFormatMapHeader5 : public CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4> {
public:
    enum { VERSION = 5 };
    unsigned char _payload[0x8416 - 5];
};
class COldFileFormatMapHeader6 : public CReplaceSerialise<COldFileFormatMapHeader6, COldFileFormatMapHeader5> {
public:
    enum { VERSION = 6 };
    unsigned char _payload[0x841A - 5];                                         // Ego_r.pdb: 0x841A bytes
};
class COldFileFormatMapHeader7 : public CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6> {
public:
    enum { VERSION = 7 };                       // its GetMaxSize is 0x00519630 (CReplaceSerialise<Header7,Header6>)
    unsigned char _payload[0x841E - 5];                                         // Ego_r.pdb: 0x841E bytes
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