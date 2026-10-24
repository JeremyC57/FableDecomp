// CBaseSerialise<CFileFormatBrushHeader,1>::ReadFromFile(CAFile&, CASuspendableProcess*) -- read a versioned file record: its byte length, then (after peeking the version byte) the whole record from the version byte on
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x008269A0, 88 bytes (+8 int3 padding), `__fastcall`, `ret 8`, returns true.
// Callees: CAFile::ReadVar<int> @ 0x00516D90, CAFile::ReadVar<unsigned char> @ 0x00516E10, and CAFile virtual slots 7
//   (+0x1C, the current position), 5 (+0x14, seek back to it) and 3 (+0x0C, Read(void*, long, CASuspendableProcess*)).
// Identity: FableWin.pdb has `bool CBaseSerialise<T,1>::ReadFromFile(CAFile&, CASuspendableProcess*)` for seven brush/map
//   record types and no other bool ReadFromFile of this shape; the only caller is CWorldMap::LoadScriptBrushesFromFile
//   (0x008210E5), so the record is the brush header (the instantiations do not depend on T beyond its first two members
//   and may be folded). The propagated label `CReplaceSerialise<CFileFormatEditMapBrushHeader,...>` is wrong.
// Layout: the record starts with ByteLength (+0, ulong) and SerialiseVersionNumber (+4, uchar); the payload follows.
// Shape: the length and the peeked version are homed in the dead file/process argument slots; the version byte is read
//   and the position restored, then ByteLength is stored and `ByteLength - 4` bytes are read from +4 (the version
//   included). The version read is not checked here (version 1 has no older format).
class CASuspendableProcess;

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

#pragma pack(push, 1)
template <class T, long Version> class CBaseSerialise {
public:
    unsigned long ByteLength;                                                   // +0x00
    unsigned char SerialiseVersionNumber;                                       // +0x04
    bool ReadFromFile(CAFile& file, CASuspendableProcess* process);
};
#pragma pack(pop)

template <class T, long Version> bool CBaseSerialise<T, Version>::ReadFromFile(CAFile& file, CASuspendableProcess* process)
{
    int length;
    file.ReadVar(&length, process);
    long position = file.GetPosition();
    unsigned char version;
    file.ReadVar(&version, process);
    file.SetPosition(position);
    ByteLength = length;
    file.Read(&SerialiseVersionNumber, length - 4, process);
    return true;
}

class CFileFormatBrushHeader;
template bool CBaseSerialise<CFileFormatBrushHeader, 1>::ReadFromFile(CAFile& file, CASuspendableProcess* process);