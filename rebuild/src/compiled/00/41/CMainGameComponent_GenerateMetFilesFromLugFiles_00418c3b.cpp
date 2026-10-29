// CMainGameComponent::GenerateMetFilesFromLugFiles() -- for every Data\Sound\*.lug script, compile it through the system manager's sound interface into a memory file written out as the matching .met file
// Leaf: GenerateMetFilesFromLugFiles
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00418C3B (unaligned: a /Os TU), length 399 bytes, EBP-biased frame (`lea ebp,[esp-1ECh]`) with the /GS cookie, plain `ret` (static: ecx unused).
// Callees: GetSoundDirectory 0x0041A2B0 (__fastcall, hidden result in ecx, L"Data\\Sound\\"; labelled CUserProfileManager::GetAutoSaveFileName), operator+(CWideString, const wchar_t*) 0x0099BF30 / (CWideString, CWideString) 0x0099BE70, CWideString() 0x0099AED0 (labelled CCharString) / (const wchar_t*) 0x0099B6B0 / ~ 0x0099B510, CAFile find first 0x00999760 / next 0x00999850, SplitPath 0x00997620, CMemoryFile(path, 3, 2) 0x009E97B0 / DumpToFile 0x009E9330 / ~ 0x00412020 (labelled CTCInventoryItem::OnDie), GFGetSystemManager 0x009A4EC0, the system manager's +0x7C interface slot 51.
// Layout: retail CMemoryFile is 0x24 bytes (engine/CMemoryFile.h is the PDB 0x28 with a 16-byte CArray); the find handle is 8 bytes and the find data 0x20C (MAX_PATH wide name + a dword); CBinaryFileDataWriter {vptr 0x0122ED7C, CMemoryFile*}.
// Shape: no `optimize("y", off)` -- the /Os large frame gives the biased EBP; frame members must not be char arrays (/GS moves char buffers next to the cookie); the file name is a named CWideString in its own block (built before SplitPath's pushes); the loop condition is computed inside the body (`more = FindNextFile(...)` before the destructors).
#pragma optimize("s", on)
#include <stddef.h>
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const wchar_t* text);           // 0x0099B6B0
    ~CWideString();                             // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);       // 0x0099BF30
class CAFile {
public:
    class CFileScanInfo {
    public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t* firstFile);   // 0x00999760
    static bool __fastcall ScanNextFile(const CWideString& pattern, CFileScanInfo& info, wchar_t* nextFile);     // 0x00999850
};   // 0x00997620
struct CFindFileData {
    wchar_t       FileName[260];
    unsigned long Attributes;
};
CWideString __fastcall GetSoundDirectory();     // 0x0041A2B0 (L"Data\\Sound\\")
enum FileAccessType { FILE_ACCESS_2 = 2 };
class CMemoryFile { public:                            // retail 0x24 bytes (PDB 0x28: the CArray is 12 bytes in retail)
    long _body[9];
    CMemoryFile(const CWideString& path, long mode, unsigned long flags);   // 0x009E97B0
    ~CMemoryFile();                             // 0x00412020
    void DumpToFile();                          // 0x009E9330
};
struct CBinaryFileDataWriter {                  // vtable 0x0122ED7C
    CMemoryFile* File;
    CBinaryFileDataWriter(CMemoryFile* file) : File(file) { }
    ~CBinaryFileDataWriter() { }
    virtual void Write(const void* data, long size);   // slot 0 (0x00418EF7)
};
struct CSoundInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50();
    virtual void CompileLugFile(const CWideString& path, CBinaryFileDataWriter* writer);   // slot 51 (+0xcc)
};
class CSystemManager { public:
    unsigned char    _pad00[0x7c];
    CSoundInterface* Sound;                     // +0x7c
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

class CMainGameComponent { public:
    static void GenerateMetFilesFromLugFiles();
};

void CMainGameComponent::GenerateMetFilesFromLugFiles()
{
    CWideString pattern = GetSoundDirectory() + L"*.lug";
    CAFile::CFileScanInfo handle;
    CFindFileData data;
    if (CAFile::StartFileScan(pattern, handle, data.FileName)) {
        bool more;
        do {
            CWideString name;
            CWideString extension;
            {
                CWideString fileName(data.FileName);
                CAFile::SplitPath(fileName, 0, 0, &name, &extension);
            }
            CMemoryFile file(GetSoundDirectory() + name + L".met", 3, 2);
            CBinaryFileDataWriter writer(&file);
            GFGetSystemManager()->Sound->CompileLugFile(GetSoundDirectory() + name + extension, &writer);
            file.DumpToFile();
            more = CAFile::ScanNextFile(pattern, handle, data.FileName);
        } while (more);
    }
}