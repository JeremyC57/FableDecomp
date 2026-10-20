// Behaviour test for CWorldMap::GetLevelStaticMapChecksum (0x00506C40): the file is rewound, the level record read,
// the file moved to the map data, the header read; the header's checksum comes back, header and file freed.
#include <cstdio>
#include <cstring>
static int g_files = 0, g_headers = 0; static long g_seeks[4]; static int g_nseek = 0;
struct CWideString { const char* Text; };
struct CCharString { const char* Text; CWideString ToWideString() const { CWideString w = { Text }; return w; } };
class CAFile { public: virtual ~CAFile() {} virtual void Seek(long p) { g_seeks[g_nseek++] = p; } };
class CDiskFileWin32 : public CAFile { public: CDiskFileWin32(const CWideString& n, int, unsigned long) { ++g_files; Name = n.Text; } ~CDiskFileWin32() { --g_files; } const char* Name; };
struct CCPPointerInfo { long RefCount; void* Data; CCPPointerInfo(void* d) : RefCount(1), Data(d) {} };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer(T* object) : Data(object) { if (object) Info = new CCPPointerInfo(object); else Info = 0; }
    ~CCountedPointer() { if (Info) { if (--Info->RefCount == 0) { delete (T*)Info->Data; delete Info; } } }
    T* operator->() const { return Data; }
};
struct CFileFormatLevel { long MapDataOffset; void ReadFromFile(CAFile*, void*) { MapDataOffset = 0x1234; } };
struct CFileFormatMapHeader {
    unsigned long StaticMapChecksum; CFileFormatMapHeader() { ++g_headers; } ~CFileFormatMapHeader() { --g_headers; }
    void ReadFromFile(CAFile* f, void*) { StaticMapChecksum = std::strcmp(((CDiskFileWin32*)f)->Name, "Level.lev") == 0 ? 0xC0FFEEu : 0; }
};
template <class T> struct CScopedPtr { T* ptr; CScopedPtr(T* p) : ptr(p) {} ~CScopedPtr() { delete ptr; } T* operator->() const { return ptr; } };
struct CWorldMap {
    unsigned long GetLevelStaticMapChecksum(const CCharString& levelName)
    {
        CCountedPointer<CAFile> file(new CDiskFileWin32(levelName.ToWideString(), 1, 2));
        CScopedPtr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
        file->Seek(0);
        CFileFormatLevel level;
        level.ReadFromFile(file.Data, 0);
        file->Seek(level.MapDataOffset);
        header->ReadFromFile(file.Data, 0);
        return header->StaticMapChecksum;
    }
};
int main()
{
    CWorldMap m; CCharString name = { "Level.lev" };
    unsigned long c = m.GetLevelStaticMapChecksum(name);
    bool ok = c == 0xC0FFEEu && g_files == 0 && g_headers == 0 && g_nseek == 2 && g_seeks[0] == 0 && g_seeks[1] == 0x1234;
    if (!ok) { std::printf("00506c40_TEST FAIL\n"); return 1; }
    std::printf("00506c40_TEST PASS\n");
    return 0;
}