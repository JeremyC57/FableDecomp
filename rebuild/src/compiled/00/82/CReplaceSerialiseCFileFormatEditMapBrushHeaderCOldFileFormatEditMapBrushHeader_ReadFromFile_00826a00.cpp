// CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader>::ReadFromFile(CAFile& file, CASuspendableProcess* process) -- read the edit-map brush header record, upgrading an older (version 1) record to version 2
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00826A00, 164 bytes, `__fastcall`, `ret 8`; FableWin.pdb
//   ?ReadFromFile@?$CReplaceSerialise@VCFileFormatEditMapBrushHeader@@VCOldFileFormatEditMapBrushHeader@@@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z;
//   caller 0x0082117A (CWorldMap brush loading). The old format is CBaseSerialise<COldFileFormatEditMapBrushHeader,1> (FableWin.pdb
//   has its GetMaxSize/GetClassVersionNumber); ConvertToThisVersion is 0x00825D40 (manifest label CAFile_ReadRigidBody is propagated).
// Layout: {ByteLength +0, SerialiseVersionNumber +4} then the payload; the retail record is 0x49 bytes (`push 49h`, `rep movsd`
//   x0x12 + `movsb`) -- GetMaxSize folds to that constant, so no version arithmetic survives.
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
template <class T, long Version> class CBaseSerialise {
public:
    unsigned long ByteLength;                                                   // +0x00
    unsigned char SerialiseVersionNumber;                                       // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return Version; }
    static long __fastcall GetMaxSize(long version, long size) { return MAX(size, sizeof(T)); }
};
class COldFileFormatEditMapBrushHeader : public CBaseSerialise<COldFileFormatEditMapBrushHeader, 1> {
public:
    unsigned char _payload[0x49 - 5];       // retail size not recoverable from this body (it folds to <= 0x49)
};
class CFileFormatEditMapBrushHeader : public CReplaceSerialise<CFileFormatEditMapBrushHeader, COldFileFormatEditMapBrushHeader> {
public:
    enum { VERSION = 2 };
    unsigned char _payload[0x49 - 5];       // retail TLC record is 0x49 bytes (Ego_r's is larger)
};
#pragma pack(pop)
template <> void __fastcall CReplaceSerialise<CFileFormatEditMapBrushHeader, COldFileFormatEditMapBrushHeader>::ConvertToThisVersion(void* data);   // 0x00825D40
template void CReplaceSerialise<CFileFormatEditMapBrushHeader, COldFileFormatEditMapBrushHeader>::ReadFromFile(CAFile& file, CASuspendableProcess* process);