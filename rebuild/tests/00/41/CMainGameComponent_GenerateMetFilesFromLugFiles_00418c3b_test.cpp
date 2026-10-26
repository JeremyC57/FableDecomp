// Behaviour test for CMainGameComponent::GenerateMetFilesFromLugFiles @ 0x00418C3B: every found .lug file is compiled into a memory file named <dir><name>.met and dumped.
#include <cstdio>
#include <cstring>
#include <string>

typedef std::string CWideString;               // narrow stand-in for the engine's wide string
static CWideString GetSoundDirectory() { return "Data\\Sound\\"; }
static void SplitPath(const CWideString& path, CWideString*, CWideString*, CWideString* name, CWideString* extension)
{
    std::string::size_type dot = path.rfind('.');
    *name = path.substr(0, dot);
    *extension = path.substr(dot);
}
struct CFindFileData { char FileName[260]; unsigned long Attributes; };
struct CFindFileHandle { long Index; long Unused; };
static const char* g_files[] = { "town.lug", "cave.lug" };
static bool FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data)
{
    if (pattern != "Data\\Sound\\*.lug") return false;
    handle.Index = 0; strcpy(data.FileName, g_files[0]); return true;
}
static bool FindNextFile(const CWideString&, CFindFileHandle& handle, CFindFileData& data)
{
    if (++handle.Index >= 2) return false;
    strcpy(data.FileName, g_files[handle.Index]); return true;
}
static std::string g_log;
class CMemoryFile { public:
    CWideString Path;
    CMemoryFile(const CWideString& path, long mode, unsigned long flags) : Path(path) { if (mode != 3 || flags != 2) g_log += "!mode;"; }
    void DumpToFile() { g_log += "dump " + Path + ";"; }
};
struct CBinaryFileDataWriter {
    CMemoryFile* File;
    CBinaryFileDataWriter(CMemoryFile* file) : File(file) { }
};
struct CSoundInterface {
    void CompileLugFile(const CWideString& path, CBinaryFileDataWriter* writer) { g_log += "compile " + path + "->" + writer->File->Path + ";"; }
};
class CSystemManager { public: CSoundInterface* Sound; };
static CSoundInterface g_sound;
static CSystemManager g_system = { &g_sound };
static CSystemManager* GFGetSystemManager() { return &g_system; }

class CMainGameComponent { public: static void GenerateMetFilesFromLugFiles(); };

void CMainGameComponent::GenerateMetFilesFromLugFiles()
{
    CWideString pattern = GetSoundDirectory() + "*.lug";
    CFindFileHandle handle;
    CFindFileData data;
    if (FindFirstFile(pattern, handle, data)) {
        bool more;
        do {
            CWideString name;
            CWideString extension;
            {
                CWideString fileName(data.FileName);
                SplitPath(fileName, 0, 0, &name, &extension);
            }
            CMemoryFile file(GetSoundDirectory() + name + ".met", 3, 2);
            CBinaryFileDataWriter writer(&file);
            GFGetSystemManager()->Sound->CompileLugFile(GetSoundDirectory() + name + extension, &writer);
            file.DumpToFile();
            more = FindNextFile(pattern, handle, data);
        } while (more);
    }
}

int main()
{
    CMainGameComponent::GenerateMetFilesFromLugFiles();
    const char* want = "compile Data\\Sound\\town.lug->Data\\Sound\\town.met;dump Data\\Sound\\town.met;"
                       "compile Data\\Sound\\cave.lug->Data\\Sound\\cave.met;dump Data\\Sound\\cave.met;";
    bool ok = g_log == want;
    printf("00418c3b_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}