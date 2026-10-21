// CWorldMap::SaveWorldToBinaryFile(CWideString& fileName) -- serialise the world topology (map count and every
// real map from index 1, region count and every real region from index 1) into a memory stream and write it to a
// new disk file
// Leaf: SaveWorldToBinaryFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FBDE0, 371 bytes, `ret 4`. PDB ?SaveWorldToBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z;
//   CMapInfo/CRegion ::SaveBinary(CDataOutputStream&) (QAE, non-const).
// Callees: CMemoryDataOutputStream ctor 0x00996AF0 / dtor 0x00996B30, CDataOutputStream::WriteSLONG 0x00993EB0,
//   CMapInfo::SaveBinary 0x004FB3E0, CRegion::SaveBinary 0x006BBFD0, CDiskFileWin32(const CWideString&,
//   FileAccessType, unsigned long) 0x0099AD80, CDiskFileWin32::Write 0x0099AA90, CMemoryDataOutputStream::GetSize
//   0x00994B40 / PeekData 0x00995E70; the inlined ~CDiskFileWin32 (vtable 0x0122D06C, Close 0x0099A920, ~CWideString
//   0x0099B510, ~CBase 0x0099A300) -- the stream/file model of the landed NAnimationEvents::CManager::SaveBinary
//   0x006F92A0.
// Layout: retail CWorldMap keeps Maps (CArray<CMapInfo>, 0x48-byte entries) at +0x20 and Regions (vector<CRegion>,
//   0x58-byte entries) at +0x2c, 4 bytes below rebuild/include/engine/CWorldMap.h (the landed
//   CWorldMap::GetMapNumberFromMapUID 0x004FB920 models Maps at +0x20 the same way), so the class is modelled locally.
struct CWideString {
    void* PStringData;                              // +0x00
    ~CWideString();                                 // 0x0099B510
};
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
struct CDiskFileWin32 : CBase {                     // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                           // 0x0099A920 (slot 2)
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
};
struct CDataOutputStream {
    void WriteSLONG(long value);                    // 0x00993EB0
};
struct CMemoryDataOutputStream : CDataOutputStream {
    unsigned char _body[0x18];
    CMemoryDataOutputStream();                      // 0x00996AF0
    ~CMemoryDataOutputStream();                     // 0x00996B30
    long GetSize() const;                           // 0x00994B40
    const void* PeekData() const;                   // 0x00995E70
};
struct CMapInfo {                                   // 0x48 bytes
    unsigned char _body[0x48];
    void SaveBinary(CDataOutputStream& stream);         // 0x004FB3E0
};
struct CRegion {                                    // 0x58 bytes
    unsigned char _body[0x58];
    void SaveBinary(CDataOutputStream& stream);         // 0x006BBFD0
};
template <class T> struct CArray {                  // STLport vector layout (12 bytes)
    T* Start;
    T* Finish;
    T* EndOfStorage;
    unsigned long size() const { return Finish - Start; }
    T& operator[](unsigned long i) { return Start[i]; }
};
struct CWorldMap {
    unsigned char    _pad00[0x20];
    CArray<CMapInfo> Maps;                          // +0x20
    CArray<CRegion>  Regions;                       // +0x2c
    void SaveWorldToBinaryFile(CWideString& fileName);
};
void CWorldMap::SaveWorldToBinaryFile(CWideString& fileName)
{
    CMemoryDataOutputStream stream;
    stream.WriteSLONG(Maps.size());
    for (unsigned long i = 1; i < Maps.size(); ++i)
        Maps[i].SaveBinary(stream);
    stream.WriteSLONG(Regions.size());
    for (unsigned long j = 1; j < Regions.size(); ++j)
        Regions[j].SaveBinary(stream);
    CDiskFileWin32 file(fileName, FILE_ACCESS_WRITE, 2);
    file.Write(stream.PeekData(), stream.GetSize(), 0);
}
