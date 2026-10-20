// CWorldMap::GetLevelStaticMapChecksum(const CCharString&) -- open the level file from disk, read its level record and map header, and return the map's static checksum
// Leaf: GetLevelStaticMapChecksum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00506C40, 249 bytes (256 with padding), `ret 4`.
//   PDB ?GetLevelStaticMapChecksum@CWorldMap@@QAEKABVCCharString@@@Z.
// Callees: operator new 0x00BFEA1A (x3), CCharString::ToWideString 0x0099E480, CDiskFileWin32::CDiskFileWin32(const
//   CWideString&, FileAccessType, unsigned long) 0x0099AD80, ~CWideString 0x0099B510, CFileLargeThemePalette ctor
//   0x0050EE40 (x2), CAFile slot 5 Seek (x2), CReplaceSerialise<CFileFormatLevel>::ReadFromFile 0x0051A240,
//   CReplaceSerialise<CFileFormatMapHeader>::ReadFromFile 0x0051CBB0, operator delete 0x00BFE9BC (x2); the counted
//   block's deleter 0x0050BE40.
// Layout: CFileFormatLevel (25 bytes, packed) and CFileFormatMapHeader (0x10822 bytes, StaticMapChecksum +0x1081E)
//   from the landed CWorldMap::GetLevelDimensions(CAFile&) 0x00506BB0, whose body this one inlines.
// Shape: the file is a CCountedPointer<CAFile> built from the new-expression (the ToWideString argument temporary gets
//   the construction flag in ebx, its slot zeroed first; the inline ctor allocates the {1, deleter, object} block only
//   for a non-null object); the header is a scoped holder deleted after the checksum is read; the file block is
//   released last (`dec [ebp]`).
void* operator new(unsigned int size);                 // 0x00BFEA1A
void operator delete(void* p);                         // 0x00BFE9BC
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CASuspendableProcess;
class CAFile
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual void Seek(long position);                  // slot 5 (+0x14)
};
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);         // 0x0099AD80
    unsigned char _body[0x18];
};
void __fastcall Destroy_0050be40(void* object);       // 0x0050BE40: the CAFile deleter stored in the block
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void* data) : RefCount(1), DeleteFunc(Destroy_0050be40), Data(data) { }
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* object) : Data(object)
    {
        if (object)
            Info = new CCPPointerInfo(object);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
struct CFileLargeThemePalette
{
    unsigned char Data[0x8400];                        // 33792 bytes
    CFileLargeThemePalette();                          // 0x0050EE40
};

#pragma pack(push, 1)
struct CFileFormatLevel
{
    unsigned long ByteLength;                          // +0x00 CReplaceSerialise
    unsigned char SerialiseVersionNumber;              // +0x04
    long          MapDataOffset;                       // +0x05
    long          ThingDataOffset;                     // +0x09
    long          BrushDataOffset;                     // +0x0D
    long          NavigationDataOffset;                // +0x11
    long          NavigationSectionsOffset;            // +0x15
    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051A240
};

struct CFileFormatMapHeader
{
    unsigned long          ByteLength;                 // +0x00 CReplaceSerialise
    unsigned char          SerialiseVersionNumber;     // +0x04
    unsigned long          ThingUIDCount[2];           // +0x05 ulong64
    unsigned long          WorldSizeX;                 // +0x0D
    unsigned long          WorldSizeY;                 // +0x11
    bool                   UsingExtraMapCells;         // +0x15
    CFileLargeThemePalette LargeThemePalette;          // +0x16
    unsigned long          VersionNumber;              // +0x8416
    unsigned long          NoSoundThemeEntries;        // +0x841A
    CFileLargeThemePalette AtmosThemePalette;          // +0x841E
    unsigned long          StaticMapChecksum;          // +0x1081E
    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051CBB0
};
#pragma pack(pop)

template <class T>
struct CScopedPtr
{
    T* ptr;
    CScopedPtr(T* p) : ptr(p) {}
    ~CScopedPtr() { delete ptr; }
    T* operator->() const { return ptr; }
};

class CWorldMap
{
public:
    unsigned long GetLevelStaticMapChecksum(const CCharString& levelName);
};

unsigned long CWorldMap::GetLevelStaticMapChecksum(const CCharString& levelName)
{
    CCountedPointer<CAFile> file(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    CScopedPtr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
    file->Seek(0);
    CFileFormatLevel level;
    level.ReadFromFile(file.Data, 0);
    file->Seek(level.MapDataOffset);
    header->ReadFromFile(file.Data, 0);
    return header->StaticMapChecksum;
}