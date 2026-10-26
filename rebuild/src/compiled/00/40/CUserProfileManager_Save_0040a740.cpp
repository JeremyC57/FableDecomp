// CUserProfileManager::Save() -- unless the new-profile values are in use, build the profile's file name and, when set, write the game state into a memory file and dump it to disk
// Leaf: Save
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040A740, length 175 bytes (176 with padding).
// Callees: the profile path builder @ 0x004082D0, CWideString::operator!=(const wchar_t*) @ 0x0099B150,
// CMemoryFile::CMemoryFile(const CWideString&, FileAccessType, unsigned long) @ 0x009E97B0,
// CUserProfileManager::SaveGameState(CAFile*) @ 0x00409FD0, CMemoryFile::DumpToFile @ 0x009E9330, CMemoryFile::Close
// @ 0x009E9390, ~CWideString @ 0x0099B510, free @ 0x00BFEA14 (the data buffer), CBase::~CBase @ 0x0099A300.
// Literal L"" @ 0x0122D70C.
// Layout: CUserProfileManager ProfileName +0x08, UseNewProfileValues +0x0c (Ego_r.pdb); CMemoryFile (vtable 0x0122E0FC)
// with the 12-byte retail CArray DataBuffer +0x10, Opened +0x1c, Pathname +0x20 (PDB 0x28 bytes, 4 more).
// Shape: as Load 0x0040D350; the memory file's inline virtual destructor stores its vtable, calls Close directly, destroys
// the pathname, frees the buffer, then the base.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
typedef unsigned short wchar16;                             // wchar_t under the era defaults
class CWideString { public:
    void* PStringData;                                      // +0
    ~CWideString();                                         // 0x0099B510
    bool operator!=(const wchar16* text) const;             // 0x0099B150
};
bool __fastcall PathExists(const CWideString& path);       // 0x00999230
bool __fastcall VerifySignature(const CWideString& path);  // 0x00409730
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

namespace N_0099a300 { struct CBase {
    virtual ~CBase();                                       // 0x0099A300
}; } using N_0099a300::CBase;
class CAFile : public CBase { public:
    long AccessType;                                        // +0x04 FileAccessType
    long Position;                                          // +0x08
    long Length;                                            // +0x0c
};
struct CByteArray {                                         // CArray<unsigned char>, 12 bytes in retail
    unsigned char* Data;
    unsigned long  Size;
    unsigned long  Capacity;
    ~CByteArray() { if (Data) free(Data); }
};
class CMemoryFile : public CAFile { public:                               // vtable 0x0122E0FC
    CByteArray  DataBuffer;                                 // +0x10
    bool        Opened;                                     // +0x1c
    CWideString Pathname;                                   // +0x20
    CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x009E97B0
    virtual ~CMemoryFile() { Close(); }
    virtual void Close();                                   // 0x009E9390
    void DumpToFile();                                      // 0x009E9330
};

class CUserProfileManager { public:
    void*       m_vtable;                                   // +0x00
    unsigned char _pad04[4];
    CWideString ProfileName;                                // +0x08
    bool        UseNewProfileValues;                        // +0x0c
    void SaveGameState(CAFile* file);                       // 0x00409FD0
    void Save();
public:
    CWideString GetProfileDir(const CWideString&, bool);   // 0x004082D0
};

void CUserProfileManager::Save()
{
    if (UseNewProfileValues)
        return;
    CWideString path = GetProfileDir(ProfileName, true);
    if (path != (const wchar16*)L"") {
        CMemoryFile file(path, FILE_ACCESS_WRITE, 2);
        if (file.Opened) {
            SaveGameState(&file);
            file.DumpToFile();
            file.Close();
        }
    }
}