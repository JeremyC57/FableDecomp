// Grade: DIFFER(252v247) -- retail loads PWADBank into esi before ToWideString (new result in edi) and keeps the count block in esi for the final release; ours loads the bank after the call and rotates esi/edi/eax
// CWorldMap::GetLevelDimensions(const CCharString&) -- open the level file (from the WAD bank when WAD loading is on, else from disk) and return its world box through the file overload
// Leaf: ?GetLevelDimensions@CWorldMap@@QAE?AVC2DBoxI@@ABVCCharString@@@Z
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
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CAFile { public: virtual void v00(); };
struct CWADBank;
class CWADFile : public CAFile {
public:
    CWADFile(CWADBank* bank, const CWideString& name, FileAccessType access, unsigned long flags);   // 0x00A39D80
    unsigned char _body[0xc];
};
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);         // 0x0099AD80
    unsigned char _body[0x18];
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info) {
            if (--info->RefCount == 0) {
                info->DeleteFunc(info->Data);
                delete info;
            }
        }
    }
    void Reset(T* object);                             // 0x0050F550
};
class CWorldMap {
public:
    unsigned char _pad00[0xa0];
    CWADBank*     PWADBank;                            // +0xA0
    unsigned char _pada4[4];
    bool          UseWADFiles;                         // +0xA8
    C2DBoxI GetLevelDimensions(CAFile* file);          // 0x00506BB0 (landed, PDB takes CAFile&)
    C2DBoxI GetLevelDimensions(const CCharString& levelName);
};
C2DBoxI CWorldMap::GetLevelDimensions(const CCharString& levelName)
{
    CCountedPointer<CAFile> file;
    if (UseWADFiles)
        file.Reset(new CWADFile(PWADBank, levelName.ToWideString(), FILE_ACCESS_READ, 2));
    else
        file.Reset(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    return GetLevelDimensions(file.Data);
}
