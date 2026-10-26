// Behaviour test for CWorldMap::LoadRegionGraph 0x00506D40: path = misc dir + name, graph created once, initialised from the file text.
#include <cstdio>
#include <cstring>

class CWideString { public:
    char Text[64];
    CWideString(const char* t) { std::strcpy(Text, t); }
    ~CWideString() { }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a.Text); std::strcat(r.Text, b.Text); return r; }
static bool g_exists = true;
static char g_lastChecked[64];
static bool PathExists(const CWideString& p) { std::strcpy(g_lastChecked, p.Text); return g_exists; }
namespace NResourceDirectoryNames { static CWideString GetMiscDir() { return CWideString("Data\\Misc\\"); } }
enum FileAccessType { FILE_ACCESS_READ = 1 };
static int g_opened = 0, g_closed = 0;
class CAFile { public: virtual ~CAFile() { } };
class CDiskFileWin32 : public CAFile { public:
    CWideString Pathname;
    CDiskFileWin32(const CWideString& name, FileAccessType, unsigned long) : Pathname(name.Text) { ++g_opened; }
    void Close() { ++g_closed; }
    virtual ~CDiskFileWin32() { Close(); }
};
class CASuspendableProcess;
class CCharString { public:
    char Text[64];
    CCharString() { Text[0] = 0; }
    void LoadFromFile(CAFile* file, CASuspendableProcess*, long) { std::strcpy(Text, static_cast<CDiskFileWin32*>(file)->Pathname.Text); }
};
class CWorldMap;
static int g_inits = 0, g_graphs = 0; static char g_initText[64];
class CRegionConnectionGraph { public:
    CRegionConnectionGraph() { ++g_graphs; }
    void InitialiseGraph(const CCharString& text, const CWorldMap&) { ++g_inits; std::strcpy(g_initText, text.Text); }
};
template<class T> class CCountedPointer { public:
    T* Data;
    T* operator->() const { return Data; }
    void Reset(T* data) { delete Data; Data = data; }
};
class CWorldMap { public:
    CCountedPointer<CRegionConnectionGraph> PRegionGraph;
    void LoadRegionGraph(const CWideString& name);
};

void CWorldMap::LoadRegionGraph(const CWideString& name)
{
    CWideString path = NResourceDirectoryNames::GetMiscDir() + name;
    if (PathExists(path))
    {
        CDiskFileWin32 file(path, FILE_ACCESS_READ, 2);
        if (!PRegionGraph.Data)
            PRegionGraph.Reset(new CRegionConnectionGraph);
        CCharString text;
        text.LoadFromFile(&file, 0, -1);
        PRegionGraph->InitialiseGraph(text, *this);
    }
}

int main()
{
    CWorldMap w; w.PRegionGraph.Data = 0; bool ok = true;
    w.LoadRegionGraph(CWideString("graph.txt"));
    if (std::strcmp(g_lastChecked, "Data\\Misc\\graph.txt") || g_graphs != 1 || g_inits != 1 || g_opened != 1 || g_closed != 1) ok = false;
    if (std::strcmp(g_initText, "Data\\Misc\\graph.txt")) ok = false;
    w.LoadRegionGraph(CWideString("graph.txt"));
    if (g_graphs != 1 || g_inits != 2) ok = false;
    g_exists = false; w.LoadRegionGraph(CWideString("x"));
    if (g_inits != 2 || g_opened != 2) ok = false;
    delete w.PRegionGraph.Data;
    if (!ok) { std::printf("00506d40_TEST FAIL\n"); return 1; }
    std::printf("00506d40_TEST PASS\n");
    return 0;
}